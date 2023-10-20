#include<stdio.h>

int main()
{
    char c= 'a',c2='z',c3='A',c4='Z';
    char b1 = '0', b2= '9', b3='.';
    
    printf("a-z: %d-%d\n",(int)c,(int)c2);
    printf("A-Z: %d-%d\n",(int)c3,(int)c4);
    printf("0-9: %d-%d\n",(int)b1,(int)b2);
    printf("(.): %d\n",(int)b3);
}