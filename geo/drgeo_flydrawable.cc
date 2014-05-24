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
#include "drgeo_flydrawable.h"

static gchar const *flyPointSize[3] =
  { "3", "5", "7" };

static gchar const *flyLineCommand[4] =
  { "dline", "line", "line" }; 

static gchar const *flyLineWidth[4] =
  { "0", "1", "1", "2" }; 

static gchar const *flyLineColor[DRGEO_NUMBER_COLOR] =
  { "black", "darkgrey", "gray", "white",
  "green", "mediumspringgreen", "blue", "skyblue",
  "crimson", "orangered", "gold", "lemonchiffon"
};

static gchar const *flyColor[DRGEO_NUMBER_COLOR] =
  { "black", "darkgrey", "gray", "white",
  "green", "mediumspringgreen", "blue", "skyblue",
  "crimson", "orangered", "gold", "lemonchiffon"
};

drgeoFlyDrawable::
drgeoFlyDrawable (drgeoFigure * figure, FILE * fileHandle,
		    drgeoPoint origin, drgeoPoint size, gdouble scale)
{
  this->figure = figure;
  this->fileHandle = fileHandle;
  this->origin = origin;
  this->size = size;
  this->scale = scale;
  drgeoPoint s;

  /* set the default color and shape of the object */
  /* point are by defaut red cross */

  /* file header */
  s = getAreaSize();
  fprintf (fileHandle, 
		   "new %f, %f\n",
		   s.getX (), s.getY ());
  fprintf (fileHandle,
		   "xrange 0,%f\nyrange 0,%f\n",
		   s.getX (), s.getY ());
}

void
drgeoFlyDrawable::drawPoint (drgeoStyle & style, drgeoPoint & point)
{
  drgeoPoint p;
  liste_elem *selection;
  double s;
  drgeoPointType type;

  p = areaToFly (point);
  s = 5;
  type = style.pointShape;

  switch (type)
	{
	  case drgeoPointRound :
	  fprintf (fileHandle,
			   "fellipse %f,%f,%f,%f,%s\n", // filled ellipse
			   p.getX (), p.getY (), s, s, 
			   flyLineColor[style.color]);
	  break;
	  case drgeoPointX :
	  fprintf (fileHandle,
			   "line %f,%f,%f,%f,%s\n",
			   p.getX () - s/2, p.getY () - s/2, 
			   p.getX () + s/2, p.getY () + s/2,
			   flyLineColor[style.color]);
	  fprintf (fileHandle,
			   "line %f,%f,%f,%f,%s\n",
			   p.getX () - s/2, p.getY () + s/2, 
			   p.getX () + s/2, p.getY () - s/2,
			   flyLineColor[style.color]);
	  break;
	  case drgeoPointRec :
	  fprintf (fileHandle,
			   "frectangle %f,%f,%f,%f,%s\n",
			   p.getX () - s/2, p.getY () - s/2, 
			   p.getX () + s/2, p.getY () + s/2,
			   flyLineColor[style.color]);
	  break;
	  case drgeoPointRoundEmpty :
	  fprintf (fileHandle,
			   "circle %f,%f,%f,%s\n",
			   p.getX (), p.getY (), s, 
			   flyLineColor[style.color]);
	  break;
	  case drgeoPointRecEmpty :
	  fprintf (fileHandle,
			   "rectangle %f,%f,%f,%f,%s\n",
			   p.getX () - s/2, p.getY () - s/2, 
			   p.getX () + s/2, p.getY () + s/2,
			   flyLineColor[style.color]);
	  break;
	} 
}

void
drgeoFlyDrawable::
drawPolygon (drgeoStyle & style, drgeoPoint * point, gint number)
{
  drgeoPoint p;
  gint nb;

  fprintf (fileHandle, "fpoly %s", flyColor[style.color]);
  for (nb = 0; nb < number; nb++)
    {
      p = areaToFly (point[nb]);
      fprintf (fileHandle, ",%f,%f", p.getX (), p.getY ());
    }
  p = areaToFly (point[0]);
  fprintf (fileHandle, ",%f,%f", p.getX (), p.getY ());
  fprintf (fileHandle, "\n");
}

void
drgeoFlyDrawable::
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
drgeoFlyDrawable::
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
drgeoFlyDrawable::
drawSegment (drgeoStyle & style, drgeoPoint & start, drgeoPoint & end)
{
  drgeoPoint p1, p2;
  char const *command;
  char const *thickness;

  p2 = areaToFly (end);
  p1 = areaToFly (start);
  if (style.mask == yes) {
	command = "line";
    thickness = flyLineWidth[0]; } 
  else {
	command = flyLineCommand[style.thick];
    thickness = flyLineWidth[style.thick + 1]; }
  if (style.thick == drgeoThicknessLarge)
	fprintf (fileHandle, "linewidth %d\n", thickness);
  fprintf (fileHandle,
		   "%s %f,%f,%f,%f,%s\n",
		   command, 
		   p1.getX (), p1.getY (), p2.getX (), p2.getY (),
		   flyLineColor[style.color]); 
  if (style.thick == drgeoThicknessLarge)
	fprintf (fileHandle, "linewidth %d\n", flyLineWidth[drgeoThicknessNormal]);
}

void
drgeoFlyDrawable::
drawCircle (drgeoStyle & style, drgeoPoint & center, drgeoPoint & point)
{
  double radius;

  radius = (point - center).norm ();
  drawCircle (style, center, radius);
}

void
drgeoFlyDrawable::
drawCircle (drgeoStyle & style, drgeoPoint & center, double radius)
{
  drgeoPoint p;
  char const *thickness;

  p = areaToFly (center);
  if (style.mask == yes)
    thickness = flyLineWidth[0];
  else
    thickness = flyLineWidth[style.thick + 1];
  if (style.thick == drgeoThicknessLarge)
	fprintf (fileHandle, "linewidth %d\n", thickness);
  fprintf (fileHandle, 
		   "circle %f,%f,%f,%s\n",
		   p.getX (), p.getY (), 2 * radius * scale, // diameter
		   flyLineColor[style.color]); 
  if (style.thick == drgeoThicknessLarge)
	fprintf (fileHandle, "linewidth %d\n", flyLineWidth[drgeoThicknessNormal]);
}

void
drgeoFlyDrawable::

drawArc (drgeoStyle & style, drgeoPoint & center, double radius,
	 double start, double length)
{
  drgeoPoint p;
  char const *thickness;

  p = areaToFly (center);
  if (style.mask == yes)
    thickness = flyLineWidth[0];
  else
    thickness = flyLineWidth[style.thick + 1];
  if (style.thick == drgeoThicknessLarge)
	fprintf (fileHandle, "linewidth %d\n", thickness);
  if (length > 0) 
    fprintf (fileHandle, "arc %f, %f, %f, %f, %f, %f, %s\n",
			 p.getX (), p.getY (), 2 * radius * scale, 2 * radius * scale,
			 start * 180.0 / M_PI, (start + length) * 180.0 / M_PI,
			 flyLineColor[style.color]); 
  else
    fprintf (fileHandle, 
			 "arc %f, %f, %f, %f, %f, %f, %s\n",
			 p.getX (), p.getY (), 
			 // ellipse's width and height
			 2 * radius * scale, 2 * radius * scale, 
			 (start + length) * 180.0 / M_PI, start * 180.0 / M_PI,
			 flyLineColor[style.color]); 
  if (style.thick == drgeoThicknessLarge)
	fprintf (fileHandle, "linewidth %d\n", flyLineWidth[drgeoThicknessNormal]);
}

void
drgeoFlyDrawable::
drawText (drgeoPoint & where, char *text, drgeoColorType fontColor)
{
  drgeoPoint p;
  drgeoPoint s;
  
  if (!text)
    return;
  p = areaToFly (where);
  s = getAreaSize();
  if (p.getX () <= 0. || p.getY () <= 0.
      || p.getX () > s.getX () || p.getY () > s.getY ())
    return;
  fprintf (fileHandle, 
		   "text %s,%f,%f,%s,%s\n",
		   flyLineColor[fontColor],
		   p.getX () + 5, p.getY () + 13,
		   "normal",
		   text);
}

drgeoPoint
drgeoFlyDrawable::getAreaCenter ()
{
  return origin;
}

drgeoPoint
drgeoFlyDrawable::getAreaSize ()
{
  return size * scale;
}


double
drgeoFlyDrawable::pixelToWorld (int pixels)
{
  return (double) (((double) pixels) / (50.0 * scale));
}

int
drgeoFlyDrawable::worldToPixel (double world)
{
  return (int) (world * scale * 50);
}


/* private */
drgeoPoint
drgeoFlyDrawable::areaToFly (drgeoPoint p)
{
  // return p * 10;
  return (p + size / 2 - origin) * scale;
}
