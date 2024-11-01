#/*****************************************************************************\
# * Redes Integradas de Telecomunicacoes
# * MIEEC/MEEC - FCT NOVA  2024/2025
# *
# * Makefile
# *
# * Makefile for democli (UDP) project
# *
# * @author  Luis Bernardo
#\*****************************************************************************/

APP_NAME= democli

GNOME_INCLUDES= `pkg-config --cflags --libs gtk+-3.0`
CFLAGS= -Wall -Wno-deprecated-declarations -g

all: $(APP_NAME)
	
clean: 
	rm -f $(APP_NAME) *.o


$(APP_NAME): main.c sock.o gui_g3.o callbacks.o gui.h sock.h callbacks.h
	gcc $(CFLAGS) -o $(APP_NAME) main.c sock.o gui_g3.o callbacks.o $(GNOME_INCLUDES) -export-dynamic

sock.o: sock.c sock.h gui.h
	gcc $(CFLAGS) -c $(GNOME_INCLUDES) sock.c -export-dynamic

gui_g3.o: gui_g3.c gui.h
	gcc $(CFLAGS) -c $(GNOME_INCLUDES) gui_g3.c -export-dynamic
	
callbacks.o: callbacks.c callbacks.h
	gcc $(CFLAGS) -c $(GNOME_INCLUDES) callbacks.c -export-dynamic
	

