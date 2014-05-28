/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-2003
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

#include "drgeo_figure.h"
#include <stdio.h>
#include <string.h>

#include "drgeo_gtkdrawable.h"
#include "drgeo_command.h"
#include "drgeo_flydrawable.h"
#include "drgeo_latexdrawable.h"
#include "drgeo_postscriptdrawable.h"
#include "xml.h"
#include "drgeo_point.h"
#include "drgeo_vector.h"
#include "drgeo_line.h"
#include "drgeo_halfLine.h"
#include "drgeo_segment.h"
#include "drgeo_circle.h"
#include "drgeo_arcCircle.h"
#include "drgeo_angle.h"
#include "drgeo_numeric.h"
#include "drgeo_equation.h"
#include "drgeo_locus.h"
#include "drgeo_polygon.h"
#include "drgeo_script.h"
#include "drgeo_buildObject.h"
#include "drgeo_dialog.h"
#include "traite.h"

extern DialogData drgeoDialogData;

typedef struct drgeoSearchValue
{
  gpointer value;
  gpointer key;
}
drgeoSearchValue;

static void _searchValue (gpointer key, gpointer value, gpointer data);
static void _freeKeyFromHash (gpointer key, gpointer value, gpointer data);

drgeoFigure::drgeoFigure (xmlNodePtr figureTree)
{
  xmlNodePtr item;
  gchar *id;
  gint myId;
  GHashTable *itemIdToAddress;
  geometricObject *fig;

  itemFilter = NULL;
  mode = SOURIS_SELECT_MODE;
  saved = TRUE;
  password = NULL;
  showGrid = false;

  for (myId = 0; myId < DRGEO_TOOLS_NUMBER; myId++)
    showTool[myId] = TRUE;

  // set a scm interface
  scmInterface = new ScmInterface (this);

  // undo/redo initialization stuff
  undoHash = g_hash_table_new (g_str_hash, g_str_equal);
  undoLevel = NULL;
  undoLevel = g_list_append (undoLevel, NULL);

  if (figureTree == NULL)
    {
      /* create a new empty figure */
      blinkOn = TRUE;
      return;
    }

  /* remove blinking as we going to re-build the items from the XML tree */
  blinkOn = FALSE;

  if (strcmp ((char *) figureTree->name, "drgeo"))
    {
      fprintf (stderr,
	       "drgeoFigure::drgeoFigure : invalid element type %s, 'drgeo' expected\n",
	       figureTree->name);
    }

  /* Let's see if the grid is attached to the figure */
  id = (gchar *) xmlGetProp (figureTree, BAD_CAST "grid");
  if (id != NULL)
    {
      if (!strcmp (id, "True"))
	showGrid = true;
      else
	showGrid = false;
      g_free (id);
    }
  
  if ((item = figureTree->xmlChildrenNode) == NULL)
    {
      // empty figure
      blinkOn = TRUE;
      return;
    }


  // hash table to get item Address from their Id
  itemIdToAddress = g_hash_table_new (g_str_hash, g_str_equal);

  while (item != NULL)
    {
      // create the item
      fig = xmlBuildItem (item, itemIdToAddress);
      addToFigureListNoRedraw (fig, TRUE);
      if (fig != NULL)
	{
	  // push the object on the hash table
	  if ((id = xml_value_get (item, "id")) == NULL)
	    fprintf (stderr,
		     "drgeoFigure::drgeoFigure : element without id\n");
	  else
	    g_hash_table_insert (itemIdToAddress, id, (void *) fig);
	}
      // get the next item
      item = item->next;
    }
  g_hash_table_foreach (itemIdToAddress, _freeKeyFromHash, NULL);
  g_hash_table_destroy (itemIdToAddress);
  blinkOn = TRUE;
}

drgeoFigure::~drgeoFigure ()
{
  liste_elem *fig;
  geometricObject *item;
  // Clear the undo pipe
  undoLevel = g_list_first (undoLevel);
  // The first node is empty, skip it
  undoLevel = g_list_next (undoLevel);
  while (undoLevel != NULL)
    {
      delete ((drgeoCommand *) (undoLevel->data));
      undoLevel = g_list_next (undoLevel);
      printf ("Free one undo item\n");
    }
  g_list_free (undoLevel);
  g_hash_table_foreach (undoHash, _freeKeyFromHash, NULL);
  g_hash_table_destroy (undoHash);
  /* free the figure */
  fig = getFigureList ();
  fig->init_lire ();
  while ((item = (geometricObject *) fig->lire (0)) != NULL)
    delete item;
}


gboolean drgeoFigure::saveAs (xmlNodePtr drgeoXml, const gchar * name)
{
  gint nb, a, c;
  xmlNodePtr tree, tree1, tree2;
  drgeoPoint center, p1, p2; 
  
  tree = xmlNewChild (drgeoXml, NULL, BAD_CAST "drgeo", NULL);
  xmlSetProp (tree, BAD_CAST "name", BAD_CAST name);

  // Save the scale and coordinate of the origin
  xml_set_double_prop (tree, "scale", area->getScale ());
  center = area->getAreaCenter ();
  xml_set_double_prop (tree, "origin_x", center.getX ());
  xml_set_double_prop (tree, "origin_y", center.getY ());
  if (showGrid)
    xmlSetProp (tree, BAD_CAST "grid", BAD_CAST "True");
  else
    xmlSetProp (tree, BAD_CAST "grid", BAD_CAST "False");

  /* eventually save the printing area if defined */
  if (((drgeoGtkDrawable *) area)->printingArea ())
    {
      p1 = ((drgeoGtkDrawable *) area)->getPrintingAreaBR ();
      p2 = ((drgeoGtkDrawable *) area)->getPrintingAreaTL ();

      tree1 = xmlNewChild (tree, NULL, BAD_CAST "boundingBox", NULL);
      xml_set_double_prop (tree1, "tl_x", p2.getX ());
      xml_set_double_prop (tree1, "tl_y", p2.getY ());
      xml_set_double_prop (tree1, "br_x", p1.getX ());
      xml_set_double_prop (tree1, "br_y", p1.getY ());
    }

  /* eventually save the customized ui */
  if (getPassword ())
    {
      tree1 = xmlNewChild (tree, NULL, BAD_CAST "customUI", NULL);
      xmlSetProp (tree1, BAD_CAST "password", BAD_CAST getPassword ());
      for (a = 0; a < DRGEO_TOOLS_NUMBER; a++)
	{
	  if (getToolState ((drgeoToolId) a))
	    tree2 =
	      xmlNewChild (tree1, NULL, BAD_CAST "tool", BAD_CAST "TRUE");
	  else
	    tree2 =
	      xmlNewChild (tree1, NULL, BAD_CAST "tool", BAD_CAST "FALSE");
	  xml_set_int_prop (tree2, "id", a);
	}
    }
  // save the data of each figure item object
  nb = figureList.nb_elem;
  figureList.init_lire ();
  for (a = 1; a <= nb; a++)
    {
      ((geometricObject *) figureList.lire (0))->save (tree, figureList);
    }
  saved = TRUE;
  return saved;
}

gboolean drgeoFigure::isSaved ()
{
  return saved;
}

gboolean drgeoFigure::exportToFlydraw (const gchar * fileName)
{
  FILE * fileHandle;
  drgeoDrawable * fly;
  drgeoGtkDrawable *gtkArea; 
  int a, nb;
  geometricObject * fig;
  drgeoPoint center, size, br, tl;

  gtkArea = (drgeoGtkDrawable *) area;
  if (gtkArea->printingArea ())
    {
      br = gtkArea->getPrintingAreaBR ();
      tl = gtkArea->getPrintingAreaTL ();
      center = (br + tl) / 2;
      size.setX (br.getX () - tl.getX ());
      size.setY (tl.getY () - br.getY ());
    }
  else
    {
      center = area->getAreaCenter ();
      size = area->getAreaSize ();
    }


  if (!(fileHandle = fopen (fileName, "w")))
    return FALSE;

  fly = new drgeoFlyDrawable (this, fileHandle, center, size, 25.0);

  /* eventually print the grid */
  if (showGrid)
    fly->showGrid ();
     
  /* scan the figure and get the output */
  nb = figureList.nb_elem;
  figureList.init_lire ();
  for (a = 1; a <= nb; a++)
    {
      fig = (geometricObject *) figureList.lire (0);
      if (doNotDraw.position ((void *) fig) == 0)
	fig->draw (*fly, FALSE);
    }
  fprintf (fileHandle, "output image.png\n");
  fclose (fileHandle);
  delete (fly);

  return TRUE;
}

gboolean drgeoFigure::exportToLatex (const gchar * fileName)
{
  FILE * fileHandle;
  drgeoDrawable * latex;
  drgeoGtkDrawable *gtkArea; 
  int a, nb;
  geometricObject * fig;
  drgeoPoint center, size, br, tl;

  gtkArea = (drgeoGtkDrawable *) area;
  if (gtkArea->printingArea ())
    {
      br = gtkArea->getPrintingAreaBR ();
      tl = gtkArea->getPrintingAreaTL ();
      center = (br + tl) / 2;
      size.setX (br.getX () - tl.getX ());
      size.setY (tl.getY () - br.getY ());
    }
  else
    {
      center = area->getAreaCenter ();
      size = area->getAreaSize ();
    }


  if (!(fileHandle = fopen (fileName, "w")))
    return FALSE;

  fprintf (fileHandle,
	   "\\documentclass[a4paper]{minimal}\n\\usepackage{pstricks}\n\\begin{document}\n");
  latex =
    new drgeoLatexDrawable (this, fileHandle, center,
			    size, 1.0);

  /* eventually print the grid */
  if (showGrid)
    latex->showGrid ();
     
  /* scan the figure and get the output */
  nb = figureList.nb_elem;
  figureList.init_lire ();
  for (a = 1; a <= nb; a++)
    {
      fig = (geometricObject *) figureList.lire (0);
      if (doNotDraw.position ((void *) fig) == 0)
	fig->draw (*latex, FALSE);
    }
  fprintf (fileHandle, "\\end{pspicture*}\n\\end{document}\n");
  fclose (fileHandle);
  delete (latex);

  return TRUE;
}

gboolean drgeoFigure::exportToPostScript (const gchar * fileName)
{
  FILE * fileHandle;
  gboolean ret;

  if (!(fileHandle = fopen (fileName, "w")))
    return FALSE;

  ret = exportToPostScriptHandle (fileHandle, g_basename (fileName));

  fclose (fileHandle);
  return ret;
}

gboolean 
drgeoFigure::exportToPostScriptHandle (FILE *fileHandle, const gchar * name)
{
  drgeoDrawable * postscript;
  drgeoGtkDrawable *gtkArea; 
  int a, nb;
  geometricObject * fig;
  drgeoPoint center, size, br, tl;

  gtkArea = (drgeoGtkDrawable *) area;

  if (gtkArea->printingArea ())
    {
      br = gtkArea->getPrintingAreaBR ();
      tl = gtkArea->getPrintingAreaTL ();
      center = (br + tl) / 2;
      size.setX (br.getX () - tl.getX ());
      size.setY (tl.getY () - br.getY ());
    }
  else
    {
      center = area->getAreaCenter ();
      size = area->getAreaSize ();
    }

  postscript =
    new drgeoPostScriptDrawable (name, this, fileHandle,
				 center, size, 1.0);

  /* eventually print the grid */
  if (showGrid)
    postscript->showGrid ();

  /* scan the figure and get the output */

  //first draw the polygon
  nb = figureList.nb_elem;
  figureList.init_lire ();
  for (a = 1; a <= nb; a++)
    {
      fig = (geometricObject *) figureList.lire (0);
      if ((doNotDraw.position ((void *) fig) == 0) &&
	  fig->getCategory () == POLYGON)
	fig->draw (*postscript, FALSE);
    }


  // then the other object
  nb = figureList.nb_elem;
  figureList.init_lire ();
  for (a = 1; a <= nb; a++)
    {
      fig = (geometricObject *) figureList.lire (0);
      if ((doNotDraw.position ((void *) fig) == 0) &&
	  fig->getCategory () != POLYGON)
	fig->draw (*postscript, FALSE);
    }
  delete (postscript);

  return TRUE;
}

void
drgeoFigure::clear ()
{
  figureList.vide ();
  freeFigureList.vide ();
}

void
drgeoFigure::updateItems ()
{
  int a, nb;
  geometricObject *fig;

  nb = figureList.nb_elem;
  figureList.init_lire ();

  for (a = 1; a <= nb; a++)
    {
      fig = (geometricObject *) figureList.lire (0);
      fig->update (*area);
    }
}


void
drgeoFigure::redraw (gchar all)
{
  // The all flag indicate if the hidden objects should be redrawn:
  //
  //  * the hidden attribute of an object can be toggle from the style
  //    box for normal redraw this flag should be FALSE
  //
  //  * it will be TRUE when the user select the style box, so he can
  //    see all objects, and therefore make invisible items visible
  //    again. 
  //
  // There are other objects, used for macro-constructions, which are
  // never displayed, only computed.
  //
  // Additionally objects in the doNotDraw list are temporarily not
  // displayed.

  int a, nb;
  geometricObject *fig;

  // Update the item, we need to do that because of the locus
  // object
  updateItems ();


  // We just draw all the items in the figure, following the rules
  // listed above.

  nb = figureList.nb_elem;
  figureList.init_lire ();

  for (a = 1; a <= nb; a++)
    {
      fig = (geometricObject *) figureList.lire (0);
      if (doNotDraw.position ((void *) fig) == 0)
	fig->draw (*area, all);
    }
}

void
drgeoFigure::undo (gint level)
{
  GList *prev;
  prev = g_list_previous (undoLevel);
  if (prev)
    {
      /* clear all the list containing item object, 
         if not this bring hazardous effect */
      selection.vide ();
      underMouse.vide ();
      highlightItem.vide ();
      if (itemFilter)
	itemFilter->reset ();
      doNotDraw.vide ();
      ((drgeoCommand *) undoLevel->data)->unexecute ();
      undoLevel = prev;
    }
  area->refresh ();
  area->updateUndoState ();
}

void
drgeoFigure::redo (gint level)
{
  GList *next;
  next = g_list_next (undoLevel);
  if (next)
    {
      /* clear all the list containing item object, 
         if not this bring hazardous effect */
      selection.vide ();
      underMouse.vide ();
      highlightItem.vide ();
      doNotDraw.vide ();
      if (itemFilter)
	itemFilter->reset ();
      ((drgeoCommand *) next->data)->execute ();
      undoLevel = next;
    }
  area->refresh ();
  area->updateUndoState ();
}

void
drgeoFigure::emptyUndoQueueEnd ()
{
  GList *rmList, *tmpList;
  drgeoCommand *rmCommand;

  rmList = NULL;
  tmpList = undoLevel;
  while (tmpList = g_list_next (tmpList))
    rmList = g_list_append (rmList, tmpList->data);

  rmList = g_list_first (rmList);
  undoLevel = g_list_first (undoLevel);
  while (rmList)
    {
      rmCommand = (drgeoCommand *) rmList->data;
      delete (rmCommand);
      undoLevel = g_list_remove (undoLevel, (gpointer) rmCommand);
      rmList = g_list_next (rmList);
    }
  g_list_free (rmList);
}

void
drgeoFigure::emptyUndoQueueAll ()
{
  GList *rmList;
  drgeoCommand *rmCommand;

  rmList = g_list_first (undoLevel);
  while (rmList)
    {
      rmCommand = (drgeoCommand *) rmList->data;
      delete (rmCommand);
      rmList = g_list_next (rmList);
    }

  g_list_free (undoLevel);
  undoLevel = NULL;
  undoLevel = g_list_append (undoLevel, NULL);
}

void
drgeoFigure::addToUndoQueue (drgeoCommand * command)
{
  drgeoCommand *rmCommand;

  if (g_list_length (undoLevel) > preferedUndoLevel ())
    {
      /* Watch out, for internal managment the undoLevel list
         contains a first empty element, we skip it */
      rmCommand = (drgeoCommand *) ((g_list_nth (undoLevel, 1))->data);
      /* we shoud permanently update this object reference */
      delete (rmCommand);
      undoLevel = g_list_remove (undoLevel, (gpointer) rmCommand);
    }

  undoLevel = g_list_append (undoLevel, (gpointer) command);
  undoLevel = g_list_last (undoLevel);
}

gboolean
drgeoFigure::undoActive ()
{
  return (gboolean) (undoLevel->data != 0);
}

gboolean
drgeoFigure::redoActive ()
{
  return (gboolean) (undoLevel->next != 0);
}

void
drgeoFigure::setMode (gint toMode,
		      buildParametersListForGeometricObject * itemBuilder)
{
  mode = toMode;
  clearSelection ();
  if (mode >= POINT_MODE && mode <= SCRIPT_MODE)
    {
      itemFilter = itemBuilder;
      itemFilter->reset ();
    }
}

liste_elem *
drgeoFigure::getSelection ()
{
  return &selection;
}

void
drgeoFigure::clearSelection ()
{
  selection.vide ();
}

void
drgeoFigure::setSelection (liste_elem * toSelection)
{
  selection.vide ();
  concatene (selection, *toSelection);
}

void
drgeoFigure::removeItemFromSelection (void *item)
{
  selection.supprime (item);
}

void
drgeoFigure::addToFigureList (geometricObject * item, char free)
{
  figureList.ajoute ((void *) item);
  if (free)
    freeFigureList.ajoute ((void *) item);
  item->draw (*area, FALSE);
}

void
drgeoFigure::addToFigureListNoRedraw (geometricObject * item, char free)
{
  if (item == NULL)
    return;
  figureList.ajoute ((void *) item);
  if (free)
    freeFigureList.ajoute ((void *) item);
}


gint drgeoFigure::addToSelection (drgeoPoint & mouse, geometricObject * item)
{
  // convention: if item == NULL this mean the user has clicked on the
  // background I need this to create new point on the background or
  // value the user can edit. See below
  gint
    cas;
  GList *
    builded;

  if (mode >= POINT_MODE && mode <= SCRIPT_MODE && item != NULL)
    {
      if (itemFilter
	  && (cas = itemFilter->inserer_figure (selection, item)) != -1)
	{
	  // The builder fonctions.
	  GList *(*builderFonction[]) (int, drgeoPoint &, drgeoFigure *) =
	  {
	  buildFreePoint, buildMiddlePoint,
	      buildIntersectionPoint, buildCoordinatePoint,
	      buildLine, buildHalfLine, buildSegment,
	      buildVector, buildCircle, buildArcCircle,
	      buildLocus, buildParallelLine,
	      buildOrthogonalLine, buildReflexion,
	      buildSymmetry, buildTranslation, buildRotation,
	      buildScale, buildNumeric, buildAngle,
	      buildCoordinateAndEquation, buildPolygon, buildScript};

	  // Enough item are selected, we can build the
	  // item corresponding to the mode we need the
	  // mouse coordinate there...

	  builded = builderFonction[mode] (cas, mouse, this);
	  /* process the list of builded in the undo
	     pipe */
	  builded = g_list_first (builded);
	  while (builded)
	    {
	      createItem ((geometricObject *) builded->data);
	      builded = g_list_next (builded);
	    }
	  clearSelection ();
	  itemFilter->reset ();
	  redraw (FALSE);
	}
    }
  else if (item == NULL && mode == POINT_MODE)
    {
      // build a free point on the plane
      geometricObject *
	new_fig;
      new_fig = new point (mouse, selection, FREE, FALSE, &figureList);
      createItem (new_fig);
      clearSelection ();
      itemFilter->reset ();
      redraw (FALSE);
    }
  else if (item == NULL && mode == NUMERIQUE_MODE)
    {
      // buid a free value, the user need to enter a value
      drgeoDialogData.mouse = mouse;
      drgeoDialogData.figure = this;
      get_edited_value ();
    }
  else if (item == NULL && mode == SCRIPT_MODE)
    {
      // build a script without input value
      geometricObject *
	new_fig;
      gchar *
	scriptCode;

      scriptCode = g_strdup ("'\"Dr. Genius\"");
      new_fig = new script (mouse, selection, SCRIPT_NITEMS, scriptCode,
			    FALSE, &figureList);
      g_free (scriptCode);
      createItem (new_fig);
      clearSelection ();
      itemFilter->reset ();
      redraw (FALSE);
    }
  else if (item != NULL)
    {
      switch (mode)
	{
	case MACRO_ENREGISTRE_MODE:
	  // FIXME Everything is handled by the drgeoMacroBuildTool.
	  break;
	case MACRO_PLAY_MODE:
	  // FIXME Everything is handled by the drgeoMacroPlayTool.
	  break;
	case SOURIS_SELECT_MODE:
	  selection.ajoute ((void *) item);
	  break;
	case MISE_EN_FORME_MODE:
	  // we want the selected item to blink
	  selection.ajoute ((void *) item);
	  break;
	case PROPERTY_MODE:
	  // we want the item to blink, we add it to the selection list
	  {
	    objectCategory
	      category;
	    int
	      type;

	    category = item->getCategory ();
	    type = item->getType ();
	    if (category == FREE_PT ||
		(category == NUMERIC && type == FREE_VALUE) ||
		(category == SCRIPT))
	      selection.ajoute ((void *) item);
	    break;
	  }
	case EFFACE_OBJET_MODE:
	  // FIXME: delete me, this now handle from
	  // drgeoDeleteTool::HandleChoice with the
	  // method removeItem to take care of undo
	  // system

	  break;
	}
    }
  return selection.nb_elem;
}

void
drgeoFigure::showRemovedItems ()
{
  doNotDraw.vide ();
}

void
drgeoFigure::hideRemovedItems (geometricObject * item)
{
  geometricObject *fig;
  liste_elem *children;

  doNotDraw.vide ();
  doNotDraw.ajoute ((void *) item);
  doNotDraw.init_lire ();
  // we research all the object who depend on the object 
  // 'item' we must delete these one also
  children = searchChildren (item, figureList);
  concatene (doNotDraw, *children);
  delete (children);
 
 // let display the figure without those items
  (this->getDrawable ())->refresh ();
}

char
drgeoFigure::doesItemExist (geometricObject * fig)
{
  liste_elem parent_orig, parent;
  int nb_figure;
  geometricObject *obj;

  nb_figure = figureList.nb_elem;
  figureList.init_lire ();
  fig->parents (parent_orig);

  while (nb_figure--)
    {
      obj = (geometricObject *) figureList.lire (0);
      if (obj->getType () == fig->getType ()
	  && obj->getMask () != alway
	  && obj->getCategory () == fig->getCategory ())
	{
	  obj->parents (parent);
	  if (liste_elem_egale (parent_orig, parent))
	    return TRUE;
	}
    }
  return FALSE;
}


liste_elem *
drgeoFigure::mouseSelection (drgeoPoint & mouse)
{
  gchar trouve;
  gint nb, a;
  geometricObject *fig;

  // Return the list of items currently located near the mouse (near
  // means here that their distance to the mouse is < range).  Only
  // items relevant to the current mode are listed.

  if (mode >= POINT_MODE && mode <= SCRIPT_MODE)
    {
      nb = figureList.nb_elem;
      figureList.init_lire ();

      underMouse.vide ();

      for (a = 1; a <= nb; a++)
	{
	  fig = (geometricObject *) (figureList.lire (0));
	  if (fig->overObject (mouse, area->getRange ())
	      && fig->objectExist ()
	      && (fig->getMask () == no
		  || (fig->getMask () == yes && mode == MISE_EN_FORME_MODE)))
	    if (itemFilter && itemFilter->utilisable1 (fig, selection))
	      underMouse.ajoute ((void *) fig);
	}
    }
  else
    {
      switch (mode)
	{
	case MACRO_ENREGISTRE_MODE:
	  // All the items currently under the mouse are interesting for
	  // the macro creation.
	  trouve = itemsUnderMouse (mouse, FALSE);
	  break;
	case MACRO_PLAY_MODE:
	  // play a macro
	  // We should check for relevant item according to the selected macro
	  // Go down asking to the macro player, referenced in the drawable
	  trouve = ((drgeoDrawableUI *) area)->player->itemsUnderMouseForMacro
	    (mouse, underMouse);
	  break;
	case SOURIS_SELECT_MODE:
	  // Drag mode
	  trouve = itemsUnderMouse (mouse, FALSE);

	  break;
	case MISE_EN_FORME_MODE:
	  // Style mode
	  trouve = itemsUnderMouse (mouse, TRUE);
	  break;
	case PROPERTY_MODE:
	  // Property mode
	  objectCategory category;
	  int type;

	  trouve = itemsUnderMouse (mouse, FALSE);
	  if (trouve)
	    {
	      underMouse.init_lire ();
	      nb = underMouse.nb_elem;
	      do
		{
		  fig = (geometricObject *) underMouse.lire (0);
		  category = fig->getCategory ();
		  type = fig->getType ();
		  if (category != FREE_PT &&
		      (category != NUMERIC || type != FREE_VALUE) &&
		      (category != SCRIPT))
		    underMouse.supprime ((void *) fig);
		}
	      while (--nb != 0);
	    }
	  break;
	case ANIMATION_MODE:
	  trouve = itemsUnderMouse (mouse, FALSE);
	  if (trouve)
	    filterItems (underMouse, FREE_PT_ON_CURVE);
	  break;
	case EFFACE_OBJET_MODE:
	  // Remove mode
	  trouve = itemsUnderMouse (mouse, FALSE);
	  break;
	}
    }
  return &underMouse;
}

void
drgeoFigure::filterItems (liste_elem &items, objectCategory categories)
{
  gint nb;
  geometricObject *fig;
  objectCategory category;

  items.init_lire ();
  nb = items.nb_elem;
  do
    {
      fig = (geometricObject *) items.lire (0);
      if ( (fig->getCategory() & categories) == 0)
	underMouse.supprime ((void *) fig);
    }
  while (--nb != 0);

}

void
drgeoFigure::dragSelection (drgeoPoint & start, drgeoPoint & mouse)
{
  geometricObject *fig;

  // Only move the first item in the selection list otherwise result
  // may be unpredictable.
  selection.init_lire ();
  fig = (geometricObject *) selection.lire (0);
  if (fig)
    {
      start = mouse - start;
      fig->move (start);
      this->updateItems ();
    }
}

void
drgeoFigure::dropSelection (drgeoPoint & start, drgeoPoint & mouse)
{
  geometricObject *fig;

  // Only move the first item in the selection list otherwise result
  // may be unpredictable.  Same thing as dragSelection currently.
  selection.init_lire ();
  fig = (geometricObject *) selection.lire (0);
  if (fig)
    {
      start = mouse - start;
      fig->move (start);
      this->updateItems ();
    }
}


void
drgeoFigure::addUndoHash (gpointer key, gpointer value)
{
  /* add key/value in the table, check if the pair already
     exist before */
  gchar *skey;

  skey = g_strdup_printf ("%X", GPOINTER_TO_UINT (key));
  /* is the key already used? */
  if (g_hash_table_lookup (undoHash, (gconstpointer) skey) == NULL)
    {
      g_hash_table_insert (undoHash, (gpointer) skey, value);
    }
  else
    g_free (skey);
}

void
drgeoFigure::updateUndoHash (gpointer key, gpointer value)
{
  /* The key need to be converted as a string. This is expected
     by the item buidler. We have to take care in creating and
     freeing memory for the key string eventually */
  gchar *skey;
  gpointer origKey, origValue;

  skey = g_strdup_printf ("%X", GPOINTER_TO_UINT (key));
  /* is the key already used? */
  if (g_hash_table_lookup_extended
      (undoHash, (gconstpointer) skey, &origKey, &origValue))
    {
      /* skey exist, update the value if needed */
      if (origValue != value)
	g_hash_table_insert (undoHash, (gpointer) origKey, value);
    }
  g_free (skey);
}

gpointer
drgeoFigure::getValueFromKeyOnHash (gpointer key)
{
  gchar *skey;
  gpointer val;

  skey = g_strdup_printf ("%X", GPOINTER_TO_UINT (key));
  /* is the key already used? */
  val = g_hash_table_lookup (undoHash, (gconstpointer) skey);
  g_free (skey);
  return val;
}

gpointer
drgeoFigure::getKeyFromValueOnHash (gpointer value)
{
  drgeoSearchValue data;
  data.value = value;
  data.key = NULL;
  g_hash_table_foreach (undoHash, _searchValue, (gpointer) & data);
  if (data.key == NULL)
    return NULL;
  else
    // data.key is a * gchar get it content as a gpointer
    return GINT_TO_POINTER (strtol ((gchar *) data.key, NULL, 16));
}

void
drgeoFigure::removeKeyFromHash (gpointer key)
{
  gchar *skey;
  gpointer origKey, origValue;

  skey = g_strdup_printf ("%X", GPOINTER_TO_UINT (key));
  /* is the key already used? */
  if (g_hash_table_lookup_extended
      (undoHash, (gconstpointer) skey, &origKey, &origValue))
    {
      g_hash_table_remove (undoHash, origKey);
      g_free (origKey);
    }
  else
    g_printerr ("Warning: key %X can't be found in hash table\n", key);
  g_free (skey);
}


GHashTable *
drgeoFigure::getUndoHash ()
{
  return undoHash;
}

liste_elem *
drgeoFigure::dependantItems (geometricObject * item)
{
  /* search for both upward and backward dependences of item
     Backward dependence only concerns macro-node only used by
     the item */
  liste_elem *dependence, *lostNode, *children;
  
  dependence = new liste_elem ();
  dependence->ajoute ((void *) item);

  /* We research all the objects who depend on the object item.  We
     have to provide them in chronology order */

  children = searchChildren (item, figureList);
  concatene (*dependence , *children);
  delete (children);

  children = new liste_elem (figureList);
  substract (*children, doNotDraw);

  lostNode = searchLostNode (*children);
  delete (children);

  concatene (*lostNode, *dependence);
  delete (dependence);

  return lostNode;
}

liste_elem *
drgeoFigure::searchChildren (geometricObject * item, liste_elem & myList)
{
  liste_elem *children;
  geometricObject *fig;
  guint pos, nbFigure;

  children = new liste_elem ();
  
  nbFigure = myList.nb_elem;
  for (pos = myList.position ((void *) item) + 1 ; pos <= nbFigure; pos++)
    {
      fig = (geometricObject *) myList.lire (pos);
      if (fig->dependOn (item))
	children->ajoute ((void *) fig);      
    }
  return children;
}

liste_elem *
drgeoFigure::searchLostNode (liste_elem &myList)
{
  guint pos, nbFigure;
  geometricObject *fig, *child;
  liste_elem *children, *lostNode;
  gboolean lost;

  myList.init_lire ();
  lostNode = new liste_elem ();

  while (fig = (geometricObject *) myList.lire (0))
    {
      if (fig->getMask () != alway)
	continue;
      children = searchChildren (fig, myList);
      children->init_lire ();
      lost = true;
      // if there is only macro nodes, we got a lost one.
      while ((child = (geometricObject *) children->lire (0)) && lost)
	{
	  if (child->getMask () != alway)
	      lost = false;
	}
      delete (children);
      if (lost)
	lostNode->ajoute (fig);
    }
  return lostNode;
}

void
drgeoFigure::removeItem (geometricObject * item)
{
  drgeoCommand *command;

  emptyUndoQueueEnd ();
  command = new drgeoRemoveItemCommand (this, item);
  command->execute ();
  addToUndoQueue (command);
  area->updateUndoState ();
  area->refresh ();
}

void
drgeoFigure::createItem (geometricObject * item)
{
  drgeoCommand *command;

  emptyUndoQueueEnd ();

  command = new drgeoCreateItemCommand (this, item);
  command->execute ();
  addToUndoQueue (command);
  area->updateUndoState ();
  area->refresh ();
}

void
drgeoFigure::moveItem (geometricObject * item, drgeoVector & t)
{
  // Build the command corresponding to a move
  // Do not move the item actually
  drgeoCommand *command;

  emptyUndoQueueEnd ();
  if (item == NULL)
    item = (geometricObject *) selection.lire (1);
  /* use the selection list as items to move */
  command = new drgeoMoveItemCommand (this, item, t);
  addToUndoQueue (command);
  area->updateUndoState ();
  area->refresh ();
}

void
drgeoFigure::
setItemAttribute (geometricObject * item, drgeoAttributeType attr,
		  gpointer value)
{
  drgeoCommand *command;

  emptyUndoQueueEnd ();
  command = new drgeoSetItemAttributeCommand (this, item, attr, value);
  command->execute ();
  addToUndoQueue (command);
  area->updateUndoState ();
  area->refresh ();
}

void
drgeoFigure::setPassword (gchar * newPassword)
{
  if (password)
    g_free (password);
  if (newPassword)
    password = g_strdup (newPassword);
  else
    password = NULL;
}


gchar *
drgeoFigure::getPassword ()
{
  return password;
}

void
drgeoFigure::setToolState (drgeoToolId id, gboolean visible)
{
  showTool[id] = visible;
}

gboolean
drgeoFigure::getToolState (drgeoToolId id)
{
  return showTool[id];
}

gboolean
drgeoFigure::getShowGridState ()
{
  return showGrid;
}

void
drgeoFigure::setShowGridState (gboolean state)
{
  showGrid = state;
}

ScmInterface *
drgeoFigure::getScmInterface ()
{
  return scmInterface;
}

char
drgeoFigure::itemsUnderMouse (drgeoPoint & mouse, int toute)
{
  char trouve = FALSE, point = FALSE;
  int a, nb;
  geometricObject *fig;

  // This method does compute the list of all figure items under the
  // mouse.  Its trying to be smart when there is a point under the
  // mouse and lines.  In this case it's only returning the point as
  // being under the mouse.  This makes it easier to select points
  // with the mouse.

  nb = figureList.nb_elem;
  figureList.init_lire ();
  underMouse.vide ();

  for (a = 1; a <= nb; a++)
    {
      fig = (geometricObject *) figureList.lire (0);
      if (fig->overObject (mouse, area->getRange ())
	  && (fig->getMask () == no
	      || (fig->getMask () == yes && toute == TRUE)))
	{
	  underMouse.ajoute ((void *) fig);
	  trouve = TRUE;
	  if (fig->getCategory () & POINT)
	    point = TRUE;
	}
    }
  // If we have one point, we remove all other items from the list.
  if (point)
    {
      underMouse.init_lire ();
      nb = underMouse.nb_elem;
      do
	{
	  fig = (geometricObject *) underMouse.lire (0);
	  if ((fig->getCategory () & POINT) == 0)
	    underMouse.supprime ((void *) fig);
	}
      while (--nb != 0);
    }
  return trouve;
}

gint
drgeoFigure::preferedUndoLevel ()
{
  atoi (drgeoConfigGet (":undoLevel"));
}

void
drgeoFigure::flashSelection (bool drawGrey)
{
  int nb;
  drgeoColorType color;
  geometricObject *fig;

  if (mode == ANIMATION_MODE)
    return;

  if (!blinkOn)
    return;

  if (drawGrey)
    {
      // Display selected items in grey color, they are currently in
      // normal color.
      nb = selection.nb_elem;
      highlightItem.vide ();
      if (nb == 0)
	return;
      // Fill the highlightItem list with the selected items.
      concatene (highlightItem, selection);
      highlightItem.init_lire ();
      while (nb-- != 0)
	{
	  fig = (geometricObject *) (highlightItem.lire (0));
	  color = fig->getStyle ().color;
	  fig->getStyle ().color = drgeoColorGrey;
	  fig->draw (*area, FALSE);
	  fig->getStyle ().color = color;
	}
    }
  else
    {
      // Display highlight items in normal color, they are actually
      // in grey color.  There is just to redraw them.
      nb = highlightItem.nb_elem;
      highlightItem.init_lire ();
      while (nb-- != 0)
	{
	  fig = (geometricObject *) (highlightItem.lire (0));
	  fig->draw (*area, FALSE);
	}
      highlightItem.vide ();
      concatene (highlightItem, selection);
    }
}

geometricObject *
drgeoFigure::xmlBuildItem (xmlNodePtr item, GHashTable * itemIdToAddress)
{
  gchar *itemType, *attr;
  geometricObject *fig = NULL;
  gint i;

  itemType = (gchar *) item->name;
  if (!strcmp (itemType, "point"))
    {
      fig = new point (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "line"))
    {
      fig = new line (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "halfLine"))
    {
      fig = new halfLine (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "segment"))
    {
      fig = new segment (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "vector"))
    {
      fig = new vector (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "circle"))
    {
      fig = new circle (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "arcCircle"))
    {
      fig = new arcCircle (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "locus"))
    {
      fig = new locus (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "repere"))
    {
      /* FIXME: need some hack
         fig = new repere (item, itemIdToAddress, &figureList);
       */
    }
  else if (!strcmp (itemType, "angle"))
    {
      fig = new angle (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "numeric"))
    {
      fig = new numeric (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "equation"))
    {
      fig = new equation (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "polygon"))
    {
      fig = new polygon (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "script"))
    {
      fig = new script (item, itemIdToAddress, &figureList);
    }
  else if (!strcmp (itemType, "customUI"))
    {
      attr = (gchar *) xmlGetProp (item, BAD_CAST "password");
      setPassword (attr);
      g_free (attr);
      item = xml_search_next (item->xmlChildrenNode, "tool");
      while (item)
	{
	  if (xml_get_value_int (item, "id", &i))
	    {
	      attr = (gchar *) xmlNodeGetContent (item);
	      if (!strcmp (attr, "TRUE"))
		setToolState ((drgeoToolId) i, TRUE);
	      else
		setToolState ((drgeoToolId) i, FALSE);
	      g_free (attr);
	    }
	  item = xml_search_next (item->next, "tool");
	}
    }
  else
    {
      fprintf (stderr, "xmlBuildItem : unknown element %s\n", itemType);
      return NULL;
    }
  return fig;
}

static void
_searchValue (gpointer key, gpointer value, gpointer data)
{
  if (((drgeoSearchValue *) data)->value == value)
    ((drgeoSearchValue *) data)->key = key;
}

static void
_freeKeyFromHash (gpointer key, gpointer value, gpointer data)
{
  g_free (key);
}
