#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
    fork();
    fork();
    fork();
    printf("\nMy PID is %d\n", getpid());
    printf("\nMy parent PID is %d\n", getppid());
}