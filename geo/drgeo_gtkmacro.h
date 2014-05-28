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

#ifndef DRGEO_GTK_MACRO_H
#define DRGEO_GTK_MACRO_H

#include <gtk/gtk.h>

#include "drgeo_drawable.h"
#include "drgeo_macro.h"

enum macroBuildDialogPage
{
  pageIntro = 0,
  pageInput,
  pageOutput,
  pageName
};

class drgeoGtkMacroBuildDialog:public drgeoMacroBuildDialog
{
public:
  drgeoGtkMacroBuildDialog (class drgeoMacroBuilder * builder);
  ~drgeoGtkMacroBuildDialog ();
  void show ();
  void hide ();
  void clear ();
  void add (geometricObject * item);
  void handleCurrentPage ();
  void handleFinish ();
  void handleCancel ();
  macroBuildDialogPage currentPage ();
  GtkNotebook * notebookWidget ();
  GtkButton * nextWidget ();
  GtkButton * previousWidget ();
private:
  GtkWidget *p_dialog, *p_input, *p_output, *p_text, *p_entry, 
    *p_next, *p_previous, *p_notebook;
  GtkListStore *p_inputStore, *p_outputStore;
};

class drgeoGtkMacroPlayDialog:public drgeoMacroPlayDialog
{
public:
  drgeoGtkMacroPlayDialog (drgeoMacroPlayer * player);
  ~drgeoGtkMacroPlayDialog ();
  void show ();
  void hide ();
  void add (geometricObject * item);
  void handleFinish ();
  void handleSelect (gchar *macro);
  void handleUnSelect ();
  gint currentPage ();
  GtkNotebook * notebookWidget ();
  GtkButton * nextWidget ();
  GtkButton * previousWidget ();
private:
  char *macroName;
  GtkWidget  *p_notebook,*p_dialog, *p_list, *p_description,
    *p_next, *p_previous;
  GtkListStore *p_listStore;
};

#endif
