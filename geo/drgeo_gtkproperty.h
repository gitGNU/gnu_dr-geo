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

#ifndef DRGEO_GTK_PROPERTY_H
#define DRGEO_GTK_PROPERTY_H

#include <gtk/gtk.h>
#include "drgeo_drawable.h"

class drgeoGtkPropertyDialog:public drgeoPropertyDialog
{
public:

  drgeoGtkPropertyDialog (class drgeoGtkDrawable * aDrawable);
  ~drgeoGtkPropertyDialog ();
  void show ();
  void hide ();
  // to edit the property of the selected item
  void edit (class geometricObject * aItem);
  void moveItem (drgeoVector & t);
  void setValue (gdouble value);
  void setScript (gchar * script);
  void refresh ();
  geometricObject *getItem ();
    drgeoDrawable & getDrawable ();
private:
    drgeoGtkDrawable * drawable;
  geometricObject *item;
  GtkWidget *dialog;
  objectCategory category;
  gint type;
};

#endif
