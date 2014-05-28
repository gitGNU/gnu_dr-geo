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

#ifndef DRGEO_dxfDRAWABLE_H
#define DRGEO_dxfDRAWABLE_H

#include "drgeo_drawable.h"

/* Implement a dxf drawalbe using pstrick package */
class drgeodxfDrawable:public drgeoDrawable
{
public:
  drgeodxfDrawable
    (drgeoFigure * figure, FILE * fileHandle, drgeoPoint origin,
     drgeoPoint size, gdouble scale);
  drgeodxfDrawable ()
  {
  };
  // These are the concrete implementation of the
  virtual void drawPoint (drgeoStyle & style, drgeoPoint & point);
  virtual void drawPolygon
    (drgeoStyle & style, drgeoPoint * point, gint number);
  virtual void drawLine
    (drgeoStyle & style, drgeoPoint & start, drgeoPoint & end);
  virtual void drawHalfLine
    (drgeoStyle & style, drgeoPoint & start, drgeoVector & vect);
  virtual void drawSegment
    (drgeoStyle & style, drgeoPoint & start, drgeoPoint & end);
  virtual void drawCircle
    (drgeoStyle & style, drgeoPoint & center, double radius);
  virtual void drawCircle
    (drgeoStyle & style, drgeoPoint & center, drgeoPoint & point);
  virtual void drawArc
    (drgeoStyle & style, drgeoPoint & center,
     double radius, double start, double length);
  virtual void drawText (drgeoPoint & where, char *text,
			 drgeoColorType fontColor);
  virtual drgeoPoint getAreaCenter ();
  virtual drgeoPoint getAreaSize ();
  virtual double pixelToWorld (int pixels);
  virtual int worldToPixel (double world);

protected:
  drgeoPoint areaTodxf (drgeoPoint p);
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


#endif /* DRGEO_dxfDRAWABLE_H */
