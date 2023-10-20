#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>

//part c: implement repositry program
int main()
{
    key_t key1= ftok("repository.c",'A');
    int shmid;
    shmid = shmget(key1, 1024, 0666 | IPC_CREAT); //change size accordingly

    char* shmlink = (char*) shmat(shmid, (void *)0, 0);

    //initialize shared memory with Hello world C program- 'critical section'
    sprintf(shmlink,"#include<stdio.h>\nint main() {printf(\"Hello World\");}");


    return 0;
}