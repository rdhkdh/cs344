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

void sem_signal(int semid, int numsem){
    struct sembuf semp;
    semp.sem_num=numsem;
    semp.sem_op=1;
    semp.sem_flg=0;
    printf("Trying to unlock...\n");
    semop(semid, &semp, 1);
    printf("Unlocked semaphore ID: %d\n",semid);
}

//part e: reader program
int main()
{
    union semun arg1, arg2, dummy;

    key_t key2 = ftok("repository.c",'B'); //obtain a key
    key_t key3 = ftok("repository.c",'C');

    //declare appropriate semaphores
    int semid_rwmutex = semget(key2, 1, 0666 | IPC_CREAT); //for readers and writers
    int semid_mutex = semget(key3, 1, 0666 | IPC_CREAT); //for modifying read_count

    arg1.val = 1; //initialize semaphores
    semctl(semid_rwmutex, 0, SETVAL, arg1);
    arg2.val = 1;
    semctl(semid_mutex, 0, SETVAL, arg2);

    key_t key1 = ftok("repository.c",'A'); //obtain another key
    int shmid = shmget(key1, 256, 0666 | IPC_CREAT); //get the shared memory object
    char* shmlink = (char*) shmat(shmid, (void *)0, 0); //attach shared mem object

    //---------------reader part------------
    int read_count=0;
    sem_wait(semid_mutex, 1); //obtain access to modify read_count
    read_count++; 
    if (read_count == 1) //first reader
    { sem_wait(semid_rwmutex, 1); }
    sem_signal(semid_mutex, 1); //release read_count
    
    // reading is performed 
    sleep(10);
    printf("Shared memory accessed for reading. Current read count= %d\n",read_count);
    printf("\n%s\n\n",shmlink);
    
    sem_wait(semid_mutex, 1); //obtain access to modify read_count
    read_count--; 
    if (read_count == 0) //last reader
    { sem_signal(semid_rwmutex, 1); }
    sem_signal(semid_mutex, 1); //release read_count
    
    //--------------------------------------

    semctl(semid_rwmutex, 0, IPC_RMID, dummy); //remove semaphores
    semctl(semid_mutex, 0, IPC_RMID, dummy);
    shmdt(shmlink); //detach shared memory object

    return 0;
}