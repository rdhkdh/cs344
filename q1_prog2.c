#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/types.h>
#include<string.h>
#include<stdlib.h>

int main()
{
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key,90000,0666|IPC_CREAT);
    char *str = (char*) shmat(shmid,(void*)0,0);

    //------------------------parsing data---------------------------------------
    int no_of_lines= atoi(str);
    str += (strlen(str)+1);


    char* batsmen[7] = {"Gambhir","Sehwag","Pujara","Tendulkar","Kohli","Raina","Dhoni"};
    int balls[7]={0};
    int runs[7]={0};
    int out[7]={0}; //out=1-> b and print bowler, not out=0-> *
    char* out_bowler[7];

    char bowler[10],batsman[10],run_status[10];

    while(no_of_lines>0)
    {
        printf("%s",str);
        int len = strlen(str);
        int space_index=0,comma_index=0;
        int j=0,k=0,k1=0;
        for(int i=0;i<len;i++)
        {
            if(str[i]==' ') { space_index++; if(space_index<=4) {continue;} }
            if(str[i]==',') {comma_index++; continue;} //comma1= run, comma2= comment
            
            if(space_index==1) { bowler[j++] = str[i]; }
            bowler[j]='\0';
            if(space_index==3) { batsman[k++]= str[i]; }
            batsman[k]='\0';
            if(comma_index==1) { run_status[k1++] = str[i]; }
            run_status[k1]='\0';
            
        }
        printf("bowler: %s\n",bowler);
        printf("batsman: %s\n",batsman);
        printf("run_status: %s\n\n",run_status);

        for(int i=0;i<7;i++)
        {
            if( strcmp(batsmen[i],batsman)==0 )
            {   
                balls[i]++;
                if( strcmp(run_status,"FOUR")==0 ) {runs[i]+=4;}
                else if( strcmp(run_status,"SIX")==0 ) {runs[i]+=6;}
                else if( strcmp(run_status,"OUT")==0 ) {out[i]=1; out_bowler[i]=bowler;}
                else if( strcmp(run_status,"no run")==0 ) {}
                else if( strcmp(run_status,"1 run")==0 ) {runs[i]+=1;}
                else if( strcmp(run_status,"2 runs")==0 ) {runs[i]+=2;}
                else //bye or leg bye or wide
                {
                    for(int i2=0;i2<strlen(run_status);i2++)
                    {
                        int flag=0; //space index
                        int y = ( (int)run_status[0] ) - 48; //no of runs or byes

                        if(run_status[i2]==' ') {flag=1; continue;}
                        // if(flag==1 && run_status[i2]=='r') {runs[i]+=y; break;} // y runs

                        // y byes or leg byes-> no run, ball counted
                        if(flag==1 && run_status[i2]=='w') {balls[i]--; break;} //wide= means ball not counted
                    }
                }

                break; // do not search list of batsmen further
            }
        }

        str += strlen(str)+1;
        no_of_lines--;
    }

    printf("---------------------report-----------------------------\n");
    for(int i=0;i<7;i++)
    {
        if(out[i]==0)
        { printf("%s * %d %d\n",batsmen[i],runs[i],balls[i]); }
        if(out[i]==1)
        {
            printf("%s b %s %d %d\n",batsmen[i],out_bowler[i],runs[i],balls[i]);
        }
    }

    shmdt(str); //detach
    shmctl(shmid,IPC_RMID,NULL); //destroy shared memory
}