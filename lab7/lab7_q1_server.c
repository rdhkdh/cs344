/*NOTE: run seminit.c first, to create the semaphore set
then run q1_client.c to generate requests
then run q1_server.c to serve requests */

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
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

int main() //web server program
{
    key_t mem_key = ftok("q1_server.c",65); //key for shared memory
    int shmid = shmget(mem_key,SHM_SIZE,0666|IPC_CREAT);
    int* data = shmat(shmid, (void *)0, 0); // attach to the segment to get a pointer to it
    if (data == (int *)(-1)) { perror("shmat"); exit(1); }

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

    int req_served=0; //no of requests served

    //fork child process 1
    pid_t pid1;
    pid1 = fork(); 
    if(pid1==0) //child 1
    {
        int *arr1 = (int*) shmat(shmid,(void*)0,0); //ptr attached to shared mem seg of size 25
        int* head1 = arr1;
        int x = sem_wait(semid); //obtain lock on shared mem
        while(req_served!=100)
        {
            if(x==1)
            { 
                //dequeue http request, print http request number
                req_served++;
                printf("HTTP request served: %d\n",*arr1);
                *arr1=0; //mark that request as served
                while(*arr1==0) {arr1++;} //go to next location
                //implement circular queue operations
               
                int y = sem_signal(semid); //release lock on shared mem
                if(y==-1) { printf("sem_signal: error in unlocking.\n"); exit(EXIT_FAILURE); }
            }
            else{
                //x==-1, do nothing- wait
            }
        }
        shmdt(arr1); //detach from shared memory
    }
    if(pid1>0)
    { //parent
        wait(NULL);
    }

    //fork child process 2
    pid_t pid2;
    pid2 = fork(); 
    if(pid2==0)
    {
        int *arr2 = (int*) shmat(shmid,(void*)0,0); //ptr attached to shared mem seg of size 25
        int* head2 = arr2;
        int x2 = sem_wait(semid);
        while(req_served!=100)
        {
            if(x2==1)
            { 
                //dequeue http request, print http request number
                req_served++;
                printf("HTTP request served: %d\n",*arr2);
                *arr2=0; //mark that request as served 
                while(*arr2==0) {arr2++;} //go to next location
                //implement circular queue operations

                //release shared mem
                int y2 = sem_signal(semid);
                if(y2==-1) { printf("sem_signal: error in unlocking.\n"); exit(EXIT_FAILURE); }
            }
            else{
                //x2==-1, do nothing- wait
            }
        }
        shmdt(arr2); //detach from shared memory
    }
    if(pid2>0)
    { //parent
        wait(NULL);
    }

    /* remove the semaphore */
    if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        perror("semctl");
        exit(1);
    }

    shmdt(data); //detach shared memory for web server program (main)
    shmctl(shmid,IPC_RMID,NULL); //remove shared memory

    return 0;
}