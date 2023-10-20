#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<sys/wait.h>
#include<string.h>
#include<math.h>
#include <unistd.h>

struct my_msgbuf { //create a struct for message passing
    long mtype;
    int row_no;
    int col_no;
};

double child_calc(int img_subset[3][3],int Kx[3][3],int Ky[3][3])
{
    int gx=0,gy=0;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            gx += (img_subset[i][j]*Kx[i][j]);
            gy += (img_subset[i][j]*Ky[i][j]);
        }
    }
    int x = (gx*gx)+(gy*gy);
    if(x>=255*255 || x<0) {x=255*255;}
    double G = sqrt(x);
    return G;
}

int main()
{
    FILE* fp= fopen("ML2-input.txt","r");
    int Kx_size[2], Ky_size[2], img_size[2];
    int Kx[3][3], Ky[3][3], img[256][256];

    fscanf(fp,"%d %d",&Kx_size[0],&Kx_size[1]);
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            fscanf(fp,"%d",&Kx[i][j]);
        }
    }

    fscanf(fp,"%d %d",&Ky_size[0],&Ky_size[1]);
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            fscanf(fp,"%d",&Ky[i][j]);
        }
    }

    fscanf(fp,"%d %d",&img_size[0],&img_size[1]);
    for(int i=0;i<256;i++)
    {
        for(int j=0;j<256;j++)
        {
            fscanf(fp,"%d",&img[i][j]);
        }
    }
    fclose(fp);

    struct my_msgbuf buf;
    int msqid;
    key_t key;

    //obtain a key
    key = ftok("sender.c", 65); // you can use any existing pathname for ftok. 
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    /* connect to the queue. Note: The queue must already exist, else receiver.c gives error. */
    if ((msqid = msgget(key, 0644)) == -1) { 
        perror("msgget");
        exit(1);
    }
    printf("q1b Receiver: ready to receive messages.\n");
    
    int no_of_children=0;
    FILE* fp1 = fopen("q1b_output.txt","w");
    for(int i=1;i<=256*256;i++)
    {
        if (msgrcv(msqid, (struct msgbuf *)&buf, sizeof(buf), 0, 0) == -1) { //here mtype=0 means next msg in queue is read
            perror("msgrcv");
            exit(1);
        }
        printf("q1b Receiver: %d %d\n", buf.row_no, buf.col_no);
        int img_subset[3][3];
        img_subset[0][0] = img[buf.row_no-1][buf.col_no-1];
        img_subset[0][1] = img[buf.row_no-1][buf.col_no];
        img_subset[0][2] = img[buf.row_no-1][buf.col_no+1];
        img_subset[1][0] = img[buf.row_no][buf.col_no-1];
        img_subset[1][1] = img[buf.row_no][buf.col_no];
        img_subset[1][2] = img[buf.row_no][buf.col_no+1];
        img_subset[2][0] = img[buf.row_no+1][buf.col_no-1];
        img_subset[2][1] = img[buf.row_no+1][buf.col_no];
        img_subset[2][2] = img[buf.row_no+1][buf.col_no+1];
        
        pid_t pid;
        pid = fork(); //fork a child process
        no_of_children++;
        if(pid>0)
        { //parent process
            if(no_of_children>=8) { wait(NULL); }
        }
        if(pid==0)
        { // child process
            double edge_value = child_calc(img_subset, Kx, Ky);
            fprintf(fp1,"%d %d %lf\n",buf.row_no, buf.col_no, edge_value);
            exit(EXIT_SUCCESS);
        }
        
    }
    fclose(fp1);
}