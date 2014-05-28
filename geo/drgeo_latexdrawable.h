/*
 *  Dr Genius an interactive geometry software
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

#ifndef DRGEO_LATEXDRAWABLE_H
#define DRGEO_LATEXDRAWABLE_H

#include "drgeo_drawable.h"

/* Implement a latex drawalbe using pstrick package */
class drgeoLatexDrawable:public drgeoDrawable
{
public:
  drgeoLatexDrawable
    (drgeoFigure * figure, FILE * fileHandle, drgeoPoint origin,
     drgeoPoint size, gdouble scale);
  drgeoLatexDrawable ()
  {
  };
  // These are the concrete implementation of the
  virtual void drawPoint (drgeoPoint & point);
  virtual void drawPolygon
    (drgeoPoint * point, gint number);
  virtual void drawLine
    (drgeoPoint & start, drgeoPoint & end);
  virtual void drawHalfLine
    (drgeoPoint & start, drgeoVector & vect);
  virtual void drawSegment
    (drgeoPoint & start, drgeoPoint & end);
  virtual void drawCircle
    (drgeoPoint & center, double radius);
  virtual void drawCircle
    (drgeoPoint & center, drgeoPoint & point);
  virtual void drawArc
    (drgeoPoint & center,
     double radius, double start, double length);
  virtual void drawText (drgeoPoint & where, char *text,
			 drgeoColorType fontColor);
  virtual drgeoPoint getAreaCenter ();
  virtual drgeoPoint getAreaSize ();
  virtual double pixelToWorld (int pixels);
  virtual int worldToPixel (double world);

protected:
  drgeoPoint areaToLatex (drgeoPoint p);
  /*
     orgin: coordinate in world unit of the point at the
     center of the visible area.

     size: size of the visible area in world unit.

     scale: scale factor, 1.0 means 1 world unit = 1.0 cm
     2.0 means 1 world unit = 2.0 cm
   */
  drgeoPoint origin, size;
  FILE *fileHandle;
};


#endif /* DRGEO_LATEXDRAWABLE_H */
