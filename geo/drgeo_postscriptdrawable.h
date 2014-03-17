/*
 *  Dr Genius an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-2000
 * hilaire@seul.org 
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

#ifndef DRGEO_POSTSCRIPTDRAWABLE_H
#define DRGEO_POSTSCRIPTDRAWABLE_H

#include "drgeo_latexdrawable.h"

/* Implement a postscript drawalbe using pstrick package */
class drgeoPostScriptDrawable:public drgeoLatexDrawable
{
public:
  drgeoPostScriptDrawable
    (const gchar * name, drgeoFigure * figure, FILE * fileHandle, drgeoPoint origin,
     drgeoPoint size, gdouble scale);
  // These are the concrete implementation of the
  void drawPoint (drgeoStyle & style, drgeoPoint & point);
  void drawPolygon (drgeoStyle & style, drgeoPoint * point, gint number);
  void drawSegment (drgeoStyle & style, drgeoPoint & start, drgeoPoint & end);
  void drawCircle (drgeoStyle & style, drgeoPoint & center, double radius);
  void drawCircle
    (drgeoStyle & style, drgeoPoint & center, drgeoPoint & point);
  void drawArc
    (drgeoStyle & style, drgeoPoint & center,
     double radius, double start, double length);
  void drawText (drgeoPoint & where, char *text, drgeoColorType fontColor);
private:
    drgeoPoint areaToPostScript (drgeoPoint p);
};


#endif /* DRGEO_POSTSCRIPTDRAWABLE_H */
