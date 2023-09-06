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
    int count=0;

    FILE* fp;
    fp= fopen("q1d.txt","a");

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
            fprintf(fp,"\nChild PID %d, number %d: ",child_pid,i);
            while(temp!=1)
            {
                fprintf(fp,"%d ",temp);
                if(temp%2==0) { temp=temp/2; }
                else { temp= (3*temp)+1; }
            }

            exit(1);
        }
        else { // parent process 
            if(i==1 || i==3 || i==5 || i==7 || i==9) 
            {
                wait(NULL); // parent will wait for the child to complete
                
            } 
        }
    }
    fclose(fp);
    
}