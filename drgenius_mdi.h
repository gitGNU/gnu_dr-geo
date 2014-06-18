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

#ifndef DRGENIUS_MDI_H
#define DRGENIUS_MDI_H

// #define G_PLATFORM_WIN32

#include <gtk/gtk.h>
#include <gmodule.h>
#include <libguile.h>
#include "drgeo_printer.h"
#include "xmlinclude.h"
#include "drgenius_view.h"

// This class handle the multi document interface

class drgeniusMDI
{
 public:
  drgeniusMDI ();
  ~drgeniusMDI ();
  void newGeometricDocument (xmlNodePtr tree);
  void newTextDocument (xmlNodePtr tree);
  void openSession (gchar *filename);
  void evaluateScmFigure (gchar *filenalme);
  void closeView (drgeniusView *v);
  void renameView (drgeniusView *v, const gchar *newName);
  drgeniusView * activeView ();
  GList * viewList ();
  void reconcile_grayout_undo ();
  void reconcile_grayout_widget ();
  GList *addView (drgeniusView *v);
  GList *removeView (drgeniusView *v);
  void setMessage (const gchar * msg);
  void setNullView ();
  void setActiveView (gint page);
  void setActiveView (drgeniusView *v);
  GtkWindow * mainWindow ();
  void setTransientDialog (GtkWindow * dialog);
  GtkWidget *getMenuItem (gint n);
  void addMacroMenuItem (gchar *name, gchar *description);
  void setMacroMenuItemDescription (gchar *name, gchar *description);
 private:
  GtkWidget *p_undoButton, *p_undoItem, *p_redoButton, 
    *p_redoItem, *p_gridItem, *p_gridItemSc, *p_renameItem, *p_renameItemSc, *p_customUiItem,
    *p_saveItem, *p_saveAsItem, *p_saveMultipleItem, *p_printFigure,
    *p_exportItem, *p_exportPreferencesItem, *p_closeItem, 
    *p_drgeoStatusbar, *p_noteBook, *p_drgeoMDIVbox, *p_nullViewWidget,
    *p_windowsMenu, *p_macroMenu, *p_animationMenu;
  GtkTooltip *p_tooltipsMacro;
  GtkWindow *p_mainWindow;
  drgeniusView *p_activeView;
  GList *p_viewList;
  
};

//thunk declaration for scm_internal_catch
extern SCM thunk(void *data);

// Helper functions to get the name of avalaible macro in the registry
char *firstMacroName ();
char *nextMacroName ();
gboolean saveMacro (gchar * name, xmlNodePtr tree);
gboolean loadMacro (xmlNodePtr macroXml);

// Callback for the Session Save Dialog
static void dialogSaveSession_response (gint r, GtkTreeSelection *select);
static void saveSelectedRow (GtkTreeModel *model, GtkTreePath *path,
			     GtkTreeIter *iter, gpointer data);
static void on_notebook_switchPage (GtkNotebook *notebook, 
				    GtkNotebookPage *page,
				    gint p, gpointer data);
static void on_windowsMenu_cb (GtkWidget * widget, gpointer v);
static void on_macroMenu_cb (GtkWidget * widget, gpointer v);

// Callback to libglade
extern "C" {
/*********************************/
/* Handlers to the MDI interface */
/*********************************/
G_MODULE_EXPORT static void nothing_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT gboolean on_drgeoMain_delete_event (GtkWidget *widget, 
						    GdkEvent *event, 
						    gpointer user_data);
G_MODULE_EXPORT void on_drgeoMain_destroy_event (GtkWidget *widget, 
						 GdkEvent *event,
						 gpointer user_data);
G_MODULE_EXPORT void undo_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void redo_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void new_figure_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void new_editor_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void close_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void quit_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void on_about_activate (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void open_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void evaluate_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void save_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void save_as_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void save_multiple_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void print_figure_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void print_view (gint reply, GtkOptionMenu *list, 
				 int printerCount, 
				 PrintDestination *pDestinations);
G_MODULE_EXPORT void export_figure_to_flydraw_cb (GtkWidget * widget, 
						  gpointer data);
G_MODULE_EXPORT void export_figure_to_latex_cb (GtkWidget * widget, 
						gpointer data);
G_MODULE_EXPORT void export_figure_to_postscript_cb (GtkWidget * widget, 
						     gpointer data);
G_MODULE_EXPORT void export_figure_to_png_cb (GtkWidget * widget, 
					      gpointer data);
G_MODULE_EXPORT void define_printing_area_cb (GtkWidget * widget, 
					      gpointer data);
G_MODULE_EXPORT void remove_printing_area_cb (GtkWidget * widget, 
					      gpointer data);
G_MODULE_EXPORT void save_session_ok_cb (GtkWidget * widget, 
					 GtkWidget * dialog);
G_MODULE_EXPORT void export_figure_flydraw_ok_cb (GtkWidget * widget, 
						  GtkWidget * dialog);
G_MODULE_EXPORT void export_figure_latex_ok_cb (GtkWidget * widget, 
						GtkWidget * dialog);
G_MODULE_EXPORT void export_figure_postscript_ok_cb (GtkWidget * widget, 
						     GtkWidget * dialog);
G_MODULE_EXPORT void export_figure_png_ok_cb (GtkWidget * widget, 
					      GtkWidget * dialog);
G_MODULE_EXPORT void gridItem_cb (GtkWidget * widget, 
				  gpointer data);
G_MODULE_EXPORT void rename_view_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void rename_view_string(gint reply, GtkWidget *entry);
G_MODULE_EXPORT void on_entryRenameView_activate (GtkWidget *w, gpointer data);
G_MODULE_EXPORT void customize_interface_cb (GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void on_contents_activate (GtkWidget *widget, gpointer data);
/*----- Property box ----------- */
G_MODULE_EXPORT void preferences_box_cb (GtkWidget * widget, gpointer data);
/*------ Animation -------*/
G_MODULE_EXPORT void on_animation1_activate(GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void on_animation2_activate(GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void on_animation3_activate(GtkWidget * widget, gpointer data);
G_MODULE_EXPORT void on_animation4_activate(GtkWidget * widget, gpointer data);
}

#endif /* DRGENIUS_MDI_H */
