/*
 *  Dr Genius interactive geometry software
 * (C) Copyright Hilaire Fernandes  2002
 * 
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
#include <time.h>
#include <unistd.h>

#include <string.h>


#include "drgeo_adaptDialog.h"
#include "drgeo_figure.h"
#include "drgeo_menu.h"
#include "drgenius_mdi.h"

#ifndef G_PLATFORM_WIN32
   #include <crypt.h>
#endif 

extern drgeniusMDI *mdi;

static GError* error = NULL;
static GtkBuilder *xml = gtk_builder_new ();
static drgeoFigure *myFigure;
static geoView *myChild;
static GtkWindow *dialog;
gchar *toolName[] =
  { "pointMenu", "curveMenu", "transformationMenu", "numericMenu",
  "macroMenu", "otherMenu", "moveItem", "freePoint", "middle",
  "intersection", "coordinatesPoint", "line", "halfLine", "segment",
  "vector", "circle", "arcCircle", "locus", "polygon",
  "parallel", "perpendicular", "reflexion", "symmetry", "translation",
  "rotation", "scale", "distance", "angle", "equation", "script",
  "buildMacro", "runMacro", "deleteItem", "styleItem", "propertyItem"
};


static void
updateDrgeoDialog ()
{
  /* update the dialog according to the unselected tools in this
   * figure */
  gint i;

  for (i = 0; i < DRGEO_TOOLS_NUMBER; i++)
    setSensitiveState (toolName[i], myFigure->getToolState ((drgeoToolId) i));
}

void
adaptDrgeoDialog (geoView * child)
/* Adapt the Drgeo dialog */
{
  GtkWidget *w;

  if (xml != NULL)
    /* only one adaptDrgeoDialog at a time */
    return;
  myFigure = child->figure ();
  myChild = child;

  if (!gtk_builder_add_from_file (xml, DRGEO_GLADEDIR "/uiAdaptDialog.glade", &error))
	  {
	    g_warning ("Couldn't load builder file: %s", error->message);
	    g_error_free (error);
	  }
  dialog = GTK_WINDOW (GTK_WIDGET (gtk_builder_get_object (xml, "uiAdaptDialog")));
  gtk_builder_connect_signals (xml, &error);

  mdi->setTransientDialog (GTK_WINDOW (dialog));

  /* Force the toolbar style, we do not want the default one from the Gnome Desktop */
  w = GTK_WIDGET (gtk_builder_get_object (xml, "tb0"));
  gtk_toolbar_set_style (GTK_TOOLBAR (w), GTK_TOOLBAR_ICONS);
  // FIXME gtk_toolbar_set_button_relief (GTK_TOOLBAR (w), GTK_RELIEF_HALF);
  w = GTK_WIDGET (gtk_builder_get_object (xml, "tb1"));
  gtk_toolbar_set_style (GTK_TOOLBAR (w), GTK_TOOLBAR_ICONS);
  // FIXME gtk_toolbar_set_button_relief (GTK_TOOLBAR (w), GTK_RELIEF_NONE);
  w = GTK_WIDGET (gtk_builder_get_object (xml, "tb2"));
  gtk_toolbar_set_style (GTK_TOOLBAR (w), GTK_TOOLBAR_ICONS);
  // FIXME gtk_toolbar_set_button_relief (GTK_TOOLBAR (w), GTK_RELIEF_NONE);
  w = GTK_WIDGET (gtk_builder_get_object (xml, "tb2"));
  gtk_toolbar_set_style (GTK_TOOLBAR (w), GTK_TOOLBAR_ICONS);
  // FIXME gtk_toolbar_set_button_relief (GTK_TOOLBAR (w), GTK_RELIEF_NONE);
  w = GTK_WIDGET (gtk_builder_get_object (xml, "tb3"));
  gtk_toolbar_set_style (GTK_TOOLBAR (w), GTK_TOOLBAR_ICONS);
  // FIXME gtk_toolbar_set_button_relief (GTK_TOOLBAR (w), GTK_RELIEF_NONE);
  w = GTK_WIDGET (gtk_builder_get_object (xml, "tb4"));
  gtk_toolbar_set_style (GTK_TOOLBAR (w), GTK_TOOLBAR_ICONS);
  // FIXME gtk_toolbar_set_button_relief (GTK_TOOLBAR (w), GTK_RELIEF_NONE);
  w = GTK_WIDGET (gtk_builder_get_object (xml, "tb5"));
  gtk_toolbar_set_style (GTK_TOOLBAR (w), GTK_TOOLBAR_ICONS);
  // FIXME gtk_toolbar_set_button_relief (GTK_TOOLBAR (w), GTK_RELIEF_NONE);
  w = GTK_WIDGET (gtk_builder_get_object (xml, "tb6"));
  gtk_toolbar_set_style (GTK_TOOLBAR (w), GTK_TOOLBAR_ICONS);
  // FIXME gtk_toolbar_set_button_relief (GTK_TOOLBAR (w), GTK_RELIEF_NONE);

  updateDrgeoDialog ();
}

void
on_pointMenuHide_clicked (GtkWidget * w, gpointer data)
{
  gboolean activated;

  activated = !getSensitiveState (GTK_BIN(w)->child);
  gtk_widget_set_sensitive (GTK_BIN (w)->child, activated);
  /* set the state of the tools packed in this barre */
  setSensitiveState ("freePoint", activated);
  setSensitiveState ("middle", activated);
  setSensitiveState ("intersection", activated);
  setSensitiveState ("coordinatesPoint", activated);

}

void
on_curveMenuHide_clicked (GtkWidget * w, gpointer data)
{
  gboolean activated;

  activated = !getSensitiveState (GTK_BIN(w)->child);
  gtk_widget_set_sensitive (GTK_BIN (w)->child, activated);
  /* set the state of the tools packed in this barre */
  setSensitiveState ("line", activated);
  setSensitiveState ("halfLine", activated);
  setSensitiveState ("segment", activated);
  setSensitiveState ("vector", activated);
  setSensitiveState ("circle", activated);
  setSensitiveState ("arcCircle", activated);
  setSensitiveState ("locus", activated);
  setSensitiveState ("polygon", activated);
}

void
on_transformationMenuHide_clicked (GtkWidget * w, gpointer data)
{
  gboolean activated;

  activated = !getSensitiveState (GTK_BIN(w)->child);
  gtk_widget_set_sensitive (GTK_BIN (w)->child, activated);
  /* set the state of the tools packed in this barre */
  setSensitiveState ("parallel", activated);
  setSensitiveState ("perpendicular", activated);
  setSensitiveState ("reflexion", activated);
  setSensitiveState ("symmetry", activated);
  setSensitiveState ("translation", activated);
  setSensitiveState ("rotation", activated);
  setSensitiveState ("scale", activated);
}

void
on_numericMenuHide_clicked (GtkWidget * w, gpointer data)
{
  gboolean activated;

  activated = !getSensitiveState (GTK_BIN(w)->child);
  gtk_widget_set_sensitive (GTK_BIN (w)->child, activated);
  /* set the state of the tools packed in this barre */
  setSensitiveState ("distance", activated);
  setSensitiveState ("angle", activated);
  setSensitiveState ("equation", activated);
  setSensitiveState ("script", activated);
}

void
on_macroMenuHide_clicked (GtkWidget * w, gpointer data)
{
  gboolean activated;

  activated = !getSensitiveState (GTK_BIN(w)->child);
  gtk_widget_set_sensitive (GTK_BIN (w)->child, activated);
  /* set the state of the tools packed in this barre */
  setSensitiveState ("buildMacro", activated);
  setSensitiveState ("runMacro", activated);
}

void
on_otherMenuHide_clicked (GtkWidget * w, gpointer data)
{
  gboolean activated;

  activated = !getSensitiveState (GTK_BIN(w)->child);
  gtk_widget_set_sensitive (GTK_BIN (w)->child, activated);
  /* set the state of the tools packed in this barre */
  setSensitiveState ("moveItem", activated);
  setSensitiveState ("deleteItem", activated);
  setSensitiveState ("styleItem", activated);
  setSensitiveState ("propertyItem", activated);
}

void
on_toolHide_clicked (GtkWidget * w, gpointer data)
{
  if (getSensitiveState (GTK_BIN(w)->child))
    gtk_widget_set_sensitive (GTK_BIN (w)->child, FALSE);
  else
    gtk_widget_set_sensitive (GTK_BIN (w)->child, TRUE);
}

void 
on_uiAdaptDialog_response (GtkWidget * widget, gint b, gpointer data)
{
  GtkWidget *p_dialog, *p_entry;
  GError* error = NULL;
  GtkBuilder *p_tree = gtk_builder_new (); 

  switch (b)
    {
    case GTK_RESPONSE_CLOSE:
      gtk_widget_destroy (GTK_WIDGET (dialog));
      g_object_unref (G_OBJECT (xml));
      xml = NULL;
      /* Eventually needed */
      // FIXME : do we need myChild->reconcile_popup_menu ();      
      break;
    case 1:
      /* Lock */
      /* Ask for a password if none was specified before */
      if (myFigure->getPassword ())
	{
	  /* Ask the previous password when it is not null */
	  if (!gtk_builder_add_from_file (p_tree, DRGEO_GLADEDIR "/relockDialog.glade", &error))
	  {
	    g_warning ("Couldn't load builder file: %s", error->message);
	    g_error_free (error);
	  }
	  p_dialog = GTK_WIDGET (gtk_builder_get_object (p_tree, "relockDialog"));
	  p_entry = GTK_WIDGET (gtk_builder_get_object (p_tree, "relockEntry"));
	  mdi->setTransientDialog (GTK_WINDOW (p_dialog));
	  
	  g_signal_connect (G_OBJECT (p_dialog), "response",
			    G_CALLBACK (on_relockDialog_response),
			    (gpointer) p_entry);
	  g_object_unref (G_OBJECT (p_tree));
	}
      else
	{
	  if (!gtk_builder_add_from_file (p_tree, DRGEO_GLADEDIR "/lockDialog.glade", &error))
	  {
	    g_warning ("Couldn't load builder file: %s", error->message);
	    g_error_free (error);
	  }
	  p_dialog = GTK_WIDGET (gtk_builder_get_object (p_tree, "lockDialog"));
	  p_entry = GTK_WIDGET (gtk_builder_get_object (p_tree, "lockEntry"));
	  mdi->setTransientDialog (GTK_WINDOW (p_dialog));
	  
	  g_signal_connect (G_OBJECT (p_dialog), "response",
			    G_CALLBACK (on_lockDialog_response),
			    (gpointer) p_entry);
	  g_object_unref (G_OBJECT (p_tree));	  
	}
      break;
    case 2:
      /* Unock */
      if (myFigure->getPassword ())
	{
	  if (!gtk_builder_add_from_file (p_tree, DRGEO_GLADEDIR "/unlockDialog.glade", &error))
	  {
	    g_warning ("Couldn't load builder file: %s", error->message);
	    g_error_free (error);
	  }
	  p_dialog = GTK_WIDGET (gtk_builder_get_object (p_tree, "unlockDialog"));
	  p_entry = GTK_WIDGET (gtk_builder_get_object (p_tree, "unlockEntry"));
	  mdi->setTransientDialog (GTK_WINDOW (p_dialog));
	  
	  g_signal_connect (G_OBJECT (p_dialog), "response",
			    G_CALLBACK (on_unlockDialog_response),
			    (gpointer) p_entry);
	  g_object_unref (G_OBJECT (p_tree));	  
	}
      break;
    }
}


void 
on_relockDialog_response (GtkWidget *dialog, gint r, GtkWidget *entry)
{
  char *result, *password;
  gint i;
  GtkWidget *deniedDialog;

  switch (r)
    {
    case GTK_RESPONSE_OK:
      /* The interface is already locked, get the password to relock
	 it with the new customization */
      password = (gchar *) gtk_entry_get_text (GTK_ENTRY (entry));
      if (password == NULL)
	break;

#ifndef G_PLATFORM_WIN32
      result = crypt (password, myFigure->getPassword ());
#endif
      if (!strcmp (result, myFigure->getPassword ()))
	{
	  /* access granted */
	  /* Record and Update the changes to the UI */
	  for (i = 0; i < DRGEO_TOOLS_NUMBER; i++)
	    myFigure->setToolState ((drgeoToolId) i,
				    getSensitiveStateByName (toolName[i]));
	  myChild->updateUserInterface ();
	}
      else
	{
	  /* access denied */
	  deniedDialog = gtk_message_dialog_new
	    (mdi->mainWindow (),
	     GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
	     GTK_BUTTONS_OK,
	     "Password does not match\n Access denied!");
	  gtk_dialog_run (GTK_DIALOG (deniedDialog));
	  gtk_widget_destroy (deniedDialog);
	}
      break;
    case GTK_RESPONSE_CANCEL:
      break;
    }
  gtk_widget_destroy (dialog);
}

void 
on_lockDialog_response (GtkWidget *dialog, gint r, GtkWidget *entry)
{
  gint i;
  unsigned long seed[2];
  char salt[] = "$1$........";
  const char *const seedchars =
    "./0123456789ABCDEFGHIJKLMNOPQRST" "UVWXYZabcdefghijklmnopqrstuvwxyz";
  char *cryptPassword, *password;

  switch (r)
    {
    case GTK_RESPONSE_OK:
      password = (gchar *) gtk_entry_get_text (GTK_ENTRY (entry));
      if (password == NULL)
	break;

      seed[0] = time (NULL);
      seed[1] = getpid () ^ (seed[0] >> 14 & 0x30000);

      /* Turn it into printable characters from `seedchars'. */
      for (i = 0; i < 8; i++)
	salt[3 + i] = seedchars[(seed[i / 5] >> (i % 5) * 6) & 0x3f];
      
#ifndef G_PLATFORM_WIN32
      cryptPassword = crypt (password, salt);
#endif      
      /* Save the crypted password into the figure */
      myFigure->setPassword (cryptPassword);
      
      /* Record and Update the changes to the UI */
      for (i = 0; i < DRGEO_TOOLS_NUMBER; i++)
	myFigure->setToolState ((drgeoToolId) i,
				getSensitiveStateByName (toolName[i]));
      myChild->updateUserInterface ();
      break;
    case GTK_RESPONSE_CANCEL:
      break;
    }
  gtk_widget_destroy (dialog);
}

void 
on_unlockDialog_response (GtkWidget *dialog, gint r, GtkWidget *entry)
{
  GtkWidget *deniedDialog;
  char *result, *password;
  gint i;


  switch (r)
    {
    case GTK_RESPONSE_OK:
      password = (gchar *) gtk_entry_get_text (GTK_ENTRY (entry));
      if (password == NULL)
	break;

#ifndef G_PLATFORM_WIN32
      result = crypt (password, myFigure->getPassword ());
#endif
      if (!strcmp (result, myFigure->getPassword ()))
	{
	  /* access granted */
	  /* Set visible all the tool in the interface */
	  for (i = 0; i < DRGEO_TOOLS_NUMBER; i++)
	    myFigure->setToolState ((drgeoToolId) i, TRUE);
	  myChild->updateUserInterface ();
	  /* forget the password */
	  myFigure->setPassword (NULL);
	  updateDrgeoDialog ();
	}
      else
	{
	  /* access denied */
	  deniedDialog = gtk_message_dialog_new
	    (mdi->mainWindow (),
	     GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
	     GTK_BUTTONS_OK,
	     _("Password does not match\n Access denied!"));
	  gtk_dialog_run (GTK_DIALOG (deniedDialog));
	  gtk_widget_destroy (deniedDialog);
	}
      break;
    case GTK_RESPONSE_CANCEL:
      break;
    }
  gtk_widget_destroy (dialog);
}

gint
on_uiAdaptDialog_delete_event (GtkWidget * w, GdkEventAny * e, gpointer data)
{
  gtk_object_destroy (GTK_OBJECT (xml));
  gtk_widget_destroy (GTK_WIDGET (dialog));
  xml = NULL;
 /* Eventually needed */
  // FIXME: do we need it? reconcile_popup_menu (myChild);
  return FALSE;
}


gboolean
getSensitiveState (GtkWidget * button)
/* return the senstitive state of the button */
{
  return GTK_WIDGET_IS_SENSITIVE (button);
}

gboolean
getSensitiveStateByName (gchar * buttonName)
{
  // the sensitivity is on the image of the button
  return getSensitiveState 
    (GTK_BIN(GTK_WIDGET (gtk_builder_get_object (xml, buttonName)))->child);
}

void
setSensitiveState (gchar * button, gboolean state)
{
  GtkWidget *w;

  w = GTK_WIDGET (gtk_builder_get_object (xml, button));
  gtk_widget_set_sensitive (GTK_BIN (w)->child, state);
}
