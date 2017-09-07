/* Arquivo: revstr_stream_svc.c (Lasaro Dumer; 29 ago\. 2017 - 30 mar. 2017) */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "revstr_stream.h"

void *connection_handler(void *);
int main(int argc, char *argv[])
{
	struct sockaddr_in	svc_address;
	struct sockaddr_in	clnt_address;
	int			svc_socket;
	long int clnt_socket;
	socklen_t		clnt_len,svc_len;

	// if	(argc!=2)  {
	// 	fprintf(stderr,"Usage:\n%s <server_port>\n\n",argv[0]);
	// 	exit(1);
	// }

	svc_socket = socket(AF_INET, SOCK_STREAM, 0);
	if	(svc_socket < 0)  {
		fprintf(stderr,"%s: error at socket(): [%d]%s\n",argv[0],errno,strerror(errno));
		exit(1);
	}
	svc_len = sizeof(svc_address);
	bzero((char *)&svc_address,svc_len);
	svc_address.sin_family = AF_INET;
	svc_address.sin_addr.s_addr = INADDR_ANY;
	svc_address.sin_port = htons(SOCKET_PORT);
	printf("%d %d\n", SOCKET_PORT, ntohs(svc_address.sin_port));
	if	(bind(svc_socket,(struct sockaddr *)&svc_address,svc_len) < 0)  {
		fprintf(stderr,"%s: error at bind(): [%d]%s\n",argv[0],errno,strerror(errno));
		exit(1);
	}
	listen(svc_socket, 5);
	while	(1)  {
		clnt_socket = accept(svc_socket,(struct sockaddr *)&clnt_address, &clnt_len);
		if	(clnt_socket < 0)  {
			fprintf(stderr,"%s: error at accept(): [%d]%s\n",argv[0],errno,strerror(errno));
			exit(1);
		}
		pthread_t clnt_thread;
		if( pthread_create( &clnt_thread , NULL ,  connection_handler , (void*) clnt_socket) < 0)
		{
			printf("could not create thread\n");
			exit(1);
		}
	}
	return 0;
}

char* RevString(char* inStr){
	char tmp;
	size_t i, len = strlen(inStr)-1;
	size_t j = len;
	for (i = 0; i <= (len/2); i++) {
		tmp = inStr[i];
		inStr[i] = inStr[j];
		inStr[j] = tmp;
		j--;
	}
	return inStr;
}

void* connection_handler(void * clnt_socket) {
	long int sock = (long int)clnt_socket;
	printf("[%li]Connection open.\n", sock);
	int read_size;
	char client_message[SIZE_LIMIT];
	while( (read_size = read(sock , &client_message , sizeof(char)*(SIZE_LIMIT+1))) > 0 )
	{
		printf("[%li]Received: '%s'\n", sock, client_message);
		char* server_message = RevString(client_message);
		write(sock, server_message, sizeof(char)*(SIZE_LIMIT+1));
	}
	close(sock);
	printf("[%li]Connection closed.\n",sock);
}
