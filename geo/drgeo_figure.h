/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-1999
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

#ifndef drgeo_figure_h
#define drgeo_figure_h

#include <gtk/gtk.h>
#include "couleur.h"

class drgeoFigure;
class drgeoCommand;
class ScmInterface;

#include "liste_elem.h"
#include "drgeo_geometricObject.h"
#include "mode_obj.h"
#include "drgeo_drawable.h"
#include "drgeo_command.h"
#include "drgenius_config.h"
#include "drgeo_scm_interface.h"

#define DRGEO_TOOLS_NUMBER 35
/* start at 35 up to 35+DRGEO_TOOLS_SHORTCUT */
#define DRGEO_TOOLS_SHORTCUT 8
enum drgeoToolId
{
  pointMenuId = 0,
  curveMenuId,
  transformationMenuId,
  numericMenuId,
  macroMenuId,
  otherMenuId,
  moveItemId,
  pointId,
  middleId,
  intersectionId,
  coordinateId,
  lineId,
  halfLineId,
  segmentId,
  vectorId,
  circleId,
  arcCircleId,
  locusId,
  polygonId,
  parallelId,
  perpendicularId,
  reflexionId,
  symmetryId,
  translationId,
  rotationId,
  scaleId,
  distanceId,
  angleId,
  equationId,
  scriptId,
  buildMacroId,
  runMacroId,
  deleteItemId,
  lookItemId,
  propertyItemId,
  pointScId,   /* short cut, ie duplicated reference to a functionality */
  segmentScId,
  lineScId,
  distanceScId,
  lookItemScId,
  propertyItemScId,
  moveItemScId,
  deleteItemScId
};

class drgeoFigure
{
public:
  // New figure, eventualy from a XML a tree
  drgeoFigure (xmlNodePtr figureTree);
  ~drgeoFigure ();
  drgeoDrawable *getDrawable ()
  {
    return (area);
  }

  void setDrawable (drgeoDrawable * view)
  {
    area = view;
  }
  // Save the figure under the given file name
  // the name actually come from the mdi name
  gboolean saveAs (xmlNodePtr drgeoXml, const gchar * name);

  // Is the figure saved ?
  gboolean isSaved ();

  // Export to flydraw
  gboolean exportToFlydraw (const gchar * fileName);

  // Export to latex
  gboolean exportToLatex (const gchar * fileName);

  // Export to PostScript
  gboolean exportToPostScript (const gchar * fileName);

  // Export to PostScript with file handle
  gboolean exportToPostScriptHandle (FILE *fileHandle, const gchar * name);

  // remove all figure items from the figure
  void clear ();

  // Update the figure items
  void updateItems ();

  // Redraw the figure on the drgeoDrawable
  void redraw (gchar all);

  // Temporarily hide items candidate for a removal.
  void hideRemovedItems (geometricObject * item);
  void showRemovedItems ();

  // Set the interactive mode (free point, middle point, line , segment, etc.)
  void setMode (gint toMode,
		buildParametersListForGeometricObject * itemBuilder);
  // Return the actual tool mode
  int getMode ()
  {
    return mode;
  }
  // Manipulate the selected figure item (geometricObject)
  // It's used to construct new figure item depending on the mode
  liste_elem *getSelection ();
  liste_elem *getFigureList ()
  {
    return &figureList;
  }
  void clearSelection ();
  // FIXME: What is the purpose of this function ?
  void setSelection (liste_elem * toSelection);
  void removeItemFromSelection (void *item);

  // Add item to the figureList and eventually to freeFigureList
  void addToFigureList (geometricObject * item, char free);

  // Add item to the figureList and eventually to freeFigureList but do not draw it
  void addToFigureListNoRedraw (geometricObject * item, char free);


  // This depends on the mode we are.
  // We don't need to check if this item is relevant to the current mode
  // It's done by mouseSelection ()
  int addToSelection (drgeoPoint & mouse, geometricObject * item);

  // Check if fig is already constructed by compring parent item
  // Should always return FALSE for free points
  char doesItemExist (geometricObject * fig);

  // The method is invoked to make the selection flash.
  void flashSelection (bool drawGrey);


  // Return the list of figure item that are close to the mouse
  // and relevant to the current mode
  liste_elem *mouseSelection (drgeoPoint & mouse);
  void filterItems (liste_elem &items, objectCategory categories);

  // This methods are invoked during mouse moves.
  void dragSelection (drgeoPoint & start, drgeoPoint & mouse);
  void dropSelection (drgeoPoint & start, drgeoPoint & mouse);

  // helper function to build a drgeo figure from a XML file
  geometricObject *xmlBuildItem (xmlNodePtr item,
				 GHashTable * itemIdToAddress);

  // Undo and Redo operations
  void undo (gint level);
  void redo (gint level);

  /* Delete the undo operation from the actual position of the
     undoLevel list to its end. Used when stacking new undo
     level */
  void emptyUndoQueueEnd ();
  /* Delete all the undo operation in the undo queue */
  void emptyUndoQueueAll ();
  void addToUndoQueue (drgeoCommand * command);
  // Inform about the availability of undo/ redo operation
  gboolean undoActive ();
  gboolean redoActive ();
  // Manipulate the undo hash table
  void addUndoHash (gpointer key, gpointer value);
  void updateUndoHash (gpointer key, gpointer value);
  gpointer getKeyFromValueOnHash (gpointer value);
  gpointer getValueFromKeyOnHash (gpointer key);
  void removeKeyFromHash (gpointer key);
  GHashTable *getUndoHash ();
  liste_elem *dependantItems (geometricObject * item);

  // Search for direct+undirect relatives of item in myList
  liste_elem *searchChildren (geometricObject * item, liste_elem & myList);
  // Search for the lost node in myList
  liste_elem *searchLostNode (liste_elem & myList);

  // Here the method to do anything we want with a geometric figure
  void removeItem (geometricObject * item);
  void createItem (geometricObject * item);
  void moveItem (geometricObject * item, drgeoVector & t);
  void setItemAttribute (geometricObject * item, drgeoAttributeType attr,
			 gpointer value);
  /* password to lock change ot the user interface */
  void setPassword (gchar * newPassword);
  gchar *getPassword ();

  /* set/get a tool state in the user interface (logical part of the change) */
  void setToolState (drgeoToolId id, gboolean visible);
  gboolean getToolState (drgeoToolId id);

  gboolean getShowGridState ();
  void setShowGridState (gboolean state);

  ScmInterface * getScmInterface ();

  // The following objects are responsible for selecting the items
  // that are valid in the context of the current mode.
  //
  // Only the items matching allowed by these filters are added to the
  // selection.
  //
  // These filters also indicate when an object can be created with
  // from the current selection.

  // These filters are able to handle many variations in the input,
  // for example:
  //    a middle point can be created from two points or one segment,
  //    so it is valid to select a point or segment, but once you've
  //    selected a point you cannot select a segment anymore, you can
  //    just select an other point distinct from the first.

  buildParametersListForPoint pointFilter;
  buildParametersListForMiddlePoint middlePointFilter;
  buildParametersListForIntersectionPoint intersPointFilter;
  buildParametersListForCoordinatePoint markPointFilter;
  buildParametersListForLine lineFilter;
  buildParametersListForHalfLine halfLineFilter;
  buildParametersListForSegment segmentFilter;
  buildParametersListForHalfLine vectorFilter;
  buildParametersListForCircle circleFilter;
  buildParametersListForArcCircle arcCircleFilter;
  buildParametersListForLocus locusPointFilter;
  buildParametersListForParallelLine parallelLineFilter;
  buildParametersListForOrthogonalLine orthogonalLineFilter;
  buildParametersListForReflexion reflexionFilter;
  buildParametersListForSymmetry symmetryFilter;
  buildParametersListForTranslation translationFilter;
  buildParametersListForRotation rotationFilter;
  buildParametersListForScale scaleFilter;
  buildParametersListForNumeric numericFilter;
  buildParametersListForAngle angleFilter;
  buildParametersListForCoordinate coordinatesFilter;
  buildParametersListForPolygon polygonFilter;
  buildParametersListForScript scriptFilter;

  liste_elem doNotDraw;

private:
  // Method used to select the items under the mouse when in select mode.
  char itemsUnderMouse (drgeoPoint & mouse, int toute);

  gint preferedUndoLevel ();


  // The drawable area used to display this figure.
  class drgeoDrawable *area;

  ScmInterface *scmInterface;
  gboolean saved, blinked, blinkOn;

  // The current mode we are in.
  gint mode;

  // All the figure item and the moveable figure item liste
  // free_figure_list is included in figure_list
  // FIXME: freeFigureList is obsolete?
  liste_elem figureList, freeFigureList;

  // List of items currently selected in the figure.
  // Push item in this list to make them blink
  liste_elem selection;

  // Figure items `under' the mouse pointer and relevant to the
  // current mode.
  liste_elem underMouse;

  // Hightlight element
  liste_elem highlightItem;

  // This points to the appropriate build* depending on the current
  // mode.
  buildParametersListForGeometricObject *itemFilter;

  // Hash table used in the undo/redo mecanism
  GHashTable *undoHash;
  GList *undoLevel;
  gchar *password;
  gboolean showTool[DRGEO_TOOLS_NUMBER];
  gboolean showGrid;
};

#endif
