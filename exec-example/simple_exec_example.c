#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char **argv) {

	int result;
	char *path;

	path = getenv("PATH");
	if(path != NULL) {
		printf("Path:%s\n",path);
	} else {
		printf("No path defined\n");
	}

	argv[0] = "exec-test";
	result = execvp("ls",argv);

	printf("SHOULD NEVER COME HERE %d\n",result);
	return 1;
}
