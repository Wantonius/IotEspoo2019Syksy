#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_func(void *arg);
void hello();

pthread_key_t my_key;

int main(int argc, char **argv) {
	pthread_t thread;
	int *thread_id;
	int i;

	pthread_key_create(&my_key,NULL);

	for(i=0;i<10;i++) {
		thread_id = (int *)malloc(sizeof(int));
		*thread_id = i+1;
		pthread_create(&thread,NULL,thread_func,(void *)thread_id);	
	}	
	pthread_exit(NULL);
}

void *thread_func(void *arg) {
	pthread_setspecific(my_key, arg);
	hello();
}

void hello() {
	int *k;
	printf("Thread [%d] says: Hello\n",*((int *)pthread_getspecific(my_key)));
	k = pthread_getspecific(my_key);
	free(k);
}
