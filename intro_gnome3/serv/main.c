/*****************************************************************************\
 * Redes Integradas de Telecomunicacoes
 * MIEEC/MEEC - FCT NOVA  2024/2025
 *
 * main.c
 *
 * Main function that starts the GUI and loops in Gtk main loop.
 *
 * @author  Luis Bernardo
\*****************************************************************************/
#include <gtk/gtk.h>
#include <arpa/inet.h>
#include <assert.h>
#include <time.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui.h"
#include "callbacks.h"
#include "sock.h"


#define GLADE_FILE	"demoserv3.glade"

/* Public variables */
int sock = -1;				// socket descriptor
GIOChannel *chan = NULL;	// socket's IO channel descriptor
guint chan_id;				// IO Channel number
// Pointer to the structure with all the elements of the GUI
ServWindowElements *main_windows;	// Has pointers to all elements of main window


int main (int argc, char *argv[]) {
	/* allocate the memory needed by our ServWindowElements struct */
	main_windows = g_slice_new (ServWindowElements);

    /* init glib threads */
    gdk_threads_init ();
	/* initialize GTK+ libraries */
	gtk_init (&argc, &argv);

	if (GUI_init_app (main_windows, GLADE_FILE) == FALSE) return 1; /* error loading UI */

	/* Socket initialization */
	if ((sock = init_socket_ipv4 (SOCK_DGRAM, 20000, FALSE)) == -1)
		return 1;
	if (!put_socket_in_mainloop (sock, main_windows, &chan_id, &chan, G_IO_IN,
			callback_dados))
		return 2;

	gtk_widget_show (main_windows->window);
	/* get GTK thread lock */
    gdk_threads_enter ();
	gtk_main ();	// Launches Gtk main loop - loops forever until the end of the program
	/* release GTK thread lock */
	gdk_threads_leave ();

	/* free memory we allocated for ServWindowElements struct */
	g_slice_free (ServWindowElements, main_windows);

	return 0;
}

