/*****************************************************************************\
 * Redes Integradas de Telecomunicacoes
 * MIEEC/MEEC - FCT NOVA  2024/2025
 *
 * callbacks.h
 *
 * Constants, vars and functions of the application logic, including socket
 *    and button callbacks
 *
 * @author  Luis Bernardo
\*****************************************************************************/
#ifndef INCL_CALLBACKS_H
#define INCL_CALLBACKS_H

#include <gtk/gtk.h>

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

// Maximum message length
#define MAX_MESSAGE_LENGTH    5000


/**** Global variables ****/

// Variables declared and initialized in 'main.c'
extern int sock;			// socket descriptor
extern GIOChannel *chan;	// socket's IO channel descriptor
extern guint chan_id;		// IO Channel number


/**** Global functions ****/

/* Callback function that handles reading events from the socket.
 * It returns TRUE to keep the callback active, and FALSE to disable the callback */
gboolean
callback_dados (GIOChannel * source, GIOCondition condition, gpointer data);

// GUI callback function called when the main window is closed
gboolean
on_window1_delete_event (GtkWidget * widget,
		GdkEvent * event, gpointer user_data);

#endif
