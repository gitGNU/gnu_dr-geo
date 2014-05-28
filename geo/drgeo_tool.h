/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-2001
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

#ifndef DRGEO_TOOL_H
#define DRGEO_TOOL_H

class drgeoTool;

#include "drgeo_drawable.h"
#include "drgeo_macro.h"


enum drgeoState
{
  drgeoStateNeutral,		// The mouse is neutral.
  drgeoStateItemGrabbed,	// An item has just been grabbed, but not yet dragged..
  drgeoStateItemDragged,	// An item is being dragged.
  drgeoStateItemChoice,		// A choice is being done by the user.
  drgeoStateItemSelected,	// An item has just been selected.
  drgeoState1stCornerSelected,  // 1st corner of the printing area selected.
  drgeoState2ndCornerSelected,  // 2nd corner of the printing area selected.
  drgeoStateAnimated            // A point is animated
};

class drgeoTool
{
public:

  virtual void initialize (class drgeoDrawableUI * drawable) = 0;
  virtual void handlePress (class drgeoDrawableUI * drawable,
			    class drgeoPoint & where) = 0;
  virtual void handleRelease (class drgeoDrawableUI * drawable,
			      class drgeoPoint & where) = 0;
  virtual void handleMouseAt (class drgeoDrawableUI * drawable,
			      class drgeoPoint & where) = 0;
  virtual void handleChoice (class drgeoDrawableUI * drawable,
			     class geometricObject * item) = 0;
  virtual void finish (class drgeoDrawableUI * drawable) = 0;
  drgeoState getDrgeoState ();
protected:
  // This state variable is used in event handling methods.
    drgeoState state;

  // This variable is used to store the location of the last item
  // selected when one item has been selected among multiple
  // items.
  drgeoPoint spot;

  // Last position of the mouse.  This information is used
  // during drag.
  drgeoPoint last;

  // Item selected during the last multiple choice selection.
  geometricObject *item;

  // Item under mouse
  geometricObject *itemUnderMouse;

  // Used when tracking move relative to spot.
  bool away;

  bool tipOn;
  // number of element under the mouse during the last mouse move
  gint last_nb_elem;
};

class drgeoNullTool:public drgeoTool
{
public:
  void initialize (class drgeoDrawableUI * drawable);
  void handlePress (class drgeoDrawableUI * drawable, drgeoPoint & where);
  void handleRelease (class drgeoDrawableUI * drawable, drgeoPoint & where);
  void handleMouseAt (class drgeoDrawableUI * drawable, drgeoPoint & where);
  void handleChoice (class drgeoDrawableUI * drawable,
		     geometricObject * item);
  void finish (class drgeoDrawableUI * drawable);
};


class drgeoPrintingAreaTool:public drgeoNullTool
{
 public:
  void initialize (class drgeoDrawableUI * drawable);
  void handlePress (class drgeoDrawableUI * drawable, drgeoPoint & where);
  void handleMouseAt (class drgeoDrawableUI * drawable, drgeoPoint & where);
  void finish (class drgeoDrawableUI * drawable);
 protected:
  drgeoPoint corner1, corner2;
};

class drgeoSelectTool:public drgeoTool
{
public:
  void initialize (class drgeoDrawableUI * drawable);
  void handlePress (class drgeoDrawableUI * drawable,
		    class drgeoPoint & where);
  void handleRelease (class drgeoDrawableUI * drawable,
		      class drgeoPoint & where);
  void handleMouseAt (class drgeoDrawableUI * drawable,
		      class drgeoPoint & where);
  void handleChoice (class drgeoDrawableUI * drawable,
		     class geometricObject * item);
  void finish (class drgeoDrawableUI * drawable);
private:
  // position where start the drag
    drgeoPoint start;
};

// Build tool
class drgeoBuildTool:public drgeoTool
{
public:
  void initialize (class drgeoDrawableUI * drawable);
  void handlePress (class drgeoDrawableUI * drawable,
		    class drgeoPoint & where);
  void handleRelease (class drgeoDrawableUI * drawable,
		      class drgeoPoint & where);
  void handleMouseAt (class drgeoDrawableUI * drawable,
		      class drgeoPoint & where);
  void handleChoice (class drgeoDrawableUI * drawable,
		     class geometricObject * item);
  void finish (class drgeoDrawableUI * drawable);
};

class drgeoMacroBuildTool:public drgeoBuildTool
{
public:
  void initialize (class drgeoDrawableUI * drawable);
  void handleChoice (class drgeoDrawableUI * drawable,
		     class geometricObject * item);
  void finish (class drgeoDrawableUI * drawable);
private:
    class drgeoMacroBuilder * builder;
  // The dialog used to control the macro builder.
  class drgeoMacroBuildDialog *dialog;
};

class drgeoMacroPlayTool:public drgeoBuildTool
{
public:

  void initialize (class drgeoDrawableUI * drawable);
  void handleChoice (class drgeoDrawableUI * drawable,
		     class geometricObject * item);
  void finish (class drgeoDrawableUI * drawable);
  gboolean selectMacro (const gchar *name);
private:
  class drgeoMacroPlayer * player;
  class drgeoMacroPlayDialog *dialog;
};

class drgeoMacroPlayToolDialogLess:public drgeoBuildTool
{
public:

  void initialize (class drgeoDrawableUI * drawable);
  void handleChoice (class drgeoDrawableUI * drawable,
		     class geometricObject * item);
  void finish (class drgeoDrawableUI * drawable);
  gboolean selectMacro (const gchar *name);
private:
  class drgeoMacroPlayer * player;
};


class drgeoAnimationDialogLess:public drgeoBuildTool
{
public:
  void initialize (class drgeoDrawableUI * drawable);
  void handlePress (class drgeoDrawableUI * drawable,
		    class drgeoPoint & where);
  void handleChoice (drgeoDrawableUI * drawable,
		     geometricObject * item);
  void finish (class drgeoDrawableUI * drawable);
  void animate ();
  void reset ();
  void setStep (gdouble aStep);
private:
  gdouble initialAbscissa, animatedAbscissa, step;
  gint timeoutTag;
  drgeoDrawableUI *drawable;
};


class drgeoStyleTool:public drgeoBuildTool
{
public:
  void initialize (class drgeoDrawableUI * drawable);
  void handleChoice (class drgeoDrawableUI * drawable,
		     class geometricObject * item);
  void finish (class drgeoDrawableUI * drawable);

private:
  // The dialog used to control the style of items.
    class drgeoStyleDialog * dialog;
};

class drgeoPropertyTool:public drgeoBuildTool
{
public:
  void initialize (class drgeoDrawableUI * drawable);
  void handleChoice (class drgeoDrawableUI * drawable,
		     class geometricObject * item);
  void finish (class drgeoDrawableUI * drawable);
private:
  // The dialog used to control the style of items.
    class drgeoPropertyDialog * dialog;
};

class drgeoDeleteTool:public drgeoBuildTool
{
public:
  void handleChoice (class drgeoDrawableUI * drawable,
		     class geometricObject * myItem);
};

#endif
