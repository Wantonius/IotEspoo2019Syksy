#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t my_mutex;
int global = 0;


void *increment_thread(void *arg) {
	int i;
	for(i=0; i<10;i++) {
	//	printf("Before mutex lock: Increment Thread: global = %d\n",global);
		pthread_mutex_lock(&my_mutex);
		global++;
		printf("in mutex lock: Increment Thread: global = %d\n",global);
		sleep(2);
		pthread_mutex_unlock(&my_mutex);
	//	printf("After mutex lock: Increment Thread: global = %d\n",global);
		sleep(1);
	}

	pthread_exit(NULL);

}

int main(int argc, char **argv) {
	pthread_t thread;
	int i;

	pthread_mutex_init(&my_mutex, NULL);

	if(pthread_create(&thread,NULL,increment_thread,NULL)) {
		printf("Failed to create thread. Exiting\n");
		return 1;
	}

	for(i=0; i<10;i++) {
	//	printf("Before mutex lock: Main Thread: global = %d\n",global);
		pthread_mutex_lock(&my_mutex);
		global++;
		printf("in mutex lock: Main Thread: global = %d\n",global);
		sleep(1);
		pthread_mutex_unlock(&my_mutex);
	//	printf("After mutex lock: Main Thread: global = %d\n",global);
		sleep(2);
	}

	pthread_exit(NULL);
}
