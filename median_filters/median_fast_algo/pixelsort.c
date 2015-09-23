/*
 * use libjpeg to read and write jpeg format file.
 *
 * usage:
 * gcc -o jpeg_sample jpeg_sample.c -ljpeg
 * ./jpeg_sample input.jpg output.jpg
 *
 *                    mm   mmmm
 *   mmmm    mmm     m"#  #    #
 *   #" "#  #   "   #" #  "mmmm"
 *   #   #   """m  #mmm#m #   "#
 *   ##m#"  "mmm"      #  "#mmm"
 *   #
 *   "
 */

#include<stdio.h>
#include<jpeglib.h>
#include<stdlib.h>
#include<time.h>

typedef int elem_type ;

#define PIX_SWAP(a,b) { elem_type temp=(a);(a)=(b);(b)=temp; }
#define PIX_STACK_SIZE 50

void pixel_qsort(elem_type *pix_arr, int npix)
{
	int i, ir, j, k, l;
	int * i_stack ;
	int j_stack ;
	
	elem_type a ;
	ir = npix ;
	l = 1 ;
	j_stack = 0 ;
	i_stack = malloc(PIX_STACK_SIZE * sizeof(elem_type)) ;
	
	for (;;) 
	{

		if (ir-l < 7) 
		{
			for (j=l+1 ; j<=ir ; j++) 
			{
				a = pix_arr[j-1];
				for (i=j-1 ; i>=1 ; i--) 
				{
					if (pix_arr[i-1] <= a) 
						break;
					pix_arr[i] = pix_arr[i-1];
				}
				pix_arr[i] = a;
			}
		
		if (j_stack == 0) 
			break;
		ir = i_stack[j_stack-- -1];
		l = i_stack[j_stack-- -1];
		}
		else 
		{
			k = (l+ir) >> 1;
			PIX_SWAP(pix_arr[k-1], pix_arr[l])
			if (pix_arr[l] > pix_arr[ir-1]) 
			{
				PIX_SWAP(pix_arr[l], pix_arr[ir-1])
			}
			if (pix_arr[l-1] > pix_arr[ir-1]) 
			{
				PIX_SWAP(pix_arr[l-1], pix_arr[ir-1])

			}
			if (pix_arr[l] > pix_arr[l-1]) 
			{
				PIX_SWAP(pix_arr[l], pix_arr[l-1])
			}
	
		i = l+1;
		j = ir;
		a = pix_arr[l-1];
		for (;;) 
		{
			do i++; 
			while (pix_arr[i-1] < a);
			
			do j--; 
			while (pix_arr[j-1] > a);
	
		if (j < i) 
			break;
		PIX_SWAP(pix_arr[i-1], pix_arr[j-1]);
		}

	pix_arr[l-1] = pix_arr[j-1];
	pix_arr[j-1] = a;
	j_stack += 2;
	
	if (j_stack > PIX_STACK_SIZE) 
	{
		printf("stack too small in pixel_qsort: aborting");
		exit(-2001) ;
	}
	
	if (ir-i+1 >= j-l) 
	{
		i_stack[j_stack-1] = ir;
		i_stack[j_stack-2] = i;
		ir = j-1;
	} 
	else 
	{
		i_stack[j_stack-1] = j-1;
		i_stack[j_stack-2] = l;
		l = i;
	}
   }
 }
 free(i_stack) ;
}

#undef PIX_STACK_SIZE
#undef PIX_SWAP


unsigned char *raw_image = NULL;
unsigned char *raw_image2 = NULL;
int p,q;

int width;
int height;
int bytes_per_pixel = 1;   /* or 1 for GRACYSCALE images */
int color_space = JCS_GRAYSCALE; /* or JCS_GRAYSCALE for grayscale images */

int fno = 9;

int dlt=1;

int read_jpeg_file( char *filename )
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	JSAMPROW row_pointer[1];

	FILE *infile = fopen( filename, "rb" );
	unsigned long location = 0;
	long long j=0, i = 0;

	if ( !infile )
	{
		printf("Error opening jpeg file %s\n!", filename );
		return -1;
	}

	cinfo.err = jpeg_std_error( &jerr );

	jpeg_create_decompress( &cinfo );

	jpeg_stdio_src( &cinfo, infile );

	jpeg_read_header( &cinfo, TRUE );

	jpeg_start_decompress( &cinfo );

    width=cinfo.image_width;
	height=cinfo.image_height;
	bytes_per_pixel=cinfo.num_components;

	printf("components = %d\n", cinfo.num_components);

	raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
	raw_image2 = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );

   unsigned char  **img = (unsigned char  **)malloc(height * sizeof(unsigned char  *));
    for (i=0; i<height; i++)
         img[i] = (unsigned char *)malloc(width*bytes_per_pixel * sizeof(unsigned char ));


   unsigned char  **oimg = (unsigned char  **)malloc(height * sizeof(unsigned char  *));
    for (i=0; i<height; i++)
         oimg[i] = (unsigned char *)malloc(width*bytes_per_pixel * sizeof(unsigned char ));

	row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );

	while( cinfo.output_scanline < cinfo.image_height )
	{
		jpeg_read_scanlines( &cinfo, row_pointer, 1 );
		for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
			raw_image[location++] = row_pointer[0][i];
	}

    location=0;
    for(i=0;i<height;i++)
     for(j=0;j<width*bytes_per_pixel;j++)
     {
        img[i][j]=raw_image[location++];
     }

    int s,d;

   for(i=0;i<height;i++)
     for(j=0;j<width*bytes_per_pixel;j++)
     {

          int *a,s,d;

          a= (int *)malloc(fno*fno*sizeof(int));

              for(s=0;s<fno;s++)
                {
                  for(d=0;d<fno;d++)
                  {
                      if(i+s>=height||j+d>=width*bytes_per_pixel)
                      {
                        a[fno*s+d]=0;
                      }
                      else
                       a[fno*s+d]=img[i+s][j+d];
                  }

                }


                pixel_qsort(a,fno*fno);

                oimg[i][j]=a[fno*fno/2];


     }

    location=0;
    for(i=0;i<height;i++)
     for(j=0;j<width*bytes_per_pixel;j++)
     {
        raw_image2[location++]=oimg[i][j];
     }


	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress( &cinfo );
	free( row_pointer[0] );
	fclose( infile );

	return 1;
}

int write_jpeg_file( char *filename )
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	JSAMPROW row_pointer[1];
	FILE *outfile = fopen( filename, "wb" );

	if ( !outfile )
	{
		printf("Error opening output jpeg file %s\n!", filename );
		return -1;
	}
	cinfo.err = jpeg_std_error( &jerr );
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = bytes_per_pixel;
	cinfo.in_color_space = color_space;

	jpeg_set_defaults( &cinfo );

	jpeg_start_compress( &cinfo, TRUE );

	while( cinfo.next_scanline < cinfo.image_height )
	{
		row_pointer[0] = &raw_image2[ cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
		jpeg_write_scanlines( &cinfo, row_pointer, 1 );
	}

	jpeg_finish_compress( &cinfo );
	jpeg_destroy_compress( &cinfo );
	fclose( outfile );

	return 1;
}


int main( int argc, char *argv[] )
{

    clock_t tStart = clock();

     if(argc!=3)
     {
        printf("\n\nError : Change input in this form: %s file1.jpg file2.jpg \n\n",argv[0]);
        return -1;
     }

     char *infilename = argv[1], *outfilename = argv[2];

     int x;

      if( read_jpeg_file(infilename) == 1 )
        {
            printf("The image was read successfully. \n");
        }
      if(write_jpeg_file(outfilename)==1)
        {
            printf("The jpeg file was filtered successfully. \n");
        }

  printf("Time taken: %lf s\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

  return 0;
}



