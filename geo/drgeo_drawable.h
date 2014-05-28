/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-2004
 * hilaire@ofset.org 
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

#ifndef DRGEO_DRAWABLE_H
#define DRGEO_DRAWABLE_H

class drgeoDrawable;

#include "drgeo_drgeoVector.h"
#include "drgeo_type.h"
#include "drgeo_drgeoStyle.h"
#include "drgeo_figure.h"
#include "drgeo_tool.h"
#include "drgeo_macro.h"


// Cursor we use
enum drgeoCursor
{
  none,
  hand,
  arrow,
  pen,
  move
};

// drgeoDrawable class is a pure virtual class, but the methods in
// charge of handling events are defined here, and must not be
// redefined by sub-classes.

class drgeoDrawable
{
public:
  drgeoDrawable ();
  void setFigure (drgeoFigure * figure);
  drgeoFigure *getFigure ();

  // The following methods are pure virtual methods, to be implemented
  // in sub-classes.  For an example of such an implementation please
  // refer to the drgeoGtkDrawable class definition.

  virtual void drawPoint (drgeoStyle & style, drgeoPoint & point) = 0;
  virtual void drawPolygon (drgeoStyle & style, drgeoPoint * point,
			    gint number) = 0;
  virtual void drawLine (drgeoStyle & style, drgeoPoint & start,
			 drgeoPoint & end) = 0;
  gboolean clipLine (drgeoStyle &style, drgeoPoint &start, drgeoVector &u,
		     gdouble worldTop, gdouble worldBottom,
		     gdouble worldLeft, gdouble worldRight);
  virtual void drawHalfLine (drgeoStyle & style, drgeoPoint & point,
			     drgeoVector & vect) = 0;
  gboolean clipHalfLine (drgeoStyle &style, drgeoPoint &start, drgeoVector &u,
			 gdouble worldTop, gdouble worldBottom,
			 gdouble worldLeft, gdouble worldRight);
  virtual void drawSegment (drgeoStyle & style, drgeoPoint & start,
			    drgeoPoint & end) = 0;
  virtual void drawCircle (drgeoStyle & style, drgeoPoint & center,
			   double radius) = 0;
  virtual void drawCircle (drgeoStyle & style, drgeoPoint & center,
			   drgeoPoint & point) = 0;
  virtual void drawArc (drgeoStyle & style, drgeoPoint & center,
			double radius, double start, double length) = 0;
  virtual void drawText (drgeoPoint & where, char *text,
			 drgeoColorType fontColor) = 0;
  virtual double stringWidth (char *text)
  {
    return 0;
  }
  virtual double stringHeight (char *text)
  {
    return .0;
  }
  virtual void showTip (drgeoPoint & where, char *text)
    {}
  virtual void removeTip ()
    {}
  void showGrid ();
  virtual void showPrintingArea (drgeoPoint& corner1, drgeoPoint& corner2)
    {}
  virtual void unsetPrintingArea ()
    {}
  virtual gboolean tipOn ()
    {}
  virtual void clear ()
    {}
  virtual double getRange ()
  {
    return .0;
  }
  virtual drgeoPoint getAreaCenter () = 0;
  virtual drgeoPoint getAreaSize () = 0;

  // Methods to convert pixel unit in world unit and vis-versa
  virtual double pixelToWorld (int pixels);
  virtual int worldToPixel (double world);


  // These methods are invoked by sub-classes.  They must not be
  // redefined as they are vital to define the event handling of
  // the drgeoDrawable.

  // redraw the whole figure
  virtual void refresh ()
    {};
  // update undo/redo state (button, menu, whatever)
  virtual void updateUndoState ()
    {};
  // Description for figure
  virtual void clearDescription ()
    {};
  virtual void addDescriptionItem (geometricObject * item)
    {};
  void updateDescription ();
  gdouble getScale ();
public:
  gdouble scale;
  drgeoFigure *figure;		// The figure being shown on this
  // drawable.

};

class drgeoMacroBuildDialog
{
public:
  virtual void show () = 0;
  virtual void hide () = 0;
  virtual void add (geometricObject * item) = 0;
  virtual ~ drgeoMacroBuildDialog ()
    {}
protected:
    class drgeoMacroBuilder * builder;
};

class drgeoMacroPlayDialog
{
public:
  virtual void show () = 0;
  virtual void hide () = 0;
  virtual void add (geometricObject * item) = 0;
  virtual ~ drgeoMacroPlayDialog ()
    {}
protected:
    class drgeoMacroPlayer * player;
};

class drgeoStyleDialog
{
public:
  virtual void show () = 0;
  virtual void hide () = 0;
  virtual void edit (class geometricObject * aItem) = 0;
  virtual ~ drgeoStyleDialog ()
    {}
protected:
  // Nothing for now.
};

class drgeoPropertyDialog
{
public:
  virtual void show () = 0;
  virtual void hide () = 0;
  virtual ~ drgeoPropertyDialog ()
    {}
  virtual void edit (class geometricObject * aItem) = 0;
protected:
  // Nothing for now.
};

class drgeoDrawableUI:public drgeoDrawable
{

  // This is a drawable plus user interface methods.

public:

  drgeoDrawableUI ();
  virtual ~ drgeoDrawableUI ();
  void setTool (class drgeoTool * tool);

  // This following methods are used to ask the user for more
  // information.

  virtual void chooseItem (class liste_elem * list) = 0;
  virtual bool askOkCancel (char *message) = 0;
  virtual drgeoMacroBuildDialog *createMacroBuildDialog (drgeoMacroBuilder *
							 builder) = 0;
  virtual drgeoMacroPlayDialog *createMacroPlayDialog (drgeoMacroPlayer *
						       player) = 0;
  // used for the dialog less mode to play macro
  void setMacroPlayer (drgeoMacroPlayer *player);
  virtual drgeoStyleDialog *createStyleDialog () = 0;
  virtual drgeoPropertyDialog *createPropertyDialog () = 0;

  // The following methods are invoked each time the user is acting on
  // the drawable.  These calls are forwarded to the current tool
  // associated to the drawable (using setTool()) for appropriate
  // handling.

  void handlePress (drgeoPoint & where);
  void handleRelease (drgeoPoint & where);
  void handleMouseAt (drgeoPoint & where);
  void handleChoice (void *item);
  virtual void setCursor (drgeoCursor cursor) = 0;

  // tools for selection mode and build mode
  drgeoTool *selectTool, *buildTool, *macroBuildTool, *deleteTool,
    *macroPlayTool, *macroPlayToolDialogLess, *styleTool, 
    *propertyTool, *printingAreaTool, *animationToolDialogLess;

  class drgeoMacroBuilder *builder;
  class drgeoMacroPlayer *player;

private:

    class drgeoTool * tool;	// The current tool in use.  This tool
  // is determining the way user actions
  // are handled.

};

#endif
