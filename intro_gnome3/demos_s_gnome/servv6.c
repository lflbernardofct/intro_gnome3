/*****************************************************************************\
 * Redes Integradas de Telecomunicacoes
 * MIEEC/MEEC - FCT NOVA  2024/2025
 *
 * servv6.c
 *
 * Command line server for UDP IPv6
 *
 * @author  Luis Bernardo
\*****************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * This program creates an IPv6 datagram socket, associates the port number and a IP Multicast group,
 * and receives a message
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
  socklen_t length;
  struct sockaddr_in6 name;
  char buf[1024];
  short int porto = 0;
  int reuse = 1;
  /* Multicast */
  char *addr_mult = "FF18:10:33::1";
  struct ipv6_mreq imr;
  /* Receiving variables */
  char straddr[100];
  struct sockaddr_in6 fromaddr;
  socklen_t fromlen = sizeof (fromaddr);

  /* Create socket */
  sock = socket (AF_INET6, SOCK_DGRAM, 0);
  if (sock < 0)
    {
      perror ("Error opening ipv6 datagram socket");
      exit (1);
    }
  if (argc >= 2)
    {
      /* The port number can be set using a command line parameter */
      porto = (short int) atoi (argv[1]);
    }
  if (argc == 3)
    {
      /* The second parameter is the IPv6 multicast address */
      addr_mult = argv[2];
    }
  /* Make the socket's IP and port reuseable - allows having several servers running on the same port in the
   * same machine */
  if (setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse,
		  sizeof (reuse)) < 0)
    {
      perror ("setsockopt SO_REUSEADDR failed");
      exit (-1);
    }
  /* Bind socket to a port */
  name.sin6_family = AF_INET6;
  name.sin6_flowinfo = 0;
  name.sin6_port = htons (porto);	/* if 0, port is defined by the system */
  name.sin6_addr = in6addr_any;	/* Default local IPv6 */
  if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {
      perror ("datagram socket binding failed");
      exit (1);
    }
  /* Multicast configuration */
  if (!inet_pton (AF_INET6, addr_mult, &imr.ipv6mr_multiaddr))
    {
      perror ("Failed conversion of multicast address");
      exit (1);
    }
  imr.ipv6mr_interface = 0;	/* Interface 0 */
  /* Join group */
  if (setsockopt (sock, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char *) &imr,
		  sizeof (imr)) == -1)
    {
      perror ("Failed association to IPv6 multicast group");
      abort ();
    }

  /* Get the socket's port number */
  length = sizeof (name);
  if (getsockname (sock, (struct sockaddr *) &name, &length))
    {
      perror ("Error getting the port number");
      exit (1);
    }
  printf ("The socket on IP Multicast %s has the port #%hu\n",
	  addr_ipv6 (&imr.ipv6mr_multiaddr), htons (name.sin6_port));
  /* Read a message from the socket */
  if (recvfrom (sock, buf, 1024, 0 /* By default it waits for data */  ,
		(struct sockaddr *) &fromaddr, &fromlen) < 0)
    perror ("Error receiving datagram packet");
  inet_ntop (AF_INET6, &fromaddr.sin6_addr, straddr, sizeof (straddr));
  printf ("Received from %s#%hu -->%s\n", straddr, ntohs (fromaddr.sin6_port),
	  buf);
  printf ("The server shutdown\n");
  close (sock);
  return 0;
}
