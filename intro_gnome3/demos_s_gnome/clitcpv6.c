/*****************************************************************************\
 * Redes Integradas de Telecomunicacoes
 * MIEEC/MEEC - FCT NOVA  2024/2025
 *
 * clitcpv6.c
 *
 * Command line client for TCP IPv6
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
#include <string.h>
#include <unistd.h>

#define DATA "Sent using TCP IPv6 ..."	/* Static message */
char *
addr_ipv6 (struct in6_addr *addr)
{
  static char buf[100];
  inet_ntop (AF_INET6, addr, buf, sizeof (buf));
  return buf;
}


/*
 * This program creates a socket and connects it to the receiver socket provided in the command line.
 * A message is sent through the connection and the socket is closed, finishing the connection.
 */
int main (int argc, char *argv[])
{
  int sock;
  struct sockaddr_in6 server;
  struct hostent *hp, *gethostbyname ();
  if (argc <= 2)
    {
      fprintf (stderr, "Usage: %s ip porto\n", argv[0]);
      exit (2);
    }

  /* Create socket */
  sock = socket (AF_INET6, SOCK_STREAM, 0);
  if (sock < 0)
    {
      perror ("opening stream socket");
      exit (1);
    }

  /* Connect socket using the name specified in the command line. */
  hp = gethostbyname2 (argv[1], AF_INET6);
  if (hp == 0)
    {
      fprintf (stderr, "%s: unknown host\n", argv[1]);
      exit (2);
    }
  server.sin6_family = AF_INET6;
  server.sin6_flowinfo = 0;
  server.sin6_port = htons (atoi (argv[2]));
  bcopy (hp->h_addr, &server.sin6_addr, hp->h_length);
  if (connect (sock, (struct sockaddr *) &server, sizeof (server)) < 0)
    {
      perror ("connecting stream socket");
      exit (1);
    }
  if (write (sock, DATA, strlen (DATA) + 1) < 0)
    perror ("writing on stream socket");
  close (sock);
  return 0;
}
