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

#ifndef DRGENIUS_VIEW_H
#define DRGENIUS_VIEW_H


#include <gtk/gtk.h>
#include "xmlinclude.h"


enum drgeniusViewType
{
  FIGURE_CHILD,
  EDITOR_CHILD
};

class drgeniusView
{
 public:
  drgeniusView (drgeniusViewType type, gboolean save);
  virtual ~ drgeniusView ();
  virtual gboolean save (gchar *filename);
  virtual gboolean saveNode (xmlNodePtr tree) = 0;
  virtual gboolean undo (gint n);
  virtual gboolean redo (gint n);
  virtual gboolean undoActive ();
  virtual gboolean redoActive ();
  void setToolbar (GtkWidget *w);
  GtkWidget * toolbar ();
  // Force the view to update its user interface (show/hide tools)
  // according to its state defined into the document object (ie
  // figure, etc.)
  virtual void updateUserInterface () = 0;
  virtual gpointer createWidgetView () = 0;
  virtual GtkWidget * widgetView () = 0;

  gboolean supportsSave ();
  drgeniusViewType childType ();
  gchar * filename ();
  gchar * name ();
  void setName (const gchar *name);
  void setFilename (const gchar *name);
 protected:
  drgeniusViewType p_childType;
  gboolean p_supportsSave;
  gchar *p_filename, *p_name;
  GtkWidget *p_toolbar;
};

#endif /* DRGENIUS_VIEW_H */
