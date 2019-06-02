#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h>

int brothers[5];
int befor=0;

void sigCathcher(int sigNum){  
   
    printf("PID %d caught one\n",getpid());
     if(befor>=0){
        kill(brothers[befor],SIGTERM);
     } 
     exit(0);  
}

int main() 
{ 
    for(int i=0;i<5;i++) // loop will run n times (n=5) 
    { 
        int pid= fork();
        // printf("PID %d\n",pid);
        if(pid == 0) 
        { 
            // printf("PID %d\n",pid);
            printf("PID %d is ready\n",getpid());
            befor=i-1;
            signal(SIGTERM,sigCathcher);
            pause();
        } 
         brothers[i]=pid;
    } 

    sleep(1);
    kill(brothers[4],SIGTERM);

    for(int i=0;i<5;i++){ // loop will run n times (n=5)
        sleep(1);
        printf("process %d is dead\n",brothers[i]);
        kill(brothers[i],SIGKILL);
    }
} 