/* Arquivo: revstr_stream_clnt.c (Lasaro Dumer; 29 ago\. 2017) */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <netdb.h>
#include "revstr_stream.h"

int main(int argc, char *argv[])
{
	struct		hostent *hostinfo;
	struct		sockaddr_in address;
	int		sockfd,result;
	socklen_t	addr_len;

	if	(argc!=2)  {
		fprintf(stderr,"Usage:\n%s <server_name>\n\n",argv[0]);
		exit(1);
	}
	hostinfo = gethostbyname(argv[1]);
	if	(!hostinfo)  {
		fprintf(stderr,"%s: erroe at gethostbyname().\n",argv[0]);
		exit(1);
	}
	if	(hostinfo->h_addrtype!=AF_INET)  {
		fprintf(stderr,"%s: \"%s\" not an IP host.\n",argv[0],argv[1]);
		exit(1);
	}
	addr_len = sizeof(address);
	bzero((char *)&address,addr_len);
	address.sin_family = AF_INET;
	bcopy((char *)hostinfo->h_addr,(char *)&address.sin_addr.s_addr,hostinfo->h_length);
	address.sin_port = htons(SOCKET_PORT);
	printf("%d %d\n", SOCKET_PORT, ntohs(address.sin_port));
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	result = connect (sockfd, (struct sockaddr *)&address, addr_len);
	if	(result == -1)  {
		fprintf(stderr,"%s: error at connect(): %s\n",argv[0],strerror(errno));
		exit(1);
	}

	bool first = true;
	char* c;
	int read_size;
	char server_message[SIZE_LIMIT];
	do
	{
		if(!first)
			printf("Received: '%s'\n--------------------\n", server_message);
		char client_message[SIZE_LIMIT];
		printf("Write something:\n");
		if(fgets(client_message, sizeof client_message, stdin) != NULL) {
			strtok(client_message, "\n");
			printf("Sending: '%s'\n", client_message);
			write(sockfd, &client_message, sizeof(char)*(SIZE_LIMIT+1));
			first = false;
		}
		else{
			break;
		}
	}while( (read_size = read(sockfd , &server_message , sizeof(char)*(SIZE_LIMIT+1))) > 0 );
	close(sockfd);
	return 0;
}
