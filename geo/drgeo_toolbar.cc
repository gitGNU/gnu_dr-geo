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

#include "drgeo_toolbar.h"
#include "drgeo_gtkdrawable.h"

drgeoToolbar::drgeoToolbar (geoView *view):
  drgeoControl (view)
{
  GError* error = NULL;
  GtkBuilder *xml = gtk_builder_new ();
  GtkBuilder *xml1 = gtk_builder_new ();
  GtkBuilder *xml2 = gtk_builder_new ();
  GtkBuilder *xml3 = gtk_builder_new ();
  GtkBuilder *xml4 = gtk_builder_new ();
  GtkBuilder *xml5 = gtk_builder_new ();
  GtkBuilder *xml6 = gtk_builder_new ();
  GtkBuilder *xml7 = gtk_builder_new ();
  GtkWidget *w;

  // Build the toolbars

  // take a reference of each bar item so we can :
  // . adjust their sensitivity
  // . attach a referecence of this class instance


  if (!gtk_builder_add_from_file (xml, DRGEO_GLADEDIR "/menuWindow.glade", &error))
  {
    g_warning ("Couldn't load builder file: %s", error->message);
    g_error_free (error);
  }
  setTopControlerWidget (xml);

  fetchWidgetParent (xml,&p_toolbar, "menuBar",
		     (gpointer) this);
  fetchWidgetParent (xml,&p_zoomEntry, "zoomEntry",
		     (gpointer) this);
  gtk_entry_set_text (GTK_ENTRY (p_zoomEntry), "100%");
  gtk_builder_connect_signals (xml, &error);
  g_object_unref (G_OBJECT (xml));
  

  /* short cut bar */
  if (!gtk_builder_add_from_file (xml1, DRGEO_GLADEDIR "/shortcutWindow.glade", &error))
  {
    g_warning ("Couldn't load builder file: %s", error->message);
    g_error_free (error);
  }
  setShortcutControlerWidget (xml1);
  fetchWidgetParent (xml1,&p_shortcutBar, "shortcutBar",
		     (gpointer) this);
  gtk_builder_connect_signals (xml1, &error);
  g_object_unref (G_OBJECT (xml1));

  /* point bar */
  if (!gtk_builder_add_from_file (xml2, DRGEO_GLADEDIR "/pointBar.glade", &error))
  {
    g_warning ("Couldn't load builder file: %s", error->message);
    g_error_free (error);
  }
  gtk_builder_connect_signals (xml2, &error);
  setPointControlerWidget (xml2);
  fetchWidgetParent (xml2,&p_pointBar, "pointBar", 
		     (gpointer) this);
  w = GTK_WIDGET (gtk_builder_get_object(xml2, "toolbar"));
  gtk_toolbar_set_tooltips (GTK_TOOLBAR (w), true);
  g_object_unref (G_OBJECT (xml2));
  
  /* curve bar */
  if (!gtk_builder_add_from_file (xml3, DRGEO_GLADEDIR "/curveBar.glade", &error))
  {
    g_warning ("Couldn't load builder file: %s", error->message);
    g_error_free (error);
  }
  gtk_builder_connect_signals (xml3, &error);
  setCurveControlerWidget (xml3);
  fetchWidgetParent (xml3,&p_curveBar, "curveBar",
		     (gpointer) this);
  g_object_unref (G_OBJECT (xml3));

  /* transformation bar */
  if (!gtk_builder_add_from_file (xml4, DRGEO_GLADEDIR "/transformation.glade", &error))
  {
    g_warning ("Couldn't load builder file: %s", error->message);
    g_error_free (error);
  }
  gtk_builder_connect_signals (xml4, &error);
  setTransformationControlerWidget (xml4);
  fetchWidgetParent (xml4,&p_transformationBar, 
		     "transformationBar",
		     (gpointer) this);
  g_object_unref (G_OBJECT (xml4));

  /* numeric bar */
  if (!gtk_builder_add_from_file (xml5, DRGEO_GLADEDIR "/numericBar.glade", &error))
  {
    g_warning ("Couldn't load builder file: %s", error->message);
    g_error_free (error);
  }
  gtk_builder_connect_signals (xml5, &error);
  setNumericControlerWidget (xml5);
  fetchWidgetParent (xml5,&p_numericBar, "numericBar",
		     (gpointer) this);
  g_object_unref (G_OBJECT (xml5));

  /* macro bar */
  if (!gtk_builder_add_from_file (xml6, DRGEO_GLADEDIR "/macroBar.glade", &error))
  {
    g_warning ("Couldn't load builder file: %s", error->message);
    g_error_free (error);
  }
  gtk_builder_connect_signals (xml6, &error);
  setMacroControlerWidget (xml6);
  fetchWidgetParent (xml6,&p_macroBar, "macroBar",
		     (gpointer) this);
  g_object_unref (G_OBJECT (xml6));

  /* other bar */
  if (!gtk_builder_add_from_file (xml7, DRGEO_GLADEDIR "/otherBar.glade", &error))
  {
    g_warning ("Couldn't load builder file: %s", error->message);
    g_error_free (error);
  }
  gtk_builder_connect_signals (xml7, &error);
  setOtherControlerWidget (xml7);
  fetchWidgetParent (xml7,&p_otherBar, "otherBar",
		     (gpointer) this);
  g_object_unref (G_OBJECT (xml7));

}

GtkWidget *
drgeoToolbar::controlerWidget ()
{
  return p_toolbar;
}

GtkWidget *
drgeoToolbar::shortcutWidget ()
{
  return p_shortcutBar;
}

void
drgeoToolbar::showToolbar (drgeoControlToolbar tool)
{
  switch (tool)
    {
    case POINT_TOOLBAR:
      gtk_widget_show (p_pointBar);
      break;
    case CURVE_TOOLBAR:
      gtk_widget_show (p_curveBar);
      break;
    case TRANSFORMATION_TOOLBAR:
      gtk_widget_show (p_transformationBar);
      break;
    case NUMERIC_TOOLBAR:
      gtk_widget_show (p_numericBar);
      break;
    case MACRO_TOOLBAR:
      gtk_widget_show (p_macroBar);
      break;
    case OTHER_TOOLBAR:
      gtk_widget_show (p_otherBar);
      break;
    }
}

void
drgeoToolbar::hideToolbar (drgeoControlToolbar tool)
{
  switch (tool)
    {
    case POINT_TOOLBAR:
      gtk_widget_hide (p_pointBar);
      break;
    case CURVE_TOOLBAR:
      gtk_widget_hide (p_curveBar);
      break;
    case TRANSFORMATION_TOOLBAR:
      gtk_widget_hide (p_transformationBar);
      break;
    case NUMERIC_TOOLBAR:
      gtk_widget_hide (p_numericBar);
      break;
    case MACRO_TOOLBAR:
      gtk_widget_hide (p_macroBar);
      break;
    case OTHER_TOOLBAR:
      gtk_widget_hide (p_otherBar);
      break;
    }
}

void
drgeoToolbar::setZoomFactor (drgeoZoomFactor zoom)
{

  g_signal_handlers_block_matched ((gpointer) p_zoomEntry,
				   G_SIGNAL_MATCH_FUNC,
				   0, 0, NULL,
				   (gpointer) on_zoom_valeur_changed,
				   NULL);

  switch (zoom)
    {
    case z100:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "100%");
      break;
    case z200:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "200%");
      break;
    case z175:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "175%");
      break;
    case z150:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "150%");
      break;
    case z125:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "125%");
      break;
    case z75:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "75%");
      break;
    case z50:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "50%");
      break;
    case z25:
      gtk_entry_set_text (GTK_ENTRY (p_zoomEntry),
			  "25%");
      break;
    }
  g_signal_handlers_unblock_matched ((gpointer) p_zoomEntry,
				     G_SIGNAL_MATCH_FUNC,
				     0, 0, NULL,
				     (gpointer) on_zoom_valeur_changed,
				     NULL);
}

void
drgeoToolbar::updateUserInterface ()
{
  drgeoFigure *figure;

  drgeoControl::updateUserInterface ();
  /* Now specific update to the toolbar class */
  figure = p_view->figure ();
  hideTool (pointScId, !figure->getToolState (pointId));
  hideTool (segmentScId, !figure->getToolState (segmentId));
  hideTool (lineScId, !figure->getToolState (lineId));
  hideTool (distanceScId, !figure->getToolState (distanceId));
  hideTool (lookItemScId, !figure->getToolState (lookItemId));
  hideTool (propertyItemScId, !figure->getToolState (propertyItemId));
  hideTool (moveItemScId, !figure->getToolState (moveItemId));
  hideTool (deleteItemScId, !figure->getToolState (deleteItemId));
}

/*****************************************/
/* Handlers of the toolbar controlers    */
/*****************************************/
gint
on_subToolbar_clicked (GtkWidget * w, GdkEventButton * evt, gpointer data)
{
  gtk_widget_hide (w);
  return TRUE;
}

void
on_drgeoPointMenu_clicked (GtkWidget * w, gpointer data)
{
  drgeoToolbar *toolbar;

  toolbar = (drgeoToolbar *) gtk_object_get_data (GTK_OBJECT (w), "parent");
  toolbar->showToolbar (POINT_TOOLBAR);
}

void
on_drgeoCurveMenu_clicked (GtkWidget * w, gpointer data)
{
  drgeoToolbar *toolbar;

  toolbar = (drgeoToolbar *) gtk_object_get_data (GTK_OBJECT (w), "parent");
  toolbar->showToolbar (CURVE_TOOLBAR);
}

void
on_drgeoTransformationMenu_clicked (GtkWidget * w, gpointer data)
{
  drgeoToolbar *toolbar;

  toolbar = (drgeoToolbar *) gtk_object_get_data (GTK_OBJECT (w), "parent");
  toolbar->showToolbar (TRANSFORMATION_TOOLBAR);
}

void
on_drgeoNumericMenu_clicked (GtkWidget * w, gpointer data)
{
  drgeoToolbar *toolbar;

  toolbar = (drgeoToolbar *) gtk_object_get_data (GTK_OBJECT (w), "parent");
  toolbar->showToolbar (NUMERIC_TOOLBAR);
}

void
on_drgeoMacroMenu_clicked (GtkWidget * w, gpointer data)
{
  drgeoToolbar *toolbar;

  toolbar = (drgeoToolbar *) gtk_object_get_data (GTK_OBJECT (w), "parent");
  toolbar->showToolbar (MACRO_TOOLBAR);
}

void
on_drgeoOtherMenu_clicked (GtkWidget * w, gpointer data)
{
  drgeoToolbar *toolbar;

  toolbar = (drgeoToolbar *) gtk_object_get_data (GTK_OBJECT (w), "parent");
  toolbar->showToolbar (OTHER_TOOLBAR);
}

void 
on_zoom_valeur_changed (GtkEditable * w, gpointer data)
{
  drgeoFigure *figure;
  drgeoGtkDrawable *area;
  drgeoControl *control;
  gfloat zoom;

  control = (drgeoControl *) (gtk_object_get_data (GTK_OBJECT (w), "parent"));
  area = control->drawable ();

  figure = area->getFigure();
  
  zoom = strtod (gtk_entry_get_text (GTK_ENTRY (w)), NULL);
  
  area->setScale (zoomToScale (zoom));
  area->refresh ();

  if (zoom == 200.)
    control->view ()->setZoomFactorMenu (z200);
  else if (zoom == 175.)
    control->view ()->setZoomFactorMenu (z175);
  else if (zoom == 150.)
    control->view ()->setZoomFactorMenu (z150);
  else if (zoom == 125.)
    control->view ()->setZoomFactorMenu (z125);
  else if (zoom == 100.)
    control->view ()->setZoomFactorMenu (z100);
  else if (zoom == 75.)
    control->view ()->setZoomFactorMenu (z75);
  else if (zoom == 50.)
    control->view ()->setZoomFactorMenu (z50);
  else if (zoom == 25.)
    control->view ()->setZoomFactorMenu (z25);
}



