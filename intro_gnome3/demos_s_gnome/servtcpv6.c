/*****************************************************************************\
 * Redes Integradas de Telecomunicacoes
 * MIEEC/MEEC - FCT NOVA  2024/2025
 *
 * servtcpv6.c
 *
 * Command line server for TCP IPv6
 *
 * @author  Luis Bernardo
\*****************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

char *
addr_ipv6(struct in6_addr *addr) {
	static char buf[100];
	inet_ntop(AF_INET6, addr, buf, sizeof(buf));
	return buf;
}

int main() {
	int sock, msgsock;
	socklen_t length;
	struct sockaddr_in6 server;
	char buf[1024];

	/* Creates socket to receive connections. */
	sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("opening stream socket");
		exit(1);
	}

	/* Binds to a name with a dynamic port and associated to all network interfaces. */
	server.sin6_family = AF_INET6;
	server.sin6_addr = in6addr_any;
	server.sin6_port = 0;
	server.sin6_flowinfo = 0;
	if (bind(sock, (struct sockaddr *) &server, sizeof(server))) {
		perror("binding stream socket");
		exit(1);
	}

	/* Get the assigned port number and write it. */
	length = sizeof(server);
	if (getsockname(sock, (struct sockaddr *) &server, &length)) {
		perror("getting socket name");
		exit(1);
	}
	if (server.sin6_family != AF_INET6) {
		perror("Invalid family");
		exit(1);
	}
	printf("Socket has ip %s and port #%hu\n", addr_ipv6(&server.sin6_addr),
			ntohs(server.sin6_port));

	/*  Starts accepting connections */
	listen(sock, 5);
	while (1) {
		msgsock = accept(sock, (struct sockaddr *) &server, &length);
		if (msgsock == -1)
			perror("accept");
		else {
			printf("Connection from %s - %hu\n", addr_ipv6(&server.sin6_addr),
					ntohs(server.sin6_port));
			bzero(buf, sizeof(buf));
			if (read(msgsock, buf, 1024) < 0)
				perror("receiving stream data");
			else
				printf("-->%s\n", buf);
			close(msgsock);
		}
	}
	return 0;
}
