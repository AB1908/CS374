#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
        char buf[100];

	if (mkfifo("messages.txt", 0666) == -1)	{
		printf("\nFIFO creation was unsuccesful.\n");
	}

        printf("\nEnter message to pass to consumer:");
        scanf("%s", buf);
	int fd = open("messages.txt", O_WRONLY);
	write(fd, buf, sizeof(buf));
        close(fd);
    
	return 0;
}