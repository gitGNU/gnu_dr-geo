/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes, Laurent Gauthier  1997-2001
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

#ifndef DRGEO_GTK_STYLE_H
#define DRGEO_GTK_STYLE_H

#include <gtk/gtk.h>

#include "drgeo_drawable.h"


class drgeoGtkStyleDialog:public drgeoStyleDialog
{
public:
  drgeoGtkStyleDialog (class drgeoGtkDrawable * aDrawable);
  ~drgeoGtkStyleDialog ();
  void show ();
  void hide ();
  void setColor (drgeoColorType color);
  void setSize (drgeoThicknessType size);
  void setShape (drgeoPointType shape);
  void setFill (bool fill);
  void setName (const gchar *name);
  void setVisibility (masked m);
  // Adjust the dialog state according to the aItem style
  void edit (class geometricObject * aItem);
private:
  drgeoGtkDrawable * drawable;
  geometricObject *item;
  GtkWidget *dialog;
  GladeXML *xml;
  objectCategory category;
};

#endif
