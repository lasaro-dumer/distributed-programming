/* Arquivo: fat_stream_svc.c (Lasaro Dumer; 29 ago\. 2017 - 30 mar. 2017) */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "fat_stream.h"

double fat(double n){
	double i, f = 1;
	for (i = 1; i <= n; i++)
		f*=i;
	return f;
}

int main(int argc, char *argv[])
{
	// printf("fat(%d)=%.f\n",1,fat(1));
	// printf("fat(%d)=%.f\n",2,fat(2));
	// printf("fat(%d)=%.f\n",3,fat(3));
	// printf("fat(%d)=%.f\n",4,fat(4));
	// printf("fat(%d)=%.f\n",5,fat(5));
	// printf("fat(%d)=%.f\n",6,fat(6));
	// printf("fat(%d)=%.f\n",10,fat(10));
	// printf("fat(%d)=%.f\n",99,fat(99));
	// printf("fat(%d)=%.f\n",100,fat(100));
	struct sockaddr_in	svc_address;
	struct sockaddr_in	clnt_address;
	int			svc_socket,clnt_socket;
	socklen_t		clnt_len,svc_len;
	double n, f;

	svc_socket = socket(AF_INET, SOCK_STREAM, 0);
	if	(svc_socket < 0)  {
		fprintf(stderr,"%s: erro em socket(): %s\n",argv[0],strerror(errno));
		exit(1);
	}
	svc_len = sizeof(svc_address);
	bzero((char *)&svc_address,svc_len);
	svc_address.sin_family = AF_INET;
	svc_address.sin_addr.s_addr = INADDR_ANY;
	svc_address.sin_port = htons(SOCKET_PORT);
	printf("%d %d\n", SOCKET_PORT, ntohs(svc_address.sin_port));
	if	(bind(svc_socket,(struct sockaddr *)&svc_address,svc_len) < 0)  {
		fprintf(stderr,"%s: erro em bind(): %s\n",argv[0],strerror(errno));
		exit(1);
	}
	listen(svc_socket, 5);
	while	(1)  {
		clnt_socket = accept (svc_socket,(struct sockaddr *)&clnt_address, &clnt_len);
		if	(clnt_socket < 0)  {
			fprintf(stderr,"%s: erro em accept(): %s\n",argv[0],strerror(errno));
			exit(1);
		}
		read(clnt_socket, &n, sizeof(double));
		printf("fatting for %.f\n", n);
		f = fat(n);
		printf("fatted %.f\n", f);
		write(clnt_socket, &f, sizeof(double));
		close(clnt_socket);
	}
	return 0;
}
