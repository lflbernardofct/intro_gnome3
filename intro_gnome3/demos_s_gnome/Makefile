#/*****************************************************************************\
# * Redes Integradas de Telecomunicacoes
# * MIEEC/MEEC - FCT NOVA  2024/2025
# *
# * Makefile
# *
# * Makefile for command line demonstration programs
# *
# * @author  Luis Bernardo
#\*****************************************************************************/
#

PROGS= \
	cliv4 servv4 cliv6 servv6 serv2v6 clitcpv6 servtcpv6 demofork demopthread

all: $(PROGS)

cliv4: cliv4.c
	gcc -Wall -g -o cliv4 cliv4.c

servv4: servv4.c
	gcc -Wall -g -o servv4 servv4.c

cliv6: cliv6.c
	gcc -Wall -g -o cliv6 cliv6.c

servv6: servv6.c
	gcc -Wall -g -o servv6 servv6.c

serv2v6: serv2v6.c
	gcc -Wall -g -o serv2v6 serv2v6.c

clitcpv6: clitcpv6.c
	gcc -Wall -g -o clitcpv6 clitcpv6.c

servtcpv6: servtcpv6.c
	gcc -Wall -g -o servtcpv6 servtcpv6.c

demofork: demofork.c
	gcc -Wall -g -o demofork demofork.c

demopthread: demopthread.c
	gcc -Wall -g -o demopthread demopthread.c -lpthread

clean:
	rm -f $(PROGS) core*

