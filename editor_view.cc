/* Dr Genius an interactive geometry software
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

#include <string.h>
#include <libintl.h>

#include "editor_view.h"
#include "drgeo_gtkhelpers.h"

#define  _(x)  gettext (x)
#define N_(x)  x

editorView::editorView (xmlNodePtr editorXml):
  drgeniusView (EDITOR_CHILD, true)
{
  static gint counter = 1;

  createWidgetView ();
  
  if (editorXml == NULL)
    {
      /* create a new name */
      p_name = g_strdup_printf (_("Text %d"), 
				counter++);
      gtk_text_view_clear (GTK_TEXT_VIEW (p_textView));
    }
  else
    {
      /* get the name from the XML Tree */		    
      p_name = (gchar *) (xmlGetProp (editorXml, BAD_CAST "name"));
      gtk_text_view_set_text (GTK_TEXT_VIEW (p_textView),
			      (gchar *) xmlNodeGetContent (editorXml));
    }  
}

editorView::
~editorView ()
{
  // FIXME: it should not be necessary
  gtk_widget_destroy (p_view);
}

gboolean
editorView::saveNode (xmlNodePtr tree)
{
  gchar *text;
  xmlNodePtr textNode;

  text = gtk_text_view_get_text (GTK_TEXT_VIEW (p_textView));
  if (strlen (text) != 0) {
    textNode = xmlNewChild (tree, NULL, BAD_CAST "text", BAD_CAST text);
    xmlSetProp (textNode, BAD_CAST "name", BAD_CAST p_name);
    g_free (text);
    return true;
  }	    
  else {
    g_free (text);
    return false;
  }
}

gpointer
editorView::createWidgetView ()
{
  p_view = gtk_scrolled_window_new (NULL, NULL);
  p_textView = gtk_text_view_new ();
  gtk_text_view_set_editable(GTK_TEXT_VIEW (p_textView),
			     TRUE);
  gtk_container_add (GTK_CONTAINER(p_view), p_textView);

  gtk_widget_show_all (p_view);

  /* No WidgetView class is created, only plain GtkWidget */
  return NULL;
}

GtkWidget *
editorView::widgetView ()
{
  return p_view;
}
