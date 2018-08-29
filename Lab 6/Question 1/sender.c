#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	char buf[] = "111HELLO THERE214GENERAL KENOBI";

	if (mkfifo("messages.txt", 0666) == -1)	{
		printf("/nFIFO creation was unsuccesful./n");
	}
	int fd = open("messages.txt", O_WRONLY);
    
	write(fd, buf, sizeof(buf));
        close(fd);
	return 0;
}