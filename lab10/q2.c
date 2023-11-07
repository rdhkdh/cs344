#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<pthread.h>
#include<unistd.h>

//global variables shared by all threads
int sudoku[9][9] = {
    {6,2,4,5,3,9,1,8,7},
    {5,1,9,7,2,8,6,3,4},
    {8,3,7,6,1,4,2,9,5},
    {1,4,3,8,6,5,7,2,9},
    {9,5,8,2,4,7,3,6,1},
    {7,6,2,3,9,1,4,5,8},
    {3,7,1,9,5,6,8,4,2},
    {4,9,6,1,8,2,5,7,3},
    {2,8,5,4,7,3,9,1,6}
};
int valid[19] = {0};

struct parameters{
    int row;
    int col;
};
struct num{
    int n;
};

bool col_check(int grid[9][9], int c)
{
    int arr[10]={0};
    for(int i=0;i<9;i++)
    {
        arr[grid[i][c]] =1; 
    }

    int flag=1;
    for(int i=1;i<=9;i++) 
    {
        if(arr[i]==0) {flag=0; break;} //not present
        else{flag=1;}
    }

    if(flag==1) {return true;}
    
    return false;
}

bool row_check(int grid[9][9], int r)
{
    int arr[10]={0};
    for(int i=0;i<9;i++)
    {
        arr[grid[r][i]] =1; 
    }

    int flag=1;
    for(int i=1;i<=9;i++) 
    {
        if(arr[i]==0) {flag=0; break;} //not present
        else{flag=1;}
    }

    if(flag==1) {return true;}
    
    return false;
}

bool box_check(int grid[9][9], int r, int c)
{
    int arr[10]={0};
    int startRow = r - r % 3, startCol = c - c % 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            arr[ grid[i+startRow][j+startCol] ] = 1;
        }
    }

    int flag=1;
    for(int i=1;i<=9;i++) 
    {
        if(arr[i]==0) {flag=0; break;} //not present
        else{flag=1;}
    }

    if(flag==1) {return true;}
    
    return false;	
}

void* col1(void* arg)
{
    struct num* x= arg;
    
    int flag=1; //assume valid
    
    if(!col_check(sudoku, x->n)) {flag=0;}
    

    if(flag==0) { printf("Column %d checked. Sudoku solution not valid.\n", x->n); }
    else{
        valid[x->n] = 1;
        printf("Column %d checked- valid.\n", x->n);
    }

    pthread_exit(0);
}

void* row1(void* arg)
{
    struct num* x= arg;
    
    int flag=1; //assume valid
    if(!row_check(sudoku, x->n)) {flag=0;}
    

    if(flag==0) { printf("Row %d checked. Sudoku solution not valid.\n", x->n); }
    else{
        valid[x->n + 9] =1;
        printf("Row %d checked- valid.\n", x->n);
    }

    pthread_exit(0);
}

void* all_box()
{
    int flag=1;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(!box_check(sudoku, 2*i, 2*j)) {flag=0; break;}
        }
        if(flag==0) {break;}
    } 

    if(flag==0) { printf("All 9 Boxes checked. Sudoku solution not valid.\n"); }
    else{
        valid[18]=1;
        printf("All 9 Boxes checked- valid.\n");
    }

    pthread_exit(0);
}

int main()
{
    pthread_t tid[19]; /* the thread identifier */
    pthread_attr_t attr[19]; /* set of thread attributes */

    /* set the default attributes of the thread */
    for(int i=0;i<19;i++)
    {
        pthread_attr_init(&attr[i]);
    }
    
    /* Allocate memory for pthread_create() arguments */
    struct num* n_info = calloc(9, sizeof(struct parameters));
    /* create the thread */
    for(int i=0;i<9;i++)
    {
        n_info[i].n = i;
        pthread_create(&tid[i], &attr[i], col1, &n_info[i]);
    }
    //sleep(1);
    
    /* Allocate memory for pthread_create() arguments */
    struct num* r_info = calloc(9, sizeof(struct parameters));
    for(int i=0;i<9;i++)
    {
        r_info[i].n = i;
        pthread_create(&tid[i+9], &attr[i+9], row1, &r_info[i]);
    }
    //sleep(1);
   
    pthread_create(&tid[18], &attr[18], all_box, NULL);
    

    /* wait for the thread to exit */
    for(int i=0;i<19;i++)
    {
        pthread_join(tid[i],NULL);
    }

    int f=1;
    for(int i=0;i<19;i++) 
    {
        if(valid[i]==0) {f=0; break;}
    }

    if(f==0) {printf("\nSudoku solution invalid.\n");}
    if(f==1) {printf("\nSudoku solution valid.\n");}

    return 0;
}