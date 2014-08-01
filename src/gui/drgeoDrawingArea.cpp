// drgeoDrawingArea.cpp
//
// Copyright (C) 2014 Gurjot Singh <bhattigurjot@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "drgeoDrawingArea.h"

drgeoPainter *painterPointer;

drgeoDrawingArea::drgeoDrawingArea ()
{
	drawingArea = gtk_drawing_area_new ();
	gtk_widget_set_events (drawingArea, gtk_widget_get_events (drawingArea)
                                           | GDK_BUTTON_PRESS_MASK);

	/* Connect Signals */
   g_signal_connect (drawingArea, "draw", G_CALLBACK (drgeo_draw_event), NULL); 
   g_signal_connect (drawingArea, "configure-event", 
                                 G_CALLBACK (drgeo_configure_event), NULL);
	
	gtk_widget_show (drawingArea);
}

drgeoDrawingArea::~drgeoDrawingArea ()
{
}

GtkWidget* 
drgeoDrawingArea::createDrawArea ()
{
	return drawingArea;
}

void 
drgeoDrawingArea::drawPoint ()
{
  g_signal_connect (drawingArea, "button-press-event", 
                                G_CALLBACK (drgeo_clicked_event), NULL);
  std::cout<<"Point Drawn"<<std::endl;
}

void 
set_surface (void)
{
  cairo_t *cr;

  cr = cairo_create (surface);

  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  cairo_destroy (cr);
}

void
paint_point (GtkWidget *widget, gdouble x, gdouble y)
{
  cairo_t *cr;
  painterPointer->cairo_point (cr, surface, x, y);

  /* Now invalidate the affected region of the drawing area. */
  gtk_widget_queue_draw_area (widget, x - 3, y - 3, 6, 6);
}

gboolean
drgeo_configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer user_data)
{
  if (surface)
    cairo_surface_destroy (surface);

  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                                                               CAIRO_CONTENT_COLOR,
                                                                               gtk_widget_get_allocated_width (widget),
                                                                               gtk_widget_get_allocated_height (widget));

  /* Initialize the surface to white */
  set_surface ();

  /* We've handled the configure event, no need for further processing. */
  return TRUE;
}

gboolean 
drgeo_draw_event (GtkWidget *widget, cairo_t *cr,  gpointer user_data)
{      
  painterPointer->cairo_background (cr, surface);
  return FALSE;
}

gboolean 
drgeo_clicked_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    /* paranoia check, in case we haven't gotten a configure event */
  if (surface == NULL)
    return FALSE;

  if (event->button == 1)
    {
      paint_point (widget, event->x, event->y);
    }
	
  /* We've handled the event, stop processing */
  return TRUE;
}