#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main()
{
    // char input[100];
    // int pipe_fd[2];
    // pid_t child_pid;
    // char command1[50];
    // char command2[50];
     
    // while(1)
    // {
    //     printf("Enter a cmd or quit to exit: ");
    //     fgets(input,sizeof(input),stdin);
    //     input[strlen(input)-1] = '\0';
    //     int f=0,i1=0,i2=0;
    //     for(int i=0;i<strlen(input);i++)
    //     {
    //         if(input[i]=='|') f=1;
    //         if(input[i]=='|' || input[i]==' ') continue;

    //         if(f) command2[i2++] = input[i];
    //         else command1[i1++] = input[i];
    //     }
    //     printf("%s\t%s\n",command1,command2);

    // if(strcmp(input,"quit")==0) { printf("User wish to quit."); return 0;}

    // if(pipe(pipe_fd)==-1)
    // {
    //     exit(EXIT_FAILURE);  
    // }

    // if((child_pid=fork()) == -1)
    // {
    //     exit(EXIT_FAILURE);
    // }
    // if(child_pid==0) //child process
    // {
    //     close(pipe_fd[1]);
    //     dup2(pipe_fd[0],STDIN_FILENO);
    //     close(pipe_fd[0]);
    //     execlp(command1,command2,NULL);
    //     exit(EXIT_FAILURE);
    // }
    // else //parent process
    // {
    //     close(pipe_fd[0]);
    //     dup2(pipe_fd[1],STDOUT_FILENO);
    //     close(pipe_fd[1]);
    //     execlp(command1,command2,NULL);
    //     exit(EXIT_FAILURE);
    // }

    // }

    
    int fd[2];
    pid_t pid;

    /* create the pipe */
    if ( pipe(fd) == -1) {
        fprintf(stderr,"Pipe failed");
        return 1;
    }

    /* fork a child process */
    pid = fork();
    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    if(pid==0){ /* child process */
        close(fd[1]);
        dup2(fd[0],STDIN_FILENO); //read end
        close(fd[0]);
        execl("./compute_sequence_2","./compute_sequence_2",NULL);
        perror("something 2");
        exit(1);
    }
    else{ /* parent process */
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO); //write end
        close(fd[1]);
        execl("./compute_sequence_1","./compute_sequence_1",NULL);
        perror("something 1");
        exit(1);
    }

    return 0;
}