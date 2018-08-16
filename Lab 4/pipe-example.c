#include <unistd.h>
#include <stdio.h>
#include <wait.h>

int main()
{
    int fd[2];
    int pipe_status = pipe(fd);
    char buf[20];
    pid_t pid;
    int fork_status = fork();

    if(fork_status == 0)
    {
        // close(fd[1]);
        sleep(1);
        int pidc;
        read(fd[0], &pidc, sizeof(int));
        pid = getppid();
        int pidp = (int)pid;
        // pid="B";
        printf("%d Child read\n",pidc);
        write(fd[1], &pidp, sizeof(int));
    }
    else if(fork_status > 0)
    {
        // buf = "Hello ";
        // close(fd[0]);
        // pid = getpid();
        // int pidi= fork_status;
        // pid="A";
        write(fd[1], &fork_status, sizeof(int));
        sleep(2);
        int readpid;
        read(fd[0], &readpid, sizeof(int));
        printf("Parent read %d",readpid);
    }
    else
    {
        printf("\nError in child creation\n");
    }
}