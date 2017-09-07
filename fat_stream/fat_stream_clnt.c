/* Arquivo: fat_stream_clnt.c (Lasaro Dumer; 29 ago\. 2017) */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include "fat_stream.h"

int main(int argc, char *argv[])
{
	double		n, f;
	struct		hostent *hostinfo;
	struct		sockaddr_in address;
	int		sockfd,result;
	socklen_t	addr_len;

	if	(argc!=3)  {
		fprintf(stderr,"Uso:\n%s <nome_do_servidor> <nome>\n\n",argv[0]);
		exit(1);
	}
	hostinfo = gethostbyname(argv[1]);
	if	(!hostinfo)  {
		fprintf(stderr,"%s: erro em gethostbyname().\n",argv[0]);
		exit(1);
	}
	if	(hostinfo->h_addrtype!=AF_INET)  {
		fprintf(stderr,"%s: \"%s\" nao e' uma maquina IP.\n",argv[0],argv[1]);
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
		fprintf(stderr,"%s: erro em connect(): %s\n",argv[0],strerror(errno));
		exit(1);
	}
	n = atoi(argv[2]);
	printf ("%.f!...",n);
	write(sockfd,&n,sizeof(double));
	read(sockfd,&f,sizeof(double));
	close(sockfd);
	printf ("=%.f\n", f);
	return 0;
}
