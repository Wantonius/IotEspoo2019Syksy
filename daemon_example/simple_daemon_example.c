#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DAEMON_NAME "My Test Daemon"
#define LOG_LEVEL LOG_ERR
#define LOG_FACILITY LOG_DAEMON

void daemonize(void) {
	pid_t pid;

	pid = fork();
	if(pid > 0) {
		printf("Parent done, exiting!\n");
		exit(EXIT_SUCCESS);
	}
	if(pid < 0) {
		printf("Failed to fork. exiting!\n");
		exit(EXIT_FAILURE);
	}

	if(setsid() < 0) {
		exit(EXIT_FAILURE);
	}
	
	pid = fork();
	if(pid > 0) {
		printf("Parent done, exiting!\n");
		exit(EXIT_SUCCESS);
	}
	if(pid < 0) {
		printf("Failed to fork. exiting!\n");
		exit(EXIT_FAILURE);
	}

	if(chdir("/") < 0) {
		exit(EXIT_FAILURE);
	}
	
	umask(0);
	
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

int main(int argc, char **argv) {

	pid_t pid, ppid;
	int i=0;

	pid = getpid();
	ppid = getppid();

	openlog(DAEMON_NAME, LOG_PID, LOG_FACILITY);
	syslog(LOG_LEVEL, "Starting daemonization, pid=%d, ppid=%d",pid,ppid);

	daemonize();
	sleep(1);
	pid = getpid();
	ppid = getppid();
	
	for(i=0;i<10;i++) {
		syslog(LOG_LEVEL, "iteration %d, pid=%d, ppid=%d",i,pid,ppid);
		sleep(1);
	}
	syslog(LOG_LEVEL,"Job done, exiting");
	return 0;
}
