#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080

int main(int argc, char **argv) {

	int sockfd, newsockfd, clilen, n;
	struct sockaddr_in server_address, client_address;

	//CREATE SOCKET

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		printf("Failed to create socket, error: %s\n",strerror(errno));
		return 1;
	}

	//DEFINE SERVER ADDRESS AND PORT

	memset((char *)&server_address,0,sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);
	
	//DEFINE SERVER ADDRESS AS A WILDCARD. CAN BE REBOUND.
	
	int a = 1;
	setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR,&a,sizeof(int));

	// BIND THE SERVER ADDRESS

	if(bind(sockfd,(struct sockaddr *)&server_address,sizeof(server_address)) < 0) {
		printf("Error in binding server, error: %s\n",strerror(errno));
		return 1;
	}

	// LISTEN AND ACCEPT CONNECTION
	
	listen(sockfd,5);
	clilen = sizeof(client_address);
	newsockfd = accept(sockfd, (struct sockaddr *)&client_address, (socklen_t *)&clilen);
	if(newsockfd < 0) {
		printf("Error opening connected socket, error: %s\n",strerror(errno));
		return 1;		
	}

	//READ FROM SOCKET AND RESPOND 200 OK STATUS TO BROWSER
	
	char buffer[2048];
	memset(buffer,0,2048);
	n = read(newsockfd,buffer,2048);
	if(n > 0) {
		printf("Browser sent us this:%s\n",buffer);
	}
	n = write(newsockfd,"HTTP/1.1 200 OK\r\n",17);
	close(newsockfd);
	close(sockfd);
}	
