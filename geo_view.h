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

#ifndef GEO_VIEW_H
#define GEO_VIEW_H

#include "drgenius_view.h"
#include "drgeo_geometricObject.h"

class drgeoGtkDrawable;
class drgeoMenu;
class drgeoToolbar;
class drgeoFigure;


enum drgeoZoomFactor
  {
    z100 = 100,
    z200 = 200,
    z175 = 175,
    z150 = 150,
    z125 = 125,
    z75 = 75,
    z50 = 50,
    z25 = 25
  };


class geoView:public drgeniusView
{
 public:
  geoView (xmlNodePtr drgeoXml);
  ~geoView ();
  gboolean save (gchar *filename);
  gboolean saveNode (xmlNodePtr tree);
  gboolean undo (gint n);
  gboolean redo (gint n);
  gboolean undoActive ();
  gboolean redoActive ();
  gpointer createWidgetView ();
  GtkWidget * widgetView ();
  drgeoFigure *figure ();
  void setZoomFactorTb (drgeoZoomFactor zoom);
  void setZoomFactorMenu (drgeoZoomFactor zoom);
  void updateUserInterface ();
  void addDescription (geometricObject *item);
  void clearDescription ();
 private:
  drgeoFigure *p_figure;
  GtkWidget *p_geoView, *p_figureArea, *p_descrTree;
  drgeoMenu *p_menu;
  drgeoToolbar *p_toolbar;
  GtkTreeStore *p_descrStore;
};


double zoomToScale (gfloat zoom);
drgeoZoomFactor scaleToZoom (double scale);


#endif /* GEO_VIEW_H */
