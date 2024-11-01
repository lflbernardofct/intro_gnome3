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

/**** Constants ****/

// Maximum message length
#define MAX_MESSAGE_LENGTH    5000

/**** Global variables ****/

// Socket descriptor
extern int sock;


/**** Global functions ****/

// Function used to send a TCP message with 'entryText' contents to the address in 'entryIP'
void send_message_ipv4 (const gchar * ip);

// GUI callback function called when 'Send' button is clicked
void on_buttonSend_clicked (GtkButton * button, gpointer user_data);

// GUI callback function called when 'Delayed Send' button is clicked
void on_buttonDelayedSend_clicked (GtkButton * button, gpointer user_data);

// GUI callback function called when 'Resend' button is clicked
void on_buttonResend_clicked (GtkButton * button, gpointer user_data);

// GUI callback function called when 'Remove IP' button is clicked
void on_buttonRemove_clicked (GtkButton * button, gpointer user_data);

// GUI callback function called when the main window is closed
gboolean on_window1_delete_event (GtkWidget * widget,
			 GdkEvent * event, gpointer user_data);

#endif
