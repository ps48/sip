#include<stdio.h>



int fx[3][3]={     {0,0,0},
                   {0,0,0},
                   {0,0,0} };

int fy[3][3]={     {1,2,1},
                   {0,0,0},
                   {-1,-2,-1} };



int main()
{
    int i,j;
    int img[3][4]={1,1,1,1,1,1,1,1,1,1,1,1};
    int count=0;

  for(i=0;i<3;i++)
   {  for(j=0;j<4;j++)
     {
        if(i!=0&&j!=0&&i!=(3-1)&&j!=(4-1))
        {

            int G =0,GX,GY;

            GY= img[i-1][j-1]*fy[0][0]+img[i-1][j]*fy[0][1]
                +img[i-1][j+1]*fy[0][2]+img[i][j-1]*fy[1][0]
                +img[i][j]*fy[1][1]+img[i][j+1]*fy[1][2]
                +img[i+1][j-1]*fy[2][0]+img[i+1][j]*fy[2][1]
                +img[i+1][j+1]*fy[2][2];

            GY= img[i-1][j-1]*fx[0][0]+img[i-1][j]*fx[0][1]
                    +img[i-1][j+1]*fx[0][2]+img[i][j-1]*fx[1][0]
                    +img[i][j]*fx[1][1]+img[i][j+1]*fx[1][2]
                    +img[i+1][j-1]*fx[2][0]+img[i+1][j]*fx[2][1]
                    +img[i+1][j+1]*fx[2][2];

            G=((abs(GX)+abs(GY))/2)/6;

            img[i][j]=G;

        count++;

        }

     }

    }

    printf("%d \n",count );

    for(i=0;i<3;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
        {
            printf("%d ",img[i][j]);

        }
    }


    return 0;
}
