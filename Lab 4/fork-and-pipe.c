// Create two children and emulate the shell command "ls -l | wc" using pipe calls

#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

int child(int child_count, int *fd)
{
    if(child_count == 0)
    {
        close(fd[0]);
        close(fd[1]);
        dup2(1,fd[1]);
        char *args[] = {"/bin/ls", "-l", "\0"};
        int exec_status = execv("/bin/ls", args);
    }
    else
    {
        char buf[100];
        close(fd[1]);
        int rd = read(fd[0], buf, 100);
        // close(fd[0]);
        // dup2(fd[1],1);
        char *args[] = {"/bin/wc", buf, "\0"};
        int exec_status = execv("/bin/wc", args);
    }
    return 0;
}

int main()
{
    int child_count, i;
    int fd[2];
    int pipe_status = pipe(fd);
    int fork_status = 1;

    for(i = 0; i < 2; i++)
    {
        if(fork_status < 0)
        {
            printf("\nChild creation unsuccessful");
            break;
        }
        else if(fork_status == 0)
        {
            printf("\nProcess %d was spawned by process %d\n", (int)getpid(), (int)getppid());
        }
        else
        {
            fork_status = fork();
            child_count = i;
            wait(0);
        }
    }

    if(fork_status == 0)
    {
        int proc_status = child(child_count, fd);
    }
    // else
    // {
    //     wait(0);
    // }
    return 0;
}