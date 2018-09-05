#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <strings.h>

int main()
{
        int read_fd, write_fd;
        int fork_status = fork();

        char writebuf[100];
        char readbuf[100];
        char terminate[] = "Bye";

        if (fork_status == 0) {
                
                mkfifo("messages-1.txt", 0666);
	        int write_fd = open("messages-1.txt", O_WRONLY);

                do {
                        // *writebuf = NULL;
                        // printf("\nClient 1:");
                        scanf("%s", writebuf);
                        write(write_fd, writebuf, 100);

                } while (strcasecmp(readbuf, "Bye"));

        }

        else if (fork_status > 0) {

                mkfifo("messages-2.txt", 0666);
	        int read_fd = open("messages-2.txt", O_RDONLY);

                do {
                        // *readbuf = NULL;
                        // scanf("%s", buf);
                        read(read_fd, readbuf, 100);
                        printf("\nClient 2:%s\n", readbuf);
                } while (strcasecmp(readbuf, "Bye"));
                
        }

        else
                printf("\nChild creation unsuccesful.\n");

	return 0;
}