#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>


int main(int argc, char **argv) {
	int sock, newsock, n, portno, clilength;
	char buffer[256];

	struct sockaddr_in server_addr, client_addr;

	if(argc < 2) {
		printf("Please enter port number: for example ./server 5000\n");	
		return 1;
	}
	portno = atoi(argv[1]);
	sock = socket(AF_INET, SOCK_STREAM,0);
	memset((char *)&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(portno);

	if(bind(sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) < 0) {
		printf("Binding of socket failed: error %s\n",strerror(errno));
		return 1;	
	}

	listen(sock,5);
	clilength = sizeof(struct sockaddr_in);

	printf("Waiting for a connection\n");

	newsock = accept(sock, (struct sockaddr *)&client_addr,(socklen_t *) &clilength);
	if(newsock < 0) {
		printf("Failed to accept connection\n");
		return 1;		
	}
	do {
		memset(buffer, 0, 256);
		n = read(newsock,buffer,256);
		if(n > 0) {
			printf("Message from client:%s\n",buffer);

		}
	} while(n>0);
	close(newsock);
	close(sock);

}		
