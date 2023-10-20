#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    FILE* fp= fopen("q1b_output.txt","r");
    FILE* fp1= fopen("q1b_output_sorted.txt","w");

    double s[256][256];
    int r,c;
    double e;

    for(int i=1;i<=256*256;i++)
    {
        fscanf(fp,"%d %d %lf", &r,&c,&e);
        s[r-1][c-1]= e;
    }
    for(int i=1;i<=256;i++)
    {
        for(int j=1;j<=256;j++)
        {
            fprintf(fp1,"(%d %d): %lf\n", i, j, s[i-1][j-1]);
        }   
    }

    fclose(fp1);
    fclose(fp);
}