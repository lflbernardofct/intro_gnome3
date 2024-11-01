/*****************************************************************************\
 * Redes Integradas de Telecomunicacoes
 * MIEEC/MEEC - FCT NOVA  2024/2025
 *
 * servv4.c
 *
 * Command line server for UDP IPv4
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
 * This program creates an IPv4 datagram socket, associates the port number and a IP Multicast group,
 * and receives a message
 */

int main (int argc, char *argv[])
{
  int sock;
  socklen_t length;
  struct sockaddr_in name;
  char buf[1024];
  short int porto = 0;
  int reuse = 1;
  /* Multicast */
  struct ip_mreq imr;
  char loop = 1;
  u_char ttl = 1;		/* rede local */
  /* Receiving variables */
  struct sockaddr_in fromaddr;
  socklen_t fromlen = sizeof (fromaddr);

  /* Create socket */
  sock = socket (AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
    {
      perror ("Error opening ipv4 datagram socket");
      exit (1);
    }
  if (argc == 2)
    {
      /* The port number can be set using a command line parameter */
      porto = (short int) atoi (argv[1]);
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
  name.sin_family = AF_INET;
  name.sin_addr.s_addr = htonl (INADDR_ANY);	/* Default local IP */
  name.sin_port = htons (porto);	/* if 0, port is defined by the system */
  if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {
      perror ("datagram socket binding failed");
      exit (1);
    }
  /* Multicast configuration */
  if (!inet_aton ("225.1.1.1", &imr.imr_multiaddr))
    {
      perror ("Failed conversion of multicast address");
      exit (1);
    }
  imr.imr_interface.s_addr = htonl (INADDR_ANY);	/* Default network device */
  /* Join group */
  if (setsockopt (sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &imr,
		  sizeof (struct ip_mreq)) == -1)
    {
      perror ("Failed association to IPv4 multicast group");
      abort ();
    }
  /* Configure socket to receive echo of sent multicast data */
  setsockopt (sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof (loop));
  /* Configura socket to only send multicast data to the local network */
  if (setsockopt (sock, IPPROTO_IP, IP_MULTICAST_TTL, (char *) &ttl,
		  sizeof (ttl)) < 0)
    {
      perror ("setsockopt IP_MULTICAST_TTL failed");
    }

  /* Get the socket's port number */
  length = sizeof (name);
  if (getsockname (sock, (struct sockaddr *) &name, &length))
    {
      perror ("Error getting the port number");
      exit (1);
    }
  printf ("The socket on IP Multicast 225.1.1.1 has the port #%hu\n",
	  ntohs (name.sin_port));
  /* Read a message from the socket */
  if (recvfrom (sock, buf, 1024, 0 /* By default it waits for data */ ,
		(struct sockaddr *) &fromaddr, &fromlen) < 0)
    perror ("Error receiving datagram packet");
  printf ("Received from %s#%hu -->%s\n", inet_ntoa (fromaddr.sin_addr),
	  ntohs (fromaddr.sin_port), buf);
  printf ("The server shutdown\n");
  close (sock);
  return 0;
}
