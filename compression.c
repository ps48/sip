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
 *
 */

#include<stdio.h>
#include<jpeglib.h>
#include<stdlib.h>
#include<time.h>

unsigned char *raw_image = NULL;

int width;
int height;
int bytes_per_pixel;   /* or 1 for GRACYSCALE images */
int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

int read_jpeg_file( char *filename )
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	JSAMPROW row_pointer[1];

	FILE *infile = fopen( filename, "rb" );
	unsigned long location = 0;
	int i = 0;

	if ( !infile )
	{
		printf("Error opening jpeg file %s\n!", filename );
		return -1;
	}

	cinfo.err = jpeg_std_error( &jerr );

	jpeg_create_decompress( &cinfo );

	jpeg_stdio_src( &cinfo, infile );

	jpeg_read_header( &cinfo, TRUE );

    width=cinfo.image_width;
	height=cinfo.image_height;
	bytes_per_pixel=cinfo.num_components;

	if(bytes_per_pixel==3)
     color_space = JCS_RGB;
   else
     color_space = JCS_GRAYSCALE;


	jpeg_start_decompress( &cinfo );

	printf("components = %d\n", cinfo.num_components);

	raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );

	row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );

	while( cinfo.output_scanline < cinfo.image_height )
	{
		jpeg_read_scanlines( &cinfo, row_pointer, 1 );
		for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
			raw_image[location++] = row_pointer[0][i];
	}

	long long a[256],len=location,no=0,j=0;

	for(i=0;i<256;i++)
	  a[i]=0;

	location=0;

	while(location!=len)
	   a[raw_image[location++]]++;

	for(i=0;i<256;i++)
	  if(a[i]>0)
	   no++;

	for(i=0;i<256;i++)
	{
        printf("%d :",i);

	  for(j=0;j<a[i];j++)
	  {
        printf("%c",'*');
	  }
	  printf("\n");
	}

   	long long pix[no],it,pixcount[no],pixcdf[no],npix[no];

    for(it=0;it<256;it++)
	  if(a[it]>0)
      {
         pix[j]=it;
         pixcount[j]=a[it];
         j++;
	  }

	for(i=0;i<no;i++)
	   {
	     if(i==0)
	      pixcdf[i]=pixcount[i];
	     else
	      pixcdf[i]=pixcdf[i-1]+pixcount[i];
	   }

	long long cmin=99999999999;

    for(i=0;i<no;i++)
      if(pixcdf[i]<cmin)

         cmin=pixcdf[i];

	for(i=0;i<no;i++)
      npix[i]=(pixcdf[i]-cmin)/(width*height-cmin)*255;


    for(i=0;i<len;i++)
       for(j=0;j<no;j++)
          if(pix[j]==i)
          {
            raw_image[i]=(char)npix[j];
            break;
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
		row_pointer[0] = &raw_image[ cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
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
            printf("The jpeg file was compressed successfully. \n");
        }

  printf("Time taken: %lf s\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

  return 0;
}


