#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <unistd.h>


int main()
{
    pid_t pid;

    // ---------------------zero arguments-----------------------------------------

    pid = fork(); /* fork a child process */
    if (pid < 0) 
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) 
    { /* child process */
        printf("Child executing...\n");
        execle("./q1a_210101088","./q1a_210101088",NULL,NULL);
    }
    else 
    { /* parent process- parent will wait for the child to complete */
        wait (NULL);
        printf("Child Complete\n");
    }

    // ---------------------1 argument-----------------------------------------

    pid = fork(); /* fork a child process */
    if (pid < 0) 
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) 
    { /* child process */
        printf("Child executing...\n");
        execl("./q1a_210101088","./q1a_210101088","10",NULL);
    }
    else 
    { /* parent process- parent will wait for the child to complete */
        wait (NULL);
        printf("Child Complete\n");
    }

    // ---------------------10 arguments-----------------------------------------

    pid = fork(); /* fork a child process */
    if (pid < 0) 
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) 
    { /* child process */
        printf("Child executing...\n");
        char* argv[] = {"./q1a_210101088","11","12","13","14","15","16","17","18","19","20",NULL};
        char* env_var[] = {"rkd",NULL};
        execve("./q1a_210101088",argv,env_var);
    }
    else 
    { /* parent process- parent will wait for the child to complete */
        wait (NULL);
        printf("Child Complete\n");
    }
}