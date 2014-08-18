// drgeoPainter.cpp
//
// Copyright (C) 2014 Gurjot Singh <bhattigurjot@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "drgeoPainter.h"
#include <iostream>

drgeoPainter::drgeoPainter ()
{
}

drgeoPainter::~drgeoPainter ()
{
}

void 
drgeoPainter::drgeo_cairo_background (cairo_t *cr, cairo_surface_t *surface)
{
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint(cr);    
}

void 
drgeoPainter::drgeo_cairo_point (cairo_t *cr, cairo_surface_t *surface, gdouble x, gdouble y)
{
  /* Paint to the surface, where we store our state */
  cr = cairo_create (surface);

  int w = 6; /*width of the line */
  cairo_set_line_width (cr, w);
  cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND); /* Round dot*/
  cairo_set_source_rgb(cr, 0.69, 0.19, 0); /* Red Colour */
  cairo_move_to (cr, x, y); 
  cairo_line_to (cr, x, y); /* a very short line is a dot */
  cairo_stroke (cr);

  cairo_destroy (cr);   
}

void 
drgeoPainter::drgeo_cairo_line (cairo_t *cr, cairo_surface_t *surface, gdouble x1, gdouble y1, 
                                        gdouble x2, gdouble y2)
{
  cr = cairo_create (surface);
  
  cairo_move_to (cr, x1, y1); /* start point of line */
  cairo_line_to (cr, x2, y2); /* end point of line */

  cairo_stroke (cr);
  cairo_destroy (cr);   
}

void 
drgeoPainter::drgeo_cairo_arc (cairo_t *cr, cairo_surface_t *surface, gdouble x, gdouble y, 
                               		gdouble r, gdouble start, gdouble end, bool direction)
{
  cr = cairo_create (surface);
  if(!direction)
  cairo_arc(cr, x, y, r, start, end);
  else
  cairo_arc_negative(cr, x, y, r, start, end); 
  
  cairo_stroke (cr);
  cairo_destroy (cr);   
}

void 
drgeoPainter::drgeo_cairo_circle (cairo_t *cr, cairo_surface_t *surface, gdouble x, gdouble y, 
                                  					gdouble r)
{
  cr = cairo_create (surface);
  
  cairo_arc (cr, x, y, r, 0, 2 * M_PI); /* draw cirle through arc */
  
  cairo_stroke (cr);
  cairo_destroy (cr);   
}