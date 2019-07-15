#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int globalData = 0;

int child_process(int processCount) {
	pid_t myPid, parentPid;
	
	myPid = getpid();
	parentPid = getppid();

	globalData++;

	printf("(%d) myPid is %d\n",processCount, myPid);
	printf("(%d) parentPid is %d\n",processCount, parentPid);
	printf("(%d) globalData is %d\n",processCount, globalData);

	sleep(8);

	parentPid = getppid();
		
	printf("(%d) After sleep. MyPid is %d\n",processCount, myPid);
	printf("(%d) After sleep. ParentPid is %d\n",processCount, parentPid);
	printf("(%d) After sleep. globalData is %d\n",processCount, globalData);
	return 0;
}

int main(int argc, char **argv) {

	pid_t child;
	int i;
	for(i=0;i<10;i++) {
		child = fork();
		if(child < 0) {
			printf("Failed to create child, exiting");
			exit(1);
		}
		if(child == 0) {
			child_process(i);
			//return 0;
		}
		printf("Started child process %d, sleeping one second\n",child);
		sleep(1);
	}
	printf("Done creating children, exiting!!\n");	
}


