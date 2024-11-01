#/*****************************************************************************\
#* Redes Integradas de Telecomunicacoes
#* MIEEC/MEEC - FCT NOVA  2024/2025
# *
# * Makefile
# *
# * Makefile for demo_glade_gtk3 projects
# *
# * @author  Luis Bernardo
#\*****************************************************************************/

MAKE=\
	/usr/bin/make

SUBDIRS=\
	serv cli demos_s_gnome tcpcli tcpserv

all: all-recursive

clean: clean-recursive

depend: depend-recursive


all-recursive:
	for i in $(SUBDIRS); do ( cd $$i; $(MAKE) all; ) done

clean-recursive:
	for i in $(SUBDIRS); do ( cd $$i; $(MAKE) clean; ) done

depend-recursive:
	for i in $(SUBDIRS); do ( cd $$i; $(MAKE) depend; ) done

	
