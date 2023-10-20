#include<stdio.h>

#include<stdlib.h>

#include<unistd.h>

#include<string.h>

#include<sys/types.h>

#include<sys/wait.h>

 

int main(){

    char s[100];

    printf("Give command of format ./compute sequence 1 | ./compute sequence 2 or 'quit' to exit: ");

    while (1)

    {

        fgets(s,sizeof(s),stdin);

 

        char *c1 = NULL;

        char *c2 = NULL;

 

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

 

        pid_t pid2 = fork();

        if(pid2 < 0){

            printf("pid fail");

            return 1;

        }

        if(pid2 == 0){

            printf("left hand side of pipe :%s \nright hand side of pipe :%s \n",c1,c2);

 

            int pipefd[2];

            if(pipe(pipefd) == -1){

                perror("Pipe Failed");

                return 1;

            }

            pid_t pid = fork();

 

            if(pid == -1){

                perror("Fork Failed");

                return 1;

            }

            if(pid == 0){

                close(pipefd[1]);

                dup2(pipefd[0],STDIN_FILENO);

                close(pipefd[0]);

 

                execlp("./compute_sequence_2","./compute_sequence_2",NULL);

                perror("execlp failed");

                exit(1);

            }

            

            else{

                close(pipefd[0]);

                dup2(pipefd[1],STDOUT_FILENO);

                close(pipefd[1]);

 

                execlp("./compute_sequence_1","./compute_sequence_1",NULL);

                perror("execlp failed");

                exit(1);

            }

        }

        else{

            while(wait(NULL)>0);

        }

    }

    return 0;

}
