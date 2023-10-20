/*NOTE: run seminit.c first, to create the semaphore set
then run q1_client.c to generate requests
then run q1_server.c to serve requests */

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#define SHM_SIZE 25

union semun {
    int val; /* used for SETVAL only */
    struct semid_ds *buf; /* for IPC_STAT and IPC_SET */
    unsigned short *array; /* used for GETALL and SETALL */
};

int sem_wait(int semid) // set to allocate a resource 
{
    struct sembuf sb = {0, -1, 0}; 
    printf("Trying to lock...\n");
    if (semop(semid, &sb, 1) == -1) 
    {
        perror("semop");
        return -1; //failed to lock
    }
    printf("Locked semaphore ID: %d\n",semid);
    return 1; //success in locking
}

int sem_signal(int semid) // set to release a resource 
{
    struct sembuf sb = {0, 1, 0}; // sem_op=1 frees the resource here
    printf("Trying to unlock...\n");
    if (semop(semid, &sb, 1) == -1) 
    {
        perror("semop");
        return -1; //failed to unlock
    }
    printf("Unlocked semaphore ID: %d\n",semid);
    return 1; //success in unlocking
}

int main()
{
    key_t sem_key;
    int semid; //key for semaphore
    union semun arg;
    if ((sem_key = ftok("q1_client.c", 'J')) == -1) {
        perror("ftok");
        exit(1);
    }
    /* grab the semaphore set created by web server */
    if ((semid = semget(sem_key, 1, 0)) == -1) {
        perror("semget");
        exit(1);
    }
    arg.val = 1; /* initialize semaphore #0 to 1: */

    key_t mem_key = ftok("q1_server.c",65); //key for shared memory
    int shmid = shmget(mem_key,SHM_SIZE,0666|IPC_CREAT);
    int* data = shmat(shmid, (void *)0, 0); // attach to the segment to get a pointer to it
    if (data == (int *)(-1)) { perror("shmat"); exit(1); }

    //generate 100 HTTP requests
    for(int i=1;i<=100;i++)
    {
        int x = sem_wait(semid); //obtain lock on shared mem
        if(x==1)
        {
            //enqueue http request
            printf("writing to segment: HTTP request no.%d\n", i);
            *data = i;
            data++;
            //implement circular queue operations
            
            int y = sem_signal(semid); //release lock on shared memory
            if(y==-1) { printf("sem_signal: error in unlocking.\n"); exit(EXIT_FAILURE); }
        }
        else{
            //x==-1, do nothing- wait
        }
        sleep(1);
    }
    sleep(10);

    /* remove the semaphore */
    if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    shmdt(data); //detach shared memory for main client program 

    return 0;
}