// Create a full process tree height h degree n.

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

    // Loop h times as degree is taken care of by later while loop
    for (i = 0; i < h; i++)
    {
        int child_count = 0;
        int fork_status = 0;

        // Make a node create n children while stopping the child from creating any children
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

        // Child needs to achieve degree n while parent need not create any more children
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