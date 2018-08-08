// Question 4: Call fork() and then call all forms of exec() with /bin/ls

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
    int fork_status;

    fork_status = (int) fork();

    if (fork_status > 0)
    {
        // execl("/bin/ls", "\0");
        // execle("/bin/ls", "\0");
        // execlp("/bin/ls", "\0");
        char* argv[2];
        argv[0] = "ls";
        argv[1] = NULL;
        // execv("/bin/ls", argv); // Unable to make this work
        // execvp("/bin/ls", argv);
        execvpe("/bin/ls", argv);
    }
    
    return 0;
}