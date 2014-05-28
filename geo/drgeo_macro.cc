/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-2004
 * hilaire@ext.cri74.org 
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

#include "drgeo_macro.h"
#include "traite.h"
#include "define.h"
#include "drgenius_mdi.h"

extern drgeniusMDI *mdi;


drgeoMacroBuilder::drgeoMacroBuilder (drgeoFigure * figure)
{
  this->name = (char *) NULL;
  this->description = (char *) NULL;
  this->figure = figure;
  figure->clearSelection ();
  setMode (drgeoMacroStartMode);
}

drgeoMacroBuilder::~drgeoMacroBuilder ()
{
  figure->clearSelection ();
}

void
drgeoMacroBuilder::setMode (drgeoMacroMode mode)
{
  this->mode = mode;
  switch (mode)
    {
    case drgeoMacroStartMode:
      figure->clearSelection ();
      break;
    case drgeoMacroInputMode:
      figure->setSelection (&input);
      break;
    case drgeoMacroOutputMode:
      figure->setSelection (&output);
      break;
    case drgeoMacroDescriptionMode:
    case drgeoMacroFinishMode:
      figure->clearSelection ();
    }
}
drgeoMacroMode
drgeoMacroBuilder::getMode ()
{
  return mode;
}

// Macro input parameters management.
void
drgeoMacroBuilder::addInput (geometricObject * item)
{
  liste_elem *selection;

  selection = figure->getSelection ();
  // reselecting a selected item make it unselected
  if (input.position ((void *) item) == 0)
    {
      input.ajoute ((void *) item);
      selection->ajoute ((void *) item);
    }
  else
    {
      // XXX Should remove the item from the clist also
      input.supprime ((void *) item);
      selection->supprime ((void *) item);
    }
}

void
drgeoMacroBuilder::clearInput ()
{
  input.vide ();
}

// Macro output parameters management.
void
drgeoMacroBuilder::addOutput (geometricObject * item)
{
  liste_elem *selection;

  selection = figure->getSelection ();
  // reselecting a selected item make it unselected
  if (output.position ((void *) item) == 0)
    {
      output.ajoute ((void *) item);
      selection->ajoute ((void *) item);
    }
  else
    {
      // XXX Should remove the item from the clist also
      output.supprime ((void *) item);
      selection->supprime ((void *) item);
    }
}
void
drgeoMacroBuilder::clearOutput ()
{
  output.vide ();
}

// Add to the current list (input or output).
void
drgeoMacroBuilder::add (geometricObject * item)
{
  if (mode == drgeoMacroInputMode)
    {
      addInput (item);
    }
  else
    {
      addOutput (item);
    }
}

// Name and description of the macro about to be created.
void
drgeoMacroBuilder::setName (const gchar *name)
{
  if (this->name)
    {
      g_free (this->name);
      this->name = NULL;
    }
  if (name)
    {
      this->name = (char *) g_malloc (strlen (name) + 1);
      strcpy (this->name, name);
    }
  else
    {
      this->name = NULL;
    }
}

gchar *
drgeoMacroBuilder::getName ()
{
  return (name);
}

void
drgeoMacroBuilder::setDescription (gchar *description)
{
  if (this->description)
    {
      g_free (this->description);
      this->description = NULL;
    }
  if (description)
    {
      this->description = description;
    }
  else
    {
      this->description = NULL;
    }
}

gchar *
drgeoMacroBuilder::getDescription ()
{
  return (description);
}

bool
drgeoMacroBuilder::checkParameters ()
{
  liste_elem liste = output, parent, l2;
  geometricObject *objet;
  int nb, ret;
  char possible = TRUE;

  // This method is used to check that everything is OK to proceed to
  // macro creation.  This is done by checking the output list against
  // the input list.

  while (!contenue (liste, input) && possible)
    {
      liste.init_lire ();
      nb = liste.nb_elem;
      while (nb != 0)
	{
	  nb--;
	  objet = (geometricObject *) liste.lire (0);
	  ret = objet->parents (parent);
	  liste.supprime ((void *) objet);
	  if (ret == 0 && input.position ((void *) objet) == 0 &&
	      objet->getCategory () != FREE_PT &&
	      objet->getCategory () != SCRIPT &&
	      !(objet->getCategory () == NUMERIC && 
		objet->getType () == FREE_VALUE))
	    /* || objet->getCategory () == LOCUS) */
	    {
	      // This object has no parent and it is not in the input
	      // list and it is not a free point nor a free value.
	      possible = FALSE;
	    }
	  else
	    {
	      if (input.position ((void *) objet) == 0)
		{
		  // Insert the parents in the list of elements to be
		  // checked, because it is not an input parameter.
		  concatene (l2, parent);
		}
	      else if (l2.position ((void *) objet) == 0)
		{
		  // This is an input parameter without parents.
		  l2.ajoute ((void *) objet);
		}
	    }
	}
      concatene (liste, l2);
      l2.vide ();
    }
  return possible;
}

macro *
drgeoMacroBuilder::createMacro ()
{
  macro *aMacro;
  geometricObject *fig;
  char valid;
  int nb;

  // Creation of the macro.
  aMacro = new macro (getName (), getDescription (),
		      input.nb_elem, output.nb_elem);

  // Insert input parameters first.
  input.init_lire ();
  nb = input.nb_elem;
  while (nb != 0)
    {
      nb--;
      fig = (geometricObject *) input.lire (0);
      aMacro->ajoute ((int *) NULL, fig->getType (), fig->getCategory (),
		      (void *) fig, FALSE, fig->getExtra ());
    }

  // Construct the remaining part of the macro.
  nb = output.nb_elem;
  output.init_lire ();
  valid = TRUE;
  while (nb != 0 && valid == TRUE)
    {
      if (kernel_position (*aMacro, (geometricObject *) output.lire (0))
	  == -1)
	{
	  valid = FALSE;
	}
      nb--;
    }
  if (!valid)
    {
      delete aMacro;		// Delete the incomplete macro.

      return (NULL);
    }
  else
    {
      return (aMacro);
    }
}


int
drgeoMacroBuilder::kernel_position (macro & aMacro, geometricObject * item)
{
  liste_elem parent;
  int *noyau_parent, nb, ret;

  // Check if the item is already registered in the macro.  If it is
  // then there is nothing to do.  This method returns -1 in case of
  // failure.
  nb = aMacro.position ((void *) item);	// The item is not yet registered.

  if (nb == 0)
    {
      // This item is not yet registered so we try to process it.
      // This procedure is recursive as it is applied to the parents
      // of this item first.
      if ((ret = item->parents (parent)) == 0 && 
	  item->getCategory () != FREE_PT &&
	  item->getCategory () != SCRIPT &&
	  !(item->getCategory () == NUMERIC && 
	    item->getType () == FREE_VALUE))
	{
	  // This parameter has no parents or is not one of the FREE
	  // object, and is therefore not constructed on the basis of
	  // input parameters for the macro.
	  return (-1);
	}
      else
	{
	  // Get the list of parents.
	  nb = parent.nb_elem;
	  noyau_parent = new int[nb + 1];
	  noyau_parent[0] = nb;
	  parent.init_lire ();

	  int a, b;
	  for (a = 1; a <= nb; a++)
	    {
	      // For each parent we apply the same processing.
	      b =
		kernel_position (aMacro, (geometricObject *) parent.lire (0));
	      if (b == -1)
		{
		  // Failure during the processing of this parent.
		  delete[]noyau_parent;
		  return b;
		}
	      else
		{
		  noyau_parent[a] = b;
		}
	    }
	}
      // Return the index of the newly added item.
      return aMacro.ajoute (noyau_parent, item->getType (),
			    item->getCategory (), (void *) item,
			    output.position (item) ? TRUE : FALSE,
			    item->getExtra ());	
    }
  else
    {
      // Return the index of the already registered item.
      return nb;
    }
}

drgeoMacroPlayer::drgeoMacroPlayer (drgeoFigure * figure)
{
  this->figure = figure;
  selectedMacro = NULL;
  figure->clearSelection ();
}

drgeoMacroPlayer::~drgeoMacroPlayer ()
{
  figure->clearSelection ();
}

bool
drgeoMacroPlayer::setMacro (const gchar *macroName)
{
  drgeoMacroRegistry *registry;

  figure->clearSelection ();
  if (macroName == NULL)
    {
      selectedMacro = NULL;
      return true;
    }
  registry = drgeoMacroRegistry::get ();
  if ((selectedMacro = registry->find (macroName)) == NULL)
    return FALSE;
  return TRUE;
}

void
drgeoMacroPlayer::setDescription (gchar *description)
{
  if (selectedMacro != NULL)
    {
      selectedMacro->setDescription (description);
      // Update the macro menu item description
      mdi->setMacroMenuItemDescription (selectedMacro->getName (), 
					description);
    }
}

gchar *
drgeoMacroPlayer::getDescription ()
{
  if (selectedMacro != NULL)
    return selectedMacro->getDescription ();
  return NULL;
}

void
drgeoMacroPlayer::add (geometricObject * item)
{
  liste_elem *selection;

  selection = figure->getSelection ();
  selection->ajoute (item);
  if (selectedMacro == NULL)
    return;
  if (selectedMacro->getInputParameterNumber () == selection->nb_elem)
    {
      // we got all the input parameter, we can execute the macro

      // For now macro-construction are not handled by the
      // undo/redo mecanism: we have to empty the queue to
      // avoid crash problem
      figure->emptyUndoQueueAll ();
      selectedMacro->play (figure);
      figure->clearSelection ();
      figure->redraw (false);
    }
}

bool
drgeoMacroPlayer::
itemsUnderMouseForMacro (drgeoPoint & mouse, liste_elem & underMouse)
{
  objectCategory category;
  int nb, a;
  liste_elem *selection, *figureList;
  char trouve = FALSE;
  geometricObject *fig;
  drgeoDrawable *area;

  // we ask to the macro what kind of type it expects
  // ie we read in the macro kernel list
  if (selectedMacro == NULL)
    return false;

  selection = figure->getSelection ();
  figureList = figure->getFigureList ();

  category = selectedMacro->lire (selection->nb_elem + 1)->category;
  /* If this is a point force the category to the genaral POINT category */
  if (category & POINT)
    category = POINT;
  nb = figureList->nb_elem;
  figureList->init_lire ();
  underMouse.vide ();
  area = figure->getDrawable ();

  for (a = 1; a <= nb; a++)
    {
      fig = (geometricObject *) figureList->lire (0);
      if (fig->overObject (mouse, area->getRange ())
	  && fig->objectExist ()
	  && fig->getMask () != alway
	  && (fig->getCategory () & category)
	  && selection->position ((void *) fig) == 0)
	{
	  underMouse.ajoute ((void *) fig);
	  trouve = true;
	}
    }
  return trouve;
}
