#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/unistd.h>

int main()
{
    int i=0,n;
    int fd[2];
    int pipe_status = pipe(fd);

    printf("\nEnter a value for n: ");
    scanf("%d",&n);
    
    int fork_status = fork();

    for(i = 0; i < n-1; i++)
    {
        if(fork_status > 0)
        {
            if (!i)
            {
                int incr;
                printf("\nEnter a value to increment:");
                scanf("%d",&incr);
                close(fd[0]);
                write(fd[1], &incr, sizeof(int));
            }
            wait(0);
            break;
        }
        else if(fork_status == 0)
        {
            sleep(1);
            printf("\n%d has created a child process %d.\n", (int)getppid(), (int)getpid());
            int incr;
            read(fd[0], &incr, sizeof(int));
            incr++;
            printf("\nThe current incremented value is %d\n", incr);
            write(fd[1], &incr, sizeof(int));
            fork_status = fork();
        }
        else
        {
            printf("\nChild process creation unsuccesful.");
        }
        if(i == n-2)
        {
            int incr;
            read(fd[0], &incr, sizeof(int));
            printf("\nThe final incremented value is %d\n", incr);
        }
    }
    return 0;
}