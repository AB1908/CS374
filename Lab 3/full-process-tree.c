// Create a full process tree height h degree n

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    int n, h, i;

    printf("\nEnter a degree for the process tree: ");
    scanf("%d", &n);
    printf("\nEnter a height for the process tree: ");
    scanf("%d", &h);

    for (i = 0; i < h; i++)
    {
        int child_count = 0;
        int fork_status = 0;

        while (child_count < n)
        {
            fork_status = fork();
            
            if(fork_status > 0)
            {
                child_count++;
                wait(0);
            }
            else
            {
                printf("\nProcess %d has created a child process %d\n", (int)getppid(), (int)getpid());
                break;
            }
        }

        if(fork_status)
        {
            break;
        }
        else
        {
            continue;
        }
    }

    return 0;
}