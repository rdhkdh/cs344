#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<pthread.h>

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
int valid[11]={0};

struct parameters{
    int row;
    int col;
    int box_no;
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

void* all_col()
{
    int flag=1; //assume valid
    for(int i=0;i<9;i++)
    {
        if(!col_check(sudoku, i)) {flag=0; break;}
    }

    if(flag==0) { printf("Columns checked. Sudoku solution not valid.\n"); }
    else{
        valid[0]=1;
        printf("Sudoku solution columns checked- valid.\n");
    }

    pthread_exit(0);
}

void* all_row()
{
    int flag=1; //assume valid
    for(int i=0;i<9;i++)
    {
        if(!row_check(sudoku, i)) {flag=0; break;}
    }

    if(flag==0) { printf("Rows checked. Sudoku solution not valid.\n"); }
    else{
        valid[1]=1;
        printf("Sudoku solution rows checked- valid.\n");
    }

    pthread_exit(0);
}

void* all_box(void* arg)
{
    struct parameters* x= arg;

    int flag=1;
    if(!box_check(sudoku, x->row, x->col)) {flag=0;}
     

    if(flag==0) { printf("Box %d checked. Sudoku solution not valid.\n", x->box_no); }
    else{
        valid[x->box_no] =1;
        printf("Box %d checked- valid.\n", x->box_no-2);
    }

    pthread_exit(0);
}

int main()
{
    pthread_t tid[11]; /* the thread identifier */
    pthread_attr_t attr[11]; /* set of thread attributes */

    /* set the default attributes of the thread */
    for(int i=0;i<11;i++)
    {
        pthread_attr_init(&attr[i]);
    }
    

    /* create the thread */
    pthread_create(&tid[0], &attr[0], all_col, NULL);
    pthread_create(&tid[1], &attr[1], all_row, NULL);

    /* Allocate memory for pthread_create() arguments */
    struct parameters* tinfo = calloc(9, sizeof(struct parameters));
    int m=0;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            tinfo[m].row = 2*i;
            tinfo[m].col = 2*j;
            tinfo[m].box_no = m+2;
            m++;
        }
    }    

    
    for(int k=2;k<=10;k++) //9 threads
    {
        pthread_create(&tid[k], &attr[k], all_box, &tinfo[k-2]);
    }

    /* wait for the thread to exit */
    for(int i=0;i<11;i++)
    {
        pthread_join(tid[i],NULL);
    }

    int f=1;
    for(int i=0;i<11;i++) 
    {
        if(valid[i]==0) {f=0; break;}
    }

    if(f==0) {printf("\nSudoku solution invalid.\n");}
    if(f==1) {printf("\nSudoku solution valid.\n");}
    
    return 0;
}