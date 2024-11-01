/*****************************************************************************\
 * Redes Integradas de Telecomunicacoes
 * MIEEC/MEEC - FCT NOVA  2024/2025
 *
 * callbacks.c
 *
 * Implements the application logic, including socket and button callbacks
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
#include "sock.h"
#include "callbacks.h"


/* Callback function that handles reading events from the TCP data socket.
 * It returns TRUE to keep the callback active, and FALSE to disable the callback */
gboolean
callback_data (GIOChannel * source, GIOCondition condition, gpointer data)
{
	static char write_buf[1024];
	static char buf[MAX_MESSAGE_LENGTH];	// buffer for reading data
	int n;
	int s= g_io_channel_unix_get_fd(source); // Gets the socket file descriptor

	if (condition & G_IO_IN)
	{
		/* Read data from the socket */
		n = recv(s, buf, MAX_MESSAGE_LENGTH, 0);
		if (n < 0)
		{
			perror("read failed");
			Log ("Read from socket failed\n");
			free_gio_channel(source);
			return FALSE;		// Removes socket's callback from main cycle
		}
		else if (n == 0)
		{
			/* Reached end of connection */
			sprintf(write_buf, "Connection %d closed\n", s);
			Log(write_buf);
			free_gio_channel(source);
			return FALSE;		// Removes socket's callback from main cycle
		} else {

			time_t tbuf;
			/* Writes date and sender of the packet */
			time (&tbuf);		// Gets current date
			sprintf (write_buf, "%sReceived %d bytes from socket %d:\n",
					ctime (&tbuf), n, s);
			Log (write_buf);
			Log(buf);	// Write the message received
			Log("\n");
			return TRUE;	// Keeps waiting for more data
		}

	}
	else if ((condition & G_IO_NVAL) || (condition & G_IO_ERR))
	{
		Log ("Detected socket error\n");
		remove_socket_from_mainloop (sock, chan_id, chan);
		chan = NULL;
		close_socket (sock);
		sock = -1;
		/* Stops the application */
		gtk_main_quit ();
		return FALSE;	// Removes socket's callback from main cycle
	}
	else
	{
		assert (0);		// Must never reach this line - aborts application with a core dump
		return FALSE;	// Removes socket's callback from main cycle
	}
}


/* Callback function that handles reading events from the TCP connect socket.
 * It returns TRUE to keep the callback active, and FALSE to disable the callback */
gboolean
callback_connection (GIOChannel *source, GIOCondition condition, gpointer data)
{
	static char write_buf[1024];
	struct sockaddr_in server;
	int msgsock;
	unsigned int length= sizeof(server);
	GIOChannel *dchan = NULL;	// socket's IO channel descriptor
	guint dchan_id;				// IO Channel number

	if (condition & G_IO_IN)
	{
		time_t tbuf;

		time (&tbuf);		// Gets current date

		/* Accept the connection and create a new socket */
		msgsock = accept (sock, (struct sockaddr *) &server, &length);
		if (msgsock == -1) {
			perror("accepting connection");
			return TRUE;
		}
		sprintf(write_buf, "%sConnection from %s - %hu in %d\n", ctime (&tbuf),
				addr_ipv4 (&server.sin_addr), ntohs (server.sin_port), msgsock);
		Log(write_buf);
		/* Prepare new callback for reading incoming data */
		if (!put_socket_in_mainloop (msgsock, main_windows, &dchan_id, &dchan, G_IO_IN,
					callback_data)) {
			Log("Failed to set data callback\n");
			close(msgsock);
		}
		// Wait for G_IO_IN event!
		return TRUE;
	}
	else if ((condition & G_IO_NVAL) || (condition & G_IO_ERR))
	{
		Log ("Detected server socket error\n");
		// It should free the GIO device before removing it
		return FALSE;	// Removes socket's callback from main cycle
	} else {
		assert(0);
		return FALSE;
	}
}


// GUI callback function called when the main window is closed
gboolean
on_window1_delete_event (GtkWidget * widget,
		GdkEvent * event, gpointer user_data)
{
	gtk_main_quit ();	// Finishes the application, terminating Gtk main loop
	return FALSE;		// Must always return FALSE, otherwise it does not close the window
}




