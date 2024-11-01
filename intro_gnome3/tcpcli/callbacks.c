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
#include "callbacks.h"
#include "sock.h"
#include <time.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include "gui.h"


// Function used to send a TCP message with 'entryText' contents to the address in 'entryIP'
void send_message_ipv4 ( const gchar * ip )
{
	const gchar *textText;
	const gchar *textIP;
	struct in_addr addr;
	int n;

	if (ip == NULL)
	{
		/* Reads IP address from entryIP box */
		textIP = gtk_editable_get_chars (GTK_EDITABLE (main_windowc->entryIP), 0, -1);
		/* Tests if it is a valid address converting it into binary format:
		 *     addr contains the binary address (format struct in_addr) */
		if (!inet_aton (textIP, &addr))
		{
			Log ("Invalid IP address\n");
			return;
		}
	}
	else
		textIP = ip;	// Uses the address received as argument

	/* Read text string to send */
	textText = gtk_editable_get_chars (GTK_EDITABLE (main_windowc->entryText), 0, -1);
	/* Tests text */
	if ((textText == NULL) || !strlen (textText))
	{
		Log ("Empty text\n");
		return;
	}

	struct sockaddr_in name;
	struct hostent *hp;

	/* Defines destination address */
	hp = gethostbyname(textIP);
	if (hp == 0) {
		perror("Invalid destination address");
		return;
	}
	// Prepares struct sockaddr_in variable 'name', with destination data
	bcopy(hp->h_addr, &name.sin_addr, hp->h_length); // define IP
	name.sin_port = htons(20000); // define Port
	name.sin_family = AF_INET; // define IPv4

	/* Socket initialization */
	if ((sock = init_socket_ipv4 (SOCK_STREAM, 0, FALSE)) == -1) {
		perror("socket creation");
		return;
	}


	/* Connect to remote host */
	  if (connect (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
	    {
	      perror ("connecting stream socket");
	      close(sock);
	      return;
	    }
	  /* Send message */
	  n= write (sock, textText, strlen (textText) + 1);
	  if (n < 0)
	    perror ("writing on stream socket");
	  /* Remember that for large files n may be shorter than the message length: */
	  /*     if n is shorter, the remaining bytes must be stored to be sent later */
	  /*     when a G_IO_OUT event is received, signaling that there is space to write */
	  /* Close socket */
	  close (sock);

	/* Writes message into sending tableview */
	GtkListStore *list= main_windowc->liststore1;
	GtkTreeIter iter;
	/* Gets local time */
	time_t tbuf;
	time(&tbuf);
	char *time_buf= strdup(ctime (&tbuf));
	time_buf[strlen(time_buf)-1]= '\0';
	// Adds entry to the data store associated to the tableview
	gtk_list_store_append(list, &iter);
	gtk_list_store_set(list, &iter, 0, textIP, 1, time_buf, -1);
	// Frees temporary memory
	free(time_buf);
}


// GUI callback function called when 'Send' button is clicked
void on_buttonSend_clicked (GtkButton * button, gpointer user_data)
{
	// user_data - not used
	// This event is asynchronous - we'll use the semaphore to synchronize with the GUI thread
	send_message_ipv4 (NULL);
}

// Timer callback used to implement delayed message sending
gboolean callback_timer (gpointer data)
{
	// data - not used
	send_message_ipv4 (NULL);
	return FALSE;		// turns timer off after the first time
	// It would continue running periodically if it returned TRUE
}

// GUI callback function called when 'Delayed Send' button is clicked
void on_buttonDelayedSend_clicked (GtkButton * button, gpointer user_data)
{
	// user_data - not used
	const gchar *textDelay;
	guint Delay = 0;
	char *pt;

	/* Gets text from entry box */
	textDelay = gtk_editable_get_chars (GTK_EDITABLE (main_windowc->entrymSec), 0, -1);
	/* tests if text is valid */
	if ((textDelay == NULL) || (strlen (textDelay) == 0))
	{
		Log ("Undefined number of mseconds\n");
		return;
	}
	/* Converts to integer */
	Delay = strtoul (textDelay, &pt, 10);
	if ((pt == NULL) || (*pt))
	{
		Log ("Invalid number of mseconds\n");
		return;
	}
	/* Delays sending message - starts timer */
	g_timeout_add (Delay, callback_timer, user_data);
	/* Note that  g_timeout_add returns an unsigned int:
	 * 		guint id = g_timeout_add(..);
	 * That should be stored, since it is required to cancel a timer, e.g.
	 * 		g_source_remove(id);
	 * Would be used to cancel the timer.
	 */
}

// GUI callback function called when 'Resend' button is clicked
void on_buttonResend_clicked (GtkButton * button, gpointer user_data)
{
	GtkTreeSelection *selection;
	GtkTreeModel	*model;
	GtkTreeIter	iter;
	gchar *ip;

	selection= gtk_tree_view_get_selection(main_windowc->treeview1);
	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		gtk_tree_model_get (model, &iter, 0, &ip, -1);
		g_print ("Selected ip is: %s\n", ip);
	} else {
		Log ("No line selected\n");
		return;
	}

	// Resend message
	send_message_ipv4 (ip);
	g_free(ip);
}


// Module variable used within function foreach_func, to compare the address value
static const gchar *remove_ip= NULL;

// Callback function called by 'gtk_tree_model_foreach' for all members of table
// It is used to remove multiple rows in one go, returned in the list 'rowref_list'
gboolean
foreach_func (GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, GList **rowref_list)
{
	const gchar *ip;
	if (remove_ip == NULL)
		return TRUE;	// Stop walking the store
	g_assert ( rowref_list != NULL );
	gtk_tree_model_get (model, iter, 0, &ip, -1);
	if ( !strcmp(ip, remove_ip) ) {
		GtkTreeRowReference *rowref;
		rowref = gtk_tree_row_reference_new(model, path);
		*rowref_list = g_list_append(*rowref_list, rowref);
	}
	return FALSE; /* do not stop walking the store, call us with next row */
}


// GUI callback function called when 'Remove IP' button is clicked
void on_buttonRemove_clicked (GtkButton * button, gpointer user_data)
{
	const gchar *textRIP;
	struct in_addr addr;

	/* Reads text in 'RemoveIP' edit box */
	textRIP = gtk_editable_get_chars (GTK_EDITABLE (main_windowc->entryRemIP), 0, -1);
	/* Tests if it is a valid address - converts it to binary format */
	if (!inet_aton (textRIP, &addr))
	{
		Log ("Invalid IP address in Remove\n");
		return;
	}
	remove_ip= textRIP;

	GList *rr_list = NULL;	/* list of GtkTreeRowReferences to remove */
	GList *node;
	gtk_tree_model_foreach(GTK_TREE_MODEL(main_windowc->liststore1), (GtkTreeModelForeachFunc) foreach_func, &rr_list);
	for ( node = rr_list; node != NULL; node = node->next ) {
		GtkTreePath *path;
		path = gtk_tree_row_reference_get_path((GtkTreeRowReference*)node->data);
		if (path) {
			GtkTreeIter iter;
			if (gtk_tree_model_get_iter(GTK_TREE_MODEL(main_windowc->liststore1), &iter, path)) {
				gtk_list_store_remove(main_windowc->liststore1, &iter);
			}
			gtk_tree_path_free (path);
		}
	}
	g_list_foreach(rr_list, (GFunc) gtk_tree_row_reference_free, NULL);
	g_list_free(rr_list);

	remove_ip= NULL;

	/*  An alternative, and less efficient, way would be to iterate the table members one by one,
	 * and remove one at a time:

	  GtkTreeModel *list_store= GTK_TREE_MODEL(main_window->liststore1);
	  GtkTreeIter iter;
	  gboolean valid, deleted;
	  gint row_count = 0;

	  do {
		  deleted= FALSE;

		  // Get the first iter in the list
		  valid = gtk_tree_model_get_iter_first (list_store, &iter);

		  while (valid)
		  {
			  // Walk through the list, reading each row
			  gchar *str_ip, *str_data;

			  // Make sure you terminate calls to gtk_tree_model_get() with a '-1' value
			  gtk_tree_model_get (list_store, &iter,
					  0, &str_ip,
					  1, &str_data,
					  -1);

			  // Do something with the data
			  g_print ("Row %d: (%s,%s)\n", row_count, str_ip, str_data);
			  if (!strcmp(str_ip,textRIP)) {
				  g_free (str_ip);
				  g_free (str_data);
				  deleted= TRUE;
				  break;
			  }
			  g_free (str_ip);
			  g_free (str_data);


			  row_count ++;
			  valid = gtk_tree_model_iter_next (list_store, &iter);
		  }

		  if (deleted) {
			  if (!gtk_list_store_remove(main_window->liststore1, &iter)) {
				  Log("Failed to remove element\n");
				  deleted= FALSE;
			  }
		  }

	  } while (deleted && valid);
	 */
}


// GUI callback function called when the main window is closed
gboolean
on_window1_delete_event (GtkWidget * widget,
		GdkEvent * event, gpointer user_data)
{
	gtk_main_quit ();	// Finishes the application, terminating Gtk main loop
	return FALSE;		// Must always return FALSE, otherwise it does not close the window
}
