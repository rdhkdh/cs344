#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

union semun { //create union semun
    int val; 
    struct semid_ds *buf; 
    short *array;
};

//part a: implement sem_wait function
void sem_wait(int semid, int numsem){
    struct sembuf semp;
    semp.sem_num=numsem;
    semp.sem_op=-1;
    semp.sem_flg=0;
    printf("Trying to lock...\n");
    semop(semid, &semp, 1);
    printf("Locked semaphore ID: %d\n",semid);
}
// int sem_wait(int semid, int numsem){
//     struct sembuf semp;
//     semp.sem_num=numsem;
//     semp.sem_op=-1;
//     semp.sem_flg=0;
//     printf("Trying to lock...\n");
//     int x = semop(semid, &semp, 1);
//     if(x==-1) 
//     {
//         //unable to lock
//         perror("semop");
//         return -1;
//     }
//     printf("Locked semaphore ID: %d\n",semid);
//     return 1;
// }

// part b: implement sem_signal function
void sem_signal(int semid, int numsem){
    struct sembuf semp;
    semp.sem_num=numsem;
    semp.sem_op=1;
    semp.sem_flg=0;
    printf("Trying to unlock...\n");
    semop(semid, &semp, 1);
    printf("Unlocked semaphore ID: %d\n",semid);
}

int main()
{
    union semun arg1, arg2;

    //part d: implement writer program
    key_t key1 = ftok("repository.c",'A'); //obtain a key
    int shmid = shmget(key1, 256, 0666 | IPC_CREAT); //get the shared memory object
    char* shmlink = (char*) shmat(shmid, (void *)0, 0); //attach shared mem object

    key_t key2 = ftok("repository.c",'B'); //obtain another key
    key_t key3 = ftok("repository.c",'C');

    //declare appropriate semaphores
    int semid_rwmutex = semget(key2, 1, 0666 | IPC_CREAT); //for readers and writers
    int semid_mutex = semget(key3, 1, 0666 | IPC_CREAT); //for modifying read_count

    arg1.val = 1; //initialize semaphores
    semctl(semid_rwmutex, 0, SETVAL, arg1);
    arg2.val = 1;
    semctl(semid_mutex, 0, SETVAL, arg2);

    //----------writer part------------------
    
    sem_wait(semid_rwmutex, 1); //perform appropriate semaphore operations- request allocation
    printf("Shared memory accessed for writing.\n");
    
    // writing is performed
    shmlink += (strlen(shmlink)); 
    int pid = getpid();
    sprintf(shmlink, "\nHello written by pid: %d",pid);
    sleep(5);

    sem_signal(semid_rwmutex, 1); //perform appropriate semaphore operations- release shared resource
    printf("Shared memory released from writing.\n");

    //------------------------------------------

    shmdt(shmlink); //detach shared memory

    return 0;
}