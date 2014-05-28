/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Free Software Foundation  1997-2002
 * hilaire@ofset.org 
 * lolo@seul.org
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

#ifndef DRGEO_GTKDRAWABLE_H
#define DRGEO_GTKDRAWABLE_H

#include "config.h"
#include <gtk/gtk.h>
#include "drgeo_drawable.h"
#include "drgeo_gtkmacro.h"
#include "geo_view.h"

// Implementation of Gtk drawable.
class drgeoGtkDrawable:public drgeoDrawableUI
{
public:
  drgeoGtkDrawable (drgeoFigure * figure, geoView *view);
  ~drgeoGtkDrawable ();
  // These are the concrete implementation of the
  void drawPoint (drgeoStyle & style, drgeoPoint & point);
  void drawPolygon (drgeoStyle & style, drgeoPoint * point, gint number);
  void drawLine (drgeoStyle & style, drgeoPoint & start, drgeoPoint & end);
  void drawHalfLine (drgeoStyle & style, drgeoPoint & start,
		     drgeoVector & vect);
  void drawSegment (drgeoStyle & style, drgeoPoint & start, drgeoPoint & end);
  void drawCircle (drgeoStyle & style, drgeoPoint & center, double radius);
  void drawCircle (drgeoStyle & style, drgeoPoint & center,
		   drgeoPoint & point);
  void drawArc (drgeoStyle & style, drgeoPoint & center, double radius,
		double start, double length);
  void drawText (drgeoPoint & where, char *text, drgeoColorType fontColor);
  double stringWidth (char *text);
  double stringHeight (char *text);
  
  void showPrintingArea (drgeoPoint& corner1, drgeoPoint& corner2);
  void _showPrintingArea ();
  void unsetPrintingArea ();
  gboolean printingArea ();
  drgeoPoint getPrintingAreaBR ();
  drgeoPoint getPrintingAreaTL ();
  void showTip (drgeoPoint & where, gchar *text);
  void _showTip ();
  void removeTip ();
  gboolean tipOn ();
  void setCursor (drgeoCursor cursor);
  double getRange ();

  void clear ();
  void chooseItem (liste_elem * list);
  bool askOkCancel (char *message);
  class drgeoMacroBuildDialog *createMacroBuildDialog (drgeoMacroBuilder *
						       builder);
  class drgeoMacroPlayDialog *createMacroPlayDialog (drgeoMacroPlayer *
						     player);
  class drgeoStyleDialog *createStyleDialog ();
  class drgeoPropertyDialog *createPropertyDialog ();

  // Interface specific to the Gtk interface.  This methods are to be
  // invoked only by the GUI code, and in particular the Gtk signal
  // handlers.  Consider these as private.
  GtkWidget *getGtkDrawable ();
  GtkWidget *getTopWidget ();
  void adjustRulersRange ();
  void exposeCallback ();
  void configureCallback ();
  void pressCallback (GdkEventButton * event);
  void releaseCallback (GdkEventButton * event);
  void moveCallback (GdkEventMotion * event);
  void moveWholeFigure (GdkEventMotion * event);
  void horizontalScrollCallback ();
  void verticalScrollCallback ();
  void updateAdjustments ();
  void timerCallback ();
  void setScale (double newScale);
  drgeoPoint getAreaCenter ();
  void setAreaCenter (drgeoPoint &p);
  drgeoPoint getAreaSize ();
  void updateUndoState ();

  // Invoked to refresh completely the display
  void _refresh ();
  // just emit an expose event to refresh the figure 
  void refresh ();  

  // Description for figure
  void clearDescription ();
  void addDescriptionItem (geometricObject * item);

  geoView * getGeoView ();

  GdkColor color[drgeoColorNumber];
  drgeoCursor currentCursor;

private:
    friend class drgeoGtkStyleDialog;

  GdkGC *figure_gc, *font_gc, *object_gc;
  GtkWidget *drawing_area, *hrule, *vrule, 
    *p_topWidget;

  GdkWindow *paintThere;

  GtkAdjustment *hadjustment, *vadjustment;
  double origin_x, origin_y;
  // Coordinates of the point located at
  // the center of the drawable (not in
  // pixels, but in world units).

  // delimit the printing area (top-left and bottom-right)
  drgeoPoint tl, br;
  gboolean printingAreaDefined;

  gint timeoutTag;
  int counter;

  GtkWidget *drgeoLayout;

  geoView *p_geoView;
  gchar *tip;
  drgeoPoint tipPosition;
};

#endif
