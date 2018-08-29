#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
// #include<stdlib.h>

int main()
{
    int i;
    pid_t p=fork();

    
    if (p == -1) {
        perror("\nFork could not be created");
        _exit(1);
    }
    
    if (p > 0) {
        waitpid(p,0,0);
        printf("\nI am a parent of PID %d",p);
        sleep(3);
        _exit(0);
    }
    
    for(i = 0; i < 5; i++)
    {
        printf("\nMy PID is %d", getpid());
        printf("\nMy parent PID is %d", getppid());
        sleep(1);
    }
    
    
    
    return 0;
}
