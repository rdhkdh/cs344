//Lab 9 Q1, 210101088, Ridhiman Kaur Dhindsa.

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

//part a: sem wait function
void sem_wait(int semid, int numsem)//numsem- which sem to operate on: 0,1,2,3,4
{
    struct sembuf semp;
    semp.sem_num=numsem; 
    semp.sem_op=-1;
    semp.sem_flg=0;
    printf("Trying to lock...\n");
    semop(semid, &semp, 1);
    printf("Locked semaphore ID: %d\n",semid);
}

//part b: sem signal function
void sem_signal(int semid, int numsem)
{
    struct sembuf semp;
    semp.sem_num=numsem;
    semp.sem_op=1;
    semp.sem_flg=0;
    //printf("Trying to unlock...\n");
    semop(semid, &semp, 1);
    //printf("Unlocked semaphore ID: %d\n",semid);
}

struct db_table{
    char name[50];
    int pid[10];
};

int main()
{
    union semun arg1;
    struct db_table db[5]; //part c: declare 5 database tables

    key_t key1 = ftok("q1.c",'A'); //part d: obtain a key
    int semid = semget(key1, 5, 0666 | IPC_CREAT); //part e: declare 5 semaphores

    arg1.val = 1; // part f: initialize semaphores
    semctl(semid, 0, SETVAL, arg1);

    
    key_t key2 = ftok("q1.c",'B'); //obtain another key
    int shmid = shmget(key2, sizeof(db), 0666 | IPC_CREAT); //part g: get the shared memory object
    struct db_table* shmlink = (struct db_table*) shmat(shmid, (void *)0, 0); //part h: attach shared mem object

    struct db_table*  shm_orig = shmlink;
    //part i: initialize each table with table number
    for(int i=1;i<=5;i++)
    {
        sprintf(shmlink->name, "%d", i);
        for(int j=0;j<10;j++) 
        {  
            shmlink->pid[j] = -1;
        }
        shmlink++;
    }

    int flag=0;
    pid_t pid1;
    //part j: create 5 children
    for(int i=1;i<=5;i++)
    {
        pid1 = fork();
        if(pid1==0) //child process
        {
            key_t key1 = ftok("q1.c",'B'); //obtain key for shared memory
            int shmid = shmget(key1, sizeof(db), 0666 | IPC_CREAT); //get the shared memory object
            struct db_table* shmlink = (struct db_table*) shmat(shmid, (void *)0, 0); //attach shared mem object

            key_t key2 = ftok("q1.c",'A'); //obtain a key for the semaphores
            int semid = semget(key2, 5, 0666 | IPC_CREAT); //grab the semaphores

            int x= getpid(); //part k
            for(int r=1;r<=5;r++) //5 rounds
            {
                sem_wait(semid,(r-1+i-1)%5); // part k(i)
                sem_wait(semid,(r-1+i)%5);
                printf("Transaction %d holding the semaphore.\n",x); // part k(ii)
                struct db_table* ptr1 = shmlink+((r-1+i-1)%5);   
                struct db_table* ptr2 = shmlink+((r-1+i)%5); 
                printf("Transaction %d operating on table number: %s %s\n",x,ptr1->name,ptr2->name);
                for(int j=0;j<10;j++)      // part k(iii)
                {
                    if(ptr1->pid[j]==-1) 
                    { ptr1->pid[j] = x; break; }
                }
                for(int j=0;j<10;j++)
                {
                    if(ptr2->pid[j]==-1) 
                    { ptr2->pid[j] = x; break; }
                }

                sleep(10); // part k(iv)
                sem_signal(semid,(r-1+i-1)%5);
                sem_signal(semid,(r-1+i)%5);
            }
            break;
        }
        if(i==5) {flag=1;}
    }
    for(int i=0;i<5;i++) // parent waits for each of the 5 children
    { 
        wait(NULL);
    }
    
    if(flag==1 && pid1>0)
    {
        struct db_table* itr; //part l
        for(int i=0;i<5;i++)
        {
            itr = shm_orig+i;
            printf("\n\nTable no. %s\n",itr->name);
            for(int j=0;j<10;j++)
            {
                printf("%d ",itr->pid[j]);
            }
        }
        printf("\n");
    }


    return 0;
}