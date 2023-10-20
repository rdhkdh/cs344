// sends message on message queue
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<string.h>

struct my_msgbuf { //create a struct for message passing
    long mtype;
    char mtext[200];
};

int main(void)
{
    struct my_msgbuf buf;
    int msqid;
    key_t key;

    key = ftok("sender.c", 'B'); // you can use any existing pathname for ftok. 
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    msqid = msgget(key, 0644 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget");
        exit(1);
    }
    
    printf("Enter lines of text, ^D to quit:\n");

    buf.mtype = 1; /* we don't really care in this case */
    while(fgets(buf.mtext,sizeof(buf.mtext),stdin), !feof(stdin)) 
    {
        char* newline = strchr(buf.mtext, '\n');
        if (newline != NULL) { *newline = '\0'; }

        if (msgsnd(msqid, (struct msgbuf *)&buf, sizeof(buf), 0) == -1)
        { perror("msgsnd"); }
    }

    if (msgctl(msqid, IPC_RMID, NULL) == -1) //delete message queue
    {
        perror("msgctl");
        exit(1);
    }

    return 0;
} 
// if multiple receivers are there, they're served in Round robin fashion
// if one sender quits, all receivers quit. But rest of the senders continue running.