#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>

int j;
int _chlids[5];


void sigCather(int sig_num)
{
    signal(SIGTERM,sigCather);
    printf("PID %d is caught one\n",getpid());
    if(j >-1){
        kill(_chlids[j],SIGTERM);
    }
}

int main() 
{ 
pid_t pid;
int zombi;
int state;

signal(SIGTERM, sigCather);

for (int i = 0; i < 5; i++)
{
    pid = fork();
    if(pid == 0){
    //chlid Process
    printf("PID %d is ready \n", getpid());
    j = i-1;
    pause(); // wait for singal
    exit(0); // become a zombi
    }
    // Parent Process cllect the chlids.
    _chlids[i] = pid;
}
    sleep(1);
    kill(_chlids[4],SIGTERM);
    sleep(1);
  
    for (size_t i = 0; i < 5; i++)
    {
        zombi = wait(&state);
        printf("process %d is dead\n",zombi);
        kill(zombi,SIGKILL);
    }


    return 0;
}
