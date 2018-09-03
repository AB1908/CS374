#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char buf[100];
	int i;
	int fork_status = 1;
	int fifo_fd = open("messages.txt", O_RDONLY);
	if (read(fifo_fd, buf, 100) == -1)
		printf("\nRead unsuccessful\n");
	close(fifo_fd);
	int pipe1_fd[2];
	int pipe2_fd[2];
	pipe(pipe1_fd);
	fork_status = fork();
	char readbuf[100];
	
	if (fork_status > 0) {
		close(pipe1_fd[0]);
		
		pipe(pipe2_fd);
		fork_status = fork();
		
		if (fork_status == 0) {
			close(pipe2_fd[1]);
			
			read(pipe2_fd[0], readbuf, 100);
			printf("\n%s\n", readbuf);	
			close(pipe2_fd[0]);
			
			return 0;
		}

		close(pipe2_fd[0]);
	}
	else if (fork_status == 0) {
 
		close(pipe1_fd[1]);

		read(pipe1_fd[0], readbuf, 100);
		printf("\n%s\n", readbuf);
		close(pipe1_fd[0]);

		return 0;
	}
	
	char *bufcopy;
	char *token;
	int size;
	
	bufcopy = strdup(buf);
	token = strtok(bufcopy, " ");
	
	while (token) {

		int caseno = (int)strtol(token, &token, 10);
		
		switch (caseno) {
			case 1:
				token = strtok(NULL, " ");
				size = (int)strtol(token, NULL, 10);
				token = strtok(NULL, " ");
				write(pipe1_fd[1], token, size);
				break;
			case 2:
				token = strtok(NULL, " ");
				size = (int)strtol(token, NULL, 10);
				token = strtok(NULL, " ");
				write(pipe2_fd[1], token, size);
		}
		
		token = strtok(NULL, " ");
	}
	close(pipe1_fd[1]);
	close(pipe2_fd[1]);
	return 0;
}