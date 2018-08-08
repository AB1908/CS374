#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
    int fork_status;
    printf("\n%d is the PID of the current process", (int)getpid());

    fork_status = fork();
    int i=0;

    if (fork_status > 0)
    {
        printf("\nChild created successfully.");
        wait(0);
        printf("\nChild terminated successfully.");
        printf("\nAfter termination of the child process, the value of i is %d.", i);
    }
    else if (fork_status == 0)
    {
        printf("\nInside the child process, the variable i has been incremented.\n");
        i++;
    }
    else
    {
        printf("\nChild creation unsuccessful.");
    }
    return 0;
}