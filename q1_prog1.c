// writer
#include<stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include<string.h>
#include <sys/shm.h>
#include<stdlib.h>
#include <unistd.h>
#define SHM_SIZE 1024

int main()
{
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key,90000,0666|IPC_CREAT);
    char *str = (char*) shmat(shmid,(void*)0,0);

    FILE* fp = fopen("week05-ML2-input.txt","r");
    char line[1024];
    while ( fgets(line, sizeof(line), fp) != NULL) 
    {
        printf("writing to segment: %s\n", line);
        strncpy(str, line, sizeof(line));
        str += (strlen(str) + 1);
    }

    shmdt(str);
}