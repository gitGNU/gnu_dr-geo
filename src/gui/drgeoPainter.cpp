// drgeoPainter.cpp
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

#include "drgeoPainter.h"
#include <iostream>

drgeoPainter::drgeoPainter ()
{
}

drgeoPainter::~drgeoPainter ()
{
}

void 
drgeoPainter::cairo_background (cairo_t *cr, cairo_surface_t *surface)
{
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint(cr);    
}

void 
drgeoPainter::cairo_point (cairo_t *cr, cairo_surface_t *surface, gdouble x, gdouble y)
{
  /* Paint to the surface, where we store our state */
  cr = cairo_create (surface);

  cairo_rectangle (cr, x - 3, y - 3, 6, 6);
  cairo_fill (cr);

  cairo_destroy (cr);   
}