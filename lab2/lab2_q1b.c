#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    char* value = getenv("n");
    //printf("%s\n",value);
    int n_arr[30];
    int n;

    if(strcmp(value,"")==0) 
    {
        // zero numbers passed
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
    else
    {
        printf("1 or more numbers passed:\n");
        // parse string for multiple values

        int ind = 0;
        int len=strlen(value);
        int c=0;
        char* str="";
        for(int ind=0;ind<len;ind++)
        {
            int val=0;
            while(value[ind]!=' ' && ind<len)
            {
                val = val*10+ (value[ind]-'0');
                ind++;
            }
            if(val>0) {n_arr[c++]=val;}
        }

        //print sequences
        for(int k=0;k<c;k++)
        {
            int temp= n_arr[k];
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