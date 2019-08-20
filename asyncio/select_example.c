#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int child_action();

int main(int argc, char **argv) {
	int pid;
	int fifo[2], flags;
	int n,m;
	char buffer[50];
	fd_set readset;

	mkfifo("/tmp/fifo1", 0666);
	mkfifo("/tmp/fifo2", 0666);

	pid = fork();
	if(pid < 0) {
		printf("Failed to fork a child. Exiting\n");
		return 1;
	}
	if(pid == 0) {
		child_action();
		return 0;
	}
	printf("Parent: before opening fifos\n");
	fifo[0] = open("/tmp/fifo1", O_RDONLY, 0);
	fifo[1] = open("/tmp/fifo2", O_RDONLY, 0);
	for(n=0;n<2;n++) {
		flags = fcntl(fifo[n],F_GETFL);
		flags = flags & O_NONBLOCK;
		fcntl(fifo[n],F_SETFL,flags);
	}
	while(1) {
		FD_ZERO(&readset);
		FD_SET(fifo[0],&readset);
		FD_SET(fifo[1],&readset);
		n = select(fifo[1]+1, &readset, NULL, NULL,NULL);
		if(n > -1) {
			for(m=0;m<2;m++) {
				if(FD_ISSET(fifo[m],&readset)) {
					memset(buffer,0,50);
					n = read(fifo[m],buffer,50);
					if(strncmp(buffer,"quit",4) == 0) {
						printf("Quit read in fifo %d\n",fifo[m]);
						exit(0);
					}
					printf("%s from fifo %d\n",buffer, fifo[m]);
				}
			}
		}		
	}
}

int child_action() {
	int n, fifo[2];
	fifo[0] = open("/tmp/fifo1",O_WRONLY);
	fifo[1] = open("/tmp/fifo2",O_WRONLY);
	printf("In child after opening fifos. Writing\n");
	sleep(2);
	n = write(fifo[0], "Terve\n",6);
	n = write(fifo[1], "Terve taas\n",11);
	sleep(2);
	n = write(fifo[0], "Back again\n",11);
	n = write(fifo[1], "Are we bored yet?\n",19);
	sleep(2);
	n = write(fifo[0], "quit\n",5);
	return 0;
}
