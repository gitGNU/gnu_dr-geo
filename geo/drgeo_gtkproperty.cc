/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  2001
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

#include <stdio.h>

#include "drgeo_gtkproperty.h"
#include "drgeo_gtkdrawable.h"
#include "drgeo_numeric.h"
#include "drgeo_command.h"
#include "drgeo_point.h"
#include "drgeo_script.h"
#include "drgeo_gtkhelpers.h"
#include "drgenius_mdi.h"

extern drgeniusMDI *mdi;
extern const char * drgeniusHelp[];

// Callback for the free point property dialog
static void
on_freePointAbscissaEntry_activate (GtkWidget * widget, gpointer data)
{
  drgeoVector v (0, 0);
  drgeoGtkPropertyDialog *dialog;
  point *item;
  gdouble val;

  val = strtod (gtk_entry_get_text (GTK_ENTRY (widget)), NULL);

  dialog = (drgeoGtkPropertyDialog *) data;
  item = (point *) dialog->getItem ();
  v.setX (val - item->getCoordinate ().getX ());
  dialog->moveItem (v);
}

static void
on_freePointOrdinateEntry_activate (GtkWidget * widget, gpointer data)
{
  drgeoVector v (0, 0);
  drgeoGtkPropertyDialog *dialog;
  point *item;
  gdouble val;

  val = strtod (gtk_entry_get_text (GTK_ENTRY (widget)), NULL);

  dialog = (drgeoGtkPropertyDialog *) data;
  item = (point *) dialog->getItem ();
  v.setY (val - item->getCoordinate ().getY ());
  dialog->moveItem (v);
}

static void
movePointWithEntriesValues (GtkWidget * widget,
			    drgeoGtkPropertyDialog * dialog)
{

  GtkEntry *abscissa, *ordinate;
  drgeoVector v;
  point *item;
  gdouble val1, val2;

  abscissa = (GtkEntry *) gtk_object_get_data (GTK_OBJECT (widget),
					       "abscissaEntry");
  ordinate = (GtkEntry *) gtk_object_get_data (GTK_OBJECT (widget),
					       "ordinateEntry");
  val1 = strtod (gtk_entry_get_text (abscissa), NULL);
  val2 = strtod (gtk_entry_get_text (ordinate), NULL);

  item = (point *) dialog->getItem ();
  v.set (val1 - item->getCoordinate ().getX (),
	 val2 - item->getCoordinate ().getY ());
  dialog->moveItem (v);
}

static void
on_freePointDialog_reponse (GtkDialog *d, gint r, gpointer data)
{
  drgeoGtkPropertyDialog *dialog;

  dialog = (drgeoGtkPropertyDialog *) data;

  switch (r)
    {
    case GTK_RESPONSE_OK:
      movePointWithEntriesValues (GTK_WIDGET (d), dialog);
      dialog->hide ();
      break;
    case GTK_RESPONSE_APPLY:
      movePointWithEntriesValues (GTK_WIDGET (d), dialog);
      break;
    case GTK_RESPONSE_CANCEL:
      dialog->hide ();
      break;
    }
}

// Callback for the edit value property dialog
static void
on_editValueEntry_activate (GtkWidget * widget, gpointer data)
{
  drgeoGtkPropertyDialog *dialog;
  gdouble val;

  val = g_ascii_strtod (gtk_entry_get_text (GTK_ENTRY (widget)), NULL);

  dialog = (drgeoGtkPropertyDialog *) data;
  dialog->setValue (val);
}

static void
on_editValueDialog_response (GtkWidget * widget, gint b, gpointer data)
{
  drgeoGtkPropertyDialog *drgeoDialog;
  drgeoDialog = (drgeoGtkPropertyDialog *) data;
  widget = GTK_WIDGET (gtk_object_get_data (GTK_OBJECT (widget),
					    "valueEntry"));
  switch (b)
    {
    case GTK_RESPONSE_OK:
      // Ok
      on_editValueEntry_activate (widget, drgeoDialog);
      drgeoDialog->hide ();
      break;
    case GTK_RESPONSE_APPLY:
      // Apply
      on_editValueEntry_activate (widget, drgeoDialog);
      break;
    case GTK_RESPONSE_CANCEL:
      // Cancel
      drgeoDialog->hide ();
      break;
    }
}

// When the user close the window containing the dialog
static gint
on_propertyDialog_delete (GtkWidget * widget, GdkEventAny * e, gpointer data)
{
  drgeoGtkPropertyDialog *dialog;
  dialog = (drgeoGtkPropertyDialog *) data;
  dialog->hide ();
  return TRUE;
}

// Callback for the script edit dialog
static void
on_scriptDialog_response (GtkWidget *widget, gint b, 
			 drgeoGtkPropertyDialog *drgeoDialog)
{
  GtkTextView *text;
  gchar *code;

  text = (GtkTextView *) gtk_object_get_data (GTK_OBJECT (widget),
					      "script");
  code = gtk_text_view_get_text (text);

  switch (b)
    {
    case GTK_RESPONSE_OK:
      // Ok
      drgeoDialog->setScript (code);
      drgeoDialog->hide ();
      break;
    case GTK_RESPONSE_APPLY:
      // Apply
      drgeoDialog->setScript (code);
      break;
    case GTK_RESPONSE_CANCEL:
      // Cancel
      drgeoDialog->hide ();
      break;
    }
}

drgeoGtkPropertyDialog::drgeoGtkPropertyDialog (drgeoGtkDrawable * aDrawable)
{
  drawable = aDrawable;
  dialog = NULL;
  category = NO_OBJECT;
  type = NO_TYPE;
}

drgeoGtkPropertyDialog::~drgeoGtkPropertyDialog ()
{
  hide ();
  if (dialog)
    {
      gtk_widget_destroy (dialog);
      dialog = NULL;
    }
}

void
drgeoGtkPropertyDialog::show ()
{
  if (dialog)
    gtk_widget_show_all (dialog);
}

void
drgeoGtkPropertyDialog::hide ()
{
  if (dialog)
    {
      gtk_widget_hide (dialog);
      /* unselected the selected item so they do no blink
         anymore */
      drawable->figure->clearSelection ();
    }
}

void
drgeoGtkPropertyDialog::edit (class geometricObject * aItem)
{
  GError* error = NULL;
  GtkBuilder *xml = gtk_builder_new ();
  GtkWidget *widget, *widget2;
  drgeoPoint p;
  gchar out[256] = "";

  if (dialog && (aItem->getCategory () != category)
      && (aItem->getType () != type))
    gtk_widget_destroy (dialog);

  // Acording to the object category, we build the corresponding dialog.
  switch (aItem->getCategory ())
    {
    case FREE_PT:

      if (category != aItem->getCategory ())
	{
	  if (!gtk_builder_add_from_file (xml, DRGEO_GLADEDIR "/drgeo2.glade", &error))
	  {
	    g_warning ("Couldn't load builder file: %s", error->message);
	    g_error_free (error);
	  }

	  dialog = GTK_WIDGET (gtk_builder_get_object (xml, "freePointPropertyDialog"));

	  //set transient
	  mdi->setTransientDialog (GTK_WINDOW (dialog));

	  widget = GTK_WIDGET (gtk_builder_get_object (xml, "abscissaEntry"));
	  gtk_signal_connect (GTK_OBJECT (widget), "activate",
			      GTK_SIGNAL_FUNC
			      (on_freePointAbscissaEntry_activate),
			      (gpointer) this);
	  gtk_object_set_data (GTK_OBJECT (dialog), "abscissaEntry", widget);

	  widget = GTK_WIDGET (gtk_builder_get_object (xml, "ordinateEntry"));
	  gtk_signal_connect (GTK_OBJECT (widget), "activate",
			      GTK_SIGNAL_FUNC
			      (on_freePointOrdinateEntry_activate),
			      (gpointer) this);
	  gtk_object_set_data (GTK_OBJECT (dialog), "ordinateEntry", widget);

	  gtk_signal_connect (GTK_OBJECT (dialog), "response",
			      GTK_SIGNAL_FUNC (on_freePointDialog_reponse),
			      (gpointer) this);

	  g_object_unref  (G_OBJECT (xml));
	}
      p = ((point *) aItem)->getCoordinate ();
      // Update the entries field
      widget = (GtkWidget *) gtk_object_get_data (GTK_OBJECT (dialog),
						  "abscissaEntry");
      snprintf (out, 255, "%f", p.getX ());
      gtk_entry_set_text (GTK_ENTRY (widget), out);

      widget = (GtkWidget *) gtk_object_get_data (GTK_OBJECT (dialog),
						  "ordinateEntry");
      snprintf (out, 255, "%f", p.getY ());
      gtk_entry_set_text (GTK_ENTRY (widget), out);
      break;
    case NUMERIC:
      // Only free value can be edited
      if (aItem->getType () == FREE_VALUE)
	{
	  if (category != aItem->getCategory ())
	    {
	      if (!gtk_builder_add_from_file (xml, DRGEO_GLADEDIR "/drgeo2.glade", &error))
		  {
		    g_warning ("Couldn't load builder file: %s", error->message);
		    g_error_free (error);
		  }
	      dialog = GTK_WIDGET (gtk_builder_get_object (xml, "editValuePropertyDialog"));

	      //set transient
	      mdi->setTransientDialog (GTK_WINDOW (dialog));

	      widget = GTK_WIDGET (gtk_builder_get_object (xml, "valueEntry"));
	      gtk_signal_connect (GTK_OBJECT (widget), "activate",
				  GTK_SIGNAL_FUNC
				  (on_editValueEntry_activate),
				  (gpointer) this);
	      gtk_object_set_data (GTK_OBJECT (dialog), "valueEntry", widget);

	      gtk_signal_connect (GTK_OBJECT (dialog), "response",
				  GTK_SIGNAL_FUNC (on_editValueDialog_response),
				  (gpointer) this);

	      g_object_unref  (G_OBJECT (xml));
	    }
	  // Update the entries field
	  widget = (GtkWidget *) gtk_object_get_data (GTK_OBJECT (dialog),
						      "valueEntry");
	  snprintf (out, 255, "%f", ((numeric *) aItem)->getValue ());
	  gtk_entry_set_text (GTK_ENTRY (widget), out);
	  break;
	}
      break;
    case SCRIPT:
      if (category != aItem->getCategory ())
	{
	  // the corresponding dialog is not opened
	  if (!gtk_builder_add_from_file (xml, DRGEO_GLADEDIR "/drgeo2.glade", &error))
	  {
	    g_warning ("Couldn't load builder file: %s", error->message);
	    g_error_free (error);
	  }
	  dialog = GTK_WIDGET (gtk_builder_get_object (xml, "scriptDialog"));

	  //set transient
	  mdi->setTransientDialog (GTK_WINDOW (dialog));

	  widget2 = GTK_WIDGET (gtk_builder_get_object (xml, "scriptText"));
	  gtk_object_set_data (GTK_OBJECT (dialog), "script", widget2);

	  gtk_signal_connect (GTK_OBJECT (dialog), "response",
			      GTK_SIGNAL_FUNC (on_scriptDialog_response),
			      (gpointer) this);

	  widget = GTK_WIDGET (gtk_builder_get_object (xml, "help"));
	  gtk_signal_connect (GTK_OBJECT (widget), "clicked",
			      GTK_SIGNAL_FUNC (open_help_cb),
			      (gpointer) (drgeniusHelp[2]));

	  gtk_signal_connect (GTK_OBJECT (dialog), "delete_event",
			      GTK_SIGNAL_FUNC (on_propertyDialog_delete),
			      (gpointer) this);


	  g_object_unref  (G_OBJECT (xml));
	}
      // Update the text field with the script
      // First empty the scriptText dialog
      widget = (GtkWidget *) gtk_object_get_data (GTK_OBJECT (dialog),
						  "script");
      gtk_text_view_set_text (GTK_TEXT_VIEW (widget),
			      ((script *) aItem)->getScript ());
      break;
    default:
      dialog = NULL;
      return;
    }
  // These are the characteristics of the item under edition:
  item = aItem;
  category = item->getCategory ();
  type = item->getType ();

  show ();
}

void
drgeoGtkPropertyDialog::moveItem (drgeoVector & v)
{
  drgeoVector t (0, 0);

  drawable->getFigure ()->moveItem (item, v);
  drawable->getFigure ()->dragSelection (t, v);
  drawable->refresh ();
}

void
drgeoGtkPropertyDialog::setValue (gdouble value)
{
  gdouble *val;

  val = (gdouble *) g_malloc (sizeof (gdouble));
  *val = value;
  drawable->getFigure ()->setItemAttribute (item, drgeoValue, (gpointer) val);
}

void
drgeoGtkPropertyDialog::setScript (gchar * script)
{
  drawable->getFigure ()->setItemAttribute (item, drgeoScript,
					    (gpointer) script);
}

void
drgeoGtkPropertyDialog::refresh ()
{
  drawable->refresh ();
}

geometricObject *
drgeoGtkPropertyDialog::getItem ()
{
  return item;
}


drgeoDrawable & drgeoGtkPropertyDialog::getDrawable ()
{
  return (*((drgeoDrawable *) drawable));
}
