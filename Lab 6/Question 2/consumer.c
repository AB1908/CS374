#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
        char buf[100];

	int fd = open("messages.txt", O_RDONLY);
	read(fd, buf, 100);
        printf("\nThe following message was received by consumer:%s", buf);
        close(fd);
    
	return 0;
}