/*****************************************************************************\
 * Redes Integradas de Telecomunicacoes
 * MIEEC/MEEC - FCT NOVA  2024/2025
 *
 * gui.h
 *
 * Graphics user interface supporting functions
 *
 * @author  Luis Bernardo
\*****************************************************************************/
#ifndef INCL_GUI_H
#define INCL_GUI_H

#include <gtk/gtk.h>

/* store the widgets which may need to be accessed in a typedef struct */
typedef struct
{
        GtkWidget               *window1;
        GtkEntry				*entryIP;
        GtkEntry				*entryText;
        GtkEntry				*entryRemIP;
        GtkEntry				*entrymSec;
        GtkTextView             *textview1;
        GtkTreeView				*treeview1;
        GtkListStore			*liststore1;
} CliWindowElements;

/**** Global variables ****/

// Pointer to the structure with all the elements of the GUI
extern CliWindowElements *main_windowc;	// Pointers to all elements of main window


/**** Global methods ****/

// Handles 'Clear' button - clears the text box and the table with sent messages
void on_buttonClear_clicked (GtkButton * button, gpointer user_data);

// Writes a message in the screen and in the command line
void Log (const gchar * str);

// We call error_message() any time we want to display an error message to the
// user. It will both show an error dialog and log the error to the terminal window.
void GUI_error_message (const gchar *message);

// Initializes all the windows and graphical callbacks of the application
gboolean
GUI_init_app (CliWindowElements *window, const char *glade_file);

#endif
