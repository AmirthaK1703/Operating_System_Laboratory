#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
   pid_t p2,p3,p4,p5,p6,p7;
   p2=fork();
   if(p2<0)
   {
      perror("process creation failed");
   }
   else if(p2==0)
   {
      printf("P2 pid:%d,parent pid:%d\n",getpid(),getppid());
   }
   else
   {
      wait(NULL);
      p3=fork();
      if(p3==0)
      {
         printf("P3 pid:%d,parent pid:%d\n",getpid(),getppid());
         p6=fork();
         if(p6==0)
         {
            printf("P6 pid:%d,parent pid:%d\n",getpid(),getppid());
         }
         else{
            wait(NULL);
            p7=fork();
            if(p7==0)
            {
               printf("P7 pid:%d,parent pid:%d\n",getpid(),getppid());
            }
            else
            {
              // wait(NULL);
            }
         }
      }
      else
      {
         wait(NULL);
         p4=fork();
         if(p4==0)
         {
            printf("P4 pid:%d,parent pid:%d\n",getpid(),getppid());
         }
         else
         {
            wait(NULL);
            p5=fork();
            if(p5==0)
            {
               printf("P5 pid:%d,parent pid:%d\n",getpid(),getppid());
            }
            else
            {
               wait(NULL);
               printf("P1 pid:%d\n",getpid());
            }
         }
      }
   }
   return 0;
}
