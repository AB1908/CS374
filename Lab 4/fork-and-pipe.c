// Create two children and emulate the shell command "ls -l | wc" using pipe calls

#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

int main()
{
    int child_count = -1;
    int i;
    int fd[2];
    int pipe_status = pipe(fd);
    int fork_status = 1;

    for(i = 1; i < 3; i++)
    {
        if(fork_status < 0)
        {
            printf("\nChild creation unsuccessful");
            break;
        }
        else
        {
            fork_status = (int)fork();
            child_count = i;
            if (fork_status == 0) // Don't fork any more if child
            {
                printf("\nProcess %d was spawned by process %d\n", (int)getpid(), (int)getppid());
                break;
            }
        }
    }

    if(fork_status != 0)
    {
        wait(0);
        close(fd[0]); // Close the remaining pipe end once children are dead
    }
    else
    {
        if(child_count == 1) // If first child, redirect STDOUT to pipe and execute ls
        {
            close(fd[0]);
            dup2(fd[1], 1);
            int exec_status = execl("/bin/ls", "ls", "-l", "/home/abhishek", (char *)NULL);
        }
        if(child_count == 2) // If second child, redirect pipe write end to STDIN for "wc" to use as input
        {
            close(fd[1]);
            int dup_status = dup2(fd[0], 0);
            int exec_status = execl("/bin/wc", "wc", (char *)NULL);
        }
    }
    return 0;
}