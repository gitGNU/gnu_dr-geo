/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-2003
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

#include <string.h>

#include "drgeo_tool.h"
#include "drgeo_point.h"

drgeoState 
drgeoTool::getDrgeoState ()
{
  return state;
}

// -------------------------------------------------
// Implementation for the Null tool, this tool does nothing.

void
drgeoNullTool::initialize (drgeoDrawableUI * drawable)
{
  // Do nothing.
  g_printerr ("initialize()\n");
}

void
drgeoNullTool::handlePress (drgeoDrawableUI * drawable, drgeoPoint & where)
{
  // Do nothing.
  g_printerr ("handlePress (%lf,%lf)\n", where.getX (), where.getY ());
}

void
drgeoNullTool::handleRelease (drgeoDrawableUI * drawable, drgeoPoint & where)
{
  // Do nothing.
  // printf ("handleRelease (%lf,%lf)\n", where.getX (), where.getY ());
}

void
drgeoNullTool::handleMouseAt (drgeoDrawableUI * drawable, drgeoPoint & where)
{
  // Do nothing.
  g_printerr ("handleMouseAt (%lf,%lf)\n", where.getX (), where.getY ());
}

void
drgeoNullTool::
handleChoice (drgeoDrawableUI * drawable, geometricObject * item)
{
  // Do nothing.
  g_printerr ("HandleChoice()\n");
}

void
drgeoNullTool::finish (drgeoDrawableUI * drawable)
{
  // Do nothing.
  g_printerr ("finish()\n");
}

// -------------------------------------------------
// Implementation for the Printing Area tool.
void 
drgeoPrintingAreaTool::initialize (class drgeoDrawableUI * drawable)
{
  // no corner of the printing area is selected
  state = drgeoStateNeutral;
}

void
drgeoPrintingAreaTool::handlePress (class drgeoDrawableUI * drawable, 
				   drgeoPoint & where)
{
  switch (state)
    {
    case drgeoStateNeutral:
      corner1 = where;
      state = drgeoState1stCornerSelected;
      break;
    case drgeoState1stCornerSelected:
      corner2 = where;
      state = drgeoState2ndCornerSelected;
      // instruct the drawable about the selected printing area
      drawable->showPrintingArea (corner1, corner2);
      break;
    case drgeoState2ndCornerSelected:
      // does the user wants to reselect the area? in this case
      // fallback to initial state
      corner1 = where;	
      state = drgeoState1stCornerSelected;
      drawable->unsetPrintingArea ();
      break;
    default:
      // We should never reach that point !
      break;
    }
}

void 
drgeoPrintingAreaTool::handleMouseAt (class drgeoDrawableUI * drawable, 
				      drgeoPoint & where)
{
  switch (state)
    {
    case drgeoStateNeutral:
      // nothing to be done
      break;
    case drgeoState1stCornerSelected:
      // we can print the moving printing area
      drawable->showPrintingArea (corner1, where);
      break;
    case drgeoState2ndCornerSelected:
      // Nothing to be done, the selected drawing area is already
      // printed
      break;
    default:
      // We should never reach that point !
      break;
    }
}

void 
drgeoPrintingAreaTool::finish (class drgeoDrawableUI * drawable)
{
  switch (state)
    {
    case drgeoState2ndCornerSelected:
      // the drawable already now the drawing area
      break;
    case drgeoState1stCornerSelected:
      // unselect the printing area, the 2 corner was not selected
      drawable->unsetPrintingArea ();
    }
}

// -------------------------------------------------
// Implementation for the Select tool.

void
drgeoSelectTool::initialize (drgeoDrawableUI * drawable)
{
  // This code is invoked when the tool selected.
  state = drgeoStateNeutral;
  tipOn = FALSE;
  last_nb_elem = 0;
}

void
drgeoSelectTool::handlePress (drgeoDrawableUI * drawable, drgeoPoint & where)
{
  liste_elem *list;
  int i;

  // Do nothing if no figure is currently associated with this drawable.
  if (drawable->figure != NULL)
    {
      drawable->removeTip ();
      switch (state)
	{
	case drgeoStateItemChoice:
	  // The user has not chosen any item, so revert to neutral
	  // state.
	  state = drgeoStateNeutral;
	  // Fall through!  This is really what is wanted here.
	case drgeoStateNeutral:
	  // The mouse button is being pressed, check if there are items
	  // near the mouse.
	  list = drawable->figure->mouseSelection (where);
	  list->init_lire ();
	  if (list->nb_elem == 1)
	    {
	      // Only one item is selected.
	      state = drgeoStateItemGrabbed;
	      handleChoice (drawable, (geometricObject *) list->lire (0));
	      last = where;
	    }
	  else if (list->nb_elem > 1)
	    {
	      // Several items are selected at once, we must prompt the
	      // user for a choice.  When the user is done with his
	      // choice the handleChoice method will be invoked.
	      state = drgeoStateItemChoice;
	      last = where;
	      drawable->chooseItem (list);
	    }
	  else
	    {
	      // The user click on the background:
	      // clear the selection
	      state = drgeoStateNeutral;
	      drawable->getFigure ()->clearSelection ();
	      itemUnderMouse = NULL;
	    }
	  break;
	case drgeoStateItemSelected:
	  // The mouse button is being pressed, check if the last
	  // selected item is near the mouse.
	  list = drawable->figure->mouseSelection (where);
	  list->init_lire ();
	  if (list->nb_elem >= 1)
	    {
	      // Check each item under the mouse to see if the last item
	      // selected is there.  If so select it, if not go back to
	      // neutral state.
	      for (i = 0; i < list->nb_elem; i++)
		{
		  if (this->item == (geometricObject *) list->lire (0))
		    {
		      state = drgeoStateItemGrabbed;
		      // The following is not necessary because it has
		      // already been performed, when the item has been
		      // selected among multiple items.
		      // handleChoice (drawable, this->item);
		      last = where;
		      break;	// Get out of the loop!  We found it.

		    }
		}
	      if (state == drgeoStateItemSelected)
		{
		  // The item last selected is not under the mouse at
		  // this point, so we are back to neutral state.
		  state = drgeoStateNeutral;
		}
	    }
	  else
	    {
	      // No item under the mouse go back to the Neutral state.
	      state = drgeoStateNeutral;
	    }
	  break;
	default:
	  // If we get there, then there's a problem with the event
	  // handling code... XXX
	  break;
	}
    }
}

void
drgeoSelectTool::
handleRelease (drgeoDrawableUI * drawable, drgeoPoint & where)
{
  // Do nothing if no figure is currently associated with this drawable.
  if (drawable->figure != NULL)
    {
      liste_elem *list;
      geometricObject *item;

      drawable->removeTip ();
      switch (state)
	{
	case drgeoStateItemChoice:
	case drgeoStateItemSelected:
	  // The user has not chosen any item, so revert to neutral
	  // state.
	  state = drgeoStateNeutral;
	  // Fall through!  This is really what is wanted here.
	case drgeoStateNeutral:
	  // The mouse button is just being released with nothing being
	  // dragged, so we only show a tip displaying the full name of
	  // the nearest item.
	  list = drawable->figure->mouseSelection (where);
	  list->init_lire ();
	  if (list->nb_elem == 1)
	    {
	      item = (geometricObject *) list->lire (0);
	      drawable->showTip (where, item->getTypeName ());
	      tipOn = TRUE;
	    }
	  else if (list->nb_elem > 1)
	    {
	      // There are several items under the mouse.
	      drawable->showTip (where, "???");
	      tipOn = TRUE;
	    }
	  break;
	case drgeoStateItemGrabbed:
	  // The user is just releasing the mouse.  Reset the state to
	  // neutral and that's all.
	  state = drgeoStateNeutral;
	  break;
	case drgeoStateItemDragged:
	  // The drag is finished, so we just drop the selected ietms
	  // where the mouse currently is.
	  state = drgeoStateNeutral;
	  drawable->figure->moveItem (NULL, where - start);
	  drawable->figure->dropSelection (last, where);
	  /* record the drag path */
	  drawable->removeTip ();

	  // Forget the item when dragged
	  drawable->getFigure ()->clearSelection ();
	  itemUnderMouse = NULL;

	  break;
	default:
	  // If we get there, then there's a problem with the event
	  // handling code... XXX
	  break;
	}
    }
}
void
drgeoSelectTool::
handleMouseAt (drgeoDrawableUI * drawable, drgeoPoint & where)
{
  int i;

  // Do nothing if no figure is currently associated with this drawable.
  if (drawable->figure != NULL)
    {
      liste_elem *list;

      switch (state)
	{
	case drgeoStateItemChoice:
	  // The user has not chosen any item, so revert to neutral
	  // state.
	  state = drgeoStateNeutral;
	  // Fall through!  This is really what is wanted here.
	case drgeoStateNeutral:
	  // The mouse is just moving over the figure, so we just query
	  // the figure to know which items are found near the current
	  // mouse position.  And a tooltip is shown displaying the full
	  // name of the nearest item.

	  list = drawable->figure->mouseSelection (where);
	  if (list->nb_elem == 1 &&
	      ((geometricObject *) list->lire (1) != itemUnderMouse))
	    {
	      // The mouse move over another item
	      itemUnderMouse = (geometricObject *) list->lire (1);
	      drawable->showTip (where, itemUnderMouse->getTypeName ());
	      tipOn = TRUE;
	      drawable->setCursor (hand);
	    }
	  else if (list->nb_elem > 1 && last_nb_elem <= 1)
	    {
	      // There are NOW several items under the mouse.
	      drawable->showTip (where, "???");
	      tipOn = TRUE;
	      drawable->setCursor (hand);
	      itemUnderMouse = NULL;
	    }
	  else if (list->nb_elem == 0 && drawable->tipOn ())
	    {
	      /* mouse not over object anymore, remove the tip */
	      drawable->removeTip ();
	      drawable->setCursor (arrow);
	      tipOn = FALSE;
	      itemUnderMouse = NULL;
	    }
	  last_nb_elem = list->nb_elem;
	  break;
	case drgeoStateItemSelected:
	  // The mouse is moving over the figure, check if we are moving
	  // away from the spot where the last item was selected.  If so
	  // go back to neutral state.
	  if (away
	      && (spot - where).norm () > (last - spot).norm ()
	      + 4.0 * drawable->getRange ())
	    {
	      state = drgeoStateNeutral;
	      drawable->figure->clearSelection ();
	      drawable->setCursor (arrow);
	    }
	  else
	    {
	      // If the last selected item is under the mouse, then show
	      // a tip about it.
	      list = drawable->figure->mouseSelection (where);
	      list->init_lire ();
	      if (list->nb_elem >= 1)
		{
		  // Check each item under the mouse to see if the last item
		  // selected is there.  If so show its tip.
		  for (i = 0; i < list->nb_elem; i++)
		    {
		      if (this->item == (geometricObject *) list->lire (0)
			  && !tipOn)
			{
			  drawable->showTip (where,
					     this->item->getTypeName ());
			  tipOn = TRUE;
			  drawable->setCursor (hand);
			  break;	// Get out of the loop!  We found it.
			}
		    }
		}
	      away = TRUE;
	      last = where;
	    }
	  break;
	case drgeoStateItemGrabbed:
	  // The user is just starting a mouse move with an item
	  // grabbed, this means a drag is starting.  Notify the
	  // figure.
	  state = drgeoStateItemDragged;
	  drawable->figure->dragSelection (last, where);
	  drawable->refresh ();
	  last = where;
	  start = where;
	  break;
	case drgeoStateItemDragged:
	  // The drag is going on.
	  drawable->figure->dragSelection (last, where);
	  drawable->refresh ();
	  last = where;
	  break;
	default:
	  // If we get there, then there's a problem with the event
	  // handling code... XXX
	  break;
	}
    }
}

void
drgeoSelectTool::
handleChoice (drgeoDrawableUI * drawable, geometricObject * item)
{
  drgeoPoint xxx;

  if (drawable->figure)
    {
      if (state == drgeoStateItemChoice)
	{
	  state = drgeoStateItemSelected;
	  this->item = item;	// Keep track of the last item
	  // selected for later reference.

	  away = FALSE;
	  spot = last;
	}
      drawable->figure->clearSelection ();
      drawable->figure->addToSelection (xxx, item);
    }
}

void
drgeoSelectTool::finish (drgeoDrawableUI * drawable)
{
  // This code is invoked when the tool is deselected.
}

// -------------------------------------------------
// Implementation for the Build tool.

void
drgeoBuildTool::initialize (drgeoDrawableUI * drawable)
{
  // This code is invoked when the tool selected.
  state = drgeoStateNeutral;
  tipOn = FALSE;
  last_nb_elem = 0;
}

void
drgeoBuildTool::handlePress (drgeoDrawableUI * drawable, drgeoPoint & where)
{
  liste_elem *list;
  int i;

  // Do nothing if no figure is currently associated with this drawable.
  if (drawable->figure != NULL)
    {
      drawable->removeTip ();
      switch (state)
	{
	case drgeoStateItemChoice:
	  // The user has not chosen any item, so revert to neutral
	  // state.
	  state = drgeoStateNeutral;
	  // Fall through!  This is really what is wanted here.
	case drgeoStateNeutral:
	  // The mouse button is being pressed, check if there are items
	  // near the mouse.
	  list = drawable->figure->mouseSelection (where);
	  list->init_lire ();
	  if (list->nb_elem == 0)
	    {
	      // no item was selected.  Just invoke handleChoice with a
	      // NULL parameter that will be transmitted to
	      // figure->addToSelection() for appropriate processing.
	      last = where;
	      handleChoice (drawable, NULL);
	    }
	  else if (list->nb_elem == 1)
	    {
	      // Only one item is selected, add it to the selection.
	      last = where;
	      handleChoice (drawable, (geometricObject *) list->lire (0));
	    }
	  else if (list->nb_elem > 1)
	    {
	      // Several items are selected at once, we must prompt the
	      // user for a choice.  When the user is done with his
	      // choice the handleChoice method will be invoked.
	      state = drgeoStateItemChoice;
	      last = where;
	      drawable->chooseItem (list);
	    }
	  break;
	default:
	  // If we get there, then there's a problem with the event
	  // handling code... XXX
	  break;
	}
    }
}

void
drgeoBuildTool::handleRelease (drgeoDrawableUI * drawable, drgeoPoint & where)
{
  // Do nothing if no figure is currently associated with this drawable.
  if (drawable->figure != NULL)
    {
      liste_elem *list;
      geometricObject *item;

      drawable->removeTip ();
      switch (state)
	{
	case drgeoStateItemChoice:
	  // The user has not chosen any item, so revert to neutral
	  // state.
	  state = drgeoStateNeutral;
	  // Fall through!  This is really what is wanted here.
	case drgeoStateNeutral:
	  // The mouse button is just being released with nothing being
	  // dragged, so we only show a tip displaying the full name of
	  // the nearest item.
	  list = drawable->figure->mouseSelection (where);
	  list->init_lire ();
	  if (list->nb_elem == 1)
	    {
	      item = (geometricObject *) list->lire (0);
	      drawable->showTip (where, item->getTypeName ());
	      tipOn = TRUE;
	    }
	  else if (list->nb_elem > 1)
	    {
	      // There are several items under the mouse.
	      drawable->showTip (where, "???");
	      tipOn = TRUE;
	    }
	  break;
	default:
	  // If we get there, then there's a problem with the event
	  // handling code... XXX
	  break;
	}
    }
}

void
drgeoBuildTool::handleMouseAt (drgeoDrawableUI * drawable, drgeoPoint & where)
{
  int i;

  // Do nothing if no figure is currently associated with this drawable.
  if (drawable->figure != NULL)
    {
      liste_elem *list;
      geometricObject *item;

      switch (state)
	{
	case drgeoStateItemChoice:
	  // The user has not chosen any item, so revert to neutral
	  // state.
	  state = drgeoStateNeutral;
	  // Fall through!  This is really what is wanted here.
	case drgeoStateNeutral:
	  // The mouse is just moving over the figure, so we just query
	  // the figure to know which items are found near the current
	  // mouse position.  And a tooltip is shown displaying the full
	  // name of the nearest item.
	  list = drawable->figure->mouseSelection (where);

	  if (list->nb_elem == 1 &&
	      ((geometricObject *) list->lire (1) != itemUnderMouse))
	    {
	      itemUnderMouse = (geometricObject *) list->lire (1);
	      drawable->showTip (where, itemUnderMouse->getTypeName ());
	      drawable->setCursor (pen);
	    }
	  else if (list->nb_elem > 1 && last_nb_elem <= 1)
	    {
	      // There are several items under the mouse.
	      drawable->showTip (where, "???");
	      drawable->setCursor (pen);
	      itemUnderMouse = NULL;
	    }
	  else if (list->nb_elem == 0 && drawable->tipOn ())
	    {
	      /* mouse not over object anymore, remove the tip */
	      drawable->removeTip ();
	      drawable->setCursor (arrow);
	      itemUnderMouse = NULL;
	    }
	  last_nb_elem = list->nb_elem;
	  break;
	default:
	  // If we get there, then there's a problem with the event
	  // handling code... XXX
	  break;
	}
    }
}

void
drgeoBuildTool::
handleChoice (drgeoDrawableUI * drawable, geometricObject * item)
{
  if (drawable->figure)
    {
      // Add the selected item to the current figure's selection.
      drawable->figure->addToSelection (last, item);
    }
}

void
drgeoBuildTool::finish (drgeoDrawableUI * drawable)
{
  // This code is invoked when the tool is deselected.
}



// -----------------------------------------------------
// Macro Build Tool, used for macro creation.
// -----------------------------------------------------

void
drgeoMacroBuildTool::initialize (drgeoDrawableUI * drawable)
{
  // First invoke the initialize() method of drgeoBuildTool
  // super-class.
  drgeoBuildTool::initialize (drawable);

  // Now let's do our stuff.
  builder = new drgeoMacroBuilder (drawable->getFigure ());

  // Create the macro dialog that will control the builder.
  dialog = drawable->createMacroBuildDialog (builder);
}

// All the other methods are inherited from drgeoBuildTool.  We only
// redefine the handleChoice() method, and of course the initialize()
// and finish().

void
drgeoMacroBuildTool::
handleChoice (drgeoDrawableUI * drawable, geometricObject * item)
{
  // Forward this item to the macro builder.
  if (dialog && item)
    {
      dialog->add (item);
    }
}

void
drgeoMacroBuildTool::finish (drgeoDrawableUI * drawable)
{
  // First invoke the initialize() method of drgeoBuildTool
  // super-class.
  drgeoBuildTool::finish (drawable);

  // Remove the macro dialog that is controlling the builder.
  delete dialog;

  // Free the drgeoMacroBuilder.
  delete builder;
}

// -----------------------------------------------------
// Macro Play Tool, used for macro execution.
// -----------------------------------------------------

void
drgeoMacroPlayTool::initialize (drgeoDrawableUI * drawable)
{
  // First invoke the initialize() method of drgeoBuildTool
  // super-class.
  drgeoBuildTool::initialize (drawable);

  // Now let's do our stuff.
  player = new drgeoMacroPlayer (drawable->getFigure ());

  // Create the macro dialog that will control the Player.
  dialog = drawable->createMacroPlayDialog (player);
}

// All the other methods are inherited from drgeoBuildTool.  We only
// redefine the handleChoice() method, and of course the initialize()
// and finish().

void
drgeoMacroPlayTool::
handleChoice (drgeoDrawableUI * drawable, geometricObject * item)
{
  drgeoPoint xxx;

  // Forward this item to the macro Player.
  if (dialog && item)
    {
      dialog->add (item);
    }
}

void
drgeoMacroPlayTool::finish (drgeoDrawableUI * drawable)
{
  // First invoke the initialize() method of drgeoPlayTool
  // super-class.
  drgeoBuildTool::finish (drawable);

  // Remove the macro dialog that is controlling the Player.
  delete dialog;

  // Free the drgeoMacroPlayer.
  delete player;
}


// -----------------------------------------------------
// Dialog Less Macro Play Tool, used for macro execution.
// From the menu barre
// -----------------------------------------------------

void
drgeoMacroPlayToolDialogLess::initialize (drgeoDrawableUI * drawable)
{
  // First invoke the initialize() method of drgeoBuildTool
  // super-class.
  drgeoBuildTool::initialize (drawable);

  // Now let's do our stuff.
  player = new drgeoMacroPlayer (drawable->getFigure ());

  // Instruct the drawable about the Player.
  drawable->setMacroPlayer (player);
}

// All the other methods are inherited from drgeoBuildTool.  We only
// redefine the handleChoice() method, and of course the initialize()
// and finish().

void 
drgeoMacroPlayToolDialogLess::
handleChoice (drgeoDrawableUI * drawable, geometricObject * item)
{
  // Forward this item to the macro Player.
  if (player && item)
    {
      player->add (item);
    }
}

void
drgeoMacroPlayToolDialogLess::
finish (drgeoDrawableUI * drawable)
{
  // First invoke the initialize() method of drgeoPlayTool
  // super-class.
  drgeoBuildTool::finish (drawable);

  // Free the drgeoMacroPlayer.
  delete player;
}

gboolean drgeoMacroPlayToolDialogLess::
selectMacro (const gchar *name)
{
  return player->setMacro (name);
}

// -----------------------------------------------------
// Dialog Less Animation Tool, used for animation.
// From the menu barre
// -----------------------------------------------------
static int
timer_cb (drgeoAnimationDialogLess * tool)
{
  if (tool->getDrgeoState () == drgeoStateAnimated)
    tool->animate ();
  return (TRUE);		// Make sure the CallBack will be
  // called again and again... 
}

void
drgeoAnimationDialogLess::initialize (drgeoDrawableUI * drawable)
{
  drgeoBuildTool::initialize (drawable);
  step = 0.05;
  timeoutTag = 0;
  itemUnderMouse = NULL;
  this->drawable = drawable;
}

void
drgeoAnimationDialogLess::handlePress (drgeoDrawableUI * drawable, drgeoPoint & where)
{
  liste_elem *list;
  int i;

  // Do nothing if no figure is currently associated with this drawable.
  if (drawable->figure != NULL)
    {
      drawable->removeTip ();
      switch (state)
	{
	case drgeoStateAnimated:
	  // user click while a point is animated,
	  // stop and reset the animation
	  reset ();
	  state = drgeoStateNeutral;
	  break;
	case drgeoStateItemChoice:
	  // The user has not chosen any item, so revert to neutral
	  // state.
	  state = drgeoStateNeutral;
	  // Fall through!  This is really what is wanted here.
	case drgeoStateNeutral:
	  // The mouse button is being pressed, check if there are items
	  // near the mouse.
	  list = drawable->figure->mouseSelection (where);
	  list->init_lire ();
	  if (list->nb_elem == 1)
	    {
	      // Only one item is selected, add it to the selection.
	      state = drgeoStateAnimated;
	      last = where;
	      handleChoice (drawable, (geometricObject *) list->lire (0));
	    }
	  else if (list->nb_elem > 1)
	    {
	      // Several items are selected at once, we must prompt the
	      // user for a choice.  When the user is done with his
	      // choice the handleChoice method will be invoked.
	      state = drgeoStateItemChoice;
	      last = where;
	      drawable->chooseItem (list);
	    }
	  break;
	default:
	  // If we get there, then there's a problem with the event
	  // handling code... XXX
	  break;
	}
    }
}
void 
drgeoAnimationDialogLess::handleChoice (drgeoDrawableUI * drawable,
					geometricObject * item)
{
  this->item = item;
  state = drgeoStateAnimated;
  animatedAbscissa = initialAbscissa = ((point * )item)->getAbscissa ();
  timeoutTag =
    gtk_timeout_add (100, (GtkFunction) timer_cb, (gpointer) this);
}

void
drgeoAnimationDialogLess::finish (drgeoDrawableUI * drawable)
{
  reset ();
}

void
drgeoAnimationDialogLess::animate ()
{
  animatedAbscissa += step;
  if (step > 0 && animatedAbscissa > 1.)
    animatedAbscissa -= 1.;
  if (step < 0 && animatedAbscissa < 0)
    animatedAbscissa += 1.;
  if (item)
    {
      ((point *) item)->setAbscissa (animatedAbscissa);
      drawable->getFigure()->updateItems ();
      drawable->refresh ();
    }
}

void
drgeoAnimationDialogLess::reset ()
{
  if (timeoutTag)
    gtk_timeout_remove (timeoutTag);
  timeoutTag = 0;
  if (item)
    ((point *) item)->setAbscissa (initialAbscissa);
  drawable->getFigure()->updateItems ();
  drawable->refresh ();
  itemUnderMouse = NULL;
}

void
drgeoAnimationDialogLess::setStep (gdouble aStep)
{
  step = aStep;
}

// -----------------------------------------------------
// Delete Tool, used for object deletion.
// -----------------------------------------------------

void
drgeoDeleteTool::
handleChoice (drgeoDrawableUI * drawable, geometricObject * myItem)
{
  char *message;

  // Forward this item to the macro builder.
  if (drawable->figure && myItem != NULL)
    {
      // Hide the removed items just to show the consequence of the
      // removal.
      drawable->figure->hideRemovedItems (myItem);

      // Ask the user if he wants to proceed with the item deletion.
      if (myItem->getName ())
	{
	  if (strlen (myItem->getName ()))
	    message = g_strdup_printf (_("Remove %s?"), myItem->getName ());
	}
      else
	message = g_strdup (_("Remove selected item?"));
      if (drawable->askOkCancel (message))
	{
	  // Remove the item if the user wants it.
	  drawable->figure->removeItem (myItem);
	}
      g_free (message);
      // Show again the list of items to be removed.
      drawable->figure->showRemovedItems ();
      drawable->figure->redraw (FALSE);
    }
}






// -----------------------------------------------------
// Style Editor Tool, used to adjust colors, ...
// -----------------------------------------------------

void
drgeoStyleTool::initialize (drgeoDrawableUI * drawable)
{
  // First invoke the initialize() method of drgeoBuildTool
  // super-class.
  drgeoBuildTool::initialize (drawable);

  dialog = drawable->createStyleDialog ();
}

// All the other methods are inherited from drgeoBuildTool.  We only
// redefine the handleChoice() method, and of course the initialize()
// and finish().

void
drgeoStyleTool::
handleChoice (drgeoDrawableUI * drawable, geometricObject * item)
{
  drgeoPoint xxx;
  // Forward this item to the style dialog.
  if (dialog && item)
    {
      // Update the dialog that match the item newly selected.
      drawable->figure->clearSelection ();
      drawable->figure->addToSelection (xxx, item);
      dialog->edit (item);
    }
}

void
drgeoStyleTool::finish (drgeoDrawableUI * drawable)
{
  // First invoke the initialize() method of drgeoBuildTool
  // super-class.
  drgeoBuildTool::finish (drawable);

  // Remove the macro dialog that is used to adjust style.
  if (dialog)
    {
      delete dialog;
    }
}

// -----------------------------------------------------
// Property Editor Tool, used to adjust properties when
// availables.
// -----------------------------------------------------

void
drgeoPropertyTool::initialize (drgeoDrawableUI * drawable)
{
  // First invoke the initialize() method of drgeoBuildTool
  // super-class.
  drgeoBuildTool::initialize (drawable);

  dialog = drawable->createPropertyDialog ();
}

void
drgeoPropertyTool::
handleChoice (drgeoDrawableUI * drawable, geometricObject * item)
{
  gint nb;
  drgeoPoint xxx;
  if (drawable->figure)
    {
      // Add the selected item to the current figure's selection.
      drawable->figure->clearSelection ();
      nb = drawable->figure->addToSelection (xxx, item);
    }
  // Forward this item to the style dialog.
  if (dialog && item && nb > 0)
    {
      // Update the dialog that match the item newly selected.
      dialog->edit (item);
    }
}

void
drgeoPropertyTool::finish (drgeoDrawableUI * drawable)
{
  // First invoke the initialize() method of drgeoBuildTool
  // super-class.
  drgeoBuildTool::finish (drawable);

  drawable->figure->clearSelection ();
  // Remove the macro dialog that is used to adjust style.
  if (dialog)
    {
      delete dialog;
    }
}
