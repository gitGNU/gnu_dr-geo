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

#ifndef EDITOR_VIEW_H
#define EDITOR_VIEW_H

#include "drgenius_view.h"

class editorView:public drgeniusView
{
 public:
  editorView (xmlNodePtr editorXml);
  ~editorView ();
  gboolean saveNode (xmlNodePtr tree);
  gpointer createWidgetView ();
  void updateUserInterface () {};
  GtkWidget * widgetView ();
 private:
  GtkWidget *p_textView, *p_view;
};



#endif /* EDITOR_VIEW_H */
