#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>

int main()
{
    int fd1[2], fd2[2];
    int pid;


    int num, sum = 0, temp;
    char str[100], rev[100];


    int pnum, psum;
    char pstr[100], prev[100];

    pipe(fd1);
    pipe(fd2);
    pid = fork();

    if(pid < 0)
    {
        printf("Fork Failed\n");
        exit(1);
    }
    else if(pid == 0)
    {
        close(fd1[1]);
        close(fd2[0]);


        read(fd1[0], &pnum, sizeof(pnum));

        temp = pnum;
        while(temp > 0)
        {
            sum += temp % 10;
            temp /= 10;
        }


        psum = sum;
        write(fd2[1], &psum, sizeof(psum));


        read(fd1[0], pstr, sizeof(pstr));

        int len = strlen(pstr);
        int i;
        for(i = 0; i < len; i++)
            rev[i] = pstr[len - i - 1];
        rev[len] = '\0';


        strcpy(prev, rev);
        write(fd2[1], prev, sizeof(prev));

        close(fd1[0]);
        close(fd2[1]);
    }
    else
    {
        close(fd1[0]);
        close(fd2[1]);

        printf("----------TASK 1------------\n");
        printf("Enter a number: ");
        scanf("%d", &num);


        pnum = num;
        write(fd1[1], &pnum, sizeof(pnum));


        read(fd2[0], &psum, sizeof(psum));
        sum = psum;
        printf("Sum of digits = %d\n", sum);

        printf("----------TASK 2------------\n");
        printf("Enter a string to reverse: ");
        scanf("%s", str);


        strcpy(pstr, str);
        write(fd1[1], pstr, sizeof(pstr));


        read(fd2[0], prev, sizeof(prev));
        strcpy(rev, prev);
        printf("Reversed String = %s\n", rev);

        close(fd1[1]);
        close(fd2[0]);
        wait(NULL);
    }
    return 0;
}
