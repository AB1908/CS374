#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
        char buf[100];

	// int fifo_status = mkfifo("messages.txt", 0666);
        // if (fifo_status == -1) {
	// 	printf("\nFIFO creation was unsuccesful.\n");
	// }

	int fd = open("messages.txt", O_RDONLY);
	read(fd, buf, 100);
        printf("\nThe following message was received:%s", buf);
        close(fd);
    
	return 0;
}