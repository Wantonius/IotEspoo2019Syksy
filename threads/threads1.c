#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global = 0;


void *thread_function(void *arg) {
	int i;
	for(i=0;i<20;i++) {
		global++;
		printf("My thread, global %d, sleeping\n",global);
		sleep(1);
		printf("My thread, global %d, after sleep\n",global);
	}	
	return NULL;
}

int main(int argc, char **argv) {
	pthread_t mythread;
	int i;
	if(pthread_create(&mythread, NULL, thread_function, NULL)) {
		printf("Error creating a thread. Exiting\n");	
		return 1;
	}
	for(i=0;i<20;i++) {
		global++;
		printf("Main thread, global %d, sleeping\n",global);
		sleep(1);
		printf("Main thread, global %d, after sleep\n",global);
	}
	if(pthread_join(mythread, NULL)) {
	printf("Error joining mythread, exiting\n");
		return 1;
	}
	printf("Main thread, after joining, global value:%d\n",global);
	return 0;
}
