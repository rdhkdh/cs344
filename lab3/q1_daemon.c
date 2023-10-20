#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<syslog.h>
#include<fcntl.h>
#include<sys/resource.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<signal.h>

void start_daemon(const char *cmd) //pass the name of program to cmd
{
    umask(0); // clear file creation mask

    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    /*
    * Get maximum number of file descriptors.
    */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
    { printf("%s: can't get file limit", cmd); exit(EXIT_FAILURE); }
    /*
    * Become a session leader to lose controlling TTY.
    */
    if ((pid = fork()) < 0) { printf("%s: can't fork", cmd); exit(EXIT_FAILURE); }
    else if (pid != 0) { exit(0); }

    setsid(); //in child
    /*
    * Ensure future opens won’t allocate controlling TTYs.
    */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) { printf("%s: can't ignore SIGHUP", cmd); exit(EXIT_FAILURE);}
    if ((pid = fork()) < 0) {printf("%s: can't fork", cmd); exit(EXIT_FAILURE);}
    else if (pid != 0) { exit(0); } //parent


    // Change the current working directory to the root so we won’t prevent file systems from being unmounted. 
    if (chdir("/") < 0) { printf("Can't change directory to /\n"); exit(EXIT_FAILURE);}
    
    //close all open file descriptors
    if (rl.rlim_max == RLIM_INFINITY)
    { rl.rlim_max = 1024; }
    for (i = 0; i < rl.rlim_max; i++)
    { close(i); }

    // Attach file descriptors 0, 1, and 2 to /dev/null.
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    //initialize logfile
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2); //facility inherited from openlog fn
        exit(1);
    }
    //write a msg of succesful creation
    syslog(LOG_INFO,"Daemon created successfully"); //facility inherited from openlog fn

    // storing the daemon pid
    int daemon_pid = getpid();
    FILE* fp= fopen("daemon_pid_info.txt","w");
    fprintf(fp,"%d",daemon_pid); 
    fclose(fp);

    //proceed with the rest of the daemon
    while(1)
    {
        sleep(20); //wait for 20 seconds before creating child
        pid_t pid1;
        pid1 = fork(); //fork a child process

        if(pid1 == 0) 
        { // child process created
            int n= getpid();
            while(n!=1)
            {
                printf("%d ",n);
                syslog(LOG_ERR,"%d\n",n); //logging the sequence to log file
                if(n%2==0) { n=n/2; }
                else { n= (3*n)+1; }
            }
            printf("\n");
            syslog(LOG_NOTICE,"child completed");
            _exit(1);
        }
        
    }
}

void stop_daemon()
{
    //first get the daemon pid from file
    int daemon_pid=-1;
    FILE* fp1= fopen("daemon_pid_info.txt","r");
    fscanf(fp1,"%d", &daemon_pid);
    fclose(fp1);

    pid_t x= daemon_pid;
    kill(x, SIGTERM);

    syslog(LOG_NOTICE, "Daemon process stopped");
}

int main(int argc, char* argv[])
{
    if(argc!=2) {printf("Too few/too many arguments.\n"); return 0;}

    
    if( strcmp(argv[1],"start")==0 )
    {

        //Become a daemon.
        start_daemon("RKDs daemon"); 

    }

    if( strcmp(argv[1],"stop")==0 )
    {   //shutdown daemon
        stop_daemon(); 
    }

    return 0;
}