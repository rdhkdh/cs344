//lab4 q1 (i)
#include<stdio.h>
#include<stdlib.h>

int main()
{
    int n=100;
    // if(scanf("%d",&n) != 1)
    // {
    //     n = 100;
    // }
    
    while(n!=1)
    {
        printf("%d ",n);
        if(n%2==0) {n=n/2;}
        else{n=3*n+1;}
    }
    printf("1\n");


    return 0;
}