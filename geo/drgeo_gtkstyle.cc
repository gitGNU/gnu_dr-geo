/*
 *  Dr Genius an interactive geometry software
 * (C) Copyright Free Software Foundation  1997-2003
 * Authors:
 * hilaire@ofset.org 
 * lolo@seul.org
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

#include <glade/glade.h>

#include "drgeo_gtkstyle.h"
#include "drgeo_gtkdrawable.h"
#include "drgeo_numeric.h"
#include "drgeo_command.h"
#include "drgeo_gtkhelpers.h"
#include "drgenius_mdi.h"

extern drgeniusMDI *mdi;
extern const char * drgeniusHelp[];

// Event handlers.
static void
style_color_button_cb (GtkWidget * widget, gpointer data)
{
  drgeoGtkStyleDialog *dialog;

  dialog = (drgeoGtkStyleDialog *)
    gtk_object_get_data (GTK_OBJECT (widget), "drgeo_style");
  dialog->setColor ((drgeoColorType) GPOINTER_TO_INT (data));
}
static void
style_size_button_cb (GtkWidget * widget, gpointer data)
{
  drgeoGtkStyleDialog *dialog;

  dialog = (drgeoGtkStyleDialog *)
    gtk_object_get_data (GTK_OBJECT (widget), "drgeo_style");
  dialog->setSize ((drgeoThicknessType) GPOINTER_TO_INT (data));
}

static void
visibility_button_cb (GtkWidget * widget, gpointer data)
{
  drgeoGtkStyleDialog *dialog;

  dialog = (drgeoGtkStyleDialog *)
    gtk_object_get_data (GTK_OBJECT (widget), "drgeo_style");
  dialog->setVisibility ((masked) GPOINTER_TO_INT (data));
}

static void
style_shape_button_cb (GtkWidget * widget, gpointer data)
{
  drgeoGtkStyleDialog *dialog;

  dialog = (drgeoGtkStyleDialog *)
    gtk_object_get_data (GTK_OBJECT (widget), "drgeo_style");
  dialog->setShape ((drgeoPointType) GPOINTER_TO_INT (data));
}


static void
style_fill_button_cb (GtkWidget * widget, gpointer data)
{
  drgeoGtkStyleDialog *dialog;

  dialog = (drgeoGtkStyleDialog *)
    gtk_object_get_data (GTK_OBJECT (widget), "drgeo_style");
  dialog->setFill ((bool) data);
}
static void
style_change_name_cb (GtkWidget * widget, gpointer data)
{
  drgeoGtkStyleDialog *dialog;

  dialog = (drgeoGtkStyleDialog *)
    gtk_object_get_data (GTK_OBJECT (widget), "drgeo_style");
  dialog->setName (gtk_entry_get_text (GTK_ENTRY (widget)));
}

static gint
on_styleDialog_close (GtkWidget * widget, drgeoGtkStyleDialog * dialog)
{
  dialog->hide ();
  return TRUE;
}

static gint
on_styleDialog_delete (GtkWidget * widget, GdkEventAny * e,
		       drgeoGtkStyleDialog * dialog)
{
  dialog->hide ();
  return TRUE;
}


// Helper functions
static void
connectWithData (GladeXML * tree, drgeoGtkStyleDialog * d,
		 gchar const * widgetName, GtkSignalFunc f, gint data)
{
  GtkWidget *widget;

  widget = glade_xml_get_widget (tree, widgetName);
  if (widget == NULL)
    g_printerr ("Cannot find widget %s\n", widgetName);
  else
    {
      gtk_signal_connect
	(GTK_OBJECT (widget), "pressed", f, GINT_TO_POINTER (data));
      gtk_object_set_data (GTK_OBJECT (widget), "drgeo_style", d);
    }
}

static GtkWidget *
connectAllColor (GladeXML * xml, drgeoGtkStyleDialog * d)
{
  connectWithData (xml, d, "black",
		   GTK_SIGNAL_FUNC (style_color_button_cb), 0);
  connectWithData (xml, d, "darkgrey",
		   GTK_SIGNAL_FUNC (style_color_button_cb), 1);
  connectWithData (xml, d, "grey",
		   GTK_SIGNAL_FUNC (style_color_button_cb), 2);
  connectWithData (xml, d, "white",
		   GTK_SIGNAL_FUNC (style_color_button_cb), 3);
  connectWithData (xml, d, "darkgreen",
		   GTK_SIGNAL_FUNC (style_color_button_cb), 4);
  connectWithData (xml, d, "green",
		   GTK_SIGNAL_FUNC (style_color_button_cb), 5);
  connectWithData (xml, d, "darkblue",
		   GTK_SIGNAL_FUNC (style_color_button_cb), 6);
  connectWithData (xml, d, "blue",
		   GTK_SIGNAL_FUNC (style_color_button_cb), 7);
  connectWithData (xml, d, "bordeaux",
		   GTK_SIGNAL_FUNC (style_color_button_cb), 8);
  connectWithData (xml, d, "red", GTK_SIGNAL_FUNC (style_color_button_cb), 9);
  connectWithData (xml, d, "orange",
		   GTK_SIGNAL_FUNC (style_color_button_cb), 10);
  connectWithData (xml, d, "yellow",
		   GTK_SIGNAL_FUNC (style_color_button_cb), 11);
}

static void
connectNameEntry (GladeXML * xml, drgeoGtkStyleDialog * d)
{
  GtkWidget *widget;

  widget = glade_xml_get_widget (xml, "name");
  gtk_signal_connect (GTK_OBJECT (widget), "activate",
		      GTK_SIGNAL_FUNC (style_change_name_cb), NULL);
  gtk_object_set_data (GTK_OBJECT (widget), "drgeo_style", d);
}

static void
connectStyle (GladeXML * xml, drgeoGtkStyleDialog * d)
{
  connectWithData (xml, d, "dashed",
		   GTK_SIGNAL_FUNC (style_size_button_cb), 0);
  connectWithData (xml, d, "normal",
		   GTK_SIGNAL_FUNC (style_size_button_cb), 1);
  connectWithData (xml, d, "large",
		   GTK_SIGNAL_FUNC (style_size_button_cb), 2);
}

static void
connectVisibility (GladeXML * xml, drgeoGtkStyleDialog * d)
{

  connectWithData (xml, d, "masked",
		   GTK_SIGNAL_FUNC (visibility_button_cb), 1);
  connectWithData (xml, d, "unmasked",
		   GTK_SIGNAL_FUNC (visibility_button_cb), 0);
}


static void
adjustColorDialog (GladeXML * xml, geometricObject * item)
{
  GtkWidget *widget;

  switch (item->getStyle ().color)
    {
    case 0:
      widget = glade_xml_get_widget (xml, "black");
      break;
    case 1:
      widget = glade_xml_get_widget (xml, "darkgrey");
      break;
    case 2:
      widget = glade_xml_get_widget (xml, "grey");
      break;
    case 3:
      widget = glade_xml_get_widget (xml, "white");
      break;
    case 4:
      widget = glade_xml_get_widget (xml, "darkgreen");
      break;
    case 5:
      widget = glade_xml_get_widget (xml, "green");
      break;
    case 6:
      widget = glade_xml_get_widget (xml, "darkblue");
      break;
    case 7:
      widget = glade_xml_get_widget (xml, "blue");
      break;
    case 8:
      widget = glade_xml_get_widget (xml, "bordeaux");
      break;
    case 9:
      widget = glade_xml_get_widget (xml, "red");
      break;
    case 10:
      widget = glade_xml_get_widget (xml, "orange");
      break;
    case 11:
      widget = glade_xml_get_widget (xml, "yellow");
      break;
    }
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (widget), TRUE);
}

static void
adjustNameEntryDialog (GladeXML * xml, geometricObject * item)
{
  GtkWidget *widget;

  widget = glade_xml_get_widget (xml, "name");
  if (item->getName ())
    gtk_entry_set_text (GTK_ENTRY (widget), item->getName ());
  else
    gtk_entry_set_text (GTK_ENTRY (widget), "");
}

static void
adjustStyleDialog (GladeXML * xml, geometricObject * item)
{
  GtkWidget *widget;

  switch (item->getStyle ().thick)
    {
    case 0:
      widget = glade_xml_get_widget (xml, "dashed");
      break;
    case 1:
      widget = glade_xml_get_widget (xml, "normal");
      break;
    case 2:
      widget = glade_xml_get_widget (xml, "large");
      break;
    }
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (widget), TRUE);

}

static void
adjustVisibilityDialog (GladeXML * xml, geometricObject * item)
{
  GtkWidget *widget;

  switch (item->getStyle ().mask)
    {
    case yes:
      widget = glade_xml_get_widget (xml, "masked");
      break;
    case no:
      widget = glade_xml_get_widget (xml, "unmasked");
      break;
    }
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (widget), TRUE);
}

void
drgeoGtkStyleDialog::show ()
{
  gtk_widget_show_all (dialog);
}

void
drgeoGtkStyleDialog::hide ()
{
  if (dialog)
    gtk_widget_hide (dialog);
  if (item)
    setName (gtk_entry_get_text
	     (GTK_ENTRY (glade_xml_get_widget (xml, "name"))));
}

void
drgeoGtkStyleDialog::edit (geometricObject * aItem)
{
  GtkWidget *widget;
  objectCategory aCategory;

  aCategory = aItem->getCategory ();
  // Acording to the object category, we build the corresponding dialog.
  if (aCategory & POINT)
    {
      item = aItem;
      if (!(category & POINT))
	{
	  // Previous dialog was not of this category
	  if (xml)
	    g_object_unref  (G_OBJECT (xml));
	  xml = glade_xml_new (DRGEO_GLADEDIR "/drgeo2.glade",
			       "pointStyleDialog", NULL);

	  if (dialog)
	    gtk_widget_destroy (dialog);
	  dialog = glade_xml_get_widget (xml, "pointStyleDialog");

	  //set transient
	  mdi->setTransientDialog (GTK_WINDOW (dialog));

	  connectWithData (xml, this, "round",
			   GTK_SIGNAL_FUNC (style_shape_button_cb), 0);
	  connectWithData (xml, this, "cross",
			   GTK_SIGNAL_FUNC (style_shape_button_cb), 1);
	  connectWithData (xml, this, "square",
			   GTK_SIGNAL_FUNC (style_shape_button_cb), 2);
	  connectWithData (xml, this, "roundEmpty",
			   GTK_SIGNAL_FUNC (style_shape_button_cb), 3);
	  connectWithData (xml, this, "squareEmpty",
			   GTK_SIGNAL_FUNC (style_shape_button_cb), 4);

	  connectWithData (xml, this, "small",
			   GTK_SIGNAL_FUNC (style_size_button_cb), 0);
	  connectWithData (xml, this, "normal",
			   GTK_SIGNAL_FUNC (style_size_button_cb), 1);
	  connectWithData (xml, this, "large",
			   GTK_SIGNAL_FUNC (style_size_button_cb), 2);

	  widget = glade_xml_get_widget (xml, "pointStyleDialogClose");
	  gtk_signal_connect (GTK_OBJECT (widget), "clicked",
			      GTK_SIGNAL_FUNC (on_styleDialog_close),
			      (gpointer) this);
	  gtk_signal_connect (GTK_OBJECT (dialog), "delete_event",
			      GTK_SIGNAL_FUNC (on_styleDialog_delete),
			      (gpointer) this);
	  widget = glade_xml_get_widget (xml, "pointStyleDialogHelp");
	  gtk_signal_connect (GTK_OBJECT (widget), "clicked",
			      GTK_SIGNAL_FUNC (open_help_cb),
			      (gpointer) (drgeniusHelp[0]));

	  connectAllColor (xml, this);
	  connectNameEntry (xml, this);
	  connectVisibility (xml, this);
	}

      switch (item->getStyle ().pointShape)
	{
	case 0:
	  widget = glade_xml_get_widget (xml, "round");
	  break;
	case 1:
	  widget = glade_xml_get_widget (xml, "cross");
	  break;
	case 2:
	  widget = glade_xml_get_widget (xml, "square");
	  break;
	case 3:
	  widget = glade_xml_get_widget (xml, "roundEmpty");
	  break;
	case 4:
	  widget = glade_xml_get_widget (xml, "squareEmpty");
	  break;
	}
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (widget), TRUE);

      switch (item->getStyle ().thick)
	{
	case 0:
	  widget = glade_xml_get_widget (xml, "small");
	  break;
	case 1:
	  widget = glade_xml_get_widget (xml, "normal");
	  break;
	case 2:
	  widget = glade_xml_get_widget (xml, "large");
	  break;
	}
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (widget), TRUE);

    }
  else if ((aCategory & DIRECTION) || (aCategory & CURVE) ||
	   (aCategory & ANGLE))
    {
      item = aItem;
      if (!(category & DIRECTION) && !(category & CURVE) &&
	  !(category & ANGLE))
	{
	  // Previous dialog was not of this category
	  if (xml)
	    g_object_unref  (G_OBJECT (xml));
	  xml = glade_xml_new (DRGEO_GLADEDIR "/drgeo2.glade",
			       "lineStyleDialog", NULL);

	  if (dialog)
	    gtk_widget_destroy (dialog);
	  dialog = glade_xml_get_widget (xml, "lineStyleDialog");

	  //set transient
	  mdi->setTransientDialog (GTK_WINDOW (dialog));

	  connectStyle (xml, this);

	  widget = glade_xml_get_widget (xml, "lineStyleDialogClose");
	  gtk_signal_connect (GTK_OBJECT (widget), "clicked",
			      GTK_SIGNAL_FUNC (on_styleDialog_close),
			      (gpointer) this);
	  gtk_signal_connect (GTK_OBJECT (dialog), "delete_event",
			      GTK_SIGNAL_FUNC (on_styleDialog_delete),
			      (gpointer) this);
	  widget = glade_xml_get_widget (xml, "lineStyleDialogHelp");
	  gtk_signal_connect (GTK_OBJECT (widget), "clicked",
			      GTK_SIGNAL_FUNC (open_help_cb),
			      (gpointer) (drgeniusHelp[0]));
	  connectAllColor (xml, this);
	  connectNameEntry (xml, this);
	  connectVisibility (xml, this);
	}
      /* adjust the state of the dialog */
      adjustStyleDialog (xml, item);
    }
  else if (aCategory & POLYGON)
    {
      item = aItem;
      if (!(category & POLYGON))
	{
	  // Previous dialog was not of this category
	  if (xml)
	    g_object_unref  (G_OBJECT (xml));
	  xml = glade_xml_new (DRGEO_GLADEDIR "/drgeo2.glade",
			       "numericStyleDialog", NULL);

	  if (dialog)
	    gtk_widget_destroy (dialog);
	  dialog = glade_xml_get_widget (xml, "numericStyleDialog");

	  //set transient
	  mdi->setTransientDialog (GTK_WINDOW (dialog));

	  widget = glade_xml_get_widget (xml, "numericStyleDialogClose");
	  gtk_signal_connect (GTK_OBJECT (widget), "clicked",
			      GTK_SIGNAL_FUNC (on_styleDialog_close),
			      (gpointer) this);
	  gtk_signal_connect (GTK_OBJECT (dialog), "delete-event",
			      GTK_SIGNAL_FUNC (on_styleDialog_delete),
			      (gpointer) this);
	  widget = glade_xml_get_widget (xml, "numericStyleDialogHelp");
	  gtk_signal_connect (GTK_OBJECT (widget), "clicked",
			      GTK_SIGNAL_FUNC (open_help_cb),
			      (gpointer) (drgeniusHelp[0]));
	  connectAllColor (xml, this);
	  connectNameEntry (xml, this);
	  connectVisibility (xml, this);
	}
    }
  else if ((aCategory & NUMERIC) || (aCategory & EQUATION) ||
	   (aCategory & SCRIPT))
    {
      item = aItem;
      if (!(category & NUMERIC) && !(category & EQUATION)
	  && !(category & SCRIPT))
	{
	  // Previous dialog was not of this category
	  if (xml)
	    g_object_unref  (G_OBJECT (xml));
	  xml = glade_xml_new (DRGEO_GLADEDIR "/drgeo2.glade",
			       "numericStyleDialog", NULL);

	  if (dialog)
	    gtk_widget_destroy (dialog);
	  dialog = glade_xml_get_widget (xml, "numericStyleDialog");

	  //set transient
	  mdi->setTransientDialog (GTK_WINDOW (dialog));

	  widget = glade_xml_get_widget (xml, "numericStyleDialogClose");
	  gtk_signal_connect (GTK_OBJECT (widget), "clicked",
			      GTK_SIGNAL_FUNC (on_styleDialog_close),
			      (gpointer) this);
	  gtk_signal_connect (GTK_OBJECT (dialog), "delete-event",
			      GTK_SIGNAL_FUNC (on_styleDialog_delete),
			      (gpointer) this);
	  widget = glade_xml_get_widget (xml, "numericStyleDialogHelp");
	  gtk_signal_connect (GTK_OBJECT (widget), "clicked",
			      GTK_SIGNAL_FUNC (open_help_cb),
			      (gpointer) (drgeniusHelp[0]));
	  connectAllColor (xml, this);
	  connectNameEntry (xml, this);
	  connectVisibility (xml, this);
	}
    }
  else
    {
      g_printerr ("Unknow category number %d\n", aCategory);
      return;
    }

  category = item->getCategory ();
  // Common set up to all the dialog
  adjustColorDialog (xml, item);
  adjustNameEntryDialog (xml, item);
  adjustVisibilityDialog (xml, item);

  show ();
}

void
drgeoGtkStyleDialog::setColor (drgeoColorType color)
{
  gpointer p;
  // Set the current item color.
  /* does something new happen ? */
  if (item->getStyle ().color == color)
    return;
  p = g_malloc (sizeof (drgeoColorType));
  *((drgeoColorType *) p) = color;
  drawable->getFigure ()->setItemAttribute (item, drgeoColor, p);
}

void
drgeoGtkStyleDialog::setSize (drgeoThicknessType size)
{
  gpointer p;
  // Set the current item size.
  /* does something new happen ? */
  if (item->getStyle ().thick == size)
    return;
  p = g_malloc (sizeof (drgeoThicknessType));
  *((drgeoThicknessType *) p) = size;
  drawable->getFigure ()->setItemAttribute (item, drgeoSize, p);
}

void
drgeoGtkStyleDialog::setVisibility (masked m)
{
  gpointer p;
  // Set the current item mask.
  /* does something new happen ? */
  if (item->getStyle ().mask == m)
    return;
  p = g_malloc (sizeof (masked));
  *((masked *) p) = m;
  g_printerr ("Mask: %d\n", m);
  drawable->getFigure ()->setItemAttribute (item, drgeoVisible, p);
}

void
drgeoGtkStyleDialog::setShape (drgeoPointType shape)
{
  gpointer p;
  // Set the current item shape.
  if (item->getStyle ().pointShape == shape)
    return;
  p = g_malloc (sizeof (drgeoPointType));
  *((drgeoPointType *) p) = shape;
  drawable->getFigure ()->setItemAttribute (item, drgeoShape, p);
}

void
drgeoGtkStyleDialog::setFill (bool fill)
{
  gpointer p;
  // Set the current item fill.
  if (item->getStyle ().fill == fill)
    return;
  p = g_malloc (sizeof (bool));
  *((bool *) p) = fill;
  drawable->getFigure ()->setItemAttribute (item, drgeoFill, p);
}

void
drgeoGtkStyleDialog::setName (const gchar *name)
{
  gpointer p;

  if (strlen (name) == 0)
    p = NULL;
  else
    p = (gpointer) g_strdup (name);

  drawable->getFigure ()->setItemAttribute (item, drgeoName, p);
}

drgeoGtkStyleDialog::drgeoGtkStyleDialog (drgeoGtkDrawable * aDrawable)
{

  drawable = aDrawable;
  dialog = NULL;
  category = NO_OBJECT;
  xml = NULL;
  item = NULL;
}

drgeoGtkStyleDialog::~drgeoGtkStyleDialog ()
{
  hide ();
  if (dialog)
    {
      gtk_widget_destroy (dialog);
      g_object_unref  (G_OBJECT (xml));
    }
}
