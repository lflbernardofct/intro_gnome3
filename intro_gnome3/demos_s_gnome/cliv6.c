/*****************************************************************************\
 * Redes Integradas de Telecomunicacoes
 * MIEEC/MEEC - FCT NOVA  2024/2025
 *
 * cliv6.c
 *
 * Command line client for UDP IPv6
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

#define DATA "Sent using UDP IPv6 ..."	/* Static message */

/*
 * This program sends an IPv6 datagram to a receiver provided in the command line
 */

char *
addr_ipv6 (struct in6_addr *addr)
{
  static char buf[100];
  inet_ntop (AF_INET6, addr, buf, sizeof (buf));
  return buf;
}


int main (int argc, char *argv[])
{
  int sock;
  struct sockaddr_in6 name;
  struct hostent *hp;

  /* Create socket */
  sock = socket (AF_INET6, SOCK_DGRAM, 0);
  if (sock < 0)
    {
      perror ("Erro na abertura de socket datagrama ipv6");
      exit (1);
    }
  /*
   * Creates name of destination socket. gethostbyname retorns a struct
   * that includes the destination IPv4 address, supporting "pc-1" or "2001:690:2005:10:33::11" addresses.
   * inet_pton could also be used for "2001:690:2005:10:33::11" addresses.
   * The port is obtained from the command line
   */
  if (argc <= 2)
    {
      fprintf (stderr, "Usage: %s ip porto\n", argv[0]);
      exit (2);
    }
  hp = gethostbyname2 (argv[1], AF_INET6);
  if (hp == 0)
    {
      fprintf (stderr, "%s: unknown address\n", argv[1]);
      exit (2);
    }
  bcopy (hp->h_addr, &name.sin6_addr, hp->h_length);
  name.sin6_family = AF_INET6;
  name.sin6_port = htons (atoi (argv[2]));	/* convert to network format */
  /* Send message */
  fprintf (stderr, "Sending packet to %s ; %d\n",
	   addr_ipv6 (&name.sin6_addr), (int) ntohs (name.sin6_port));
  if (sendto (sock, DATA, strlen (DATA) + 1 /* to send '\0' */ , 0,
	      (struct sockaddr *) &name, sizeof (name)) < 0)
    perror ("Error sending datagram");
  fprintf (stderr, "End of client\n");
  close (sock);
  return 0;
}
