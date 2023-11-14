#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#define N 400

int A[N][N]; // global matrix 1, 2 creation
int B[N][N];
int f[N][N]; //final result

pthread_mutex_t mutexsum;

struct parameters
{
    int start;
    int end;
};

void* matrix_mult(void* arg)
{
    struct parameters* x= arg;
    int sum;

    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            sum=0;
            for(int k=x->start; k<=x->end; k++)
            {
                sum += (A[i][k]*B[k][j]);
            }

            /*
            Lock a mutex prior to updating the value in the shared
            structure, and unlock it upon updating.
            */
            pthread_mutex_lock(&mutexsum); //thread synchronization via mutex
            f[i][j] += sum; //add all 4 temp matrices
            pthread_mutex_unlock(&mutexsum);
        }
    }

    pthread_exit(0);
}

int main()
{
    //matrix 1, 2 initialization
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            //srand(i+j); //can change the seed
            A[i][j] = rand() %100; //2 digit nos
            B[i][j] = rand() %100;
            f[i][j] = 0;
        }
    }

    pthread_mutex_init(&mutexsum, NULL);
    pthread_t tid[4];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    /* Allocate memory for pthread_create() arguments */
    struct parameters* tinfo = calloc(4, sizeof(struct parameters));
    for(int i=0;i<4;i++)
    {
        tinfo[i].start = 100*i;
        tinfo[i].end = (100*i) + 99; 
    }  

    // create 4 threads
    for(int k=0;k<4;k++)
    {
        pthread_create(&tid[k], &attr, matrix_mult, &tinfo[k]);
    }

    /* wait for the thread to exit */
    for(int i=0;i<4;i++)
    {
        pthread_join(tid[i],NULL);
    }

    FILE* fp = fopen("lab11_op.txt","w");
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            fprintf(fp,"%d ", f[i][j]);
        }
        fprintf(fp,"\n");
    }
    
    fclose(fp);

    pthread_mutex_destroy(&mutexsum);
    pthread_attr_destroy(&attr);
    return 0;
}