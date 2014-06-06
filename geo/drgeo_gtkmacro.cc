/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes, Laurent Gauthier  1997-2000
 * hilaire@seul.org 
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


#include "drgeo_gtkmacro.h"
#include "drgeo_gtkdrawable.h"
#include "drgeo_numeric.h"
#include "drgeo_command.h"
#include "drgeo_gtkhelpers.h"
#include "drgenius_mdi.h"

extern drgeniusMDI *mdi;

// Help functions

static gboolean
removeIfListed (GtkListStore *store, gpointer item)
{
  GtkTreeIter iter;
  gboolean valid;
  
  /* Get the first iter in the list */
  valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(store), 
					 &iter);

  while (valid)
    {
      /* Walk through the list, reading each row */
      gpointer data;

      // the item pointer is in the column 2
      gtk_tree_model_get (GTK_TREE_MODEL(store), &iter, 
                          1, &data,
                          -1);
      if (data == item)
	{
	  gtk_list_store_remove (store, &iter);
	  return true;
	}
      valid = gtk_tree_model_iter_next (GTK_TREE_MODEL(store), 
					&iter);
    }
  return false;  
}

// Event handlers.
// build macro
static void
on_buildMacroDialog_response (GtkWidget *w, gint r, gpointer data)
{
  drgeoGtkMacroBuildDialog *drgeoDialog;
  macroBuildDialogPage page;

  drgeoDialog = (drgeoGtkMacroBuildDialog *) data;
  page = drgeoDialog->currentPage ();
  switch (r)
    {
    case 1:
      // Previous
      if (page != pageIntro)
	gtk_notebook_prev_page (drgeoDialog->notebookWidget());      
      break;
    case 2:
      // Next
      if (page != pageName)
	  gtk_notebook_next_page (drgeoDialog->notebookWidget());
      else
	{
	  // Finish
	  drgeoDialog->handleFinish ();
	}
      break;
    case GTK_RESPONSE_CANCEL:
      drgeoDialog->handleCancel ();
      break;
    }
  // set the macro builder to new page
  drgeoDialog->handleCurrentPage ();
  // get the new page
  page = drgeoDialog->currentPage ();
  if (page == pageName)
    //turn the next button as the Finish button
    gtk_button_set_label (drgeoDialog->nextWidget (),
			  GTK_STOCK_APPLY);
  else
    // otherwise set it as Next
    gtk_button_set_label (drgeoDialog->nextWidget (),
			  GTK_STOCK_GO_FORWARD);
  if (page == pageIntro)
      // grey the Previous button
    gtk_widget_set_sensitive (GTK_WIDGET (drgeoDialog->previousWidget ()),
			    false);
  else
    gtk_widget_set_sensitive (GTK_WIDGET (drgeoDialog->previousWidget ()),
			    true);
}
// player macro
static void
on_playMacroDialog_response (GtkWidget *w, gint r, gpointer data)
{
  drgeoGtkMacroPlayDialog *drgeoDialog;
  gint page;

  drgeoDialog = (drgeoGtkMacroPlayDialog *) data;
  page = drgeoDialog->currentPage ();
  switch (r)
    {
    case 1:
      // Previous
      if (page == 1)
	gtk_notebook_prev_page (drgeoDialog->notebookWidget());      
      break;
    case 2:
      // Next
      if (page == 0)
	  gtk_notebook_next_page (drgeoDialog->notebookWidget());
      break;
    case GTK_RESPONSE_CANCEL:
      drgeoDialog->handleFinish ();
      break;
    }
  page = drgeoDialog->currentPage ();
  if (page == 0)
    {
      // grey the Previous button
      gtk_widget_set_sensitive (GTK_WIDGET (drgeoDialog->previousWidget ()),
			    false);
      gtk_widget_set_sensitive (GTK_WIDGET (drgeoDialog->nextWidget ()),
			    true);
    }
  else
    {
      gtk_widget_set_sensitive (GTK_WIDGET (drgeoDialog->previousWidget ()),
				true);
      gtk_widget_set_sensitive (GTK_WIDGET (drgeoDialog->nextWidget ()),
				false);
    }
}
static void
macroSelected_cb (GtkTreeSelection *selection,  
		  drgeoGtkMacroPlayDialog *dialog)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *macro;

  if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
      gtk_tree_model_get (model, &iter, 0, &macro, -1);
      dialog->handleUnSelect ();
      dialog->handleSelect (macro);
    }
}

static gboolean
delete_event_cb (GtkWidget * widget, GdkEventAny * event,
		 drgeoGtkMacroBuildDialog * dialog)
{
  dialog->hide ();
  dialog->clear ();
  return true;
}

static void
cancel_event_cb (GtkWidget * widget, drgeoGtkMacroBuildDialog * dialog)
{
  dialog->handleCancel ();
}

static void
finish_event_cb (GtkWidget * widget, gpointer druidpage,
		 drgeoGtkMacroBuildDialog * dialog)
{
  dialog->handleFinish ();
}

void
drgeoGtkMacroBuildDialog::show ()
{
  gtk_widget_show (p_dialog);
}

void
drgeoGtkMacroBuildDialog::hide ()
{
  gtk_widget_hide (p_dialog);
}

void
drgeoGtkMacroBuildDialog::clear ()
{
  // Clear the input and output lists.
  gtk_list_store_clear (GTK_LIST_STORE (p_inputStore));
  gtk_list_store_clear (GTK_LIST_STORE (p_outputStore));

  // Clear the description field.
  gtk_text_view_clear (GTK_TEXT_VIEW (p_text));

  // Clear the macro name field.
  gtk_entry_set_text (GTK_ENTRY (p_entry), "");

  // Reset the builder.
  builder->clearOutput ();
  builder->clearInput ();
  builder->setName (NULL);
  builder->setDescription (NULL);
  // XXX dirty, this prevent a crash when user close the dialog
  // and still select items
  builder->setMode (drgeoMacroDescriptionMode);
}

void
drgeoGtkMacroBuildDialog::add (geometricObject * item)
{
  GtkTreeIter iter;
  GtkListStore *store;

  switch (builder->getMode ())
    {
    case drgeoMacroInputMode:
      builder->addInput (item);
      store = p_inputStore;
      break;
    case drgeoMacroOutputMode:
      builder->addOutput (item);
      store = p_outputStore;
      break;
    default:
      // If we are not in Input mode nor Output mode, we return
      return;
    }

  if (!removeIfListed (store, (gpointer) item))
    {      
      gtk_list_store_append (store, &iter);
      gtk_list_store_set (store, &iter,
			  0, item->getTypeName (),
			  1, item,
			  -1);
    }
}

void
drgeoGtkMacroBuildDialog::handleCurrentPage ()
{
  macroBuildDialogPage page;

  page = currentPage ();

  switch (page)
    {
    case pageIntro:
      builder->setMode (drgeoMacroStartMode);
      break;
    case pageInput:
      builder->setMode (drgeoMacroInputMode);
      break;
    case pageOutput:
      builder->setMode (drgeoMacroOutputMode);
      break;
    case pageName:
      builder->setMode (drgeoMacroDescriptionMode);
      break;
    }
}

void
drgeoGtkMacroBuildDialog::handleFinish ()
{
  macro *aMacro;
  drgeoMacroRegistry *registry;

  // Invoke the builder to create the new macro.
  builder->setName (gtk_entry_get_text (GTK_ENTRY (p_entry)));

  builder->setDescription (gtk_text_view_get_text (GTK_TEXT_VIEW (p_text)));
  
  if (builder->checkParameters ())
    {
      // Create the new macro.
      aMacro = builder->createMacro ();

      // Register it.
      registry = drgeoMacroRegistry::get ();
      registry->add (aMacro);

      // Add the macro in the menu bar menu for macro
      mdi->addMacroMenuItem (builder->getName (),
			     builder->getDescription ());

      // Clean everything.
      hide ();
      clear ();
    }
}

void
drgeoGtkMacroBuildDialog::handleCancel ()
{
  hide ();
  clear ();
}

macroBuildDialogPage 
drgeoGtkMacroBuildDialog::currentPage ()
{
  gint num;
  
  num = gtk_notebook_get_current_page (GTK_NOTEBOOK (p_notebook));
  switch (num)
    {
    case 0:
      return pageIntro;
    case 1:
      return pageInput;
    case 2:
      return pageOutput;
    case 3:
      return pageName;
    default:
      // Problem if we went there
      g_printerr ("Can not find the page in the macro dialog\n");
      return pageIntro;
    }
}

GtkNotebook * 
drgeoGtkMacroBuildDialog::notebookWidget ()
{
  return GTK_NOTEBOOK (p_notebook);
}

GtkButton *
drgeoGtkMacroBuildDialog::nextWidget ()
{
  return GTK_BUTTON (p_next);
}
GtkButton *
drgeoGtkMacroBuildDialog::previousWidget ()
{
  return GTK_BUTTON (p_previous);
}

drgeoGtkMacroBuildDialog::drgeoGtkMacroBuildDialog (drgeoMacroBuilder *
						    builder)
{
  GtkBuilder *xmlDialogWidget;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;


  this->builder = builder;
  this->builder->setMode (drgeoMacroStartMode);

  xmlDialogWidget = glade_xml_new (DRGEO_GLADEDIR "/drgeo2.glade",
				   "buildMacroDialog", NULL);

  p_dialog = glade_xml_get_widget (xmlDialogWidget, "buildMacroDialog");
  mdi->setTransientDialog (GTK_WINDOW (p_dialog));
  p_input = glade_xml_get_widget (xmlDialogWidget, "input");
  p_output = glade_xml_get_widget (xmlDialogWidget, "output");
  p_entry = glade_xml_get_widget (xmlDialogWidget, "entry");
  p_text = glade_xml_get_widget (xmlDialogWidget, "text");
  p_notebook = glade_xml_get_widget (xmlDialogWidget, "notebook");
  p_next = glade_xml_get_widget (xmlDialogWidget, "next");
  p_previous = glade_xml_get_widget (xmlDialogWidget, "previous");
  
  // grey the previous button as we start with the first page
  gtk_widget_set_sensitive (p_previous, false);

  
  g_object_unref (G_OBJECT (xmlDialogWidget));

  // Connect the signals of the ...
  // response signal of the dialog
  g_signal_connect (G_OBJECT (p_dialog), "response",
		    G_CALLBACK (on_buildMacroDialog_response),
		    (gpointer) this);

  gtk_signal_connect (GTK_OBJECT (p_dialog), "delete_event",
		      GTK_SIGNAL_FUNC (delete_event_cb), this);

  // Prepare the List Store

  // input list
  p_inputStore = gtk_list_store_new (2, G_TYPE_STRING,
				     G_TYPE_POINTER);
  gtk_tree_view_set_model (GTK_TREE_VIEW(p_input),
			   GTK_TREE_MODEL (p_inputStore));
  g_object_unref (G_OBJECT (p_inputStore));
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Item",
						     renderer,
						     "text", 0,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (p_input), column);

  // output list
  p_outputStore = gtk_list_store_new (2, G_TYPE_STRING,
				      G_TYPE_POINTER);
  gtk_tree_view_set_model (GTK_TREE_VIEW(p_output),
			   GTK_TREE_MODEL (p_outputStore));
  g_object_unref (G_OBJECT (p_outputStore));
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Item",
						     renderer,
						     "text", 0,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (p_output), column);
  
}

drgeoGtkMacroBuildDialog::~drgeoGtkMacroBuildDialog ()
{
  gtk_widget_destroy (p_dialog);
}

// Event handlers.

static gboolean
play_delete_event_cb (GtkWidget * widget, GdkEventAny * event,
		      drgeoGtkMacroPlayDialog * dialog)
{
  dialog->handleFinish ();
  return true;
}

void
drgeoGtkMacroPlayDialog::handleSelect (gchar * macro)
{
  macroName = macro;
  // Inform the player of the new macro
  player->setMacro (macroName);
  // Update the description buffer of the dialog
  gtk_text_view_set_text (GTK_TEXT_VIEW (p_description), 
			  player->getDescription ());
}

void
drgeoGtkMacroPlayDialog::handleUnSelect ()
{
  // update description text buffer in case the user change it
  player->setDescription (gtk_text_view_get_text (GTK_TEXT_VIEW (p_description)));

  // Clear item selection
  player->setMacro (NULL);
  // Forget the selected macro
  macroName = NULL;
  // clear the description buffer of dialog
  gtk_text_view_clear (GTK_TEXT_VIEW (p_description));
}

void
drgeoGtkMacroPlayDialog::show ()
{
  gtk_widget_show (p_dialog);
}

void
drgeoGtkMacroPlayDialog::hide ()
{
  gtk_widget_hide (p_dialog);
}

void
drgeoGtkMacroPlayDialog::handleFinish ()
{
  // update description text buffer in case the user change it
  player->setDescription (gtk_text_view_get_text (GTK_TEXT_VIEW (p_description)));

  gtk_list_store_clear (GTK_LIST_STORE (p_listStore));

  // clear the dialog description buffer
  gtk_text_view_clear (GTK_TEXT_VIEW (p_description));
  // unselect the macro and the selected item
  player->setMacro (NULL);
  hide ();
}

void
drgeoGtkMacroPlayDialog::add (geometricObject * item)
{
  // only valid item come there
  if (currentPage () == 1)
    player->add (item);
}

drgeoGtkMacroPlayDialog::drgeoGtkMacroPlayDialog (drgeoMacroPlayer * player)
{
  GtkBuilder *xmlDialogWidget;
  drgeoMacroRegistry *registry;
  macro *aMacro;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeSelection *select;
  GtkTreeIter iter;

  this->player = player;
  macroName = NULL;
  player->setMacro (NULL);
  xmlDialogWidget = glade_xml_new (DRGEO_GLADEDIR "/drgeo2.glade",
				   "playMacroDialog", NULL);

  p_dialog = glade_xml_get_widget (xmlDialogWidget, "playMacroDialog");
  mdi->setTransientDialog (GTK_WINDOW (p_dialog));
  p_description = glade_xml_get_widget (xmlDialogWidget, "descriptionPlay");
  p_list = glade_xml_get_widget (xmlDialogWidget, "listPlay");
  p_notebook = glade_xml_get_widget (xmlDialogWidget, "notebook");
  p_next = glade_xml_get_widget (xmlDialogWidget, "next");
  p_previous = glade_xml_get_widget (xmlDialogWidget, "previous");

  g_object_unref (G_OBJECT (xmlDialogWidget));

  // first page, grey the the previous button
  gtk_widget_set_sensitive (p_previous, false);

  // Connect the signals
  gtk_signal_connect (GTK_OBJECT (p_dialog), "delete_event",
		      GTK_SIGNAL_FUNC (play_delete_event_cb), this);

  g_signal_connect (G_OBJECT (p_dialog), "response",
		    G_CALLBACK (on_playMacroDialog_response),
		    (gpointer) this);


  // Prepare List Store for macro list
  p_listStore = gtk_list_store_new (1, G_TYPE_STRING);
  gtk_tree_view_set_model (GTK_TREE_VIEW(p_list),
			   GTK_TREE_MODEL (p_listStore));
  g_object_unref (G_OBJECT (p_listStore));
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Item",
						     renderer,
						     "text", 0,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (p_list), column);
  
  // set the callback for the list widget
  select  = gtk_tree_view_get_selection (GTK_TREE_VIEW (p_list));
  gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (select), "changed",
		    G_CALLBACK (macroSelected_cb),
		    (gpointer) this);
  
  // Fill the list with the macro names.
  registry = drgeoMacroRegistry::get ();
  for (aMacro = registry->first ();
       !registry->isDone (); aMacro = registry->next ())
    {
      gtk_list_store_append (p_listStore, &iter);
      gtk_list_store_set (p_listStore, &iter,
			  0, aMacro->getName (),
			  -1);
    }
  // select the 1st macro when it exist
  if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL(p_listStore), 
				    &iter))
      gtk_tree_selection_select_iter (select, &iter);
}

drgeoGtkMacroPlayDialog::~drgeoGtkMacroPlayDialog ()
{
  gtk_widget_destroy (p_dialog);
}

gint 
drgeoGtkMacroPlayDialog::currentPage ()
{
  return gtk_notebook_get_current_page (GTK_NOTEBOOK (p_notebook));
}

GtkNotebook * 
drgeoGtkMacroPlayDialog::notebookWidget ()
{
  return GTK_NOTEBOOK (p_notebook);
}

GtkButton *
drgeoGtkMacroPlayDialog::nextWidget ()
{
  return GTK_BUTTON (p_next);
}
GtkButton *
drgeoGtkMacroPlayDialog::previousWidget ()
{
  return GTK_BUTTON (p_previous);
}


