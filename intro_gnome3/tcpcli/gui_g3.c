/*****************************************************************************\
 * Redes Integradas de Telecomunicacoes
 * MIEEC/MEEC - FCT NOVA  2024/2025
 *
 * gui_g3.c
 *
 * Implements the GUI supporting functions for Gtk+3
 *
 * @author  Luis Bernardo
\*****************************************************************************/
#include <gtk/gtk.h>
#include <arpa/inet.h>
#include <assert.h>
#include "gui.h"
#include "callbacks.h"
#include "sock.h"
#include <time.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>


// Handles 'Clear' button - clears the text box and the table with sent messages
void on_buttonClear_clicked (GtkButton * button, gpointer user_data)
{
	GtkTextBuffer *textbuf;
	GtkTextIter tbegin, tend;

	// Clear table with messages sent
	gtk_list_store_clear(main_windowc->liststore1);

	// Clear TextView
	textbuf = GTK_TEXT_BUFFER (gtk_text_view_get_buffer (main_windowc->textview1));
	gtk_text_buffer_get_iter_at_offset (textbuf, &tbegin, 0);
	gtk_text_buffer_get_iter_at_offset (textbuf, &tend, -1);
	gtk_text_buffer_delete (textbuf, &tbegin, &tend);
}


// Writes a message in the screen and in the command line
void Log (const gchar * str)
{
	GtkTextBuffer *textbuf;
	GtkTextIter tend;

	textbuf = GTK_TEXT_BUFFER (gtk_text_view_get_buffer (main_windowc->textview1));
	//Gets a reference to the last position in textbox and adds the message.
	gtk_text_buffer_get_iter_at_offset (textbuf, &tend, -1);
	gtk_text_buffer_insert (textbuf, &tend, g_strdup (str), strlen (str));
}

/*
We call error_message() any time we want to display an error message to the
user. It will both show an error dialog and log the error to the terminal
window.
 */
void
error_message (const gchar *message)
{
	GtkWidget               *dialog;

	/* log to terminal window */
	g_warning ("%s", message);

	/* create an error message dialog and display modally to the user */
	dialog = gtk_message_dialog_new (NULL,
			GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_ERROR,
			GTK_BUTTONS_OK,
			"%s", message);

	gtk_window_set_title (GTK_WINDOW (dialog), "Error!");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}


// Initializes all the windows and graphical callbacks of the application
// The GUI is loaded using GtkBuilder to read the XML file created using Glade.
gboolean
GUI_init_app (CliWindowElements *window, const char *glade_file)
{
	GtkBuilder              *builder;
	GError                  *err=NULL;
	//PangoFontDescription    *font_desc;

	/* use GtkBuilder to build our interface from the XML file */
	builder = gtk_builder_new ();
	if (gtk_builder_add_from_file (builder, glade_file, &err) == 0)
	{
		error_message (err->message);
		g_error_free (err);
		return FALSE;
	}

	/* get the widgets which will be referenced in callbacks */
	window->window1 = GTK_WIDGET (gtk_builder_get_object (builder,
			"window1"));
	window->entryIP = GTK_ENTRY (gtk_builder_get_object (builder,
			"entryIP"));
	window->entryText = GTK_ENTRY (gtk_builder_get_object (builder,
			"entryText"));
	window->entryRemIP = GTK_ENTRY (gtk_builder_get_object (builder,
			"entryRemIP"));
	window->entrymSec = GTK_ENTRY (gtk_builder_get_object (builder,
			"entrymSec"));
	window->textview1 = GTK_TEXT_VIEW (gtk_builder_get_object (builder,
			"textview1"));
	window->treeview1 = GTK_TREE_VIEW (gtk_builder_get_object (builder,
			"treeview1"));
	window->liststore1 = GTK_LIST_STORE(gtk_builder_get_object (builder,
			"liststore1"));
	/* connect signals, passing our WindowElements struct as user data */
	gtk_builder_connect_signals (builder, window);

	/* free memory used by GtkBuilder object */
	g_object_unref (G_OBJECT (builder));

	/* set the text view font */
	//font_desc = pango_font_description_from_string ("monospace 10");
	//gtk_widget_modify_font (GTK_WIDGET(window->textview1), font_desc);
	//pango_font_description_free (font_desc);

	return TRUE;
}
