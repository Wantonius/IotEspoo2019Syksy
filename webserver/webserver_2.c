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


char* get_filename(char *buffer, char *filename);
long get_filesize(char *filename);
int handle_send_header(char *filename, int socket, long filesize);
int handle_send_file(char *filename, int socket, long filesize);

int main(int argc, char **argv) {

	char buffer[2048];
	char *filename;
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
	filename = (char *)malloc(128*sizeof(char));
	//READ FROM SOCKET AND RESPOND 200 OK STATUS TO BROWSER
	do {
		memset(buffer,0,2048);
		n = read(newsockfd,buffer,2048);
		
		if(n > 0) {
			printf("Browser sent us this:%s\n",buffer);
			filename = get_filename(buffer,filename);
			printf("filename is:%s\n",filename);	
			long len = get_filesize(NULL);
			handle_send_header(NULL,newsockfd, len);
			handle_send_file(NULL,newsockfd,len);			
		}
	} while(n>0);
	
	//CLOSE SOCKETS AND FREE MEMORY
		
	free(filename);
	close(newsockfd);
	close(sockfd);
}	

char *get_filename(char *buffer, char *filename) {
	char *c;
	int loc;
	
	memset(filename,0,128);
	c = strchr(buffer,' ');
	loc = (int)(c-buffer);	
	printf("location of space:%d\n",loc);
	char *temp = &buffer[loc+1];
	int loc2;
	c = strchr(temp,' ');
	loc2 = (int)(c-temp);
	printf("location of space2:%d\n",loc2);
	for(int j=0;j<loc2;j++) {
		filename[j]=temp[j];		
	}
	return filename;
}

long get_filesize(char *filename) {
	FILE *fp;
	long len;
	
	if(filename == NULL) {
		fp = fopen("./public/index.html","r");
		if(fp == NULL) {
			return -1;
		}		
	} else {
		fp = fopen(filename,"r");
		if(fp == NULL) {
			return -1;
		}
	}
	fseek(fp,0,SEEK_END);
	len = (long)ftell(fp);
	fclose(fp);
	return len;
}

int handle_send_header(char *filename, int socket, long filesize) {
	FILE *fp;
	int n;
	char header[2048];
	
	memset(header,0,2048);
	sprintf(header,"HTTP/1.1 200 OK\r\nContent-length:%ld\r\n",filesize);
	if(filename == NULL) {	
		strcat(header,"Content-type:text/html\r\n");
		strcat(header,"\r\n");					
	} else {
		// TODO: HANDLE FILE TYPE AND ADD CORRECT HEADER
	}	
	n = write(socket,header,strlen(header));
	return n;
}

int handle_send_file(char *filename, int socket, long filesize) {
	FILE *fp;
	char filebuffer[2048];
	int n;

	memset(filebuffer,0,2048);
	if(filename == NULL) {
		fp = fopen("./public/index.html","r");
		if(fp == NULL) {
			return -1;
		}		
	} else {
		fp = fopen(filename,"r");
		if(fp == NULL) {
			return -1;
		}
	}
	while(fgets(filebuffer,2048,fp) != NULL) {
		n=write(socket,filebuffer,strlen(filebuffer));
		if(n < 0) {
			fclose(fp);
			return n;
		}
		memset(filebuffer,0,2048);		
	}
	if(feof(fp)) {
		fclose(fp);
		return 1;
	} else {
		fclose(fp);
		return -1;
	}
}
