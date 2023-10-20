// sends message on message queue
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<string.h>
#include<unistd.h>

struct my_msgbuf { //create a struct for message passing
    long mtype;
    int row_no;
    int col_no;
};

int main(void)
{
    struct my_msgbuf buf;
    int msqid;
    key_t key;

    //obtain a key
    key = ftok("sender.c", 65); // you can use any existing pathname for ftok. 
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    //create a msg queue
    msqid = msgget(key, 0644 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(1);
    }
    
    printf("Sending row-column pairs...\n");

    buf.mtype = 1; /* we don't really care in this case */
    for(int i=1;i<=256;i++) 
    {
        for(int j=1;j<=256;j++)
        {
            buf.row_no = i;
            buf.col_no = j;
            if (msgsnd(msqid, (struct msgbuf *)&buf, sizeof(buf), 0) == -1)
            { perror("msgsnd"); }
        }
    }

    sleep(5);
    if (msgctl(msqid, IPC_RMID, NULL) == -1) //delete message queue
    {
        perror("msgctl");
        exit(1);
    }

    return 0;
} 
