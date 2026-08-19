#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
   pid_t pid2,pid3;
   pid2=fork();
   if(pid2<0)
   {
      perror("Process creation pid2 failed");
   }
   if (pid2==0)
   {
      printf("p2 PID: %d, PARENT PID(p2) : %d\n",getpid(),getppid());
      pid3=fork();
      if(pid3==0)
      {
         printf("p3 PID: %d, PARENT PID(p3) : %d\n",getpid(),getppid());
      }
      else
      {
         wait(NULL);
      }
   }
   else
   {

      wait(NULL);
      printf("p1 PID:%d\n",getpid());
   }
   return 0;
}

