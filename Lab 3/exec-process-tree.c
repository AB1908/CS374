#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <wait.h>

int main()
{
    int fork_status;

    fork_status = fork();

    if(fork_status == 0)
    {
        // int exec_status = execl("tree.out", (char *)0);
        char *args[] = {"tree.out", (char *)0};
        
        // int exec_status = execlp("/home/abhishek/GitHub/CS374/Lab 3/tree.out", (char *)0);
        // int exec_status = execv("/home/abhishek/GitHub/CS374/Lab 3/tree.out", args);
        int exec_status = execvp("/home/abhishek/GitHub/CS374/Lab 3/tree.out", args);

        perror("\nError\n");
    }
    else if(fork_status > 0)
    {
        wait(0);
    }
    else
    {
        printf("\nChild creation unsuccessful");
    }
}