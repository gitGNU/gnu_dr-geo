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

#include <config.h>

#include <errno.h>

#include "drgenius_mdi.h"
#include "drgeo_figure.h"
#include "drgeo_gtkdrawable.h"
#include "geo_view.h"
#include "editor_view.h"
#include "drgenius_config.h"
#include "drgeo_adaptDialog.h"
#include "macro.h"
#include "drgenius_config.h"
#include "drgeo_gtkhelpers.h"
#include "drgeo_scm_helper.h"
#include "drgeo_tool.h"

#define DATA_BEGIN  ((char) 2)
#define DATA_END  ((char) 5)
#define DATA_ESCAPE ((char) 27)


static xmlDocPtr drgeniusXml = NULL;

static gchar *sessionPathName = NULL;
static gchar *openPathName = NULL;
static gchar *evaluatePathName = NULL;

extern drgeniusMDI *mdi;
extern gboolean texmacsMode;
extern const char* drgeniusHelp[];

drgeniusMDI::drgeniusMDI ()
{
  GError* error = NULL;
  GtkBuilder *xml = gtk_builder_new ();
  GtkWidget *w;

  p_activeView = NULL;
  p_viewList = NULL;

  // load the Glade interface
  if (!gtk_builder_add_from_file (xml, DRGEO_GLADEDIR "/drgeoMDI.glade", &error))
	  {
	    g_warning ("Couldn't load builder file: %s", error->message);
	    g_error_free (error);
	  }
  gtk_builder_connect_signals (xml, &error);

  p_mainWindow = GTK_WINDOW (GTK_WIDGET (gtk_builder_get_object(xml, "drgeoMain"))); 
  gtk_window_resize (GTK_WINDOW (p_mainWindow), 640, 480);

  p_drgeoMDIVbox = GTK_WIDGET (gtk_builder_get_object (xml, "drgeoMDIVbox")); 
  p_undoButton = GTK_WIDGET (gtk_builder_get_object (xml, "undoButton")); 
  p_undoItem = GTK_WIDGET (gtk_builder_get_object (xml, "undoItem"));
  p_redoButton = GTK_WIDGET (gtk_builder_get_object (xml, "redoButton")); 
  p_redoItem = GTK_WIDGET (gtk_builder_get_object (xml, "redoItem"));
  p_renameItem = GTK_WIDGET (gtk_builder_get_object (xml, "renameItem"));
  p_gridItem = GTK_WIDGET (gtk_builder_get_object (xml, "gridItem"));
  p_renameItemSc = GTK_WIDGET (gtk_builder_get_object (xml, "renameItemSc"));
  p_gridItemSc = GTK_WIDGET (gtk_builder_get_object (xml, "gridItemSc"));
  p_customUiItem = GTK_WIDGET (gtk_builder_get_object (xml, "customUiItem"));
  p_saveItem = GTK_WIDGET (gtk_builder_get_object (xml, "saveItem"));
  p_saveAsItem = GTK_WIDGET (gtk_builder_get_object (xml, "saveAsItem"));
  p_saveMultipleItem = GTK_WIDGET (gtk_builder_get_object (xml, "saveMultipleItem"));
  p_printFigure = GTK_WIDGET (gtk_builder_get_object (xml, "printFigure"));
  p_exportItem = GTK_WIDGET (gtk_builder_get_object (xml, "exportItem"));
  p_exportPreferencesItem = GTK_WIDGET (gtk_builder_get_object (xml, "exportPreferencesItem"));
  p_closeItem = GTK_WIDGET (gtk_builder_get_object (xml, "closeItem"));
  p_drgeoStatusbar = GTK_WIDGET (gtk_builder_get_object (xml, "drgeoStatusbar"));
  p_windowsMenu = GTK_WIDGET (gtk_builder_get_object (xml, "windowsMenu"));
  p_macroMenu = GTK_WIDGET (gtk_builder_get_object (xml, "macroMenu"));
  p_animationMenu = GTK_WIDGET (gtk_builder_get_object (xml, "animationMenu"));

  // set the submenu of macro and windows
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (p_macroMenu), gtk_menu_new ());  gtk_menu_item_set_submenu (GTK_MENU_ITEM (p_windowsMenu), gtk_menu_new ());

  // tooltips for the macro menu
  p_tooltipsMacro = gtk_tooltips_new ();

  reconcile_grayout_widget ();
  setMessage (_("Welcome to GNU Dr. Geo - Free Interactive geometry by OFSET"));
  setNullView ();
}

void 
drgeniusMDI::newGeometricDocument (xmlNodePtr tree)
{
  drgeniusView *tmpView;
  GList *tmpList;

  tmpView = (drgeniusView *) new geoView (tree);
  tmpList = addView (tmpView);

  if (tmpList != NULL)
    {
      p_activeView = tmpView;
      p_viewList = tmpList;
    }
}
 
void 
drgeniusMDI::newTextDocument (xmlNodePtr tree)
{
  drgeniusView *tmpView;
  GList *tmpList;

  tmpView = (drgeniusView *) new editorView (tree);
  tmpList = addView (tmpView);

  if (tmpList != NULL)
    {
      p_activeView = tmpView;
      p_viewList = tmpList;
    }
}

void
drgeniusMDI::openSession (gchar *filename)
{
  xmlDocPtr drgeniusXml;
  xmlNodePtr tree;
  GtkWidget *dialog;
  gint numberChild = 0;

  if ((drgeniusXml = xmlParseFile (filename)) == NULL)
    {
      dialog = gtk_message_dialog_new (mainWindow (),
				       GTK_DIALOG_DESTROY_WITH_PARENT,
				       GTK_MESSAGE_ERROR,
				       GTK_BUTTONS_CLOSE,
				       _("Error loading file '%s'"),
				       filename, g_strerror (errno));
      gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_widget_destroy (dialog);
      return;
    }
  tree = drgeniusXml->children;
  if (strcasecmp ((const char *) tree->name, "drgenius") != 0)
    {
      dialog = gtk_message_dialog_new (mainWindow (),
				       GTK_DIALOG_DESTROY_WITH_PARENT,
				       GTK_MESSAGE_ERROR,
				       GTK_BUTTONS_CLOSE,
				       "'%s' is not a Dr. Geo XML file",
				       filename, g_strerror (errno));
      gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_widget_destroy (dialog);
      return;
    }
  tree = tree->children;
  /* vistit the top child */
  while (tree != NULL)
    {
      if (strcasecmp ((const char *) tree->name, "drgeo") == 0)
	/* we get a Dr Geo figure */
	{
	  newGeometricDocument (tree);
	  numberChild++;
	}
      else if (strcasecmp ((const char *) tree->name, "text") == 0)
	/* we get a text buffer */
	{
	  newTextDocument (tree);
	  numberChild++;
	}
      else if (strcasecmp ((const char *) tree->name, "macro") == 0)
	/* we get a Dr Geo macro-construction, not a mdi child ! */
	loadMacro (tree);

      tree = tree->next;
    }

  if (numberChild == 1)
      // it was a single file, the view is in p_activeView;
      p_activeView->setFilename ((const gchar *) filename);
  else if (numberChild > 1)
    {
      g_free (sessionPathName);
      sessionPathName = g_strdup (filename);
    }
  xmlFreeDoc (drgeniusXml);

  reconcile_grayout_widget ();
}

SCM thunk(void * data)
{
    return scm_c_eval_string((char *)data);
}

void
drgeniusMDI::evaluateScmFigure (gchar *filename)
{
  /* Just evaluate the scheme figure. All specifics stuffs as creating
     the tabs figures are handle from the scheme definition */

   scm_internal_catch(SCM_BOOL_T, thunk, (void *)filename, standard_handler, NULL);
  reconcile_grayout_widget ();
}

void
drgeniusMDI::closeView (drgeniusView *v)
{
  p_viewList = removeView (v);
  reconcile_grayout_widget ();
}
void
drgeniusMDI::renameView (drgeniusView *v, const gchar *newName)
{
  GtkWidget *w;
  gint n;

  gtk_notebook_set_tab_label_text (GTK_NOTEBOOK (p_noteBook),
                                   v->widgetView (),
                                   newName);
  gtk_notebook_set_menu_label_text (GTK_NOTEBOOK (p_noteBook),
                                    v->widgetView (),
                                    newName);
  v->setName (newName);

  // Rename the menu as well
  n = gtk_notebook_page_num (GTK_NOTEBOOK (p_noteBook), v->widgetView ());
  w = gtk_bin_get_child (GTK_BIN (getMenuItem (n)));
  gtk_label_set_text (GTK_LABEL (w), newName);
}

drgeniusView *
drgeniusMDI::activeView ()
{
  return p_activeView;
}

GList * 
drgeniusMDI::viewList ()
{
  return p_viewList;
}

void 
drgeniusMDI::reconcile_grayout_undo ()
{

  if (p_activeView)
    {
      if (p_activeView->undoActive ())
	{
	  gtk_widget_set_sensitive (p_undoButton, TRUE);
	  gtk_widget_set_sensitive (p_undoItem, TRUE);
	}
      else
	{
	  gtk_widget_set_sensitive (p_undoButton, FALSE);
	  gtk_widget_set_sensitive (p_undoItem, FALSE);
	}
      if (p_activeView->redoActive ())
	{
	  gtk_widget_set_sensitive (p_redoButton, TRUE);
	  gtk_widget_set_sensitive (p_redoItem, TRUE);
	}
      else
	{
	  gtk_widget_set_sensitive (p_redoButton, FALSE);
	  gtk_widget_set_sensitive (p_redoItem, FALSE);
	}
    }
  else
    {
	  gtk_widget_set_sensitive (p_undoButton, FALSE);
	  gtk_widget_set_sensitive (p_undoItem, FALSE);
	  gtk_widget_set_sensitive (p_redoButton, FALSE);
	  gtk_widget_set_sensitive (p_redoItem, FALSE);
    }
}

void
drgeniusMDI::reconcile_grayout_widget ()
{
  /* undo-redo menus/buttons */
  reconcile_grayout_undo ();
  /* Gray out or enable buttons/menus */
  if (p_activeView)
    {
      gtk_widget_set_sensitive (p_renameItem, TRUE);
      gtk_widget_set_sensitive (p_renameItemSc, TRUE);
      gtk_widget_set_sensitive (p_closeItem, TRUE);
      gtk_widget_set_sensitive (p_saveMultipleItem, TRUE);

      /* save menu */
      if (p_activeView->childType() == FIGURE_CHILD)
	{
	  gtk_widget_set_sensitive (p_customUiItem, TRUE);
	  gtk_widget_set_sensitive (p_exportItem, TRUE);
	  gtk_widget_set_sensitive (p_exportPreferencesItem, TRUE);
	  gtk_widget_set_sensitive (p_saveAsItem, TRUE);
	  gtk_widget_set_sensitive (p_saveItem, TRUE);
	  gtk_widget_set_sensitive (p_gridItem, TRUE);
	  gtk_widget_set_sensitive (p_gridItemSc, TRUE);
	  gtk_widget_set_sensitive (p_printFigure, TRUE);
	  gtk_widget_set_sensitive (p_macroMenu, TRUE);
	  gtk_widget_set_sensitive (p_animationMenu, TRUE);
	}
      else if (p_activeView->childType() == EDITOR_CHILD)
	{
	  gtk_widget_set_sensitive (p_customUiItem, FALSE);
	  gtk_widget_set_sensitive (p_exportItem, FALSE);
	  gtk_widget_set_sensitive (p_exportPreferencesItem, FALSE);
	  gtk_widget_set_sensitive (p_saveAsItem, FALSE);
	  gtk_widget_set_sensitive (p_saveItem, FALSE);
	  gtk_widget_set_sensitive (p_gridItem, FALSE);
	  gtk_widget_set_sensitive (p_gridItemSc, FALSE);
	  gtk_widget_set_sensitive (p_printFigure, FALSE);
	  gtk_widget_set_sensitive (p_macroMenu, FALSE);
	  gtk_widget_set_sensitive (p_animationMenu, FALSE);
	}
    }
  else
    {
      /* not active view,
         grey out save, filter and undo/redo menus/buttons */
      gtk_widget_set_sensitive (p_gridItem, FALSE);
      gtk_widget_set_sensitive (p_renameItem, FALSE);
      gtk_widget_set_sensitive (p_gridItemSc, FALSE);
      gtk_widget_set_sensitive (p_renameItemSc, FALSE);
      gtk_widget_set_sensitive (p_closeItem, FALSE);
      gtk_widget_set_sensitive (p_saveMultipleItem, FALSE);
      gtk_widget_set_sensitive (p_customUiItem, FALSE);
      gtk_widget_set_sensitive (p_exportItem, FALSE);
      gtk_widget_set_sensitive (p_exportPreferencesItem, FALSE);
      gtk_widget_set_sensitive (p_saveAsItem, FALSE);
      gtk_widget_set_sensitive (p_saveItem, FALSE);
      gtk_widget_set_sensitive (p_printFigure, FALSE);
      gtk_widget_set_sensitive (p_macroMenu, FALSE);
      gtk_widget_set_sensitive (p_animationMenu, FALSE);

    }
}

GList * 
drgeniusMDI::addView (drgeniusView *v)
{
  GtkWidget *label, *w;
  int curr_page;

  if (v->name ())
    {
      if (p_viewList == NULL)
	{
	  // First remove the NullView
	  gtk_container_remove (GTK_CONTAINER (p_drgeoMDIVbox),
				p_nullViewWidget);
	  p_noteBook = gtk_notebook_new ();
	  gtk_notebook_set_scrollable (GTK_NOTEBOOK (p_noteBook), true);
	  gtk_notebook_popup_enable (GTK_NOTEBOOK (p_noteBook));
	  gtk_notebook_set_tab_pos (GTK_NOTEBOOK (p_noteBook), GTK_POS_BOTTOM);
	  gtk_box_pack_start_defaults (GTK_BOX (p_drgeoMDIVbox), p_noteBook);
	  gtk_box_reorder_child (GTK_BOX (p_drgeoMDIVbox), p_noteBook, 2);
	  
	  // set a signal when page is changed
	  g_signal_connect (G_OBJECT (p_noteBook), "switch-page",
			    G_CALLBACK(on_notebook_switchPage),
			    (gpointer) this);
	  
	  gtk_widget_show (p_noteBook);
	}
      
      label = gtk_label_new ((const gchar *) v->name ());
      curr_page = gtk_notebook_get_current_page (GTK_NOTEBOOK (p_noteBook));
      gtk_notebook_insert_page (GTK_NOTEBOOK (p_noteBook), v->widgetView (),
				label, curr_page + 1);
      gtk_notebook_next_page (GTK_NOTEBOOK (p_noteBook));
      
      // Insert a menu item in the Windows menu
      w = gtk_menu_item_get_submenu (GTK_MENU_ITEM (p_windowsMenu));
      label = gtk_menu_item_new_with_label ((const gchar *) v->name ());
      gtk_menu_shell_insert (GTK_MENU_SHELL (w), label, curr_page + 1);
      gtk_widget_show (label);
      g_signal_connect (G_OBJECT (label), "activate",
			G_CALLBACK (on_windowsMenu_cb),
			(gpointer) v);
    
      p_activeView = v;
      return g_list_insert (p_viewList, (gpointer) v, curr_page + 1);
    }
  else
    {
      delete v;
      return NULL;
    }
}

GList *
drgeniusMDI::removeView (drgeniusView *v)
{
  gint n;
  GList * tmp;

  n = gtk_notebook_page_num (GTK_NOTEBOOK (p_noteBook), v->widgetView ());
  gtk_notebook_remove_page (GTK_NOTEBOOK (p_noteBook), n);

  tmp = g_list_remove(p_viewList, (gconstpointer) v);

  delete v;

  // Remove the menu item in the toolbar
  gtk_container_remove 
    (GTK_CONTAINER (gtk_menu_item_get_submenu (GTK_MENU_ITEM (p_windowsMenu))), getMenuItem (n));
  
  if (tmp == NULL)
    {
      gtk_container_remove (GTK_CONTAINER (p_drgeoMDIVbox),
			    p_noteBook);
      p_activeView = NULL;
      setNullView ();
    }
  else
    {
      n = gtk_notebook_get_current_page (GTK_NOTEBOOK (p_noteBook));
      p_activeView = (drgeniusView *) g_list_nth_data (tmp, n);
    }

  return tmp;
}

void 
drgeniusMDI::setMessage (const gchar * msg)
{
  static gint contextId = -1;

  if (contextId != -1)
    gtk_statusbar_pop (GTK_STATUSBAR (p_drgeoStatusbar), 
		       (guint) contextId);
  contextId = (gint) gtk_statusbar_get_context_id (GTK_STATUSBAR (p_drgeoStatusbar),
						   msg);
  gtk_statusbar_push (GTK_STATUSBAR (p_drgeoStatusbar),
		      (guint) contextId,
		      msg);		  
}

void
drgeniusMDI::setNullView ()
{
  // This view is an empty shell when no more view exist
  p_nullViewWidget = gtk_drawing_area_new ();
  gtk_widget_set_size_request(p_nullViewWidget, 320, 200);
  gtk_box_pack_start_defaults (GTK_BOX (p_drgeoMDIVbox), p_nullViewWidget);
  gtk_box_reorder_child (GTK_BOX (p_drgeoMDIVbox), p_nullViewWidget, 2);
  gtk_widget_show (p_nullViewWidget);
}

void
drgeniusMDI::setActiveView (gint page)
{
  p_activeView = (drgeniusView *) g_list_nth_data (p_viewList, page);
  reconcile_grayout_widget ();
}

void 
drgeniusMDI::setActiveView (drgeniusView *v)
{
  gint page;

  page = g_list_index (p_viewList, (gconstpointer) v);
  if (page != -1)
    gtk_notebook_set_current_page (GTK_NOTEBOOK (p_noteBook), page); 
}

GtkWindow *
drgeniusMDI::mainWindow ()
{
  return p_mainWindow;
}

void
drgeniusMDI::setTransientDialog (GtkWindow *dialog)
{
  gtk_window_set_transient_for (dialog, p_mainWindow);
}

GtkWidget *
drgeniusMDI::getMenuItem (gint n)
{
  GList *children;
  gpointer w;

  children = gtk_container_get_children 
    (GTK_CONTAINER (gtk_menu_item_get_submenu (GTK_MENU_ITEM (p_windowsMenu))));
  w = g_list_nth_data (children, n);
  g_list_free (children);

  return GTK_WIDGET (w);
}

void 
drgeniusMDI::addMacroMenuItem (gchar *name, gchar *description)
{
  GtkWidget *w, *item, *tips;
  w = gtk_menu_item_get_submenu (GTK_MENU_ITEM (p_macroMenu));
  item = gtk_menu_item_new_with_label ((const gchar *) name);
  gtk_menu_append (GTK_MENU_SHELL (w), item);
  gtk_widget_show (item);

  gtk_tooltips_set_tip (GTK_TOOLTIPS (p_tooltipsMacro), item,
			description, NULL);

  g_signal_connect (G_OBJECT (item), "activate",
		    G_CALLBACK (on_macroMenu_cb),
		    (gpointer) NULL);

}

void drgeniusMDI::
setMacroMenuItemDescription (gchar *name, gchar *description)
{
  GList *children, *p;
  GtkWidget *menuItem, *label;
  GtkTooltipsData* tips;
  const gchar *aName;

  children = gtk_container_get_children 
    (GTK_CONTAINER 
     (gtk_menu_item_get_submenu (GTK_MENU_ITEM (p_macroMenu))));
  p = children;
  while (p)
    {
      menuItem = GTK_WIDGET (p->data);
      label = gtk_bin_get_child (GTK_BIN (menuItem));
      aName = gtk_label_get_text (GTK_LABEL (label));
      if (!strcmp (name, aName))
	  break;
      p = g_list_next (p);
    }
  if (p == NULL)
    g_printerr ("Cannot not find macro %s in the macro menu\n",name);
  else
    {
      tips = gtk_tooltips_data_get (menuItem);
      if (tips)
      {
	g_free(tips->tip_text);
	tips->tip_text = g_strdup (description);
      }
      else
	gtk_tooltips_set_tip (GTK_TOOLTIPS (p_tooltipsMacro), menuItem,
			      description, NULL);
    }
  g_list_free (children);
}


gboolean 
on_drgeoMain_delete_event (GtkWidget *widget, GdkEvent *event,
			   gpointer user_data)
{
  GtkWidget *d;
  gint r;

  drgeniusView *view;
  if (texmacsMode)
    {
      // just send a encapsulated postscript version of the current
      // figure to TeXmacs
      if (((view = mdi->activeView ()) != NULL ) 
	  && (view->childType () == FIGURE_CHILD))
	{
	  putc (DATA_BEGIN, stdout);
	  fprintf (stdout, "ps:");
	  (((geoView *) view)->figure ())->exportToPostScriptHandle 
	    (stdout, "Exported to TeXmacs");
	  putc (DATA_END, stdout);
	  fflush (stdout);
	}
      return true; 
    }

  d = gtk_message_dialog_new (mdi->mainWindow (),
			      GTK_DIALOG_DESTROY_WITH_PARENT,
			      GTK_MESSAGE_QUESTION,
			      GTK_BUTTONS_YES_NO,
			      _("Are you sure to quit Dr. Geo?"),
			      g_strerror (errno));

  r = gtk_dialog_run (GTK_DIALOG (d));
  gtk_widget_destroy (d);

  switch (r)
    {
    case GTK_RESPONSE_YES:
      gtk_main_quit ();
      return false;
    case GTK_RESPONSE_NO:
      return true;
    }
}

void 
on_drgeoMain_destroy_event (GtkWidget *widget, GdkEvent *event,
			     gpointer user_data)
{
  gtk_main_quit ();
}

void
undo_cb (GtkWidget * widget, gpointer data)
{
  mdi->activeView ()->undo (0);
}
void
redo_cb (GtkWidget * widget, gpointer data)
{
  mdi->activeView ()->redo (0);
}


void
open_ok_cb (GtkWidget * widget, GtkWidget * dialog)
{
  gchar *filename;


  filename = (gchar *) gtk_file_selection_get_filename (GTK_FILE_SELECTION (dialog));
  if (!filename || !g_file_test (filename, G_FILE_TEST_EXISTS))
    {
      GtkWidget *dlg;

      /* make a new dialog with the file selection as parent */
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_ERROR,
				    GTK_BUTTONS_CLOSE,
				    _("Can't open file '%s'"),
				      filename, g_strerror (errno));
      
      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);      
      return;
    }
  g_free (openPathName);
  openPathName = g_strdup (filename);

  filename = g_strdup (filename);
  gtk_widget_destroy (dialog);

  mdi->openSession (filename);

  g_free (filename);
  mdi->reconcile_grayout_widget ();
}


void
open_cb (GtkWidget * widget, gpointer data)
{
  GtkWidget *dialog;

  dialog = gtk_file_selection_new (_("Open..."));
  if (openPathName)
    gtk_file_selection_set_filename (GTK_FILE_SELECTION (dialog),
				     openPathName);

  gtk_window_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);
  mdi->setTransientDialog (GTK_WINDOW (dialog));

  gtk_signal_connect_object (GTK_OBJECT
			     (GTK_FILE_SELECTION (dialog)->cancel_button),
			     "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (dialog));
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dialog)->ok_button),
		      "clicked", GTK_SIGNAL_FUNC (open_ok_cb),
		      (gpointer) dialog);
  gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (dialog));
  gtk_widget_show (dialog);
}

void
evaluate_ok_cb (GtkWidget * widget, GtkWidget * dialog)
{
  gchar *filename;


  filename = (gchar *) gtk_file_selection_get_filename (GTK_FILE_SELECTION (dialog));
  if (!filename || !g_file_test (filename, G_FILE_TEST_EXISTS))
    {
      GtkWidget *dlg;

      /* make a new dialog with the file selection as parent */
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_ERROR,
				    GTK_BUTTONS_CLOSE,
				    _("Can't open file '%s'"),
				      filename, g_strerror (errno));
      
      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);      
      return;
    }
  g_free (evaluatePathName);
  evaluatePathName = g_strdup (filename);

  filename = g_strdup (filename);
  gtk_widget_destroy (dialog);

  mdi->evaluateScmFigure (filename);

  g_free (filename);
  mdi->reconcile_grayout_widget ();
}

void 
evaluate_cb (GtkWidget * widget, gpointer data)
{
  GtkWidget *dialog;

  dialog = gtk_file_selection_new (_("Evaluate..."));

  if (evaluatePathName)
    gtk_file_selection_set_filename (GTK_FILE_SELECTION (dialog),
				     evaluatePathName);

  gtk_window_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);
  mdi->setTransientDialog (GTK_WINDOW (dialog));

  gtk_signal_connect_object (GTK_OBJECT
			     (GTK_FILE_SELECTION (dialog)->cancel_button),
			     "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (dialog));
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dialog)->ok_button),
		      "clicked", GTK_SIGNAL_FUNC (evaluate_ok_cb),
		      (gpointer) dialog);
  gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (dialog));
  gtk_widget_show (dialog);
}

/* takes the file dialog in the data field, and the reply has to be 0 */
void
really_save (gint reply, GtkWidget *fileDialog)
{
  gchar *filename;

  filename = (gchar *) gtk_file_selection_get_filename (GTK_FILE_SELECTION (fileDialog));

  if (reply == GTK_RESPONSE_NO || !(mdi->activeView()))
      return;
  /* call the signal calling wrapper for "save" */
  if (!(mdi->activeView()->save (filename)))
    {
      GtkWidget *dlg;

      /* make a new dialog with the file selection as parent */
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_ERROR,
				    GTK_BUTTONS_CLOSE,
				    _("Error saving to '%s'!"),
				    filename, g_strerror (errno));
      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);
      return;
    }
  gtk_widget_destroy (fileDialog);
}

/* takes the file dialog in the data field, and the reply has to be 0 */
void
really_save_session (gint reply, GtkWidget *fileDialog)
{
  const gchar *filename;

  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (fileDialog));

  if (reply == GTK_RESPONSE_NO)
    return;

  /* save the XML tree in drgeniusXml */
  if (!xmlSaveFile (filename, drgeniusXml))
    {
      GtkWidget *dlg;

      /* make a new dialog with the file selection as parent */
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_ERROR,
				    GTK_BUTTONS_CLOSE,
				    _("Error saving to '%s'!"),
				    filename, g_strerror (errno));
      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);
      return;
    }
  xmlFreeDoc (drgeniusXml);
  drgeniusXml = NULL;
  gtk_widget_destroy (fileDialog);
}

void
really_export_figure_flydraw (gint reply, GtkWidget *fileDialog)
{
  const gchar *filename;
  drgeoFigure *figure;

  if (reply == GTK_RESPONSE_NO)
    return;

  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (fileDialog));
  figure = ((geoView *)mdi->activeView())->figure();
  
  if (!figure->exportToFlydraw (filename))
    {
      GtkWidget *dlg;

      /* make a new dialog with the file selection as parent */
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_ERROR,
				    GTK_BUTTONS_CLOSE,
				    _("Error exporting to '%s'!"),
				    filename, g_strerror (errno));
      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);
      return;
    }
  gtk_widget_destroy (fileDialog);
}

void
really_export_figure_latex (gint reply, GtkWidget *fileDialog)
{
  const gchar *filename;
  drgeoFigure *figure;

  if (reply == GTK_RESPONSE_NO)
    return;

  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (fileDialog));
  figure = ((geoView *)mdi->activeView())->figure();
  
  if (!figure->exportToLatex (filename))
    {
      GtkWidget *dlg;

      /* make a new dialog with the file selection as parent */
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_ERROR,
				    GTK_BUTTONS_CLOSE,
				    _("Error exporting to '%s'!"),
				    filename, g_strerror (errno));
      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);
      return;
    }
  gtk_widget_destroy (fileDialog);
}

void
really_export_figure_postscript (gint reply, GtkWidget *fileDialog)
{
  const gchar *filename;
  drgeoFigure *figure;

  if (reply == GTK_RESPONSE_NO)
    return;

  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (fileDialog));

  figure = ((geoView *)mdi->activeView())->figure();
  
  if (!figure->exportToPostScript (filename))
    {
      GtkWidget *dlg;

      /* make a new dialog with the file selection as parent */
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_ERROR,
				    GTK_BUTTONS_CLOSE,
				    _("Error exporting to '%s'!"),
				    filename, g_strerror (errno));
      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);
      return;
    }
  gtk_widget_destroy (fileDialog);
}

void
really_export_figure_png (gint reply, GtkWidget *fileDialog)
{
  const gchar *filename;
  drgeoFigure *figure;
  gchar *tmpFile, *str;

  if (reply == GTK_RESPONSE_NO)
    return;

  // we will use a temporary EPS file to generate a 600 DPI bitmapt

  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (fileDialog));
  figure = ((geoView *)mdi->activeView())->figure();
  
  tmpFile = g_strconcat (g_get_home_dir (), "/.tmpbitmap-drgeo.eps", NULL);

  if (!figure->exportToPostScript (tmpFile))
    {
      GtkWidget *dlg;

      /* make a new dialog with the file selection as parent */
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_ERROR,
				    GTK_BUTTONS_CLOSE,
				    _("Error exporting to '%s'!"),
				    filename, g_strerror (errno));
      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);
      g_free (tmpFile);
      return;
    }

  // we got a EPS, generate the PNG

  if (!g_file_test ("/usr/bin/convert", G_FILE_TEST_EXISTS))
    {
      GtkWidget *dlg;

      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_ERROR,
				    GTK_BUTTONS_CLOSE,
				    _("Error exporting to '%s'!\nExporting to PNG required the Image Magick package.\nInstall it and try again!"),
				    filename, g_strerror (errno));
      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);
    }
  else
    {

      str = g_strconcat ("convert -transparent white -colors 16 -density 300 ", tmpFile, " ", 
			 filename, NULL);
      g_spawn_command_line_async (str, NULL);
      g_free (str);      
    }  
  g_free (tmpFile);
  gtk_widget_destroy (fileDialog);
}

void 
define_printing_area_cb (GtkWidget * widget, gpointer data)
{
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  figure = ((geoView *) (mdi->activeView ()))->figure ();
  area = (drgeoGtkDrawable *) figure->getDrawable ();
  // Change the current tool to the printing area tool
  area->setTool (area->printingAreaTool);
  // we do not need any filter for this mode, set it to null
  figure->setMode (PRINTING_AREA_MODE, NULL);
  mdi->setMessage (_("Define the printing area"));
}

void 
remove_printing_area_cb (GtkWidget * widget, gpointer data)
{
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  figure = ((geoView *) (mdi->activeView ()))->figure ();
  area = (drgeoGtkDrawable *) figure->getDrawable ();
  area->unsetPrintingArea ();
}

void
save_ok_cb (GtkWidget * widget, GtkWidget * dialog)
{
  const gchar *filename;

  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (dialog));
  if (!filename)
    return;
  if (g_file_test (filename, G_FILE_TEST_EXISTS))
    {
      /* file exists so ask first before writing */
      GtkWidget *dlg;

      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_QUESTION,
				    GTK_BUTTONS_YES_NO,
				    _("File '%s' exists!\nOverwrite?"),
				    filename, g_strerror (errno));
      really_save (gtk_dialog_run (GTK_DIALOG (dlg)), dialog);      
      gtk_widget_destroy (dlg);
    }
  else
    /* just call the really save callback to issue to save signal */
    really_save (GTK_RESPONSE_YES, dialog);
}

void
save_as_cb (GtkWidget * widget, gpointer data)
{
  GtkWidget *dialog;
  drgeniusView *child;

  child = mdi->activeView ();

  if (!child)
    return;

  if (!child->supportsSave ())
    return;

  dialog = gtk_file_selection_new (_("Save As..."));
  if (child->filename ())
    gtk_file_selection_set_filename (GTK_FILE_SELECTION (dialog),
				     child->filename ());
  else
    gtk_file_selection_set_filename (GTK_FILE_SELECTION (dialog),
				     drgeoConfigGet (":figureFile"));
  gtk_window_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);
  mdi->setTransientDialog (GTK_WINDOW (dialog));

  gtk_signal_connect_object (GTK_OBJECT
			     (GTK_FILE_SELECTION (dialog)->cancel_button),
			     "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (dialog));
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dialog)->ok_button),
		      "clicked", GTK_SIGNAL_FUNC (save_ok_cb),
		      (gpointer) dialog);
  gtk_widget_show (dialog);
}

void
save_cb (GtkWidget * widget, gpointer data)
{
  drgeniusView *child;

  child = mdi->activeView ();

  if (!child)
    return;

  if (!child->supportsSave ())
    return;

  /* if we have a filename */
  if (child->filename ())
    /* call the signal calling wrapper for "save" */
    child->save (child->filename());
  else
    /* just call the above callback for "Save As..." */
    save_as_cb (NULL, data);
}
void
save_session_ok_cb (GtkWidget * widget, GtkWidget * dialog)
{
  const gchar *filename;

  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (dialog));
  if (!filename)
    return;
  g_free (sessionPathName);
  sessionPathName = g_strdup (filename);
  if (g_file_test (filename, G_FILE_TEST_EXISTS))
    {
      /* file exists so ask first before writing */
      GtkWidget *dlg;

      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_QUESTION,
				    GTK_BUTTONS_YES_NO,
				    _("File '%s' exists!\nOverwrite?"),
				    filename, g_strerror (errno));
      really_save_session (gtk_dialog_run (GTK_DIALOG (dlg)), dialog);      
      gtk_widget_destroy (dlg);
    }
  else
    really_save_session (GTK_RESPONSE_YES, dialog);
}

void
export_figure_flydraw_ok_cb (GtkWidget * widget, GtkWidget * dialog)
{
  const gchar *filename;

  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (dialog));
  if (!filename)
    return;
  if (g_file_test (filename, G_FILE_TEST_EXISTS))
    {
      /* file exists so ask first before writing */
      GtkWidget *dlg;
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_QUESTION,
				    GTK_BUTTONS_YES_NO,
				    _("File '%s' exists!\nOverwrite?"),
				    filename, g_strerror (errno));
      really_export_figure_flydraw 
	(gtk_dialog_run (GTK_DIALOG (dlg)), dialog);      
      gtk_widget_destroy (dlg);
    }
  else
    really_export_figure_flydraw (GTK_RESPONSE_YES, dialog);
}

void
export_figure_latex_ok_cb (GtkWidget * widget, GtkWidget * dialog)
{
  const gchar *filename;

  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (dialog));
  if (!filename)
    return;
  if (g_file_test (filename, G_FILE_TEST_EXISTS))
    {
      /* file exists so ask first before writing */
      GtkWidget *dlg;
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_QUESTION,
				    GTK_BUTTONS_YES_NO,
				    _("File '%s' exists!\nOverwrite?"),
				    filename, g_strerror (errno));
      really_export_figure_latex 
	(gtk_dialog_run (GTK_DIALOG (dlg)), dialog);      
      gtk_widget_destroy (dlg);
    }
  else
    really_export_figure_latex (GTK_RESPONSE_YES, dialog);
}

void
export_figure_postscript_ok_cb (GtkWidget * widget, GtkWidget * dialog)
{
  const gchar *filename;

  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (dialog));
  if (!filename)
    return;
  if (g_file_test (filename, G_FILE_TEST_EXISTS))
    {
      /* file exists so ask first before writing */
      GtkWidget *dlg;

      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_QUESTION,
				    GTK_BUTTONS_YES_NO,
				    _("File '%s' exists!\nOverwrite?"),
				    filename, g_strerror (errno));
      really_export_figure_postscript 
	(gtk_dialog_run (GTK_DIALOG (dlg)), dialog);      
      gtk_widget_destroy (dlg);
    }
  else
    really_export_figure_postscript (GTK_RESPONSE_YES, dialog);
}

void
export_figure_png_ok_cb (GtkWidget * widget, GtkWidget * dialog)
{
  const gchar *filename;

  filename = gtk_file_selection_get_filename (GTK_FILE_SELECTION (dialog));
  if (!filename)
    return;
  if (g_file_test (filename, G_FILE_TEST_EXISTS))
    {
      /* file exists so ask first before writing */
      GtkWidget *dlg;

      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_QUESTION,
				    GTK_BUTTONS_YES_NO,
				    _("File '%s' exists!\nOverwrite?"),
				    filename, g_strerror (errno));
      really_export_figure_png (gtk_dialog_run (GTK_DIALOG (dlg)), dialog);      
      gtk_widget_destroy (dlg);
    }
  else
    really_export_figure_png (GTK_RESPONSE_YES, dialog);
}

void
save_multiple_cb (GtkWidget * widget, gpointer data)
{
  GError* error = NULL;
  GtkBuilder *xmlSessionWidget = gtk_builder_new ();
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeSelection *select;
  GtkTreeStore *store;
  GtkTreeIter iter;
  GList *childList;
  gint childNumber, i = 0;
  drgeniusView *child;
  GtkWidget *tree, *dialog;
  gchar *text[2], *dataName[] = {
    N_("Interactive 2D Figure"),
    N_("Console"),
    N_("Text Buffer"),
  };

  childList = mdi->viewList ();
  childNumber = g_list_length (childList);
  if (childNumber == 0)
    return;

  if (!gtk_builder_add_from_file (xmlSessionWidget, DRGEO_GLADEDIR "/drgenius2.glade", &error))
	  {
	    g_warning ("Couldn't load builder file: %s", error->message);
	    g_error_free (error);
	  }

  gtk_builder_connect_signals (xmlSessionWidget, &error);

  dialog = GTK_WIDGET (gtk_builder_get_object (xmlSessionWidget, "dialogSaveSession"));
  mdi->setTransientDialog (GTK_WINDOW (dialog));

  // Prepare the tree and the model
  tree = GTK_WIDGET (gtk_builder_get_object (xmlSessionWidget, "treeviewSelection"));
  g_object_unref (G_OBJECT (xmlSessionWidget));
  // Type - Name - Adress 
  store = gtk_tree_store_new (3, G_TYPE_STRING, G_TYPE_STRING, 
			      G_TYPE_POINTER);
  gtk_tree_view_set_model (GTK_TREE_VIEW (tree), GTK_TREE_MODEL (store));
  g_object_unref (G_OBJECT (store));
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes (_("Type"),
						     renderer,
						     "text", 0,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);
  column = gtk_tree_view_column_new_with_attributes (_("Name"),
						     renderer,
						     "text", 1,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);

  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (tree));
  gtk_tree_selection_set_mode (GTK_TREE_SELECTION (select), 
			       GTK_SELECTION_MULTIPLE);
  
  while (i < childNumber)
    {
      if ((child = (drgeniusView *) g_list_nth_data (childList, i)) == NULL)
	break;
      text[1] = child->name ();
      switch (child->childType ())
	{
	case FIGURE_CHILD:
	  text[0] = _(dataName[0]);
	  break;
	case EDITOR_CHILD:
	  text[0] = _(dataName[2]);
	  break;
	}
      gtk_tree_store_append (store, &iter, NULL);  /* Acquire a top-level iterator */
      gtk_tree_store_set (store, &iter,
			  0, text[0],
			  1, text[1],
			  2, child,
			  -1);
      i++;
    }

  /* Next we add the list of avalaible macro-construction */
  text[0] = N_("Macro-construction");
  text[1] = firstMacroName ();
  while (text[1] != NULL)
    {
      gtk_tree_store_append (store, &iter, NULL);  /* Acquire a top-level iterator */
      gtk_tree_store_set (store, &iter,
			  0, _(text[0]),
			  1, text[1],
			  -1);
      text[1] = nextMacroName ();
    }

  dialogSaveSession_response 
    (gtk_dialog_run(GTK_DIALOG(dialog)), select);
  
  gtk_widget_destroy (dialog);

}
void 
print_figure_cb (GtkWidget * widget, gpointer data)
{
  drgeniusView *child;
  GError* error = NULL;
  GtkBuilder *xml = gtk_builder_new ();
  GtkWidget *w;
  GtkOptionMenu *list;
  GtkMenu *printers;
  int n, printerCount, defaultPrinter;
  PrintDestination *pDestinations, *pd;

  child = mdi->activeView ();
  if (!child)
    return;

  if (!gtk_builder_add_from_file (xml, DRGEO_GLADEDIR "/drgeo2.glade", &error))
	  {
	    g_warning ("Couldn't load builder file: %s", error->message);
	    g_error_free (error);
	  }
  gtk_builder_connect_signals (xml, &error);
  w = GTK_WIDGET (gtk_builder_get_object (xml, "printFigureDialog")); 
  list = GTK_OPTION_MENU (GTK_WIDGET (gtk_builder_get_object (xml, "printerList"))); 
  
  mdi->setTransientDialog (GTK_WINDOW (w));


  /* set the printer list */
  printers = GTK_MENU (gtk_menu_new ());

  /* Get the printer list */
  utilPrinterGetPrinters (&printerCount, &defaultPrinter, 
			  &pDestinations, NULL,NULL);
  /* Create the menu entries */ 
  for (n = 1, pd = pDestinations; n <= printerCount; n++, pd++)
      gtk_menu_append_label (printers, (const gchar *) pd->pdPrinterName);
  gtk_option_menu_set_menu (list, GTK_WIDGET (printers));

  print_view (gtk_dialog_run (GTK_DIALOG (w)), list, printerCount,
	      pDestinations);

  /* Free the pDestinations */
  g_free (pDestinations);
  gtk_widget_destroy (w);

}

void
print_view (gint reply, GtkOptionMenu *list, int printerCount,
	    PrintDestination *pDestinations)
{
  int selectedPrinter, errorNumber;
  gchar *tmpFile, *command;
  drgeoFigure *figure;

  if (reply == GTK_RESPONSE_ACCEPT)
    {

      selectedPrinter = gtk_option_menu_get_history (list);
  
      tmpFile = g_strconcat (g_get_home_dir (), "/.tmpprint-drgeo.eps", NULL);
      figure = ((geoView *)mdi->activeView())->figure();

      if (!figure->exportToPostScript (tmpFile))
	{
	  GtkWidget *dlg;

	  dlg = gtk_message_dialog_new (mdi->mainWindow (),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_MESSAGE_ERROR,
					GTK_BUTTONS_CLOSE,
					_("Error printing the current figure!"),
					g_strerror (errno));
	  gtk_dialog_run (GTK_DIALOG (dlg));
	  gtk_widget_destroy (dlg);
	  g_free (tmpFile);
	  return;
	}
      
      /* Send to printer */
      command = g_strdup_printf (pDestinations[selectedPrinter].pdCommand, tmpFile);
      g_spawn_command_line_async (command, NULL);
		 
      g_free (tmpFile);
      g_free (command);

      /* Use something with GS?:

	 gs -q -sDEVICE=pswrite -sPAPERSIZE=a4 -sOutputFile=fig.ps figure_test.eps -dNOPROMPT -dBATCH

       */
    }
}


void
export_figure_to_flydraw_cb (GtkWidget * widget, gpointer data)
{
  drgeniusView *child;
  gchar *fileName;
  gchar **split;
  GtkWidget *dlg;

  child = mdi->activeView ();

  if (!child)
    return;

  /* only export figure right now */
  if (child->childType () != FIGURE_CHILD)
    {
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_WARNING,
				    GTK_BUTTONS_CLOSE,
				    _("Only geometric figure\ncan be exported to Flydraw!"),
				    NULL, g_strerror (errno));

      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);
      return;
    }
  /* if we have a filename */
  if (child->filename ())
    {
      split = g_strsplit (g_path_get_basename (child->filename ()), ".", 2);
      fileName = g_strconcat (split[0], ".draw", NULL);
      g_strfreev (split);
    }
  else
    fileName = g_strdup (drgeoConfigGet (":flydrawFile"));

  dlg = gtk_file_selection_new (_("Export As..."));
  gtk_window_set_modal (GTK_WINDOW (dlg), TRUE);
  gtk_file_selection_set_filename (GTK_FILE_SELECTION (dlg), fileName);
  gtk_window_position (GTK_WINDOW (dlg), GTK_WIN_POS_MOUSE);
  gtk_signal_connect_object (GTK_OBJECT
			     (GTK_FILE_SELECTION (dlg)->cancel_button),
			     "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (dlg));
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dlg)->ok_button),
		      "clicked", GTK_SIGNAL_FUNC (export_figure_flydraw_ok_cb),
		      (gpointer) dlg);
  gtk_widget_show (dlg);
  g_free (fileName);
}

void
export_figure_to_latex_cb (GtkWidget * widget, gpointer data)
{
  drgeniusView *child;
  gchar *fileName;
  gchar **split;
  GtkWidget *dlg;

  child = mdi->activeView ();

  if (!child)
    return;

  /* only export figure right now */
  if (child->childType () != FIGURE_CHILD)
    {
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_WARNING,
				    GTK_BUTTONS_CLOSE,
				    _("Only geomteric figure\ncan be exported to LaTex!"),
				    NULL, g_strerror (errno));

      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);
      return;
    }
  /* if we have a filename */
  if (child->filename ())
    {
      split = g_strsplit (g_path_get_basename (child->filename ()), ".", 2);
      fileName = g_strconcat (split[0], ".tex", NULL);
      g_strfreev (split);
    }
  else
    fileName = g_strdup (drgeoConfigGet (":latexFile"));

  dlg = gtk_file_selection_new (_("Export As..."));
  gtk_window_set_modal (GTK_WINDOW (dlg), TRUE);
  gtk_file_selection_set_filename (GTK_FILE_SELECTION (dlg), fileName);
  gtk_window_position (GTK_WINDOW (dlg), GTK_WIN_POS_MOUSE);
  gtk_signal_connect_object (GTK_OBJECT
			     (GTK_FILE_SELECTION (dlg)->cancel_button),
			     "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (dlg));
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dlg)->ok_button),
		      "clicked", GTK_SIGNAL_FUNC (export_figure_latex_ok_cb),
		      (gpointer) dlg);
  gtk_widget_show (dlg);
  g_free (fileName);
}

void
export_figure_to_postscript_cb (GtkWidget * widget, gpointer data)
{
  drgeniusView *child;
  gchar *fileName;
  gchar **split;
  GtkWidget *dlg;

  child = mdi->activeView ();

  if (!child)
    return;

  /* only export figure right now */
  if (child->childType() != FIGURE_CHILD)
    {
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_WARNING,
				    GTK_BUTTONS_CLOSE,
				    _("Only geomteric figure\ncan be exported to PostScript!"),
				    NULL, g_strerror (errno));
      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);
      return;
    }
  /* if we have a filename */
  if (child->filename())
    {
      split = g_strsplit (g_path_get_basename (child->filename()), ".", 2);
      fileName = g_strconcat (split[0], ".eps", NULL);
      g_strfreev (split);
    }
  else
    fileName = g_strdup (drgeoConfigGet (":psFile"));

  dlg = gtk_file_selection_new (_("Export As..."));
  mdi->setTransientDialog (GTK_WINDOW (dlg));
  gtk_window_set_modal (GTK_WINDOW (dlg), TRUE);
  gtk_file_selection_set_filename (GTK_FILE_SELECTION (dlg), fileName);
  gtk_window_position (GTK_WINDOW (dlg), GTK_WIN_POS_MOUSE);
  gtk_signal_connect_object (GTK_OBJECT
			     (GTK_FILE_SELECTION (dlg)->cancel_button),
			     "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (dlg));
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dlg)->ok_button),
		      "clicked",
		      GTK_SIGNAL_FUNC (export_figure_postscript_ok_cb),
		      (gpointer) dlg);
  gtk_widget_show (dlg);
  g_free (fileName);
}

void
export_figure_to_png_cb (GtkWidget * widget, gpointer data)
{
  drgeniusView *child;
  gchar *fileName;
  gchar **split;
  GtkWidget *dlg;

  child = mdi->activeView ();

  if (!child)
    return;

  /* only export figure right now */
  if (child->childType() != FIGURE_CHILD)
    {
      dlg = gtk_message_dialog_new (mdi->mainWindow (),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_WARNING,
				    GTK_BUTTONS_CLOSE,
				    _("Only geomteric figure\ncan be exported to PNG!"),
				    NULL, g_strerror (errno));
      gtk_dialog_run (GTK_DIALOG (dlg));
      gtk_widget_destroy (dlg);
      return;
    }
  /* if we have a filename */
  if (child->filename())
    {
      split = g_strsplit (g_path_get_basename (child->filename()), ".", 2);
      fileName = g_strconcat (split[0], ".png", NULL);
      g_strfreev (split);
    }
  else
    {
      split = g_strsplit (g_path_get_basename (drgeoConfigGet (":psFile")), ".", 2);
      fileName = g_strconcat (split[0], ".png", NULL);
      g_strfreev (split);
    }
  dlg = gtk_file_selection_new (_("Export As..."));
  mdi->setTransientDialog (GTK_WINDOW (dlg));
  gtk_window_set_modal (GTK_WINDOW (dlg), TRUE);
  gtk_file_selection_set_filename (GTK_FILE_SELECTION (dlg), fileName);
  gtk_window_position (GTK_WINDOW (dlg), GTK_WIN_POS_MOUSE);
  gtk_signal_connect_object (GTK_OBJECT
			     (GTK_FILE_SELECTION (dlg)->cancel_button),
			     "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (dlg));
  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dlg)->ok_button),
		      "clicked",
		      GTK_SIGNAL_FUNC (export_figure_png_ok_cb),
		      (gpointer) dlg);
  gtk_widget_show (dlg);
  g_free (fileName);
}

void 
gridItem_cb (GtkWidget * widget, gpointer data)
{
  drgeoFigure *figure;
  drgeoGtkDrawable *area;

  figure = ((geoView *) (mdi->activeView ()))->figure ();
  figure->setShowGridState (!figure->getShowGridState ());  
  area = (drgeoGtkDrawable *) figure->getDrawable ();
  area->refresh ();
}

void
rename_view_cb (GtkWidget * widget, gpointer data)
{
  GError* error = NULL;
  GtkBuilder *xml = gtk_builder_new ();  
  GtkWidget *w, *entry;

  if (!mdi->activeView ())
    return;

  if (!gtk_builder_add_from_file (xml, DRGEO_GLADEDIR "/drgeo2.glade", &error))
	  {
	    g_warning ("Couldn't load builder file: %s", error->message);
	    g_error_free (error);
	  }
  gtk_builder_connect_signals (xml, &error);
  w = GTK_WIDGET (gtk_builder_get_object (xml, "renameViewDialog")); 
  entry = GTK_WIDGET (gtk_builder_get_object (xml, "entryRenameView")); 

  g_signal_connect (G_OBJECT (entry), "activate",
		    G_CALLBACK(on_entryRenameView_activate),
		    (gpointer) w);
  
  mdi->setTransientDialog (GTK_WINDOW (w));

  rename_view_string (gtk_dialog_run (GTK_DIALOG (w)), entry);
  gtk_widget_destroy (w);
}

void on_entryRenameView_activate (GtkWidget *w, gpointer data)
{

  gtk_dialog_response (GTK_DIALOG (data), GTK_RESPONSE_OK);
}


void
rename_view_string (gint reply, GtkWidget *entry)
{
  const gchar *str;

  if (reply == GTK_RESPONSE_OK)
    {
      str = gtk_entry_get_text (GTK_ENTRY (entry)); 
      mdi->renameView (mdi->activeView (), str);
    }
}

void
customize_interface_cb (GtkWidget * widget, gpointer data)
{
  drgeniusView *child;

  child = mdi->activeView ();

  switch (child->childType())
    {
    case FIGURE_CHILD:
      adaptDrgeoDialog ( (geoView *) child);
      break;
    case EDITOR_CHILD:
      /* No toolbar for now */
      break;
    }
}

void 
on_contents_activate (GtkWidget *widget, gpointer data)
{
  open_help_cb (NULL, drgeniusHelp[3]);
}

void
new_figure_cb (GtkWidget * widget, gpointer data)
{
  mdi->newGeometricDocument (NULL);
  mdi->reconcile_grayout_widget ();
}

void
new_editor_cb (GtkWidget * widget, gpointer data)
{
  mdi->newTextDocument (NULL);
  mdi->reconcile_grayout_widget ();
}

void
close_cb (GtkWidget * widget, gpointer data)
{
  GtkWidget *d;
  gint r;

  d = gtk_message_dialog_new (mdi->mainWindow (),
			      GTK_DIALOG_DESTROY_WITH_PARENT,
			      GTK_MESSAGE_QUESTION,
			      GTK_BUTTONS_YES_NO,
			      _("Are you sure to close this figure?"),
			      g_strerror (errno));

  r = gtk_dialog_run (GTK_DIALOG (d));
  gtk_widget_destroy (d);

  if (r == GTK_RESPONSE_YES)
    {
      mdi->closeView (mdi->activeView());
    }
}
void
quit_cb (GtkWidget * widget, gpointer data)
{
    on_drgeoMain_delete_event (NULL, NULL, NULL);
}

void
on_about_activate (GtkWidget * widget, gpointer data)
{
  GtkWidget *dialog, *l, *hbox, *vbox, *img, *ofsetLogo;
  gchar *str;


  dialog = gtk_dialog_new_with_buttons (_("About Dr. Geo"),
					mdi->mainWindow (),
					GTK_DIALOG_MODAL,
					GTK_STOCK_OK,
					GTK_RESPONSE_ACCEPT,
					NULL);
  

  // Pack logo and info
  hbox = gtk_hbox_new (FALSE, 4);
  img = gtk_image_new_from_file (DRGEO_ICONDIR "/drgeo-book.png");
  ofsetLogo = gtk_image_new_from_file (DRGEO_ICONDIR "/ofset-logo.png");
  // VERSION
  str = g_strconcat ("GNU Dr. Geo " VERSION, _("\nFree Interactive Geometry\n\n(c) Copyright 1996-2005 H. Fernandes  \n(c) Copyright 2000 L. Gauthier\n(c) Copyright 2000-2005 OFSET\nTranslation: translator_name"), NULL);
  l = gtk_label_new (str);
  g_free (str);
  gtk_box_pack_start (GTK_BOX (hbox), ofsetLogo, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX (hbox), l, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (hbox), img, FALSE, FALSE, 0);

  vbox = gtk_vbox_new (FALSE, 0);
  l = gtk_label_new ("http://www.ofset.org/drgeo");
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), l, FALSE, FALSE, 0);
  
  
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG(dialog)->vbox), vbox, FALSE, FALSE, 0);
  gtk_widget_show_all (dialog);
  
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}


// Helper functions to get the name of avalaible macro in the registry
char *
firstMacroName ()
{
  drgeoMacroRegistry *registry;
  macro *mac;

  registry = drgeoMacroRegistry::get ();

  if ((mac = registry->first ()) == NULL)
    return NULL;
  return mac->getName ();
}

char *
nextMacroName ()
{
  drgeoMacroRegistry *registry;
  macro *mac;

  registry = drgeoMacroRegistry::get ();

  if ((mac = registry->next ()) == NULL)
    return NULL;
  return mac->getName ();
}

gboolean
saveMacro (gchar * name, xmlNodePtr tree)
{
  drgeoMacroRegistry *registry;

  registry = drgeoMacroRegistry::get ();
  return registry->save (name, tree);
}

gboolean
loadMacro (xmlNodePtr macroXml)
{
  drgeoMacroRegistry *registry;

  registry = drgeoMacroRegistry::get ();
  return registry->load (macroXml);
}

void 
dialogSaveSession_response (gint r, GtkTreeSelection *select)
{
  GtkWidget *dialog;

  switch (r)
    {
    case 2:
      // save all
      // select all row, then go through
      gtk_tree_selection_select_all (select);
    case 1:
      // Save selection
      drgeniusXml = xmlNewDoc (BAD_CAST "1.0");
      drgeniusXml->xmlRootNode = xmlNewDocNode (drgeniusXml, NULL,
				     BAD_CAST "drgenius", NULL);
      gtk_tree_selection_selected_foreach
	(select, saveSelectedRow, (gpointer) drgeniusXml);
      dialog = gtk_file_selection_new (_("Save Session As..."));
      if (!sessionPathName)
	gtk_file_selection_set_filename (GTK_FILE_SELECTION (dialog),
					 drgeoConfigGet (":sessionFile"));
      else
	gtk_file_selection_set_filename (GTK_FILE_SELECTION (dialog),
					 sessionPathName);
      
      gtk_window_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);
      mdi->setTransientDialog (GTK_WINDOW (dialog));
      gtk_signal_connect_object (GTK_OBJECT
				 (GTK_FILE_SELECTION (dialog)->cancel_button),
				 "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
				 GTK_OBJECT (dialog));
      gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (dialog)->ok_button),
			  "clicked", GTK_SIGNAL_FUNC (save_session_ok_cb),
			  (gpointer) dialog);
      gtk_widget_show (dialog);
      break;
    case GTK_RESPONSE_CANCEL:
      return;
    }
}  

void 
saveSelectedRow (GtkTreeModel *model, GtkTreePath *path,
		 GtkTreeIter *iter, gpointer data)
{
  gchar *type, *name;
  gpointer item;
  xmlNodePtr tree;

  
  tree = (xmlNodePtr) data;
  gtk_tree_model_get (model, iter, 
		     0, &type,
		     1, &name,
		     -1);
 
  if (!strcmp (type, _("Interactive 2D Figure")) ||
      !strcmp (type, _("Text Buffer")))
    {
     gtk_tree_model_get (model, iter, 
			 2, &item, -1);
       ((drgeniusView *) item )->saveNode (tree->xmlRootNode);
    }
  else if (!strcmp (type, _("Macro-construction")))
    saveMacro (name, tree->xmlRootNode);
  else
    {
      GtkWidget *dialog;
	// unknown type
	dialog = gtk_message_dialog_new (mdi->mainWindow (),
					 GTK_DIALOG_DESTROY_WITH_PARENT,
					 GTK_MESSAGE_ERROR,
					 GTK_BUTTONS_CLOSE,
					 "saveSelectedRow: Unknown data type '%s' to save\n",
					 type, g_strerror (errno));
      gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_widget_destroy (dialog);
    }
  g_free (type);
  g_free (name);
}

void 
on_notebook_switchPage (GtkNotebook *notebook, GtkNotebookPage *page, 
			gint p, gpointer data)
{
  mdi = (drgeniusMDI *) data;
  mdi->setActiveView (p);
}

void on_windowsMenu_cb (GtkWidget * widget, gpointer v)
{
  mdi->setActiveView ((drgeniusView *) v);
}

void on_macroMenu_cb (GtkWidget * widget, gpointer v)
{
  drgeoFigure *figure;
  drgeoGtkDrawable *area;
  GtkWidget *label;
  const gchar *text;

  // go in macro mode
  if (mdi->activeView ()->childType () != FIGURE_CHILD)
    {
      g_printerr ("drgenius_mdi::on_macroMenu_cb not a FIGURE_CHILD while accessing macro-construction menu\n");
      return;
    }
  figure = ((geoView *) mdi->activeView ())->figure ();
  area = (drgeoGtkDrawable *) figure->getDrawable ();
  area->setTool (area->macroPlayToolDialogLess);
  figure->setMode (MACRO_PLAY_MODE, NULL);
  mdi->setMessage (_("Play a recorded macro-construction"));

  // Switch the macroPlayToolDialogLess to the selected macro from the
  // menu
  label = gtk_bin_get_child (GTK_BIN (widget));
  text = gtk_label_get_text (GTK_LABEL (label));
  if (!((drgeoMacroPlayToolDialogLess *)area->macroPlayToolDialogLess)
      ->selectMacro (text))
    g_printerr ("drgenius_mdi::on_macroMenu_cb can't select macro %s\n",
		text);
}

static gboolean
is_clist_row_selected (GtkCList * clist, gint row)
{
  GList *node = g_list_first (clist->selection);
  while (node)
    {
      if (row == GPOINTER_TO_INT (node->data))
	return (TRUE);
      node = g_list_next (node);
    }
  return (FALSE);
}


/* ----------------- Preferences box CODE  -------------------- */
void
preferences_box_cb (GtkWidget * widget, gpointer data)
{
  initPreferencesBox ();
}

/* ------- Animation --------------*/

static void
setAnimationMode (gdouble step)
{
  drgeoFigure *figure;
  drgeoGtkDrawable *area;
  GtkWidget *label;
  const gchar *text;

  // go in animation mode
  if (mdi->activeView ()->childType () != FIGURE_CHILD)
    {
      g_printerr ("drgenius_mdi::on_animation?_activate not a FIGURE_CHILD while switching to animation menu\n");
      return;
    }
  figure = ((geoView *) mdi->activeView ())->figure ();
  area = (drgeoGtkDrawable *) figure->getDrawable ();
  if (figure->getMode () != ANIMATION_MODE)
    {
      area->setTool (area->animationToolDialogLess);
      figure->setMode (ANIMATION_MODE, NULL);
      mdi->setMessage (_("Select a free point to animate it"));
    }

  // Set the drgeoAnimationDialogLess to the selected step
  ((drgeoAnimationDialogLess *)area->animationToolDialogLess)
    ->setStep (step);
}

void
on_animation1_activate (GtkWidget * widget, gpointer data)
{
  setAnimationMode (0.01);
}
void
on_animation2_activate (GtkWidget * widget, gpointer data)
{
  setAnimationMode (0.02);

}
void
on_animation3_activate (GtkWidget * widget, gpointer data)
{
  setAnimationMode (0.04);

}
void
on_animation4_activate (GtkWidget * widget, gpointer data)
{
  setAnimationMode (0.08);

}
