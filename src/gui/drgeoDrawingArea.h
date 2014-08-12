/*
 * drgeoDrawingArea.h
 *
 * Copyright (C) 2014 Gurjot Singh <bhattigurjot@gmail.com>
	 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
	 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DRGEODRAWINGAREA_H
#define DRGEODRAWINGAREA_H

#include <gtk/gtk.h>
#include <iostream>
#include <vector>
#include "drgeoPainter.h"
#include "../kernel/drgeoContainer.h"

static cairo_surface_t *surface = NULL;
extern gdouble x,y;

class drgeoDrawingArea
{
	public:
		drgeoDrawingArea ();
		virtual ~drgeoDrawingArea ();

		GtkWidget *drawingArea;
		GtkWidget *createDrawArea ();

		void drawEntity (int);
};

extern "C"
{
void set_surface (void);
void check_coord (gdouble dx, gdouble dy);
void paint_point (GtkWidget *widget, gdouble x, gdouble y);
void paint_line (GtkWidget *widget, gdouble startX, gdouble startY, gdouble endX, gdouble endY);
void paint_circle (GtkWidget *widget, gdouble startX, gdouble startY, gdouble endX, gdouble endY);
gboolean drgeo_configure_event (GtkWidget *widget, GdkEventConfigure *event, 
                                                        gpointer user_data);
gboolean drgeo_draw_event (GtkWidget *widget, cairo_t *cr,  gpointer user_data);
gboolean drgeo_clicked_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data);
gboolean drgeo_motion_event(GtkWidget *widget, GdkEventMotion *event, gpointer user_data); 
}

#endif /* DRGEODRAWINGAREA_H */