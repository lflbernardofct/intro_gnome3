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
#include "callbacks.h"
#include "sock.h"
#include "gui.h"
#include <time.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define GLADE_FILE	"democli3.glade"

/* Public variables */
int sock = -1;					// socket descriptor
// Pointer to the structure with all the elements of the GUI
CliWindowElements *main_windowc;	// Has pointers to all elements of main window


int main (int argc, char *argv[]) {
	/* allocate the memory needed by our CliWindowElements struct */
	main_windowc = g_slice_new (CliWindowElements);


    /* init glib threads */
    gdk_threads_init ();
	/* initialize GTK+ libraries */
	gtk_init (&argc, &argv);

	if (GUI_init_app (main_windowc, GLADE_FILE) == FALSE) return 1; /* error loading UI */

	gtk_widget_show (main_windowc->window1);
	/* get GTK thread lock */
    gdk_threads_enter ();
	gtk_main ();	// Launches Gtk main loop - loops forever until the end of the program
	/* release GTK thread lock */
	gdk_threads_leave ();

	/* free memory we allocated for CliWindowElements struct */
	g_slice_free (CliWindowElements, main_windowc);

	return 0;
}

