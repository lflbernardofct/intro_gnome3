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


/* Callback function that handles reading events from the UDP socket.
 * It returns TRUE to keep the callback active, and FALSE to disable the callback */
gboolean
callback_dados (GIOChannel * source, GIOCondition condition, gpointer data)
{
	static char write_buf[1024];
	static char buf[MAX_MESSAGE_LENGTH];	// buffer for reading data
	struct in_addr ip;
	short unsigned int porto;
	int n;

	if (condition == G_IO_IN)
	{
		/* Read data from the socket */
		n = read_data_ipv4 (sock, buf, MAX_MESSAGE_LENGTH, &ip, &porto);
		if (n <= 0)
		{
			Log ("Read from socket failed\n");
			return TRUE;		// Keeps waiting for more data
		}
		else
		{
			time_t tbuf;
			short unsigned int m;
			char *pt;
			/* Writes date and sender of the packet */
			time (&tbuf);		// Gets current date
			sprintf (write_buf, "%sReceived %d bytes from %s:%hu\n",
					ctime (&tbuf), n, inet_ntoa (ip), porto);
			Log (write_buf);
			/* Read the message fields */
			pt = buf;
			READ_BUF (pt, &m, sizeof(m));	// Reads short (2 bytes) to m and moves pointer
											// pt points to the third byte
			m = ntohs (m);	// Converts the number to host format
			if (m != n - 2)
			{
				sprintf (write_buf,
						"Invalid 'length' field (%d != %d)\n", m,
						n - 2);
				Log (write_buf);
				return TRUE;	// Keeps waiting for more data
			}
			/* Writes data to the memo box - assumes that it ends with '\0' */
			Log (pt);	// pt points to the first byte after the length, the begin of a string
			Log ("\n");
			return TRUE;	// Keeps waiting for more data
		}

	}
	else if ((condition == G_IO_NVAL) || (condition == G_IO_ERR))
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


// GUI callback function called when the main window is closed
gboolean
on_window1_delete_event (GtkWidget * widget,
		GdkEvent * event, gpointer user_data)
{
	gtk_main_quit ();	// Finishes the application, terminating Gtk main loop
	return FALSE;		// Must always return FALSE, otherwise it does not close the window
}




