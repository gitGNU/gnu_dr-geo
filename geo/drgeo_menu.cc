/* Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-1999
 * hilaire.fernandes@iname.com 
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

#include <stdlib.h>
#include "drgeo_menu.h"
#include "drgeo_gtkdrawable.h"
#include "geo_view.h"
#include "drgeo_control.h"
#include "drgenius_mdi.h"

extern drgeniusMDI *mdi;

drgeoMenu::drgeoMenu (geoView *view):
  drgeoControl (view)
{
  GladeXML *xml;
  GtkWidget *w;

  // Build the pop up menu 
  xml = glade_xml_new (DRGEO_GLADEDIR"/drgeoMDI.glade","geoMenu", NULL);
  glade_xml_signal_autoconnect (xml);
  
  // take a reference of each menu item so we can :
  // . adjust their sensitivity
  // . attach a referecence of this class instance
  
  setTopControlerWidget (xml);
  setPointControlerWidget (xml);
  setCurveControlerWidget (xml);
  setTransformationControlerWidget (xml);
  setNumericControlerWidget (xml);
  setMacroControlerWidget (xml);
  setOtherControlerWidget (xml);

  fetchWidgetParent (xml,&p_popupMenu, "geoMenu",
		     (gpointer) this);
  fetchWidgetParent (xml, &p_zoom200,
		     "zoom200Item", (gpointer) this);
  fetchWidgetParent (xml, &p_zoom175,
		     "zoom175Item", (gpointer) this);
  fetchWidgetParent (xml, &p_zoom150,
		     "zoom150Item", (gpointer) this);
  fetchWidgetParent (xml, &p_zoom125,
		     "zoom125Item", (gpointer) this);
  fetchWidgetParent (xml, &p_zoom100,
		     "zoom100Item", (gpointer) this);
  fetchWidgetParent (xml, &p_zoom75,
		     "zoom75Item", (gpointer) this);
  fetchWidgetParent (xml, &p_zoom50,
		     "zoom50Item", (gpointer) this);
  fetchWidgetParent (xml, &p_zoom25,
		     "zoom25Item", (gpointer) this);


  // Attach the menu to the Drawable widget
  
  w = ((drgeoGtkDrawable *)(view->figure()->getDrawable ()))->getGtkDrawable ();
  g_signal_connect_swapped (G_OBJECT (w), "event",
			    G_CALLBACK (showGeoMenu), 
			    G_OBJECT (p_popupMenu));

  g_object_unref (G_OBJECT (xml));
}

drgeoMenu::~drgeoMenu ()
{
}

GtkWidget *
drgeoMenu::controlerWidget ()
{
  return p_popupMenu;
}

void
drgeoMenu::setZoomFactor (drgeoZoomFactor zoom)
{
  GtkWidget *activeZoom;
  switch (zoom)
    {
    case z100:
      activeZoom = p_zoom100;
      break;
    case z200:
      activeZoom = p_zoom200;
      break;
    case z175:
      activeZoom = p_zoom175;
      break;
    case z150:
      activeZoom = p_zoom150;
      break;
    case z125:
      activeZoom = p_zoom125;
      break;
    case z75:
      activeZoom = p_zoom75;
      break;
    case z50:
      activeZoom = p_zoom50;
      break;
    case z25:
      activeZoom = p_zoom25;
      break;
    }

  gtk_check_menu_item_set_active 
    (GTK_CHECK_MENU_ITEM (activeZoom), 
    true);
}
static gboolean
showGeoMenu (GtkWidget *menu, GdkEvent *event )
{
  GdkEventButton *bevent;
  
  if (event->type == GDK_BUTTON_PRESS) {
    bevent = (GdkEventButton *) event;   
    if (bevent->button == 3)
      {
	gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, NULL,
			bevent->button, bevent->time);
	/* Tell calling code that we have handled this event; the buck
	 * stops here. */
	return true;
      }
  }
  return false;
}



