#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
int main()
{
   pid_t p2,p3,p4,p6,p7,p8;
   p2=fork();
   if(p2<0)
   {
      perror("Process creation failed\n");
   }
   else if(p2==0)
   {
      printf("p2 Process ID :%d PARENT ID :%d\n",getpid(),getppid());
      p4=fork();
      if(p4==0)
      {
         printf("p4 PROCESS ID : %d PARENT ID :%d\n",getpid(),getppid());
         p6=fork();
         if(p6==0)
         {
            printf("p6 PROCESS ID :%d PARENT ID:%d\n",getpid(),getppid());
         }
         else
         {
            wait(NULL);
         }
      }
      else
      {
         wait(NULL);
      }
   }
   else
   {
      wait(NULL);
      p3=fork();
      if(p3==0)
      {
         printf("p3 PROCESS ID :%d PARENT ID:%d\n",getpid(),getppid());
         p7=fork();
         if(p7==0)
         {
            printf("p7 PROCESS ID:%d PARENT ID:%d\n",getpid(),getppid());
         }
         else
         {
            wait(NULL);
            p8=fork();
            if(p8==0)
            {
               printf("p8 PROCESS ID :%d PARENT ID %d\n",getpid(),getppid());
            }
            else
            {
               wait(NULL);

            }
         }
      }
      else
      {
        wait(NULL);
        printf("p1 PROCESS ID :%d\n",getpid());
      }
   }
}



