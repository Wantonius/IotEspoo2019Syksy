#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int child_action(int pipefd) {
	int m;
	char buffer[256];
	
	memset(buffer,0,256);
	printf("Send message to the parent\n");
	fgets(buffer,255,stdin);
	m = write(pipefd, buffer, strlen(buffer));
	printf("Wrote %d bytes to parent\n",m);	
	return 0;
}

int main(int argc, char **argv) {
	int pipefd[2], n;
	int pid;
	char buffer[256];

	n = pipe(pipefd);
	if(n < 0) {
		printf("No pipes created. Exiting!\n");
		return 1;
	}
	pid = fork();
	if(pid == 0) {
		child_action(pipefd[1]);
		return 0;
	}	
	printf("Waiting for a message from child process\n");
	memset(buffer,0,256);
	n = read(pipefd[0], buffer, 256);	
	if(n > 0) {
		printf("Message (bytes:%d) from child:%s\n",n,buffer);
	}
	return 0;
}
