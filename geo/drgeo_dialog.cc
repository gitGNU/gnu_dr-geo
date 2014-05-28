/* Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-1999
 * hilaire.fernandes@iname.com 
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
#include <string.h>
#include <libintl.h>

#include "drgeo_dialog.h"
#include "drgeo_drawable.h"
#include "drgeo_gtkdrawable.h"
#include "drgeo_numeric.h"
#include "drgenius_mdi.h"

extern drgeniusMDI *mdi;

// To enter free value
static GtkWidget *drgeoEditDialog = NULL, *drgeoEntry;

// Used in the style dialod callback, I know it's ugly, but so easy
static drgeoFigure *selected_figure;

DialogData drgeoDialogData;


static void drgeo_edit_dialog_cb (GtkWidget * dialog,
				  gint button, gpointer entry);

void
get_edited_value ()
{
  // build the drgeo Edit Dialog


  drgeoEditDialog = gtk_message_dialog_new (NULL,
					    GTK_DIALOG_DESTROY_WITH_PARENT,
					    GTK_MESSAGE_QUESTION,
					    GTK_BUTTONS_OK_CANCEL,
					    _("Enter a value"));
  mdi->setTransientDialog (GTK_WINDOW (drgeoEditDialog));


  drgeoEntry = gtk_entry_new  ();

  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (drgeoEditDialog)->vbox),
		      drgeoEntry, TRUE, TRUE, 2);
  
  
  g_signal_connect (GTK_OBJECT (drgeoEditDialog), "response",
		    G_CALLBACK (drgeo_edit_dialog_cb),
		    GTK_OBJECT (drgeoEntry));
  
  gtk_widget_show_all (drgeoEditDialog);

}


static void
drgeo_edit_dialog_cb (GtkWidget * dialog, gint response, gpointer entry)
{
  geometricObject *item;
  gdouble value;

  switch (response)
    {
    case GTK_RESPONSE_OK:
      value =  atof (gtk_entry_get_text (GTK_ENTRY (entry)));      
      item = new numeric (drgeoDialogData.mouse, value,
			  (numericType) FREE_VALUE, FALSE);
      (drgeoDialogData.figure)->createItem (item);
      //Just go throught, this is what we want
    case GTK_RESPONSE_CANCEL:
    case GTK_RESPONSE_DELETE_EVENT:
      gtk_widget_destroy (dialog);      
      break;
    }
}
