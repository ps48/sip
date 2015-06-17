/*
 * use libjpeg to read and write jpeg format file.
 *
 * usage:
 * gcc -o jpeg_sample jpeg_sample.c -ljpeg
 * ./jpeg_sample image1.jpg image2.jpg
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
unsigned char *raw_image2 = NULL;


int compare_jpeg_file( char *filename, char *filename2 )
{
	struct jpeg_decompress_struct cinfo,cinfo2;
	struct jpeg_error_mgr jerr,jerr2;

	JSAMPROW row_pointer[1],row_pointer2[1];

	FILE *infile2 = fopen(filename2,"rb");
	unsigned long location2 =0;
	int i=0;

	if(!infile2)
	{
        printf("Error opening jpeg file %s\n!",filename2);
        return -1;
	}

	FILE *infile = fopen( filename, "rb" );
	unsigned long location = 0;


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

	cinfo2.err = jpeg_std_error(& jerr2);
	jpeg_create_decompress(&cinfo2);
	jpeg_stdio_src(&cinfo2,infile2);
	jpeg_read_header(&cinfo2,TRUE);
	jpeg_start_decompress(&cinfo2);


	if(cinfo.image_width!=cinfo2.image_width&&cinfo.image_height!=cinfo2.image_width)
    {
            printf("images don't have same dimensions. \nCannot calculate percentage error\n");

            return 1;
    }

	raw_image2 = (unsigned char*)malloc( cinfo2.output_width*cinfo2.output_height*cinfo2.num_components );
	row_pointer2[0] = (unsigned char *)malloc( cinfo2.output_width*cinfo2.num_components );

	raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
	row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );


	while( cinfo.output_scanline < cinfo.image_height )
	{
		jpeg_read_scanlines( &cinfo, row_pointer, 1 );
		for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
			raw_image[location++] = row_pointer[0][i];
	}

	while( cinfo2.output_scanline < cinfo2.image_height )
	{
		jpeg_read_scanlines( &cinfo2, row_pointer2, 1 );
		for( i=0; i<cinfo2.image_width*cinfo2.num_components;i++)
			raw_image2[location2++] = row_pointer2[0][i];
	}

	long long j,flag=0,p_error=0,p_tot=0;

	for(i=0,j=0;i<location&&j<location2;i++,j++)
	{
        if(raw_image[i]!=raw_image2[j])
        {
            p_error++;
            flag=1;
        }

        p_tot++;
	}

	printf("Percentage error is :%lf\n",(double)p_error/p_tot*100);


	if(flag==0)
	{
        return 0;
	}


	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress( &cinfo );
	free( row_pointer[0] );
	fclose( infile );
   	jpeg_finish_decompress( &cinfo2 );
	jpeg_destroy_decompress( &cinfo2 );
	free( row_pointer2[0] );
	fclose( infile2 );


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

     x=compare_jpeg_file(infilename,outfilename);

      if(x==0)
      {
        printf("Images are same\n");
      }
      else
      {
          if(x==1)
          {
            printf("Images are different\n");
          }
      }

     printf("Time taken: %lf s\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
     return 0;
}
