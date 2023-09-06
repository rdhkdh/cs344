#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(int argc,char* argv[]) 
{
    if(argc<2) { printf("n not entered in command line arguments.\n"); return 0; }

    int n = atoi(argv[1]);
    long x;

    FILE* fp;
    fp= fopen("q1b.txt","a");

    pid_t pid;
    pid = fork(); //fork a child process

    if(pid < 0) //error occured
    {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if(pid == 0) 
    { // child process created
        //fprintf(fp,"Child PID %d: ",pid);
        while(n!=1)
        {
            fprintf(fp,"Child PID %d: %d\n",pid,n);
            if(n%2==0) { n=n/2; }
            else { n= (3*n)+1; }
        }
        x = ftell(fp);
        fprintf(fp,"File position indicator: %li\n",x);
        fclose(fp);
        _exit(1);
        //execlp("/bin/ls","ls",NULL);
        
    }
    else { // parent process 
        wait(NULL); // parent will wait for the child to complete
        printf("Child Complete\n");
        int parent_pid = getpid();
        printf("Parent PID: %d\n",parent_pid);
    }
}