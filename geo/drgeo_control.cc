/* Dr Genius an interactive geometry software
 * (C) Copyright Hilaire Fernandes  2003
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


#include "drgenius_mdi.h"
#include "drgeo_control.h"
#include "drgeo_gtkdrawable.h"

extern drgeniusMDI *mdi;

drgeoControl::drgeoControl (geoView *view)
{
  p_view = view;
}

drgeoControl::~drgeoControl ()
{
}

void drgeoControl::
hideTool (drgeoToolId i, gboolean state)
{
  if (state)
    gtk_widget_hide (p_menuItem[i]);
  else
    gtk_widget_show (p_menuItem[i]);    
}

void drgeoControl::
setTopControlerWidget (GtkBuilder *xml)
{
  fetchWidgetParent (xml, &p_menuItem[pointMenuId],
		     "pointMenu", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[curveMenuId],
		     "curveMenu", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[transformationMenuId],
		     "transformationMenu", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[numericMenuId],
		     "numericMenu", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[macroMenuId],
		     "macroMenu", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[otherMenuId],
		     "otherMenu", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[moveItemId],
		     "moveItem", (gpointer) this);
}

void drgeoControl::
setPointControlerWidget (GtkBuilder *xml)
{
  fetchWidgetParent (xml, &p_menuItem[pointId],
		     "freePointItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[middleId],
		     "middleItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[intersectionId],
		     "intersectionItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[coordinateId],
		     "coordinatesItem", (gpointer) this);
}

void drgeoControl::
setCurveControlerWidget (GtkBuilder *xml)
{
  fetchWidgetParent (xml, &p_menuItem[lineId],
		     "lineItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[halfLineId],
		     "halfLineItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[segmentId],
		     "segmentItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[vectorId],
		     "vectorItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[circleId],
		     "circleItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[arcCircleId],
		     "arcCircleItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[locusId],
		     "locusItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[polygonId],
		     "polygonItem", (gpointer) this);
}

void drgeoControl::
setTransformationControlerWidget (GtkBuilder *xml)
{
  fetchWidgetParent (xml, &p_menuItem[parallelId],
		     "parallelLineItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[perpendicularId],
		     "orthogonalLineItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[reflexionId],
		     "axialSymmetryItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[symmetryId],
		     "centralSymmetryItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[translationId],
		     "translationItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[rotationId],
		     "rotationItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[scaleId],
		     "scaleItem", (gpointer) this);
}

void drgeoControl::
setNumericControlerWidget (GtkBuilder *xml)
{
  fetchWidgetParent (xml, &p_menuItem[distanceId],
		     "distanceLengthItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[angleId],
		     "angleItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[equationId],
		     "coordinatesEquationItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[scriptId],
		     "scriptItem", (gpointer) this);
}

void drgeoControl::
setMacroControlerWidget (GtkBuilder *xml)
{
  fetchWidgetParent (xml, &p_menuItem[buildMacroId],
		     "constructMacroItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[runMacroId],
		     "executeMacroItem", (gpointer) this);
}

void drgeoControl::
setOtherControlerWidget (GtkBuilder *xml)
{
  fetchWidgetParent (xml, &p_menuItem[deleteItemId],
		     "deleteObjectItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[lookItemId],
		     "lookItem", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[propertyItemId],
		     "propertyItem", (gpointer) this);
}

void drgeoControl::
setShortcutControlerWidget (GtkBuilder *xml)
{
  fetchWidgetParent (xml, &p_menuItem[pointScId],
		     "freePointItemSc", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[segmentScId],
		     "segmentItemSc", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[lineScId],
		     "lineItemSc", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[distanceScId],
		     "distanceLengthItemSc", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[lookItemScId],
		     "lookItemSc", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[propertyItemScId],
		     "propertyItemSc", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[moveItemScId],
		     "moveItemSc", (gpointer) this);
  fetchWidgetParent (xml, &p_menuItem[deleteItemScId],
		     "deleteObjectItemSc", (gpointer) this);

}

void
drgeoControl::hideToolbar (drgeoControlToolbar tool)
{
}

void
drgeoControl::updateUserInterface ()
{
  gint i;
  drgeoFigure *figure;
  
  figure = p_view->figure ();
  
  for (i = 0; i < DRGEO_TOOLS_NUMBER; i++)
      hideTool ((drgeoToolId) i, !figure->getToolState (((drgeoToolId) i)));

}

geoView *
drgeoControl::view ()
{
  return p_view;
}

void
drgeoControl::reconcileControler ()
{
  drgeoFigure *figure;
  gint i;

  figure = p_view->figure ();
  /* check for the enabled tools */
  for (i = 0; i < DRGEO_TOOLS_NUMBER; i++)
    {
      if (figure->getToolState ((drgeoToolId) i))
	gtk_widget_show (p_menuItem[i]);
      else
	gtk_widget_hide (p_menuItem[i]);
    }
}

drgeoGtkDrawable *
drgeoControl::drawable ()
{
  return (drgeoGtkDrawable *) p_view->figure()->getDrawable();
}


// HELPERS

void fetchWidgetParent (GtkBuilder *xml, GtkWidget * *w, gchar *name, gpointer parent)
{
  *w = GTK_WIDGET (gtk_builder_get_object (xml, name));
  gtk_object_set_data (GTK_OBJECT (*w), "parent", parent);
}


/*********************************/
/* Handlers of the controlers    */
/*********************************/
void
drgeo_free_point_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (POINT_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (POINT_MODE, &(figure->pointFilter));

  area->refresh ();
  
  mdi->setMessage (_("Free point: click on the plane or over a curve"));
}

void
drgeo_middle_point_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (POINT_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (POINT_MILIEU_MODE, &(figure->middlePointFilter));

  area->refresh ();

  mdi->setMessage (_("Middle: select two points or a segment"));

}

void
drgeo_intersection_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (POINT_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (POINT_INTER_MODE, &(figure->intersPointFilter));

  area->refresh ();
  mdi->setMessage (_("Intersection(s): select two curves"));
}

void
drgeo_mark_point_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (POINT_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (POINT_REPERE_MODE, &(figure->markPointFilter));

  area->refresh ();
  mdi->setMessage (_("Coordinate point: select two numbers (its coordinates)"));
}
void
drgeo_line_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (CURVE_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (DROITE_MODE, &(figure->lineFilter));

  area->refresh ();

  mdi->setMessage (_("Line: select two points"));
}

void
drgeo_half_line_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (CURVE_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (DEMI_DROITE_MODE, &(figure->halfLineFilter));

  area->refresh ();
  mdi->setMessage (_("Ray: select its origin then another point"));
}

void
drgeo_segment_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (CURVE_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (SEGMENT_MODE, &(figure->segmentFilter));

  area->refresh ();
  mdi->setMessage (_("Segment: select its extremities"));
}

void
drgeo_vector_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (CURVE_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (VECTEUR_MODE, &(figure->vectorFilter));

  area->refresh ();

  mdi->setMessage (_("Vector: select its origin and extremity"));
}

void
drgeo_circle_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (CURVE_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (CERCLE_MODE, &(figure->circleFilter));

  area->refresh ();
  mdi->setMessage (_("Circle: select its center and a point or its radius (number or segment)"));
}

void
drgeo_arc_circle_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (CURVE_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (ARC_CERCLE_MODE, &(figure->arcCircleFilter));

  area->refresh ();  
  mdi->setMessage (_("Arc: select three points"));
}

void
drgeo_locus_point_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (CURVE_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (LIEU_POINT_MODE, &(figure->locusPointFilter));

  area->refresh ();
  mdi->setMessage (_("Locus: select a free point and a dependent point"));
}

void
drgeo_polygon_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (CURVE_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (POLYGON_MODE, &(figure->polygonFilter));

  area->refresh ();
  mdi->setMessage (_("Polygon: select its vertices, then click over the first one"));
}

void
drgeo_parallel_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (TRANSFORMATION_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (DROITE_PARALLELE_MODE, &(figure->parallelLineFilter));
  area->refresh ();
  mdi->setMessage (_("Parallel line: select a point and a direction (line, ray, segment, vector) "));
}

void
drgeo_orthogonal_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (TRANSFORMATION_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (DROITE_ORTHOGONALE_MODE, &(figure->orthogonalLineFilter));
  area->refresh ();
  mdi->setMessage (_("Orthogonal line: select a point and a direction (line, ray, segment, vector)"));
}

void
drgeo_reflexion_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (TRANSFORMATION_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (REFLEXION_MODE, &(figure->reflexionFilter));
  area->refresh ();
  mdi->setMessage (_("Axial symmetry: select a geometric object and a line"));
}

void
drgeo_symmetry_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (TRANSFORMATION_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (SYMETRIE_MODE, &(figure->symmetryFilter));
  area->refresh ();
  mdi->setMessage (_("Central symmetry: select a geometric object and the center"));
}

void
drgeo_translation_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (TRANSFORMATION_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (TRANSLATION_MODE, &(figure->translationFilter));
  area->refresh ();
  mdi->setMessage (_("Translation: select a geometric object and a vector"));
}

void
drgeo_rotation_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (TRANSFORMATION_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (ROTATION_MODE, &(figure->rotationFilter));
  area->refresh ();
  mdi->setMessage (_("Rotation: select a geometric object, the center and an angle or a scalar"));
}

void
drgeo_scale_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (TRANSFORMATION_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (HOMOTHETIE_MODE, &(figure->scaleFilter));
  area->refresh ();
  mdi->setMessage (_("Scale: select a geometric object, the center and a scalar"));
}

void
drgeo_numeric_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (NUMERIC_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (NUMERIQUE_MODE, &(figure->numericFilter));
  area->refresh ();
  mdi->setMessage (_("Numeric: click on the back to edit a free value or select geometric object(s) to calculate distance"));
}

void
drgeo_angle_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (NUMERIC_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (ANGLE_MODE, &(figure->angleFilter));
  area->refresh ();
  mdi->setMessage (_("Angle: select three points (geometric angle) or two vectors (oriented angle)"));
}

void
drgeo_coordinates_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (NUMERIC_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (COORDONNEES_MODE, &(figure->coordinatesFilter));
  area->refresh ();
  mdi->setMessage (_("Coordinates & Equation: select a point, a vector, a line or a circle"));

}

void
drgeo_script_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (NUMERIC_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->buildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (SCRIPT_MODE, &(figure->scriptFilter));
  area->refresh ();
  mdi->setMessage (_("Script: select n objects (n >=0) then click on the background"));
}

void
drgeo_delete_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (OTHER_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->deleteTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (EFFACE_OBJET_MODE, NULL);
  area->refresh ();
  mdi->setMessage (_("Delete object: select the object to delete"));
}

void
drgeo_look_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (OTHER_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->styleTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (MISE_EN_FORME_MODE, NULL);
  area->refresh ();
  mdi->setMessage (_("Look: select the object to change its look"));
}

void
drgeo_property_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (OTHER_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->propertyTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (PROPERTY_MODE, NULL);
  area->refresh ();
  mdi->setMessage (_("Property: select the object to change its property"));

}

void
drgeo_macro_build_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure * figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (MACRO_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->macroBuildTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (MACRO_ENREGISTRE_MODE, NULL);
  area->refresh ();
  mdi->setMessage (_("Construct a macro-construction "));

}

void
drgeo_macro_play_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure * figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->hideToolbar (MACRO_TOOLBAR);
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->macroPlayTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (MACRO_PLAY_MODE, NULL);

  area->refresh ();
  mdi->setMessage (_("Play a recorded macro-construction"));
}

void
drgeo_zoom_25_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->view ()->setZoomFactorTb (z25);

  area = control->drawable ();
  area->setScale (7.5);
  area->refresh ();
}
void
drgeo_zoom_50_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->view ()->setZoomFactorTb (z50);

  area = control->drawable ();
  area->setScale (15.0);
  area->refresh ();
}
void
drgeo_zoom_75_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->view ()->setZoomFactorTb (z75);

  area = control->drawable ();
  area->setScale (22.5);
  area->refresh ();
}
void
drgeo_zoom_100_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->view ()->setZoomFactorTb (z100);

  area = control->drawable ();
  area->setScale (30.0);
  area->refresh ();
}
void
drgeo_zoom_125_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->view ()->setZoomFactorTb (z125);

  area = control->drawable ();
  area->setScale (37.5);
  area->refresh ();
}
void
drgeo_zoom_150_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->view ()->setZoomFactorTb (z150);

  area = control->drawable ();
  area->setScale (45.0);
  area->refresh ();
}
void
drgeo_zoom_175_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->view ()->setZoomFactorTb (z175);

  area = control->drawable ();
  area->setScale (52.5);
  area->refresh ();
}
void
drgeo_zoom_200_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  control->view ()->setZoomFactorTb (z200);

  area = control->drawable ();
  area->setScale (60.0);
  area->refresh ();
}

void
drgeo_select_cb (GtkWidget * widget, gpointer data)
{
  drgeoControl *control;
  drgeoGtkDrawable *area;
  drgeoFigure *figure;

  control = (drgeoControl *) gtk_object_get_data (GTK_OBJECT (widget), "parent");
  area = control->drawable ();
  // Change the current tool in the drawable
  area->setTool (area->selectTool);

  figure = area->figure;
  // Change the currend mode of the drgeo figure
  figure->setMode (SOURIS_SELECT_MODE, NULL);

  area->refresh ();
  mdi->setMessage (_("Select: select and move an object"));
}
