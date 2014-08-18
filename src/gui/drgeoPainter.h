/*
 * drgeoPainter.h
 *
 * Copyright (C) 2014 Gurjot Singh <bhattigurjot@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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

#ifndef DRGEOPAINTER_H
#define DRGEOPAINTER_H

#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

class drgeoPainter
{		
	public:
		drgeoPainter ();
		virtual ~drgeoPainter ();
		
		void drgeo_cairo_background (cairo_t *cr, cairo_surface_t *surface);
		void drgeo_cairo_point (cairo_t *cr, cairo_surface_t *surface, gdouble x, gdouble y);
		void drgeo_cairo_line (cairo_t *cr, cairo_surface_t *surface, gdouble x1, gdouble y1, 
		                       				gdouble x2, gdouble y2);
		void drgeo_cairo_arc (cairo_t *cr, cairo_surface_t *surface, gdouble x, gdouble y, gdouble r, 
		                      				gdouble start, gdouble end, bool direction);
		void drgeo_cairo_circle (cairo_t *cr, cairo_surface_t *surface, gdouble x, gdouble y, gdouble r);
		
};

#endif /* DRGEOPAINTER_H */