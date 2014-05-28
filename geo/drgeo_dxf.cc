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

#include <stdio.h>
#include <math.h>

#include "couleur.h"
#include "drgeo_drgeoStyle.h"
#include "drgeo_dxfdrawable.h"

static gchar *dxfPointSize[3] =
  { "dotscale=1.5,", "dotscale=2.5,", "dotscale=2.5," };

static gchar *dxfPointType[5] =
  { "dotstyle=*,", "", "dotstyle=square*,", "dotstyle=o,",
"dotstlye=square*," };

static gchar *dxfLineType[4] =
  { "linestyle=dotted,", "linestyle=dashed,", "", "linewidth=0.05," };

static gchar *dxfLineColor[DRGEO_NUMBER_COLOR] =
  { "linecolor=black", "linecolor=darkgrey", "linecolor=gray",
"linecolor=white",
  "linecolor=green", "linecolor=green", "linecolor=blue", "linecolor=blue",
  "linecolor=red", "linecolor=red", "linecolor=yellow", "linecolor=yellow"
};

static gchar *dxfColor[DRGEO_NUMBER_COLOR] =
  { "black", "darkgrey", "gray", "white",
  "green", "green", "blue", "blue",
  "red", "red", "yellow", "yellow"
};

drgeodxfDrawable::
drgeodxfDrawable (drgeoFigure * figure, FILE * fileHandle,
		    drgeoPoint origin, drgeoPoint size, gdouble scale)
{
  this->figure = figure;
  this->fileHandle = fileHandle;
  this->origin = origin;
  this->size = size;
  this->scale = scale;
  fprintf (fileHandle, "\\begin{pspicture*}(0,0)(%f,%f)\n",
	   size.getX (), size.getY ());
  /* set the default color and shape of the object */
  /* point are by defaut red cross */
  fprintf (fileHandle, "\\psset{dotstyle=x, dotscale=2.0, linewidth=0.02}\n");
  /* draw the frame where will be located the figure */
  fprintf (fileHandle, "\\psframe(0,0)(%f,%f)\n", size.getX (), size.getY ());
}

void
drgeodxfDrawable::drawPoint (drgeoStyle & style, drgeoPoint & point)
{
  drgeoPoint p;

  p = areaTodxf (point);
  fprintf (fileHandle,
	   "\\psdots[%s%s%s](%f,%f)\n",
	   dxfPointSize[style.thick + 1],
	   dxfPointType[style.pointShape],
	   dxfLineColor[style.color], p.getX (), p.getY ());
}

void
drgeodxfDrawable::
drawPolygon (drgeoStyle & style, drgeoPoint * point, gint number)
{
  drgeoPoint p;
  gint nb;

  fprintf (fileHandle,
	   "\\pspolygon[fillstyle=crosshatch, hatchcolor=%s]\n",
	   dxfColor[style.color]);
  for (nb = 0; nb < number; nb++)
    {
      p = areaTodxf (point[nb]);
      fprintf (fileHandle, "(%f,%f)\n", p.getX (), p.getY ());
    }
}

void
drgeodxfDrawable::
drawLine (drgeoStyle & style, drgeoPoint & start, drgeoPoint & end)
{
  // start : one point on the line
  // end : a direction vector of the line

  // world boundaries
  double worldLeft, worldTop, worldRight, worldBottom, d;

  worldLeft = origin.getX () - size.getX () / 2;
  worldRight = origin.getX () + size.getX () / 2;
  worldTop = origin.getY () + size.getY () / 2;
  worldBottom = origin.getY () - size.getY () / 2;

  clipLine (style, start, end, worldTop, worldBottom, 
	    worldLeft, worldRight);
}

void
drgeodxfDrawable::
drawHalfLine (drgeoStyle & style, drgeoPoint & start, drgeoVector & vect)
{
  double worldLeft, worldTop, worldRight, worldBottom;

  worldLeft = origin.getX () - size.getX () / 2;
  worldRight = origin.getX () + size.getX () / 2;
  worldTop = origin.getY () + size.getY () / 2;
  worldBottom = origin.getY () - size.getY () / 2;

  clipHalfLine (style, start, vect, worldTop, worldBottom, 
		worldLeft, worldRight);
}

void
drgeodxfDrawable::
drawSegment (drgeoStyle & style, drgeoPoint & start, drgeoPoint & end)
{
  drgeoPoint p1, p2;
  char *thickness;

  p2 = areaTodxf (end);
  p1 = areaTodxf (start);
  if (style.mask == yes)
    thickness = dxfLineType[0];
  else
    thickness = dxfLineType[style.thick + 1];
  fprintf (fileHandle,
	   "\\psline[%s%s](%f,%f)(%f,%f)\n",
	   thickness, dxfLineColor[style.color],
	   p1.getX (), p1.getY (), p2.getX (), p2.getY ());
}

void
drgeodxfDrawable::
drawCircle (drgeoStyle & style, drgeoPoint & center, drgeoPoint & point)
{
  double radius;

  radius = (point - center).norm ();
  drawCircle (style, center, radius);
}

void
drgeodxfDrawable::
drawCircle (drgeoStyle & style, drgeoPoint & center, double radius)
{
  drgeoPoint p;
  char *thickness;

  p = areaTodxf (center);
  if (style.mask == yes)
    thickness = dxfLineType[0];
  else
    thickness = dxfLineType[style.thick + 1];
  fprintf (fileHandle, "\\pscircle[%s%s](%f,%f){%f}",
	   thickness, dxfLineColor[style.color],
	   p.getX (), p.getY (), radius);
}

void
drgeodxfDrawable::

drawArc (drgeoStyle & style, drgeoPoint & center, double radius,
	 double start, double length)
{
  drgeoPoint p;
  char *thickness;

  p = areaTodxf (center);
  if (style.mask == yes)
    thickness = dxfLineType[0];
  else
    thickness = dxfLineType[style.thick + 1];
  if (length > 0)
    fprintf (fileHandle, "\\psarc[%s%s](%f,%f){%f}{%f}{%f}",
	     thickness, dxfLineColor[style.color],
	     p.getX (), p.getY (), radius,
	     start * 180.0 / M_PI, (start + length) * 180.0 / M_PI);
  else
    fprintf (fileHandle, "\\psarc[%s%s](%f,%f){%f}{%f}{%f}",
	     thickness, dxfLineColor[style.color],
	     p.getX (), p.getY (), radius,
	     (start + length) * 180.0 / M_PI, start * 180.0 / M_PI);

}

void
drgeodxfDrawable::
drawText (drgeoPoint & where, char *text, drgeoColorType fontColor)
{
  drgeoPoint p;

  if (!text)
    return;
  p = areaTodxf (where);
  if (p.getX () <= 0. || p.getY () <= 0.
      || p.getX () > size.getX () || p.getY () > size.getY ())
    return;
  fprintf (fileHandle, "\\rput[bl]{0}(%f,%f){%s}\n",
	   p.getX () + 0.1, p.getY () + 0.1, text);
}

drgeoPoint
drgeodxfDrawable::getAreaCenter ()
{
  return origin;
}

drgeoPoint
drgeodxfDrawable::getAreaSize ()
{
  return size;
}


double
drgeodxfDrawable::pixelToWorld (int pixels)
{
  return (double) (((double) pixels) / (50.0 * scale));
}

int
drgeodxfDrawable::worldToPixel (double world)
{
  return (int) (world * scale * 50);
}


/* private */
drgeoPoint
drgeodxfDrawable::areaTodxf (drgeoPoint p)
{
  return p + size / 2 - origin;
}
