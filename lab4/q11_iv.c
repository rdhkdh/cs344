#include<stdio.h>

#include<stdlib.h>

#include<unistd.h>

#include<string.h>

#include<sys/types.h>

#include<sys/wait.h>

 

int main(){

    char s[200];

    while (1)

    {

        printf("Give command of foramt 'command1 | command2 | command3' or 'quit' to exit: ");

        fgets(s,sizeof(s),stdin);

 

        char *c1 = NULL;

        char *c2 = NULL;

        char *c3 = NULL;

 

        s[strcspn(s,"\n")] = '\0';

        if(strcmp(s,"quit") == 0)break;

 

        char *tk = strtok(s,"|");

        if(tk){

            c1 = tk;

            tk = strtok(NULL,"|");

        }

        if(tk){

            c2 = tk;

            tk = strtok(NULL,"|");

        }

        if(tk){

            c3 = tk;

        }

 

        printf("Command1 :%s \nCommand2 :%s \nCommand3 :%s \n",c1,c2,c3);

 

        if(c1 && c2 && c3){

            int pipe1[2];

            int pipe2[2];

 

            if(pipe(pipe1) == -1 || pipe(pipe2) == -1){

                perror("Pipe Failed");

                return 1;

            }

            pid_t child1 = fork() , child2= fork();

            if(child1 == -1){

                perror("Fork of child 1 Failed");

                return 1;

            }

            if(child1 == 0){

                close(pipe1[0]);

                dup2(pipe1[1],STDOUT_FILENO);

                close(pipe1[1]);

 

                execlp("/bin/sh","/bin/sh","-c",c1,NULL);

                perror("execlp of command 1 failed");

                exit(1);

            }

            else{

                close(pipe1[1]);

                if(child2 == -1){

                    perror("Fork of child 2 Failed");

                    return 1;

                }

                if(child2 == 0){

                    close(pipe1[1]);

                    close(pipe2[0]);

 

                    dup2(pipe1[0],STDIN_FILENO);    

                    close(pipe1[0]);

 

                    dup2(pipe2[1],STDOUT_FILENO);

                    close(pipe2[1]);

 

                    execlp("/bin/sh","/bin/sh","-c",c2,NULL);

                    perror("execlp of command 1 failed");

                    exit(1);                

                }else{

                    close(pipe1[0]);

                    close(pipe2[1]);

 

                    dup2(pipe2[0],STDIN_FILENO);

                    close(pipe2[0]);

 

                    execlp("/bin/sh","/bin/sh","-c",c3,NULL);

                    perror("execlp of command 3 failed");

                    exit(1);   

                }

            }

        }else {

            printf("Invalid input format\n");

        }

    }

    return 0;

}