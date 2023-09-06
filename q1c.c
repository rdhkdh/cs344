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
    fp= fopen("q1c.txt","a");

    for(int i=1;i<=50;i++)
    {
        pid_t pid;
        pid = fork(); //fork a child process

        if(pid < 0) //error occured
        {
            fprintf(stderr, "Fork Failed\n");
            return 1;
        }
        else if(pid == 0) 
        { // child process created
            
            int temp=i*n;
            int child_pid = getpid();
            fprintf(fp,"\nChild PID %d: ",child_pid);
            while(temp!=1)
            {
                fprintf(fp,"%d ",temp);
                if(temp%2==0) { temp=temp/2; }
                else { temp= (3*temp)+1; }
            }
    
            exit(1);
        }
        else { // parent process 
            wait(NULL); // parent will wait for the child to complete
            printf("Child %d Complete\n",i);
            int parent_pid = getpid();
            printf("Parent PID: %d\n\n",parent_pid);
        }
    }
    fclose(fp);
    
}