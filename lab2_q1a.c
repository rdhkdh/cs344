#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
    int n;
    int n_arr[30];
    if(argc==1)
    {
        printf("zero arguments passed (n=100): ");
        n=100;
        while(n!=1)
        {
            printf("%d ",n);
            if(n%2==0) {n=n/2;}
            else{n=3*n+1;}
        }
        printf("\n");
    }
    if(argc==2)
    {
        printf("1 argument passed: ");
        n= atoi(argv[1]);
        while(n!=1)
        {
            printf("%d ",n);
            if(n%2==0) {n=n/2;}
            else{n=3*n+1;}
        }
        printf("\n");
    }
    if(argc>2)
    {
        printf("more than 1 argument passed:\n");
        for(int i=1; i<argc; i++)
        {
            n_arr[i-1] = atoi(argv[i]);
        }
        for(int j=0;j<argc-1;j++)
        {
            int temp= n_arr[j];
            while(temp!=1)
            {
                printf("%d ",temp);
                if(temp%2==0) {temp=temp/2;}
                else{temp=3*temp+1;}
            }
            printf("\n");
        }
    }


}