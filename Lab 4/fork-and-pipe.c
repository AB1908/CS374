// Create two children and emulate the shell command "ls -l | wc" using pipe calls

#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

int child(int child_count, int *fd)
{
    if(child_count == 1)
    {
        // close(0);
        close(fd[0]);
        close(1);
        // dup2(1, fd[1]);
        dup(fd[1]);
        // char args[] = {"ls", "\0"};
        int exec_status = execl("/bin/ls", "ls", "-l", "/home/abhishek");
        // printf("%d", exec_status);
    }
    else if(child_count == 2)
    {
        // printf("Hello Darkness my old friend");
        // close(0);
        // dup(fd[0]);
        char buf[1000];
        // close(fd[1]);
        int rd = read(fd[0], buf, 1000);
        // close(fd[0]);
        // dup2(fd[1],1);
        // char *args[] = {"wc", buf, "\0"};
        // int exec_status = execv("/bin/wc", args);
        int exec_status = execl("/bin/wc", "wc", buf);
        printf("%d",exec_status);
    }
    return 0;
}

int main()
{
    int child_count = -1;
    int i;
    int fd[2];
    int pipe_status = pipe(fd);
    int fork_status = 1;

    for(i = 0; i < 3; i++)
    {
        if(fork_status < 0)
        {
            printf("\nChild creation unsuccessful");
            break;
        }
        else if(fork_status == 0)
        {
            printf("\nProcess %d was spawned by process %d\n", (int)getpid(), (int)getppid());
            break;
        }
        else
        {
            fork_status = (int)fork();
            child_count = i;
            wait(0);
        }
    }

    if(fork_status != 0)
    {
        // printf("Man");
        // wait(0);
    }
    else
    {
        int proc_status = child(child_count, fd);
        // if (!child_count)
        {
            // int exec_status = execl("/bin/ls", "ls", "-l", "/home/abhishek");           
        }
    }
    return 0;
}