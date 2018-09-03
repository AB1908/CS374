#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
	int errno;
	char buf[] = "1 5 HELLO 2 5 THERE";

	if (mkfifo("messages.txt", 0666) == -1)	{
		if (errno = EEXIST)
			printf("\nSender: %s\n", strerror(errno));
		
	}

	int fd = open("messages.txt", O_WRONLY);
    	write(fd, buf, sizeof(buf));
	close(fd);
	return 0;
}