/* Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-2000
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

#ifndef drgeo_menu_h
#define drgeo_menu_h

#include "drgeo_control.h"

class drgeoMenu: public drgeoControl
{
 public:
  drgeoMenu (geoView *view);
  ~drgeoMenu ();
  GtkWidget * controlerWidget ();
  void setZoomFactor (drgeoZoomFactor zoom);
 private:
  GtkWidget *p_popupMenu, *p_zoom200, *p_zoom175, *p_zoom150, *p_zoom125, *p_zoom100,
    *p_zoom75, *p_zoom50, *p_zoom25;  
};

// Sepcific callback for the menu

static gboolean
showGeoMenu (GtkWidget *menu, GdkEvent *event );

#endif
