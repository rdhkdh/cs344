#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    char input[150];
    char command1[50];
    char command2[50];
    char command3[50];

    printf("Enter a cmd or quit to exit: ");
    fgets(input,sizeof(input),stdin);

    input[strlen(input)-1] = '\0';
    if(strcmp(input,"quit")==0) { printf("User wished to quit.\n"); return 0;}

    int f=0,i1=0,i2=0,i3=0;
    for(int i=0;i<strlen(input);i++)
    {
        if(input[i]=='|') f++;
        if(input[i]=='|' || input[i]==' ') continue;

        if(f==0) command1[i1++] = input[i];
        if(f==1) command2[i2++] = input[i];
        if(f==2) command3[i3++] = input[i];
    }
    printf("%s\t%s\t%s\n",command1,command2,command3);

    int fd[2];
    pid_t pid, pid2;

    if ( pipe(fd) == -1) {
        fprintf(stderr,"Pipe failed");
        return 1;
    }

    pid = fork();
    // if(pid>0) // write output of command1 to pipe
    // {
    //     close(fd[0]);
    //     dup2(fd[1],STDOUT_FILENO); //write end
    //     close(fd[1]);
    //     execl(command1,command1,NULL);
    //     perror("something 1");
    //     exit(1);
    // }
    if(pid==0) //read as input of command2
    {
        close(fd[0]);
        dup2(fd[1],STDIN_FILENO); //read end
        close(fd[1]);
        execl(command2,command2,NULL);
        perror("something 2");

        int fd2[2];

        if ( pipe(fd2) == -1) {
            fprintf(stderr,"Pipe failed");
            return 1;
        }

        pid2 = fork();

        if(pid2>0) // write output of command2
        {
            close(fd2[0]);
            dup2(fd2[1],STDOUT_FILENO); //write end
            close(fd2[1]);
            execl(command3,command3,NULL);
            perror("something 1");
            exit(1);
        }
        if(pid2==0) //read as input of command3
        {

        }
    }
    
}