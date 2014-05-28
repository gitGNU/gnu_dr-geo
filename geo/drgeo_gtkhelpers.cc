/*
 *  Dr Genius an interactive geometry software
 * (C) Copyright Hilaire Fernandes  2003
 * hilaire@ofset.org 
 * 
 *
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public Licences as by published
 * by the Free Software Foundation; either version 2; or (at your option)
 * any later version
 *
 * This program is distributed in the hope that it will entertaining,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILTY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Publis License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <gtk/gtk.h>
#include <errno.h>
#include <libintl.h>

#include "drgeo_gtkhelpers.h"
#include "drgenius_config.h"
#include "drgenius_mdi.h"

#define  _(x)  gettext (x)
#define N_(x)  x

extern drgeniusMDI* mdi;

gchar * 
gtk_text_view_get_text (GtkTextView *view)
{
  GtkTextBuffer *description;
  GtkTextIter start, end;

  description = gtk_text_view_get_buffer (view);
  gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (description),
				  &start);
  gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (description),
				&end);

  return gtk_text_buffer_get_text (GTK_TEXT_BUFFER (description),
				   &start, &end, false);
}

void 
gtk_text_view_clear (GtkTextView *view)
{
  GtkTextBuffer *description;
  GtkTextIter start, end;

  description = gtk_text_view_get_buffer (view);
  gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (description),
				  &start);
  gtk_text_buffer_get_end_iter (GTK_TEXT_BUFFER (description),
				&end);
  gtk_text_buffer_delete (GTK_TEXT_BUFFER (description),
			  &start, &end);

}

void 
gtk_text_view_set_text (GtkTextView *view, gchar *text)
{
  GtkTextBuffer *description;

  gtk_text_view_clear (view);
  description = gtk_text_view_get_buffer (view);
  gtk_text_buffer_set_text (description, (const gchar *) text, -1);  
}

gint 
open_help_cb (GtkWidget *widget, const char *url)
{
  gchar *str;
  static gchar *lang = NULL;
  GtkWidget *d;
  // Fixme write this

  if (lang == NULL)
    {
      lang = g_strndup (pango_language_to_string(gtk_get_default_language ())
			, 2);      
      str = g_strconcat(DRGEO_HELPDIR, "/", lang, NULL);
      if (!g_file_test (str, G_FILE_TEST_EXISTS))
	{
	  d = gtk_message_dialog_new (mdi->mainWindow (),
				      GTK_DIALOG_DESTROY_WITH_PARENT,
				      GTK_MESSAGE_ERROR,
				      GTK_BUTTONS_CLOSE,
				      _("This help directory does not exist:\n'%s'\nMaybe the manual is not installed or it is not yet translated in your language.\n\nIf you want to translate the manual contact ofset@ofset.org."),
				      str, g_strerror (errno));	
	  gtk_dialog_run (GTK_DIALOG (d));
	  gtk_widget_destroy (d);
	  
	  g_free (lang);
	  g_free (str);
	  

	  // Try to find the default one in C

	  str = g_strconcat (DRGEO_HELPDIR, "/c", NULL);

	  if (g_file_test (str, G_FILE_TEST_EXISTS))
	    {	  
	      d = gtk_message_dialog_new (mdi->mainWindow (),
					  GTK_DIALOG_DESTROY_WITH_PARENT,
					  GTK_MESSAGE_WARNING,
					  GTK_BUTTONS_CLOSE,
					  _("A default online manual\nin English will be used."),
					  g_strerror (errno));	
	      gtk_dialog_run (GTK_DIALOG (d));
	      gtk_widget_destroy (d);	      
	      lang = g_strdup ("c");
	    }
	  else
	    {
	      d = gtk_message_dialog_new (mdi->mainWindow (),
					  GTK_DIALOG_DESTROY_WITH_PARENT,
					  GTK_MESSAGE_WARNING,
					  GTK_BUTTONS_CLOSE,
					  _("No default online manual can be found."),
					  g_strerror (errno));	
	      gtk_dialog_run (GTK_DIALOG (d));
	      gtk_widget_destroy (d);	      
	      lang = NULL;
	      return false;
	    }
	}
      g_free (str);
    }

  str = g_strconcat (drgeoConfigGet(":htmlViewer"), " ",
		     DRGEO_HELPDIR, "/", lang, "/", url, NULL);

  g_spawn_command_line_async ((const gchar *) str, 
			      NULL);

  g_free (str);
  return true;
}

void 
gtk_menu_append_label (GtkMenu *menu, const gchar *textItem)
{
  GtkWidget *menuItem;

  menuItem = gtk_menu_item_new_with_label (textItem);
  gtk_widget_show (menuItem);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuItem);
}
