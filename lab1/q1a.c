#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(int argc,char* argv[]) 
{
    if(argc<2) { printf("n not entered in command line arguments.\n"); return 0; }

    int n = atoi(argv[1]);

    pid_t pid;
    pid = fork(); //fork a child process

    if(pid < 0) //error occured
    {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if(pid == 0) 
    { // child process created
        while(n!=1)
        {
            printf("%d\n",n);
            if(n%2==0) { n=n/2; }
            else { n= (3*n)+1; }
        }
        _exit(1);
        //execlp("/bin/ls","ls",NULL);
        
    }
    else { // parent process 
        wait(NULL); // parent will wait for the child to complete
        printf("Child Complete\n");
    }
}