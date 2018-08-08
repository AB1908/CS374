#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/unistd.h>

int main()
{
    int i=0,n;

    printf("\nEnter a value for n: ");
    scanf("%d",&n);
    
    int fork_status = 0;

    for( i=0; i<n ; i++ )
    {
        if (fork_status > 0)
        {
            wait(0);
            break;
        }
        else if (fork_status == 0)
        {
            printf("\n%d has created a child process %d.\n", (int)getppid(), (int)getpid());
            fork_status = fork();
        }
        else
        {
            printf("\nChild process creation unsuccesful.");
        }
    }
    return 0;
}
