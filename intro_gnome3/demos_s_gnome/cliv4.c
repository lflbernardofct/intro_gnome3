/*****************************************************************************\
 * Redes Integradas de Telecomunicacoes
 * MIEEC/MEEC - FCT NOVA  2024/2025
 *
 * cliv4.c
 *
 * Command line client for UDP IPv4
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

#define DATA "Sent using UDP IPv4 ..."	/* Static message */

/*
 * This program sends an IPv4 datagram to a receiver provided in the command line
 */

int main (int argc, char *argv[])
{
  int sock;
  struct sockaddr_in name;
  struct hostent *hp;
  u_char ttl = 1;		/* rede local */

  /* Create socket */
  sock = socket (AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
    {
      perror ("Erro na abertura de socket datagrama");
      exit (1);
    }
  /*
   * Creates name of destination socket. gethostbyname retorns a struct
   * that includes the destination IPv4 address, supporting "pc-1" or "10.1.55.1" addresses.
   * inet_aton or inet_pton could also be used for "10.1.55.1" addresses.
   * The port is obtained from the command line
   */
  if (argc <= 2)
    {
      fprintf (stderr, "Usage: %s ip porto\n", argv[0]);
      exit (2);
    }
  hp = gethostbyname (argv[1]);
  if (hp == 0)
    {
      fprintf (stderr, "%s: unkonwn address\n", argv[1]);
      exit (2);
    }
  bcopy (hp->h_addr, &name.sin_addr, hp->h_length);
  name.sin_family = AF_INET;
  name.sin_port = htons (atoi (argv[2]));	/* convert to network format */
  /* Configure socket to send datagrams to the local network only for multicast addresses (TTL=1) */
  if (setsockopt (sock, IPPROTO_IP, IP_MULTICAST_TTL, (char *) &ttl,
		  sizeof (ttl)) < 0)
    {
      perror ("setsockopt IP_MULTICAST_TTL failed");
    }
  /* Send message */
  fprintf (stderr, "Sending packet to %s ; %d\n",
	   inet_ntoa(name.sin_addr), (int) ntohs (name.sin_port));
  if (sendto (sock, DATA, strlen (DATA) + 1 /* to send '\0' */ , 0,
	      (struct sockaddr *) &name, sizeof (name)) < 0)
    perror ("Error sending datagrama");
  close (sock);
  return 0;
}
