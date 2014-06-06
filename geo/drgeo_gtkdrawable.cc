/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Free Software Foundation  1997-2003
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


#include <math.h>
#include <errno.h>

#include "drgeo_gtkdrawable.h"
#include "drgeo_gtkstyle.h"
#include "drgeo_gtkproperty.h"
#include "drgeo_gtkdrawable.h"
#include "drgeo_menu.h"
#include "drgenius_mdi.h"

extern drgeniusMDI *mdi;

#define WIDGET_HEIGHT(w) (((w)->allocation).height)
#define WIDGET_WIDTH(w) (((w)->allocation).width)

#define SCREEN_X(x) ((int) (width / 2.0 - origin_x * scale + x * scale))
#define SCREEN_Y(y) ((int) (height / 2.0 + origin_y * scale - y * scale))

static gdouble moveX, moveY;
static drgeoCursor previousCursor = none;

static gboolean
drawing_area_button_press_cb (GtkWidget * w, GdkEventButton * event,
			      gpointer data);
static gboolean
drawing_area_button_release_cb (GtkWidget * w, GdkEventButton * event,
				gpointer data);
static gboolean
drawing_area_move_cb (GtkWidget * w, GdkEventMotion * event, gpointer data);
static gboolean
drawing_area_expose_cb (GtkWidget * w, GdkEventExpose * e, gpointer data);
static gboolean
drawing_area_configure_cb (GtkWidget * w, GdkEventConfigure * e,
			   gpointer data);
static int timer_cb (drgeoGtkDrawable * drawable);
static void vadjustment_cb (GtkAdjustment * w, gpointer data);
static void hadjustment_cb (GtkAdjustment * w, gpointer data);
static void choose_item_cb (GtkWidget * w, geometricObject * choice);
static void dummy_cb (GtkWidget * w, gpointer data);

drgeoGtkDrawable::drgeoGtkDrawable (drgeoFigure * figure, geoView *view)
{
  GtkWidget *table, *widget, *widget1, *toolbar, *icon;
  static int counter = 1;	/* Used to generated title for new
				   documents.  */
  char *view_title;

  this->figure = figure;
  p_geoView = view;
  tip = NULL;

  scale = 30.0;			/* default is 30 pixels for 1 unit.  */
  origin_x = 0.0;
  origin_y = 0.0;

  printingAreaDefined = false;

  /* table to hold the rules, adjustments, drawing area */
  table = gtk_table_new (4, 3, FALSE);

  p_topWidget = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (p_topWidget), GTK_SHADOW_IN);
  gtk_container_add (GTK_CONTAINER (p_topWidget), table);


  /* The drawing area.  */
  drawing_area = gtk_drawing_area_new ();
  paintThere = NULL;
  figure_gc = NULL;

  // Do not use the gtkLayout now, I need to know why
  // it doesnt resize correctly when the parent widget is resized
  //
  // drgeoLayout = gtk_layout_new (NULL, NULL);
  // gtk_layout_put (GTK_LAYOUT (drgeoLayout), drawing_area, 0, 0);

  /* This was just to test the ability to plug widget in the figure
     widget = gtk_button_new_with_label ("Hello there !");
     gtk_layout_put (GTK_LAYOUT (drgeoLayout), widget, 30,30);
   */

  gtk_table_attach (GTK_TABLE (table), drawing_area, 1, 2, 2, 3,
		    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL | GTK_SHRINK),
		    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL | GTK_SHRINK),
		    0, 0);

  gtk_widget_grab_focus (drawing_area);
  gtk_widget_set_events (drawing_area, GDK_BUTTON_PRESS_MASK
			 | GDK_BUTTON_RELEASE_MASK
			 | GDK_POINTER_MOTION_MASK
			 | GDK_EXPOSURE_MASK | GDK_POINTER_MOTION_HINT_MASK);

  gtk_signal_connect (GTK_OBJECT (drawing_area),
		      "button_press_event",
		      GTK_SIGNAL_FUNC (drawing_area_button_press_cb), NULL);

  gtk_signal_connect (GTK_OBJECT (drawing_area),
		      "button_release_event",
		      GTK_SIGNAL_FUNC (drawing_area_button_release_cb), NULL);

  gtk_signal_connect (GTK_OBJECT (drawing_area),
		      "motion_notify_event",
		      GTK_SIGNAL_FUNC (drawing_area_move_cb), NULL);

  gtk_signal_connect (GTK_OBJECT (drawing_area),
		      "expose_event",
		      GTK_SIGNAL_FUNC (drawing_area_expose_cb), NULL);

  gtk_signal_connect (GTK_OBJECT (drawing_area),
		      "configure_event",
		      GTK_SIGNAL_FUNC (drawing_area_configure_cb),
		      drgeoLayout);



  /* Create the vertical and horizontal rulers.  */
  hrule = gtk_hruler_new ();
  vrule = gtk_vruler_new ();

  /* Horizontal adjustment.  */
  hadjustment =
    GTK_ADJUSTMENT (gtk_adjustment_new (0.0, -100.0, 100.0, 0.1, 0.5, 2.0));

  gtk_signal_connect (GTK_OBJECT (hadjustment),
		      "value_changed",
		      GTK_SIGNAL_FUNC (hadjustment_cb), (gpointer) hrule);

  /* Vertical adjustment.  */
  vadjustment =
    GTK_ADJUSTMENT (gtk_adjustment_new (0.0, -100.0, 100.0, 0.1, 0.5, 2.0));

  gtk_signal_connect (GTK_OBJECT (vadjustment),
		      "value_changed",
		      GTK_SIGNAL_FUNC (vadjustment_cb), (gpointer) vrule);

  /* Horizontal scrollbar. */
  widget = gtk_hscrollbar_new (GTK_ADJUSTMENT (hadjustment));

  gtk_table_attach (GTK_TABLE (table), widget, 1, 2, 3, 4,
		    (GtkAttachOptions) (GTK_FILL | GTK_SHRINK),
		    (GtkAttachOptions) (GTK_SHRINK), 0, 0);


  /* Vertical scrollbar. */
  widget = gtk_vscrollbar_new (GTK_ADJUSTMENT (vadjustment));

  gtk_table_attach (GTK_TABLE (table), widget, 2, 3, 2, 3,
		    (GtkAttachOptions) (GTK_SHRINK),
		    (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 0, 0);


  /* Macro definition used for rulers.  */
#define EVENT_METHOD(i, x) GTK_WIDGET_GET_CLASS(i)->x

  // GTK_WIDGET_CLASS(GTK_OBJECT(i)->klass)->x

  /* Setup the horizontal ruler.  */
  gtk_ruler_set_metric (GTK_RULER (hrule), GTK_PIXELS);

  g_signal_connect_swapped (G_OBJECT (drawing_area),"motion_notify_event",
			    G_CALLBACK( EVENT_METHOD (hrule,motion_notify_event)),
			    G_OBJECT (hrule));
  
  gtk_table_attach (GTK_TABLE (table), hrule, 1, 2, 1, 2,
		    (GtkAttachOptions) (GTK_FILL | GTK_SHRINK),
		    (GtkAttachOptions) (GTK_SHRINK), 0, 0);

  /* setup the vertical ruler */
  gtk_ruler_set_metric (GTK_RULER (vrule), GTK_PIXELS);

  g_signal_connect_swapped (G_OBJECT (drawing_area),"motion_notify_event",
			    G_CALLBACK( EVENT_METHOD (vrule,motion_notify_event)),
			    G_OBJECT (vrule));

  gtk_table_attach (GTK_TABLE (table), vrule, 0, 1, 2, 3,
		    (GtkAttachOptions) (GTK_SHRINK),
		    (GtkAttachOptions) (GTK_FILL | GTK_SHRINK), 0, 0);

  gtk_widget_show_all (p_topWidget);

  /* Associate the information about the figure to each of the
     important widgets for it.  And keep track of these widgets in the
     figure data structure.  */
  gtk_object_set_data (GTK_OBJECT (table), "figure", this);
  gtk_object_set_data (GTK_OBJECT (drawing_area), "figure", this);
  gtk_object_set_data (GTK_OBJECT (hrule), "figure", this);
  gtk_object_set_data (GTK_OBJECT (vrule), "figure", this);
  gtk_object_set_data (GTK_OBJECT (hadjustment), "figure", this);
  gtk_object_set_data (GTK_OBJECT (vadjustment), "figure", this);

  currentCursor = arrow;
}

drgeoGtkDrawable::~drgeoGtkDrawable ()
{
  gtk_timeout_remove (timeoutTag);
}

void
drgeoGtkDrawable::drawPoint (drgeoStyle & style, drgeoPoint & point)
{
  int x, y, radius;
  double width, height;

  switch (style.thick)
    {
    case drgeoThicknessDashed:
      radius = 2;
      break;
    case drgeoThicknessNormal:
      radius = 3;
      break;
    case drgeoThicknessLarge:
      radius = 4;
      break;
    }

  height = (double) WIDGET_HEIGHT (drawing_area);
  width = (double) WIDGET_WIDTH (drawing_area);

  // Compute the on screen point.

  x = SCREEN_X (point.getX ());
  y = SCREEN_Y (point.getY ());

  // Adapt the GC to the style.

  if (style.mask == yes)
    gdk_gc_set_background (figure_gc, &color[drgeoColorYellow]);

  gdk_gc_set_foreground (figure_gc, &color[style.color]);
  gdk_gc_set_fill (figure_gc, GDK_SOLID);

  switch (style.pointShape)
    {
    case drgeoPointRound:
      if (style.mask == yes)
	gdk_gc_set_line_attributes (figure_gc, 2, GDK_LINE_DOUBLE_DASH,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
      else
	gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_SOLID,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
      gdk_draw_arc (paintThere, figure_gc, TRUE, x - radius,
		    y - radius, radius * 2, radius * 2, 0, 360 * 64);
      gdk_draw_arc (paintThere, figure_gc, FALSE, x - radius,
		    y - radius, radius * 2, radius * 2, 0, 360 * 64);
      break;
    case drgeoPointX:
      if (style.mask == yes)
	gdk_gc_set_line_attributes (figure_gc, 2, GDK_LINE_DOUBLE_DASH,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
      else
	gdk_gc_set_line_attributes (figure_gc, 2, GDK_LINE_SOLID,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);

      gdk_draw_line (paintThere, figure_gc, x - radius,
		     y - radius, x + radius, y + radius);
      gdk_draw_line (paintThere, figure_gc, x - radius,
		     y + radius, x + radius, y - radius);
      break;
    case drgeoPointRec:
      if (style.mask == yes)
	gdk_gc_set_line_attributes (figure_gc, 2, GDK_LINE_DOUBLE_DASH,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
      else
	gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_SOLID,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
      gdk_draw_rectangle (paintThere, figure_gc, TRUE,
			  x - radius, y - radius, radius << 1, radius << 1);
      break;
    case drgeoPointRoundEmpty:
      if (style.mask == yes)
	gdk_gc_set_line_attributes (figure_gc, 2, GDK_LINE_DOUBLE_DASH,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
      else
	gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_SOLID,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
      gdk_draw_arc (paintThere, figure_gc, FALSE, x - radius,
		    y - radius, radius * 2, radius * 2, 0, 360 * 64);
      break;
    case drgeoPointRecEmpty:
      if (style.mask == yes)
	gdk_gc_set_line_attributes (figure_gc, 2, GDK_LINE_DOUBLE_DASH,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
      else
	gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_SOLID,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
      gdk_draw_rectangle (paintThere, figure_gc, FALSE,
			  x - radius, y - radius, radius << 1, radius << 1);
      break;
    }
}

void
drgeoGtkDrawable::
drawPolygon (drgeoStyle & style, drgeoPoint * point, gint number)
{
  double width, height;
  GdkPoint *gdkPoints, *pt;
  gint nb;

  height = (double) WIDGET_HEIGHT (drawing_area);
  width = (double) WIDGET_WIDTH (drawing_area);

  // Compute the on screen point.
  gdkPoints = g_new (GdkPoint, number);
  pt = gdkPoints;

  for (nb = 0; nb < number; nb++)
    {
      pt->x = SCREEN_X (point[nb].getX ());
      pt->y = SCREEN_Y (point[nb].getY ());
      pt++;
    }

  // Adapt the GC to the style.

  if (style.mask == yes)
    gdk_gc_set_background (figure_gc, &color[drgeoColorYellow]);

  gdk_gc_set_foreground (figure_gc, &color[style.color]);

  gdk_gc_set_fill (figure_gc, GDK_STIPPLED);
  gdk_draw_polygon (paintThere, figure_gc, TRUE, gdkPoints, number);
}

void
drgeoGtkDrawable::
drawLine (drgeoStyle & style, drgeoPoint & start, drgeoPoint & end)
{
  // start : one point on the line
  // end : a direction vector of the line
  // world boundaries
  static double width, height;

  // size of the drawing area in world unit
  width = pixelToWorld (WIDGET_WIDTH (drawing_area));
  height = pixelToWorld (WIDGET_HEIGHT (drawing_area));

  clipLine (style, start, end, origin_y + height / 2, origin_y - height / 2,
	    origin_x - width / 2, origin_x + width / 2);
}

void
drgeoGtkDrawable::
drawHalfLine (drgeoStyle & style, drgeoPoint & point, drgeoVector & vect)
{
  double width, height;

  // size of the drawing area in world unit
  width = pixelToWorld (WIDGET_WIDTH (drawing_area));
  height = pixelToWorld (WIDGET_HEIGHT (drawing_area));

  clipHalfLine (style, point, vect, origin_y + height / 2, origin_y - height / 2,
		origin_x - width / 2, origin_x + width / 2);  
}

void
drgeoGtkDrawable::
drawSegment (drgeoStyle & style, drgeoPoint & start, drgeoPoint & end)
{
  int x1, y1, x2, y2;
  double width, height;

  height = (double) WIDGET_HEIGHT (drawing_area);
  width = (double) WIDGET_WIDTH (drawing_area);
  // Compute the correct on screen location.
  x1 = SCREEN_X (start.getX ());
  y1 = SCREEN_Y (start.getY ());
  x2 = SCREEN_X (end.getX ());
  y2 = SCREEN_Y (end.getY ());


  // Adapt the GC to the style.
  gdk_gc_set_foreground (figure_gc, &color[style.color]);
  gdk_gc_set_fill (figure_gc, GDK_SOLID);
  if (style.mask == yes)
    {
      gdk_gc_set_background (figure_gc, &color[drgeoColorYellow]);
      gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_DOUBLE_DASH,
				  GDK_CAP_ROUND, GDK_JOIN_MITER);
    }
  else
    switch (style.thick)
      {
      case drgeoThicknessDashed:
	gdk_gc_set_background (figure_gc, &color[drgeoColorWhite]);
	gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_DOUBLE_DASH,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
	break;
      case drgeoThicknessNormal:
	gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_SOLID,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
	break;
      case drgeoThicknessLarge:
	gdk_gc_set_line_attributes (figure_gc, 2, GDK_LINE_SOLID,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
	break;
      }

  // Draw the line.
  gdk_draw_line (paintThere, figure_gc, x1, y1, x2, y2);
}

void
drgeoGtkDrawable::
drawCircle (drgeoStyle & style, drgeoPoint & center, drgeoPoint & point)
{
  double radius;

  radius = (point - center).norm ();
  drawCircle (style, center, radius);
}

void
drgeoGtkDrawable::
drawCircle (drgeoStyle & style, drgeoPoint & center, double radius)
{
  int x, y, iradius;
  double width, height;

  height = (double) WIDGET_HEIGHT (drawing_area);
  width = (double) WIDGET_WIDTH (drawing_area);

  // Compute the correct on screen location.
  x = SCREEN_X (center.getX ());
  y = SCREEN_Y (center.getY ());
  iradius = (int) (radius * scale);

  // Adapt the GC to the style.
  gdk_gc_set_foreground (figure_gc, &color[style.color]);
  gdk_gc_set_fill (figure_gc, GDK_SOLID);

  if (style.mask == yes)
    {
      gdk_gc_set_background (figure_gc, &color[drgeoColorYellow]);
      gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_DOUBLE_DASH,
				  GDK_CAP_ROUND, GDK_JOIN_MITER);
    }
  else
    switch (style.thick)
      {
      case drgeoThicknessDashed:
	gdk_gc_set_background (figure_gc, &color[drgeoColorWhite]);
	gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_DOUBLE_DASH,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
	break;
      case drgeoThicknessNormal:
	gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_SOLID,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
	break;
      case drgeoThicknessLarge:
	gdk_gc_set_line_attributes (figure_gc, 2, GDK_LINE_SOLID,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
	break;
      }

  // Draw the arc.
  gdk_draw_arc (paintThere, figure_gc, style.fill,
		x - iradius, y - iradius, iradius * 2, iradius * 2, 0,
		360 * 64);
}

void
drgeoGtkDrawable::

drawArc (drgeoStyle & style, drgeoPoint & center,
	 double radius, double start, double length)
{
  int x, y, iradius;
  double width, height;

  height = (double) WIDGET_HEIGHT (drawing_area);
  width = (double) WIDGET_WIDTH (drawing_area);

  // Compute the correct on screen location.
  x = SCREEN_X (center.getX ());
  y = SCREEN_Y (center.getY ());
  iradius = (int) (radius * scale);

  // Adapt the GC to the style.
  gdk_gc_set_foreground (figure_gc, &color[style.color]);
  gdk_gc_set_fill (figure_gc, GDK_SOLID);

  if (style.mask == yes)
    {
      gdk_gc_set_background (figure_gc, &color[drgeoColorYellow]);
      gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_DOUBLE_DASH,
				  GDK_CAP_ROUND, GDK_JOIN_MITER);
    }
  else
    switch (style.thick)
      {
      case drgeoThicknessDashed:
	gdk_gc_set_background (figure_gc, &color[drgeoColorWhite]);
	gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_DOUBLE_DASH,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
	break;
      case drgeoThicknessNormal:
	gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_SOLID,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
	break;
      case drgeoThicknessLarge:
	gdk_gc_set_line_attributes (figure_gc, 2, GDK_LINE_SOLID,
				    GDK_CAP_ROUND, GDK_JOIN_MITER);
	break;
      }

  // Draw the arc.
  gdk_draw_arc (paintThere, figure_gc, style.fill,
		x - iradius, y - iradius, iradius * 2, iradius * 2,
		(gint) (180.0 * start / M_PI) * 64,
		(gint) (180.0 * length / M_PI) * 64);
}

void
drgeoGtkDrawable::
drawText (drgeoPoint & where, char *text, drgeoColorType fontColor)
{
  int x, y, h;
  double width, height;
  PangoLayout *layout;

  if (!text)
    return;
  height = (double) WIDGET_HEIGHT (drawing_area);
  width = (double) WIDGET_WIDTH (drawing_area);

  // Compute the correct on screen location.
  x = SCREEN_X (where.getX ());
  y = SCREEN_Y (where.getY ());

  gdk_gc_set_foreground (font_gc, &color[fontColor]);
  layout = gtk_widget_create_pango_layout (drawing_area, (const gchar *) text);
  pango_layout_get_pixel_size (layout, NULL, &h);
  gdk_draw_layout (paintThere, font_gc, x, y - h, layout);
}

double
drgeoGtkDrawable::stringWidth (char *text)
{
  PangoLayout *layout;
  gint w;

  layout = gtk_widget_create_pango_layout (drawing_area, 
					   (const gchar *) text);
  pango_layout_get_pixel_size (layout, &w, NULL);
  return pixelToWorld (w);
}

double
drgeoGtkDrawable::stringHeight (char *text)
{
  PangoLayout *layout;
  gint h;

  layout = gtk_widget_create_pango_layout (drawing_area, 
					   (const gchar *) text);
  pango_layout_get_pixel_size (layout, NULL, &h);
  return pixelToWorld (h);
}


void 
drgeoGtkDrawable::showPrintingArea (drgeoPoint& corner1, 
				    drgeoPoint& corner2)
{
  printingAreaDefined = true;
  if (corner1.getX () <= corner2.getX ())
    {
      tl.setX (corner1.getX ());
      br.setX (corner2.getX ());
    } 
  else
    {
      tl.setX (corner2.getX ());
      br.setX (corner1.getX ());
    }
  if (corner1.getY () >= corner2.getY ())
    {
      tl.setY (corner1.getY ());
      br.setY (corner2.getY ());
    }
  else
    {
      tl.setY (corner2.getY ());
      br.setY (corner1.getY ());
    }
  gtk_widget_queue_draw (drawing_area);
}

void
drgeoGtkDrawable::_showPrintingArea ()
{
  int x, y, h, w;
  double width, height;

  height = (double) WIDGET_HEIGHT (drawing_area);
  width = (double) WIDGET_WIDTH (drawing_area);

  x = SCREEN_X (tl.getX ());
  y = SCREEN_Y (tl.getY ());

  w = SCREEN_X (br.getX ()) - x;
  h = SCREEN_Y (br.getY ()) - y;

  gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_SOLID,
			      GDK_CAP_ROUND, GDK_JOIN_MITER);
  gdk_gc_set_foreground (figure_gc, &color[drgeoColorPrintingArea]);
  gdk_gc_set_fill (figure_gc, GDK_SOLID);
  gdk_draw_rectangle (paintThere, figure_gc, TRUE,
		      x, y, w, h);
}

void
drgeoGtkDrawable::unsetPrintingArea ()
{
  printingAreaDefined = false;
  gtk_widget_queue_draw (drawing_area);   
}

gboolean
drgeoGtkDrawable::printingArea ()
{
  return printingAreaDefined;
}

drgeoPoint 
drgeoGtkDrawable::getPrintingAreaBR ()
{
  return br;
}
 
drgeoPoint 
drgeoGtkDrawable::getPrintingAreaTL ()
{
  return tl;
}


void 
drgeoGtkDrawable::showTip (drgeoPoint & where, gchar *text)
{
  if (tip)
    g_free (tip);
  tip = g_strdup (text);
  tipPosition = where;
  gtk_widget_queue_draw (drawing_area);   
}

void
drgeoGtkDrawable::_showTip ()
{
  // The same thing as drawText for now.  XXX
  int x, y, h, w;
  double width, height;
  PangoLayout *layout;

  height = (double) WIDGET_HEIGHT (drawing_area);
  width = (double) WIDGET_WIDTH (drawing_area);

  layout = gtk_widget_create_pango_layout (drawing_area, 
					   (const gchar *) tip);

  pango_layout_get_pixel_size (layout, &w, &h);

  x = SCREEN_X (tipPosition.getX ()) + 16;
  y = SCREEN_Y (tipPosition.getY ()) - 16;

  gdk_gc_set_line_attributes (figure_gc, 0, GDK_LINE_SOLID,
			      GDK_CAP_ROUND, GDK_JOIN_MITER);
  gdk_gc_set_foreground (figure_gc, &color[drgeoColorTips]);
  gdk_gc_set_fill (figure_gc, GDK_SOLID);
  gdk_draw_rectangle (paintThere, figure_gc, TRUE,
		      x - 4, y - h, w + 6, h + 6);

  gdk_gc_set_foreground (figure_gc, &color[drgeoColorBlack]);
  gdk_gc_set_fill (figure_gc, GDK_SOLID);
  gdk_draw_rectangle (paintThere, figure_gc, FALSE,
		      x - 4, y - h, w + 6, h + 6);

  // Draw the string.
  gdk_gc_set_foreground (font_gc, &color[drgeoColorBlack]);
  gdk_draw_layout (paintThere, font_gc, x, y - h + 3, layout);
  
}

void
drgeoGtkDrawable::removeTip ()
{
  if (tip)
    {
      g_free (tip);
      tip = NULL;
      gtk_widget_queue_draw (drawing_area);         
    }
}

gboolean
drgeoGtkDrawable::tipOn ()
{
  if (tip) return true;
  else return false;
}

void
drgeoGtkDrawable::setCursor (drgeoCursor cursor)
{
  GdkCursor *c;

  currentCursor = cursor;
  switch (cursor)
    {
    case hand:
      c = gdk_cursor_new (GDK_HAND1);
      break;
    case arrow:
      c = gdk_cursor_new (GDK_LEFT_PTR);
      break;
    case pen:
      c = gdk_cursor_new (GDK_PENCIL);
      break;
    case move:
      c = gdk_cursor_new (GDK_HAND2);
      break;
    }
  gdk_window_set_cursor (drawing_area->window, c);
  gdk_cursor_destroy (c);
}

double
drgeoGtkDrawable::getRange ()
{
  // 3 pixels on screen.
  return (4.0 / scale);
}

void
drgeoGtkDrawable::clear ()
{
  // Clear the whole drawable area.
  gint widthi, heighti;
  gdk_window_get_size ((drawing_area->window), &widthi, &heighti);
  gdk_draw_rectangle (paintThere, drawing_area->style->white_gc, TRUE,
		      0, 0, widthi, heighti);
}

void
drgeoGtkDrawable::chooseItem (liste_elem * list)
{
  // This method is responsible for prompting the user to select an
  // item from a list and add it to the current selection.  It is
  // onvoked when there are several items under the mouse.
  GtkWidget *menu, *menu_item;
  int nb_choice, i;
  geometricObject *choice;

  menu = gtk_menu_new ();
  nb_choice = list->nb_elem;
  list->init_lire ();
  for (i = 0; i < nb_choice; i++)
    {
      choice = (geometricObject *) list->lire (0);
      menu_item = gtk_menu_item_new_with_label (choice->getTypeName ());
      gtk_menu_append (GTK_MENU (menu), menu_item);

      // The callback will be called with the choice pointer, so we
      // can know wich figure object was choosen.  The drawable is
      // passed to the signal handler.

      gtk_signal_connect (GTK_OBJECT (menu_item), "activate",
			  GTK_SIGNAL_FUNC (choose_item_cb),
			  (gpointer) choice);
      gtk_object_set_data (GTK_OBJECT (menu_item), "figure", this);
      gtk_widget_show (menu_item);
    }
  gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, NULL, 1, 0);
}

static void
choose_item_cb (GtkWidget * w, geometricObject * choice)
{
  drgeoDrawableUI *drawable;
  drgeoPoint where;

  drawable =
    (drgeoDrawableUI *) gtk_object_get_data (GTK_OBJECT (w), "figure");
  if (drawable != NULL)
    {
      drawable->handleChoice (choice);
    }
}

bool
drgeoGtkDrawable::askOkCancel (char *message)
{
  GtkWidget *dialog;
  int reply;

  dialog = gtk_message_dialog_new (mdi->mainWindow (),
				   GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_QUESTION,
				   GTK_BUTTONS_YES_NO,
				   message, g_strerror (errno));
 
  reply = gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);


  if (reply == GTK_RESPONSE_YES)
    return TRUE;
  else
    return FALSE;

}

void
drgeoGtkDrawable::adjustRulersRange ()
{
  double height, width;

  /* Compute width and height of the drawing_area.  */
  height = (double) WIDGET_HEIGHT (drawing_area);
  width = (double) WIDGET_WIDTH (drawing_area);

  gtk_ruler_set_range (GTK_RULER (hrule),
		       origin_x - width / (scale * 2.0),
		       origin_x + width / (scale * 2.0),
		       origin_x, origin_x + width / (scale * 2.0));
  gtk_ruler_set_range (GTK_RULER (vrule),
		       origin_y + height / (scale * 2.0),
		       origin_y - height / (scale * 2.0),
		       origin_y, origin_y + height / (scale * 2.0));
}

GtkWidget *
drgeoGtkDrawable::getGtkDrawable ()
{
  return (drawing_area);
}

GtkWidget *
drgeoGtkDrawable::getTopWidget ()
{
  return (p_topWidget);
}

void
drgeoGtkDrawable::pressCallback (GdkEventButton * event)
{
  drgeoPoint where;
  double width, height;

  // Only invoke the handlePress() method if the button1 is pressed.
  // We are ignoring all other press events.
  if (event->button == 1)
    {
      height = (double) WIDGET_HEIGHT (drawing_area);
      width = (double) WIDGET_WIDTH (drawing_area);

      // Compute the correct on screen location.
      where.set (origin_x + (event->x - width / 2.0) / scale,
		 origin_y - (event->y - height / 2.0) / scale);
      // Handle correctly the event.
      handlePress (where);
    }
}

void
drgeoGtkDrawable::releaseCallback (GdkEventButton * event)
{
  drgeoPoint where;
  double width, height;

  height = (double) WIDGET_HEIGHT (drawing_area);
  width = (double) WIDGET_WIDTH (drawing_area);

  // Compute the correct on screen location.
  where.set (origin_x + (event->x - width / 2.0) / scale,
	     origin_y - (event->y - height / 2.0) / scale);
  // Handle correctly the event.
  handleRelease (where);
}

void
drgeoGtkDrawable::moveCallback (GdkEventMotion * event)
{
  static GdkModifierType state;
  drgeoPoint where;
  double width, height;

  height = (double) WIDGET_HEIGHT (drawing_area);
  width = (double) WIDGET_WIDTH (drawing_area);

  // Compute the correct on screen location.
  where.set (origin_x + (event->x - width / 2.0) / scale,
	     origin_y - (event->y - height / 2.0) / scale);
  // Handle correctly the event.
  handleMouseAt (where);
}

void
drgeoGtkDrawable::moveWholeFigure (GdkEventMotion * event)
{
  gtk_adjustment_set_value (hadjustment,
			    hadjustment->value - (event->x - moveX) / 4);
  gtk_adjustment_set_value (vadjustment,
			    vadjustment->value - (event->y - moveY) / 4);

  moveX = event->x;
  moveY = event->y;
}

void
drgeoGtkDrawable::exposeCallback ()
{
  // Redraw everything.
  clear ();
  // Display the printing area
  if (printingAreaDefined)
    _showPrintingArea ();
  // Print the grid
  if (figure->getShowGridState ())
    showGrid ();
  _refresh ();
  if (tip)
    _showTip ();
}

void
drgeoGtkDrawable::configureCallback ()
{
  char fillMask[] = { 0x2, 0x1 };
  GdkBitmap *pixmap;

  if (paintThere == NULL)
    paintThere = drawing_area->window;

  if (figure_gc == NULL)
    {
      /* Create necessary GCs.  */
      figure_gc = gdk_gc_new (paintThere);
      pixmap = gdk_bitmap_create_from_data (paintThere, fillMask, 2, 2);
      gdk_gc_set_stipple (figure_gc, pixmap);
      gdk_pixmap_unref (pixmap);
      font_gc = gdk_gc_new (paintThere);
      object_gc = gdk_gc_new (paintThere);

#define SET_COLOR(color, R,G,B) \
{ \
      color.pixel = 0; \
      color.red = R << 8; \
      color.green = G << 8; \
      color.blue = B << 8;  \
      gdk_color_alloc (gtk_widget_get_colormap(drawing_area), &(color)); \
}

      // Fill the color map. XXX: Add error handling.
      SET_COLOR (color[drgeoColorBlack], 0, 0, 0);
      SET_COLOR (color[drgeoColorWhite], 255, 255, 255);
      SET_COLOR (color[drgeoColorBlue], 80, 177, 255);
      SET_COLOR (color[drgeoColorDarkBlue], 45, 56, 255);
      SET_COLOR (color[drgeoColorGrey], 170, 170, 170);
      SET_COLOR (color[drgeoColorDarkGrey], 90, 90, 90);
      SET_COLOR (color[drgeoColorGreen], 0, 235, 0);
      SET_COLOR (color[drgeoColorDarkGreen], 0, 100, 0);
      SET_COLOR (color[drgeoColorRed], 235, 0, 0);
      SET_COLOR (color[drgeoColorBordeaux], 145, 0, 0);
      SET_COLOR (color[drgeoColorYellow], 255, 240, 33);
      SET_COLOR (color[drgeoColorOrange], 255, 153, 43);
      SET_COLOR (color[drgeoColorTips], 255, 255, 192);
      SET_COLOR (color[drgeoColorPrintingArea], 240, 240, 240);

      gdk_gc_set_foreground (font_gc, &color[drgeoColorBlack]);
      gdk_gc_set_foreground (figure_gc, &color[drgeoColorBlack]);
      gdk_gc_set_foreground (object_gc, &color[drgeoColorBlack]);

      gdk_window_set_background (paintThere, &color[drgeoColorWhite]);

      /* FIXE Load appropriate fonts.  */

      // Initialize the timer for regular flashes.
      timeoutTag =
	gtk_timeout_add (500, (GtkFunction) timer_cb, (gpointer) this);
    }
  adjustRulersRange ();
}

static int
timer_cb (drgeoGtkDrawable * drawable)
{
  // Invoke the timerCallBack() method.
  drawable->timerCallback ();
  return (TRUE);		// Make sure the CallBack will be
  // called again and again... 

}

void
drgeoGtkDrawable::timerCallback ()
{
  // Make the current selection blink.
  counter++;
  counter &= 1;
  if (figure && counter == 1)
    {
      figure->flashSelection (TRUE);
    }
  else if (figure && counter == 0)
    {
      figure->flashSelection (FALSE);
    }
}

void
drgeoGtkDrawable::horizontalScrollCallback ()
{
  origin_x = hadjustment->value;
  adjustRulersRange ();
  refresh ();
}

void
drgeoGtkDrawable::verticalScrollCallback ()
{
  origin_y = -vadjustment->value;
  adjustRulersRange ();
  refresh ();
}

// udpate the adjustment according to the origin
void
drgeoGtkDrawable::updateAdjustments ()
{
  gtk_adjustment_set_value (GTK_ADJUSTMENT (hadjustment), origin_x);
  gtk_adjustment_set_value (GTK_ADJUSTMENT (vadjustment), -origin_y);
}

void
drgeoGtkDrawable::setScale (double newScale)
{
  scale = newScale;
  adjustRulersRange ();
}

drgeoPoint
drgeoGtkDrawable::getAreaCenter ()
{
  static drgeoPoint c;
  c.set (origin_x, origin_y);
  return c;

}

void
drgeoGtkDrawable::setAreaCenter (drgeoPoint &p)
{
  origin_x = p.getX ();
  origin_y = p.getY ();
}

drgeoPoint
drgeoGtkDrawable::getAreaSize ()
{
  drgeoPoint s;
  s.set (pixelToWorld (WIDGET_WIDTH (drawing_area)),
	 (pixelToWorld (WIDGET_HEIGHT (drawing_area))));
  return s;
}

void
drgeoGtkDrawable::updateUndoState ()
{
  mdi->reconcile_grayout_undo ();
}


void
drgeoGtkDrawable::_refresh ()
{
  // Do nothing if no figure is currently associated with this drawable.
  if (figure != NULL)
    {
      if (figure->getMode () == MISE_EN_FORME_MODE)
	figure->redraw (TRUE);
      else
	figure->redraw (FALSE);
    }
}

void
drgeoGtkDrawable::refresh ()
{
  // request a redaw (trigger an expose event)
  gtk_widget_queue_draw (drawing_area); 
} 

void
drgeoGtkDrawable::clearDescription ()
{
  p_geoView->clearDescription ();
}

void
drgeoGtkDrawable::addDescriptionItem (geometricObject * item)
{
  p_geoView->addDescription (item);
}

geoView *
drgeoGtkDrawable::getGeoView ()
{
  return p_geoView;
}

// Regular signal handlers.  You don't have to care about that.
static gboolean
drawing_area_button_press_cb (GtkWidget * w, GdkEventButton * event,
			      gpointer data)
{
  drgeoGtkDrawable *drawable = NULL;

  drawable =
    (drgeoGtkDrawable *) gtk_object_get_data (GTK_OBJECT (w), "figure");
  if (event->state & GDK_CONTROL_MASK)
    {
      // the user start to move the whole figure
      moveX = event->x;
      moveY = event->y;
      previousCursor = drawable->currentCursor;
      drawable->setCursor (move);
      return TRUE;
    }
  else if (event->button != 3)
    {
      drawable =
	(drgeoGtkDrawable *) gtk_object_get_data (GTK_OBJECT (w), "figure");
      drawable->pressCallback (event);
      return TRUE;
    }
  // reserve the right button (3) for the popup
  return FALSE;
}

static gboolean
drawing_area_button_release_cb (GtkWidget * w, GdkEventButton * event,
				gpointer data)
{
  drgeoGtkDrawable *drawable =
    (drgeoGtkDrawable *) gtk_object_get_data (GTK_OBJECT (w), "figure");

  if (previousCursor != none)
    {
      drawable->setCursor (previousCursor);
      previousCursor = none;
    }
  drawable->releaseCallback (event);
  return TRUE;
}

static gboolean
drawing_area_move_cb (GtkWidget * w, GdkEventMotion * event, gpointer data)
{
  drgeoGtkDrawable *drawable = NULL;

  drawable =
    (drgeoGtkDrawable *) gtk_object_get_data (GTK_OBJECT (w), "figure");

  if ((event->state & GDK_CONTROL_MASK) && (event->state & GDK_BUTTON1_MASK))
    drawable->moveWholeFigure (event);
  else
    drawable->moveCallback (event);
  return false;
}

static gboolean
drawing_area_expose_cb (GtkWidget * w, GdkEventExpose * e, gpointer data)
{
  drgeoGtkDrawable *drawable = NULL;

  drawable =
    (drgeoGtkDrawable *) gtk_object_get_data (GTK_OBJECT (w), "figure");
  drawable->exposeCallback ();
  return true;
}

static gboolean
drawing_area_configure_cb (GtkWidget * w, GdkEventConfigure * e,
			   gpointer data)
{
  drgeoGtkDrawable *drawable = NULL;

  drawable =
    (drgeoGtkDrawable *) gtk_object_get_data (GTK_OBJECT (w), "figure");
  drawable->configureCallback ();

  return TRUE;
}

static void
hadjustment_cb (GtkAdjustment * w, gpointer data)
{
  drgeoGtkDrawable *drawable = NULL;

  drawable =
    (drgeoGtkDrawable *) gtk_object_get_data (GTK_OBJECT (data), "figure");
  drawable->horizontalScrollCallback ();
}

static void
vadjustment_cb (GtkAdjustment * w, gpointer data)
{
  drgeoGtkDrawable *drawable = NULL;

  drawable =
    (drgeoGtkDrawable *) gtk_object_get_data (GTK_OBJECT (data), "figure");
  drawable->verticalScrollCallback ();
}

drgeoMacroBuildDialog *
drgeoGtkDrawable::createMacroBuildDialog (drgeoMacroBuilder * builder)
{
  class drgeoGtkMacroBuildDialog *dialog;

  this->builder = builder;
  // Create the dialog, and return a pointer to it.
  dialog = new drgeoGtkMacroBuildDialog (builder);
  dialog->show ();
  return dialog;
}

drgeoMacroPlayDialog *
drgeoGtkDrawable::createMacroPlayDialog (drgeoMacroPlayer * player)
{
  class drgeoGtkMacroPlayDialog *dialog;

  this->player = player;
  // Create the dialog, and return a pointer to it.
  dialog = new drgeoGtkMacroPlayDialog (player);
  dialog->show ();
  return dialog;
}

drgeoStyleDialog *
drgeoGtkDrawable::createStyleDialog ()
{
  class drgeoStyleDialog *dialog;

  // Create the dialog, and return a pointer to it.
  dialog = new drgeoGtkStyleDialog (this);
  return dialog;
}

drgeoPropertyDialog *
drgeoGtkDrawable::createPropertyDialog ()
{
  class drgeoPropertyDialog *dialog;

  // Create the dialog, and return a pointer to it.
  dialog = new drgeoGtkPropertyDialog (this);
  return dialog;
}

void
dummy_cb (GtkWidget * w, gpointer data)
{
  g_printerr ("Dummy callback\n");
}
