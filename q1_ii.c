//lab4 q1 (ii)
#include<stdio.h>
#include<stdlib.h>

int main()
{
    int n,temp;

    while(n!=1)
    {
        scanf("%d",&n);
        temp=n;

        while(temp!=1)
        {
            printf("%d ",temp);
            if(temp%2==0) {temp=temp/2;}
            else{temp=3*temp+1;}
        }
        printf("1\n");
    }
    
    return 0;
}