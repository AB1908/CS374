#include <unistd.h>
#include <stdio.h>
#include <wait.h>

int main()
{
    int fd[2];
    char buf[20];
    pid_t pid;

    int pipe_status = pipe(fd);
    int fork_status = fork();

    if(fork_status == 0)
    {
        sleep(1);
        int pidc;
        read(fd[0], &pidc, sizeof(int));
        pid = getppid();
        int pidp = (int)pid;
        printf("%d Child read\n",pidc);
        write(fd[1], &pidp, sizeof(int));
        close(fd[0]);
        close(fd[1]);
    }
    else if(fork_status > 0)
    {
        write(fd[1], &fork_status, sizeof(int));
        sleep(2);
        int readpid;
        read(fd[0], &readpid, sizeof(int));
        printf("Parent read %d",readpid);
        close(fd[0]);
        close(fd[1]);
    }
    else
    {
        printf("\nError in child creation\n");
    }
}