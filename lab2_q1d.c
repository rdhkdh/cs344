#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <unistd.h>

int main()
{
    pid_t pid;

    // ---------------------zero arguments, execl() sys call-----------------------------------------

    pid = fork(); /* fork a child process */
    if (pid < 0) 
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if(pid == 0) 
    { /* child process */
        printf("Child executing...\n");
        execl("./q1b_210101088","./q1b_210101088",NULL);
    }
    else 
    { /* parent process- parent will wait for the child to complete */
        wait (NULL);
        printf("Child Complete\n");
    }

    // ---------------------1 env variable, execle() sys call-----------------------------------------

    pid = fork(); /* fork a child process */
    if (pid < 0) 
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) 
    { /* child process */
        printf("Child executing...\n");
        char* env_var[] = {"n=10",NULL};
        execle("./q1b_210101088","./q1b_210101088",NULL,env_var);
    }
    else 
    { /* parent process- parent will wait for the child to complete */
        wait (NULL);
        printf("Child Complete\n");
    }

    // ---------------------10 env variables, execve() sys call-----------------------------------------

    pid = fork(); /* fork a child process */
    if (pid < 0) 
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) 
    { /* child process */
        printf("Child executing...\n");
        char* argv[] = {"./q1b_210101088",NULL};
        char* env_var[] = {"n=11 12 13 14 15 16 17 18 19 20",NULL};
        execve("./q1b_210101088",argv,env_var);
    }
    else 
    { /* parent process- parent will wait for the child to complete */
        wait (NULL);
        printf("Child Complete\n");
    }
}