#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
   pid_t pid2,pid3;
   int n;
   int i,sum1=0,sum2=0,sum3=0;
   printf("Enter the value of n:");
   scanf("%d",&n);
   pid2=fork();
   if(pid2<0)
   {
      printf("Process creation failed");
   }

   else if(pid2==0)
   {
      printf("P2 PID :%d,Parent PID:%d\n",getpid(),getppid());
      for(i=0;i<=n;i++)
      {
         if(i%2!=0)
         {
            sum1=sum1+i;
         }
      }
      printf("Sum of odd numbers : %d\n",sum1);
   }
   else
   {
      wait(NULL);
      pid3=fork();
      if(pid3==0)
      {
         printf("P3 pid :%d,parent PID:%d\n",getpid(),getppid());
         for(i=0;i<=n;i++)
         {
            if(i%2==0)
            {
               sum2=sum2+i;
            }
         }
         printf("Sum of even numbers: %d\n",sum2);
      }
      else
      {
         wait(NULL);
         printf("P1 pid :%d\n",getpid());
         for(i=0;i<=n;i++)
         {
            sum3=sum3+i;
         }
         printf("Sum of n numbers:%d\n",sum3);

      }

   }
   return 0;
}
