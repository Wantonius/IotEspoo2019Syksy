#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LOGFILE "/tmp/logfile"
#define FIFOFILE "/tmp/fifofile"

void daemonize(void) {

	int pid;

	pid = fork();
	if(pid < 0) {
		printf("Failed to create a new process, Error:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(pid > 0) {
		printf("Grandparent process exiting!\n");
		exit(EXIT_SUCCESS);
	}

	if(setsid() < 0) {
		printf("Failed to create a new session, Error:%s\n",strerror(errno));
		exit(EXIT_FAILURE);	
	}

	pid = fork();
	if(pid < 0) {
		printf("Failed to create a new process, Error:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(pid > 0) {
		printf("Parent process exiting!\n");
		exit(EXIT_SUCCESS);
	}

	if(chdir("/")<0) {
		printf("Failed to change directory, Error:%s\n",strerror(errno));
		exit(EXIT_FAILURE);	
	}
	umask(0);
	
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

int main(int argc, char **argv) {
	int fifofd, logfilefd,n;
	char buffer[256];

	daemonize();

	n = mkfifo(FIFOFILE,0777);
	logfilefd = open(LOGFILE, O_WRONLY | O_APPEND | O_CREAT, 0777);
	while(1) {
		fifofd = open(FIFOFILE, O_RDONLY);
		do {
		 	memset(buffer,0,256);
			n = read(fifofd, buffer,256);
			if(n > 0) {
				write(logfilefd,buffer,strlen(buffer));
			}		
		} while(n > 0);
		close(fifofd);	
	}
	close(logfilefd);
}
