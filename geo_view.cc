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

#include "geo_view.h"
#include "drgeo_figure.h"
#include "drgeo_menu.h"
#include "drgeo_toolbar.h"
#include "drgeo_gtkdrawable.h"
#include "drgenius_config.h"

extern gchar *toolName[];

gchar *defaultFigureName = _("Figure %d");

/* Callbacks */
static void
descriptionSelected_cb (GtkTreeSelection *selection, drgeoFigure *figure);


geoView::geoView (xmlNodePtr drgeoXml):
  drgeniusView (FIGURE_CHILD, true)
{
  static gint counter = 1;
  drgeoGtkDrawable *drawable;
  GtkWidget *paned, *widget1, *widget2;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkTreeSelection *select;
  double propValue, scale = 0;
  drgeoPoint p;
  xmlNodePtr child;
  
  p_figure = new drgeoFigure (drgeoXml);
  
  if (drgeoXml == NULL)
    p_name = g_strdup_printf (_(drgeoConfigGet (":figureName")), counter++);
  else
    p_name = (gchar *) (xmlGetProp (drgeoXml, BAD_CAST "name"));

  // Define the drawable
  drawable = (drgeoGtkDrawable *) createWidgetView ();
  p_figure->setDrawable ((drgeoDrawable *) drawable);

  // We need to update the scale and coordinate of the origin in the
  // drawable
  if (drgeoXml != NULL)
    {
      if (xml_get_value_double (drgeoXml, "scale", &scale))
	drawable->setScale (scale);      
      if (xml_get_value_double (drgeoXml, "origin_x", &propValue))
	{
	  p.setX (propValue);
	  if (xml_get_value_double (drgeoXml, "origin_y", &propValue))
	    {
	      p.setY (propValue);
	      drawable->setAreaCenter (p);
	    }    
	}
      drawable->updateAdjustments ();
      // get the bounding box if it exists
      child = xml_search_child (drgeoXml, "boundingBox");
      if (child)
	{
	  drgeoPoint tl, br;

	  xml_get_value_double (child, "tl_x", &propValue);
	  tl.setX (propValue);
	  xml_get_value_double (child, "tl_y", &propValue);
	  tl.setY (propValue);

	  xml_get_value_double (child, "br_x", &propValue);
	  br.setX (propValue);
	  xml_get_value_double (child, "br_y", &propValue);
	  br.setY (propValue);
	  drawable->showPrintingArea (tl, br);
	}
    }


  //build the popup menu and toolbar and attach them to the drawable
  //and the view
  p_menu = new drgeoMenu (this);  
  p_toolbar = new drgeoToolbar (this);

  // Adjust the scale in the toolbar and menu
  if (scale != 0)
    {
      setZoomFactorTb (scaleToZoom (scale));
      setZoomFactorMenu (scaleToZoom (scale));
    }

  p_geoView = gtk_vbox_new (false, 0);
  gtk_box_pack_start (GTK_BOX (p_geoView),
		      p_toolbar->controlerWidget (),
		      false, false,0);

  paned = gtk_hpaned_new ();
  gtk_paned_set_position (GTK_PANED (paned), 0);
  gtk_box_pack_start (GTK_BOX (p_geoView),
		      paned,
		      true, true,0);

  //
  // Pack the description widget in the Hpaned
  //

  // to hold the description: a tree and a model
  p_descrStore = gtk_tree_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);
  p_descrTree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (p_descrStore));
  g_object_unref (G_OBJECT (p_descrStore));
  renderer = gtk_cell_renderer_text_new ();

  column = gtk_tree_view_column_new_with_attributes (_("Items"),
						     renderer,
						     "text", 0,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (p_descrTree), column);

  widget1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (widget1),
					 p_descrTree);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (widget1),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  // Ornaments
  widget2 = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (widget2), GTK_SHADOW_IN);
  gtk_container_add (GTK_CONTAINER (widget2), widget1);
  gtk_paned_add1 (GTK_PANED (paned), widget2);



  // set the callback for the tree widget
  select  = gtk_tree_view_get_selection (GTK_TREE_VIEW (p_descrTree));
  gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (select), "changed",
		    G_CALLBACK (descriptionSelected_cb),
		    (gpointer) p_figure);
  
  //
  // Pack a horizontal box in the Hpaned,
  // then pack the short cut bar
  // and the top widget

  widget1 = gtk_hbox_new (false, 0);
  gtk_paned_add2 (GTK_PANED (paned), 
		  widget1);
  gtk_box_pack_start (GTK_BOX (widget1),
		      p_toolbar->shortcutWidget (),
		      false, false,0);
  gtk_box_pack_start (GTK_BOX (widget1),
		      drawable->getTopWidget (),
		      true, true,0);

  drawable->updateDescription ();
  updateUserInterface ();
  
  gtk_widget_show_all (p_geoView);
}

geoView::
~geoView ()
{
  delete ((drgeoGtkDrawable *) p_figure->getDrawable ());
  delete (p_figure);  
}

gboolean
geoView::save (gchar *filename)
{
  gint ret;
  xmlDocPtr xml;
  
  if (this->p_filename != filename)
    {
      g_free (this->p_filename);
      this->p_filename = g_strdup (filename);
    }

  xml = xmlNewDoc (BAD_CAST "1.0");
  xml->xmlRootNode = xmlNewDocNode (xml, NULL,
			     BAD_CAST "drgenius", NULL);
  p_figure->saveAs (xml->xmlRootNode, p_name);
  ret = xmlSaveFile (filename, xml);
  xmlFreeDoc (xml);
  return (ret != -1);
}

gboolean
geoView::saveNode (xmlNodePtr tree)
{
  return p_figure->saveAs (tree, p_name);
}

gboolean
geoView::undo (gint n)
{
  p_figure->undo (n);
  return TRUE;
}

gboolean
geoView::redo (gint n)
{
  p_figure->redo (n);
  return TRUE;
}

gboolean 
geoView::undoActive ()
{
  return p_figure->undoActive ();  
}

gboolean 
geoView::redoActive ()
{
  return p_figure->redoActive ();  
}

gpointer 
geoView::createWidgetView ()
{
  drgeoGtkDrawable *drawable;

  drawable = new drgeoGtkDrawable (p_figure, this);
  return (gpointer) drawable;
}

GtkWidget * 
geoView::widgetView ()
{
  return p_geoView;
}

drgeoFigure *
geoView::figure ()
{
  return p_figure;
}

void
geoView::setZoomFactorTb (drgeoZoomFactor zoom)
{
  p_toolbar->setZoomFactor (zoom);
}

void
geoView::setZoomFactorMenu (drgeoZoomFactor zoom)
{
  p_menu->setZoomFactor (zoom);
}

void 
geoView::updateUserInterface ()
{  
  p_toolbar->updateUserInterface ();
  p_menu->updateUserInterface ();
}


void 
geoView::addDescription (geometricObject *item)
{
  GtkTreeIter parentIter, childIter;
  gchar **itemDescription;
  GList *parent;


  item->updateDescription ();
  itemDescription = item->getDescription ();

  gtk_tree_store_append (p_descrStore, &parentIter, NULL);  /* Acquire a top-level iterator */
  gtk_tree_store_set (p_descrStore, &parentIter,
		      0, itemDescription[0],
		      1, item,
		      -1);

  parent = item->getParentList ();
  while (parent)
    {
      item = (geometricObject *) parent->data;
      ++itemDescription;
      gtk_tree_store_append (p_descrStore, &childIter, &parentIter);
      gtk_tree_store_set (p_descrStore, &childIter,
			  0, itemDescription[0],
			  1, item,
			  -1);
      parent = g_list_next (parent);
    }
}

void
geoView::clearDescription ()
{
  gtk_tree_store_clear (GTK_TREE_STORE (p_descrStore));
}


/* Callback */

static void
descriptionSelected_cb (GtkTreeSelection *selection, drgeoFigure *figure)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  geometricObject *item;

  figure->clearSelection ();
  if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
      gtk_tree_model_get (model, &iter, 1, &item, -1);      
      if (item)
	figure->getSelection ()->ajoute ((void *) item);
    }
}

double zoomToScale (gfloat zoom)
{
  return (3. * zoom / 10.);
}

drgeoZoomFactor scaleToZoom (double scale)
{
  return (drgeoZoomFactor) (int) (scale * 10. / 3.);
}
