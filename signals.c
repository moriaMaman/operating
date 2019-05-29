// C program to illustrate 
// pipe system call in C 
// shared by Parent and Child 
// #include <stdio.h> 
// #include <string.h>
// #include <stdlib.h>
// #include <unistd.h> 
// #include <sys/types.h>
// #include <sys/wait.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include "md5.h"

char  secondBuf[6000];
int str_len=2;

using namespace std;


void hendler(int signum){
    str_len=strlen(secondBuf);
    if(str_len==32){
       printf("encrypted by process %d: %s\n",getpid(),secondBuf);
       kill(0,SIGKILL);
       exit(0);
    }
    else{
        exit(1);
    }
}

int main() 
{ 


    char  parentBuf[100];
    char  firstBuf[300];
    string md5Ans;
    int myPipeFd1[2], pid, nbytes,myPipeFd2[2];

    
    
    if (pipe(myPipeFd1) < 0){//בדיקה האם הפייפים נוצרו כמו שצריך
        perror("first pipe failed");
        exit(1); 
    }
   if (pipe(myPipeFd2) < 0){//בדיקה האם הפייפים נוצרו כמו שצריך
        perror("second pipe ailed");
        exit(1); 
    }
  
    /* continued */
    pid=fork();
    if(pid<0){//בדיקה האם התהליך נוצר כמו שצריך
        perror("fork failed");
        exit(1);
    }

    if (pid > 0) {//בדיקה האם התהליך הוא האבא או הבן
      /*parent process*/
      printf("plain input: ");
      scanf("%s",parentBuf);
      close(myPipeFd1[0]);
      write(myPipeFd1[1],parentBuf,strlen(parentBuf)+1);
      sleep (1);
      close(myPipeFd2[1]);
      read(myPipeFd2[0],secondBuf,128);
      str_len=strlen(secondBuf);

      kill(pid,SIGINT);
      hendler(SIGINT);
      exit(0);
    } 
  
    else { 
        /*child process*/
        wait(NULL);
        close(myPipeFd1[1]);
        read(myPipeFd1[0],firstBuf,20);
        md5Ans= md5(firstBuf);
        char newChar[md5Ans.length()+1];
        strcpy(newChar,md5Ans.c_str());
        write(myPipeFd2[1],newChar,strlen(newChar)+1);
        close(myPipeFd2[0]);
        signal(SIGINT,hendler);
        wait(NULL);
        } 
    return 0; 
}

