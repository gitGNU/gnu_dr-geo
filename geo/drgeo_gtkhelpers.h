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

#ifndef DRGEO_GTKHELPERS_H
#define DRGEO_GTKHELPERS_H

// Return an allocated UTF-8 string
gchar * gtk_text_view_get_text (GtkTextView *view);
void gtk_text_view_clear (GtkTextView *view);
// Clear the text in the view and set a new text
void gtk_text_view_set_text (GtkTextView *view, gchar *text);
gint open_help_cb (GtkWidget *widget, const char *url);
void gtk_menu_append_label (GtkMenu *menu, const gchar *textItem);
#endif /* DRGEO_GTKHELPERS_H */
