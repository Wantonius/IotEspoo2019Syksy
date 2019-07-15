#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>

int main(int argc, char **argv) {

	int children = 0;
	int pid;
	int i;
	int status, sleeping;
	
	printf("Forking 10 children\n");	
	for(i=0;i<10;i++) {
		pid = fork();
		if(pid == 0) {
			srand(time(NULL));
			sleeping = rand() % 10 + 10;
			printf("I am child %d and I'll sleep for %d seconds\n",getpid(),sleeping);
			sleep(sleeping);
			printf("Child %d, Slept %d seconds, dying\n",getpid(),sleeping);
			if (i % 2 == 0) {
				exit(EXIT_SUCCESS);			
			} else {
				exit(EXIT_FAILURE);
			}
		} else {
			children++;
			sleep(1);	
		}
	}	
	while(children > 0) {
		pid = wait(&status);
		children--;
		printf("Child %d died, status %d\n",pid,status);
	}
	printf("All done, exiting\n");
	return 0;
}
