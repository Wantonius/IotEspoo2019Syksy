#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int global = 0;

pthread_mutex_t worker_mutex;
pthread_mutex_t control_mutex;
pthread_cond_t control_cond;

void *worker_thread(void *arg) {
	int id = *((int *)arg);
	printf("Worker Thread:%d, before mutex, global=%d\n",id,global);
	pthread_mutex_lock(&worker_mutex);
	sleep(2);
	global++;
	printf("Worker Thread:%d, in mutex, global=%d\n",id,global);
	if(global == 2) {	
		printf("Worker Thread:%d, global = %d, waking control thread\n",id,global);
		pthread_cond_broadcast(&control_cond);		
	}
	pthread_mutex_unlock(&worker_mutex);
	printf("Worker Thread:%d, after mutex, global=%d\n",id,global);	
	pthread_exit(NULL);
}

void *control_thread(void *arg) {
	int id = *((int *)arg);
	printf("Control Thread:%d, before control condition, global=%d\n",id,global);
	pthread_mutex_lock(&control_mutex);
	pthread_cond_wait(&control_cond,&control_mutex);
	printf("Control Thread:%d, in control condition, global=%d\n",id,global);	
	global++;
	sleep(3);
	pthread_mutex_unlock(&control_mutex);
	printf("Control Thread:%d, after control condition, global=%d\n",id,global);	
	pthread_exit(NULL);	
}

int main(int argc, char **argv) {
	pthread_t worker1, worker2, control;
	int worker1_id=100,worker2_id=200,control_id=300;
	
	pthread_mutex_init(&worker_mutex,NULL);
	pthread_mutex_init(&control_mutex,NULL);
	pthread_cond_init(&control_cond,NULL);

	if(pthread_create(&control, NULL, control_thread, &control_id)) {
		printf("Failed to create control thread. Exiting. Error %s\n",strerror(errno));
		return 1;
	}
	if(pthread_create(&worker1, NULL, worker_thread, &worker1_id)) {
		printf("Failed to create worker 1 thread. Exiting\n");
		return 1;
	}
	if(pthread_create(&worker2, NULL, worker_thread, &worker2_id)) {
		printf("Failed to create worker 2 thread. Exiting\n");
		return 1;
	}
	printf("Main Thread, after thread creation, before control condition, global=%d\n",global);
	pthread_mutex_lock(&control_mutex);
	pthread_cond_wait(&control_cond, &control_mutex);
	printf("Main Thread, after control condition, global=%d\n",global);
	global++;
	sleep(3);
	pthread_mutex_unlock(&control_mutex);
	printf("Main Thread, after control mutex, global=%d\n",global);	
	pthread_join(control, NULL);
	printf("Main Thread, threads done, global=%d\n",global);	
	return 0;
}		
