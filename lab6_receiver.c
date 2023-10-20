// receives message from message queue
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf { //create a struct for message passing
    long mtype;
    char mtext[200];
};

int main(void)
{
    struct my_msgbuf buf;
    int msqid;
    key_t key;

    if ((key = ftok("sender.c", 'B')) == -1) {  /* same key as sender.c */
        perror("ftok");
        exit(1);
    }

    /* connect to the queue. Note: The queue must already exist, else receiver.c gives error. */
    if ((msqid = msgget(key, 0644)) == -1) { 
        perror("msgget");
        exit(1);
    }
    
    printf("Receiver: ready to receive messages.\n");

    for(;;) { /* receiver program never quits! */
        if (msgrcv(msqid, (struct msgbuf *)&buf, sizeof(buf), 0, 0) == -1) { //here mtype=0 means next msg in queue is read
            perror("msgrcv");
            exit(1);
        }
        printf("Receiver: %s\n", buf.mtext);
    }

    return 0;
}