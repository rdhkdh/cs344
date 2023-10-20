#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{
    char* str= "10 23 20 30";
    char* token = strtok(str," ");

    while( token!=NULL )
    {
        printf("%s\n",token);
        //strtok(NULL);
        token=strtok(NULL," ");
    }
}