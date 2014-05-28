/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-1999
 * hilaire.fernandes@iname.com 
 * 
 * This code is copyright Laurent Gauthier 1999
 * lolo@seul.org
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

#include <math.h>
#include <gtk/gtk.h>
#include "drgeo_drawable.h"

drgeoDrawable::drgeoDrawable ()
{
  // Nothing special there
}

void
drgeoDrawable::setFigure (drgeoFigure * figure)
{
  this->figure = figure;
  //  refresh ();
}

drgeoFigure *
drgeoDrawable::getFigure ()
{
  return (figure);
}

gboolean
drgeoDrawable::clipLine (drgeoStyle &style, drgeoPoint &start, drgeoVector &u, 
			 gdouble worldTop, gdouble worldBottom,
			 gdouble worldLeft, gdouble worldRight)
{
  static gdouble d;
  static drgeoPoint m, c[2], anchor;
  gboolean anchorSet = FALSE;
  gint pt = 0;

  if (u.getX () == 0 && u.getY () == 0)
    return false;

  // m a point on the line
  m = start + 10 * u;
  if (ABS (start.getX () - m.getX ()) < EPSILON)
    {
      c[0].set (start.getX (), worldTop);
      c[1].set (start.getX (), worldBottom);
      pt = 2;
      anchor = c[1] + drgeoPoint (.5, .5);
    }
  else
    {
      // line slope
      d = u.getY () / u.getX ();
      if (ABS (d) < EPSILON)
	{
	  c[0].set (worldLeft, start.getY ());
	  c[1].set (worldRight, start.getY ());
	  pt = 2;
	  anchor = c[0] + drgeoPoint (.5, .5);
	}
      else
	{
	  // ordonnee a l'origine
	  double b = start.getY () - d * start.getX ();
	  double y = d * worldLeft + b;
	  if (y >= worldBottom && y < worldTop)
	    {
	      c[pt++].set (worldLeft, y);
	      anchor = c[0] + drgeoPoint (.5, .5);
	      anchorSet = TRUE;
	    }
	  y = d * worldRight + b;
	  if (y >= worldBottom && y < worldTop)
	    {
	      c[pt++].set (worldRight, y);
	    }
	  double x = (worldTop - b) / d;
	  if (x >= worldLeft && x < worldRight && pt < 2)
	    {
	      c[pt++].set (x, worldTop);
	      if (!anchorSet)
		{
		  anchor = c[pt - 1] + drgeoPoint (.5, -.5);
		  anchorSet = TRUE;
		}
	    }
	  x = (worldBottom - b) / d;
	  if (x >= worldLeft && x < worldRight && pt < 2)
	    {
	      c[pt++].set (x, worldBottom);
	      anchor = c[1] + drgeoPoint (.5, .5);
	    }
	}
    }
  if (pt == 2)
    {
      drawSegment (style, c[0], c[1]);
      if (style.name)
	drawText (anchor, style.name, drgeoColorBlack);
      return true;
    }  
  return false;
}

gboolean
drgeoDrawable::clipHalfLine (drgeoStyle &style, drgeoPoint &start, drgeoVector &u, 
			     gdouble worldTop, gdouble worldBottom,
			     gdouble worldLeft, gdouble worldRight)
{
  static drgeoPoint c[2];
  char inDrawingArea = FALSE;

  if (u.getX () == 0 && u.getY () == 0)
    return false;
  c[0] = start;
  if (ABS (u.getX ()) < EPSILON)
    {
      if (u.getY () > 0)
	c[1].set (start.getX (), worldTop);
      else
	c[1].set (start.getX (), worldBottom);
      inDrawingArea = TRUE;
    }
  else
    {
      double d = u.getY () / u.getX ();
      if (ABS (d) < EPSILON)
	{
	  if (u.getX () > 0)
	    c[1].set (worldRight, start.getY ());
	  else
	    c[1].set (worldLeft, start.getY ());
	  inDrawingArea = TRUE;
	}
      else
	{
	  // ordonnee a l'origine
	  double b = start.getY () - d * start.getX ();
	  double y = d * worldLeft + b;
	  if (y >= worldBottom && y < worldTop && start.getX () > worldLeft
	      && u.getX () < 0)
	    {
	      c[1].set (worldLeft, y);
	      inDrawingArea = TRUE;
	      goto end_halfLine_externalClipping;
	    }
	  y = d * worldRight + b;
	  if (y >= worldBottom && y < worldTop && start.getX () < worldRight
	      && u.getX () > 0)
	    {
	      c[1].set (worldRight, y);
	      inDrawingArea = TRUE;
	      goto end_halfLine_externalClipping;
	    }
	  double x = (worldBottom - b) / d;
	  if (x >= worldLeft && x < worldRight && start.getY () > worldBottom
	      && u.getY () < 0)
	    {
	      c[1].set (x, worldBottom);
	      inDrawingArea = TRUE;
	      goto end_halfLine_externalClipping;
	    }
	  x = (worldTop - b) / d;
	  if (x >= worldLeft && x < worldRight && start.getY () < worldTop
	      && u.getY () > 0)
	    {
	      c[1].set (x, worldTop);
	      inDrawingArea = TRUE;
	      goto end_halfLine_externalClipping;
	    }
	}
    }
end_halfLine_externalClipping:
  if (inDrawingArea)
    {
      drawSegment (style, c[0], c[1]);
      return true;
    }
  return false;
}


void 
drgeoDrawable::showGrid ()
{
  double worldLeft, worldTop, worldRight, worldBottom;
  double x, y;
  drgeoStyle style, styleAxe;
  drgeoPoint s1,s2, size, center;

  // size of the drawing area in world unit
  size = getAreaSize ();
  center = getAreaCenter ();
  worldLeft = center.getX () - size.getX () / 2;
  worldRight = center.getX () + size.getX () / 2;
  worldTop = center.getY () + size.getY () / 2;
  worldBottom = center.getY () - size.getY () / 2;

  style.color = drgeoColorGrey;
  style.thick = drgeoThicknessDashed;

  s1.setY (worldTop);
  s2.setY (worldBottom);

  for (x = worldLeft; x <= worldRight; x++)
    {    
      s1.setX (ceil (x));
      s2.setX(s1.getX ());
      if (s1.getX () == 0)
	  drawSegment (styleAxe, s1, s2);
      else
	drawSegment (style, s1, s2);
    }

  s1.setX (worldLeft);
  s2.setX (worldRight);
  for (y = worldBottom; y <= worldTop; y++)
    {
      s1.setY (ceil (y));
      s2.setY (s1.getY ());
      if (s1.getY () == 0)
	  drawSegment (styleAxe, s1, s2);
	else
	  drawSegment (style, s1, s2);
   
    }
}

double
drgeoDrawable::pixelToWorld (int pixels)
{
  return (double) (((double) pixels) / scale);
}

int
drgeoDrawable::worldToPixel (double world)
{
  return (int) (world * scale);
}

void
drgeoDrawable::updateDescription ()
{
  liste_elem *figureList;
  geometricObject *item;
  gint nb;

  clearDescription ();
  figureList = figure->getFigureList ();
  nb = figureList->nb_elem;
  figureList->init_lire ();
  while (nb-- != 0)
    addDescriptionItem ((geometricObject *) figureList->lire (0));
}


// The following methods are invoked by subclasses of the
// drgeoDrawable class, and they implement the event handling
// behaviour of the drgeoDrawable.

// In particular they are responsible for invocation of drag, drop and
// selection related methods of the drgeoFigure object.

// These methods are operating using real-world coordinates, and not
// screen coordinates.

// Implementation of drgeoDrawableUI methods.

gdouble
drgeoDrawable::getScale ()
{
  return scale;
}

drgeoDrawableUI::drgeoDrawableUI ()
{
  this->tool = NULL;
  selectTool = new drgeoSelectTool;
  animationToolDialogLess = new drgeoAnimationDialogLess;
  printingAreaTool = new drgeoPrintingAreaTool;
  buildTool = new drgeoBuildTool;
  styleTool = new drgeoStyleTool;
  propertyTool = new drgeoPropertyTool;
  macroBuildTool = new drgeoMacroBuildTool;
  deleteTool = new drgeoDeleteTool;
  macroPlayTool = new drgeoMacroPlayTool;
  macroPlayToolDialogLess = new drgeoMacroPlayToolDialogLess;
  setTool (selectTool);
}

drgeoDrawableUI::~drgeoDrawableUI ()
{
  printf ("drgeoDrawableUI destructor \n");
  delete animationToolDialogLess;
  delete macroPlayToolDialogLess;
  delete macroPlayTool;
  delete printingAreaTool;
  delete deleteTool;
  delete macroBuildTool;
  delete styleTool;
  delete buildTool;
  delete selectTool;
}

void
drgeoDrawableUI::setTool (drgeoTool * tool)
{
  if (this->tool)
    {
      // Finish with the current tool.
      this->tool->finish (this);
    }
  this->tool = tool;
  if (this->tool)
    {
      // And initialize the new one.
      // The parameter received could be a NULL pointer, this is why
      // we are running initialize inside this test.
      this->tool->initialize (this);
    }
}

void
drgeoDrawableUI::setMacroPlayer (drgeoMacroPlayer *player)
{
  this->player = player;
}

void
drgeoDrawableUI::handlePress (drgeoPoint & where)
{
  if (tool)
    {
      tool->handlePress (this, where);
    }
}

void
drgeoDrawableUI::handleRelease (drgeoPoint & where)
{
  if (tool)
    {
      tool->handleRelease (this, where);
    }
}

void
drgeoDrawableUI::handleMouseAt (drgeoPoint & where)
{
  if (tool)
    {
      tool->handleMouseAt (this, where);
    }
}

void
drgeoDrawableUI::handleChoice (void *item)
{
  if (tool)
    {
      tool->handleChoice (this, (geometricObject *) item);
    }
}
