#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int count = 0;

void myHandler(int signum) {
	count++;
}

int main(int argc, char **argv) {

	struct sigaction sigact;

	sigact.sa_flags = 0;
	sigact.sa_handler = &myHandler;
	
	sigfillset(&sigact.sa_mask);

	if(sigaction(SIGINT, &sigact, NULL) < 0) {
		printf("Failed to install signal handler\n");
		return 1;
	}
	printf("Tap Ctrl+C\n");
	while(count < 10) {
		printf("Waiting for SIGINT, count %d\n",count);
		sleep(10);
	}
	printf("Done with SIGINT\n");
	return 0;
}
