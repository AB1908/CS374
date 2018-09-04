#define _POSIX_C_SOURCE 200809L

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
	int errno;
	char *buf = NULL;
	size_t size = 0;
	// FILE *stream;

	if (mkfifo("messages.txt", 0666) == -1)
		if (errno = EEXIST)
			printf("\nProducer: %s\n", strerror(errno));

	printf("\nEnter message to pass to consumer:");
	getline(&buf, &size, stdin);
	int fd = open("messages.txt", O_WRONLY);
	write(fd, buf, size);
	close(fd);
	
	free(buf);
	return 0;
}