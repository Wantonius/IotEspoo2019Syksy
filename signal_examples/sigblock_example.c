#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char **argv) {

	sigset_t signalset;
	sigset_t originalset;

	sigemptyset(&signalset);
	
	sigaddset(&signalset,SIGINT);

	if(sigprocmask(SIG_BLOCK, &signalset, &originalset) < 0) {
		printf("Error in blocking SIGINT\n");
		return 1;		
	}
	printf("Blocked SIGINT, Sleeping for 10 seconds, try Ctrl+C\n");
	sleep(10);
	if(sigprocmask(SIG_UNBLOCK, &signalset, NULL) < 0) {
		printf("Error in unblocking SIGINT\n");
		return 1;
	}	

	printf("Done, exiting\n");
	return 0;
}
