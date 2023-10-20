/*NOTE: run seminit.c first, to create the semaphore set
then run q1_client.c to generate requests
then run q1_server.c to serve requests */

#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun
{
    int val;               /* used for SETVAL only */
    struct semid_ds *buf;  /* for IPC_STAT and IPC_SET */
    unsigned short *array; /* used for GETALL and SETALL */
};

int main(void)
{
    key_t key;
    int semid;
    union semun arg;
    if ((key = ftok("q1_client.c", 'J')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    /* create a semaphore set with 1 semaphore: */
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1)
    {
        perror("semget");
        exit(1);
    }
    /* initialize semaphore #0 to 1: */
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("semctl");
        exit(1);
    }
}