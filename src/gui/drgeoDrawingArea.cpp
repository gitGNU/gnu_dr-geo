// drgeoDrawingArea.cpp
//
// Copyright (C) 2014 Gurjot Singh <bhattigurjot@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "drgeoDrawingArea.h"

drgeoPainter *painterPointer;
gdouble x, y, startX, startY, endX, endY, setX, setY, thirdX, thirdY;
int entity;
std::vector<drgeoContainer> point;
std::vector<drgeoContainer>::iterator it;
GdkWindow* win;
GdkCursor *cur;

/* Flags*/
bool mFirstClick = false;
bool mSecondClick = false;
bool mThirdClick = false;
bool mPaintFlag = false;
bool mExist = false;

drgeoDrawingArea::drgeoDrawingArea ()
{
	drawingArea = gtk_drawing_area_new ();
	gtk_widget_set_events (drawingArea, gtk_widget_get_events (drawingArea)
	                       | GDK_BUTTON_PRESS_MASK
	                       | GDK_POINTER_MOTION_MASK
	                       | GDK_POINTER_MOTION_HINT_MASK);

	/* Connect Signals */
	g_signal_connect (drawingArea, "draw", G_CALLBACK (drgeo_draw_event), NULL); 
	g_signal_connect (drawingArea, "configure-event", 
	                  G_CALLBACK (drgeo_configure_event), NULL);

	gtk_widget_show (drawingArea);
}

drgeoDrawingArea::~drgeoDrawingArea ()
{
}

GtkWidget* 
drgeoDrawingArea::createDrawArea ()
{
	return drawingArea;
}

void 
drgeoDrawingArea::drawEntity (int x)
{
	entity = x;
	mFirstClick = true;
	g_signal_connect (drawingArea, "button-press-event", 
	                  G_CALLBACK (drgeo_clicked_event), NULL);
	if (entity > 1)
	{
		g_signal_connect (drawingArea, "motion-notify-event",
		                  G_CALLBACK (drgeo_motion_event), NULL);
	}    
}

void 
set_surface (void)
{
	cairo_t *cr;

	cr = cairo_create (surface);

	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_paint (cr);

	cairo_destroy (cr);
}

void
paint_point (GtkWidget *widget, gdouble x, gdouble y)
{
	cairo_t *cr;
	painterPointer->drgeo_cairo_point (cr, surface, x, y);

	drgeoContainer cont;
	cont.setValue (x,y);
	point.push_back (cont);

	std::cout<<"Vector Size: "<<point.size ()<<std::endl;
	std::cout<<"x: "<<cont.getValueX ()<<" and y: "<<cont.getValueY ()<<std::endl;

	/* Now invalidate the affected region of the drawing area. */
	gtk_widget_queue_draw_area (widget, x - 3, y - 3, 7, 7);
}

void
paint_line (GtkWidget *widget, gdouble startX, gdouble startY, gdouble endX, gdouble endY)
{
	cairo_t *cr;
	painterPointer->drgeo_cairo_line (cr, surface, startX, startY, endX, endY);

	g_signal_handlers_disconnect_by_func (widget, (gpointer)drgeo_motion_event, NULL);
	gdk_window_set_cursor( win, NULL );

	/* Invalidate the complete widget for now*/
	gtk_widget_queue_draw (widget);
}

void
paint_circle (GtkWidget *widget, gdouble startX, gdouble startY, gdouble endX, gdouble endY)
{
	cairo_t *cr;

	gdouble radius = sqrt(pow((endX-startX), 2) + pow((endY-startY), 2));

	painterPointer->drgeo_cairo_circle (cr, surface, startX, startY, radius);

	g_signal_handlers_disconnect_by_func (widget, (gpointer)drgeo_motion_event, NULL);
	gdk_window_set_cursor( win, NULL );

	/* Invalidate the complete widget for now*/
	gtk_widget_queue_draw (widget);
}

void paint_arc (GtkWidget *widget, gdouble startX, gdouble startY, gdouble midX, gdouble midY,
                gdouble endX, gdouble endY)
{
	cairo_t *cr;

	gdouble cx, cy, radius, startA, endA, x, y, m, x4, y4;
	gdouble mid12x, mid12y,mid23x,mid23y,slope12,slope23,slopePerp12,slopePerp23, co1,co2;
	bool direction = false;

	mid12x = ((startX+midX)/2);
	mid12y = ((startY+midY)/2);

	mid23x = ((midX+endX)/2);
	mid23y = ((midY+endY)/2);

	slope12 = (midY-startY)/(midX-startX); /* Slope of line between point 1 and 2 */
	slope23 = (endY-midY)/(endX-midX);  /* Slope of line between point 2 and 3 */

	slopePerp12 = -(1/slope12);  /* Slope of perpendicular 1 */
	slopePerp23 = -(1/slope23);  /* Slope of perpendicular 1 */

	co1 = mid12y -(slopePerp12*mid12x);  /* Constant 1 */
	co2 = mid23y -(slopePerp23*mid23x);  /* Constant 2 */

	cx = (co1 - co2)/(slopePerp23 - slopePerp12); /* X-coordinate of Center point */
	cy = (slopePerp12 * cx) + co1;  /* Y-coordinate of Center point */

	radius = sqrt(pow((cx-startX), 2) + pow((cy-startY), 2));  /* Radius of an Arc */

	startA = atan2((startY-cy),(startX-cx));  /* Starting Angle */
	endA = atan2 ((endY-cy),(endX-cx)) ;  /* Ending Angle */

	x=midX-startX;
	y4=startY-endY;
	x4=startX-endX;
	m=(((x)*(y4))/(x4))+startY;

	if(midY<m)
	{
		if(startX<endX)
		{
			direction = false;
		}
		else if(startX>endX)
		{
			direction = true;
		}
	}
	else if(midY>m)
	{
		if (startX<endX)
		{
			direction = true;
		}
		else if (startX>endX)
		{
			direction = false;
		}
	}

	painterPointer->drgeo_cairo_arc (cr, surface, cx, cy, radius, startA, endA, direction);

	g_signal_handlers_disconnect_by_func (widget, (gpointer)drgeo_motion_event, NULL);
	gdk_window_set_cursor( win, NULL );
	/* Now invalidate the affected region of the drawing area. */
	gtk_widget_queue_draw (widget);
}

gboolean
drgeo_configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer user_data)
{
	if (surface)
		cairo_surface_destroy (surface);

	surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
	                                             CAIRO_CONTENT_COLOR,
	                                             gtk_widget_get_allocated_width (widget),
	                                             gtk_widget_get_allocated_height (widget));

	/* Initialize the surface to white */
	set_surface ();

	/* Initialize gdkwindow and cursor */
	win = gtk_widget_get_window (widget);
	cur = gdk_cursor_new (GDK_PENCIL);

	/* We've handled the configure event, no need for further processing. */
	return TRUE;
}

gboolean 
drgeo_draw_event (GtkWidget *widget, cairo_t *cr,  gpointer user_data)
{      
	painterPointer->drgeo_cairo_background (cr, surface);
	return FALSE;
}

gboolean 
drgeo_clicked_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
	/* paranoia check, in case we haven't gotten a configure event */
	if (surface == NULL)
		return FALSE;

	if (event->button == 1)
	{
		if (entity == 1) /* Point entity is selected */
		{
			paint_point (widget, event->x, event->y);
		}
		if (entity > 1) /* Line entity is selected */
		{
			if (mFirstClick && mExist)
			{
				startX = setX;
				startY = setY;
				mFirstClick = false;
				mSecondClick = true;
				mExist = false;
			}
			else if (!mFirstClick && mSecondClick && mExist)
			{
				endX = setX;
				endY = setY;
				mSecondClick = false;
				mExist = false;
				if (entity == 2)
					paint_line (widget, startX, startY, endX, endY);
				else if (entity == 3)
					paint_circle (widget, startX, startY, endX, endY);
				else if(entity == 4)
					mThirdClick = true;
			}
			else if(!mFirstClick && !mSecondClick && mThirdClick && mExist)
			{
				thirdX = setX;
				thirdY = setY;
				mThirdClick = false;
				mExist = false;
				paint_arc (widget, startX, startY, endX, endY, thirdX, thirdY);
			}
		}
	}
	/* We've handled the event, stop processing */
	return TRUE;
}

gboolean 
drgeo_motion_event(GtkWidget *widget, GdkEventMotion *event, gpointer user_data)
{      
	check_coord (event->x, event->y);

	return FALSE;
}

/* Checks if the point already exists */
void
check_coord (gdouble dx, gdouble dy)
{
	for (it = point.begin(); it != point.end(); ++it)
	{ 
		gdouble it_x, it_y;
		it_x = it->getValueX();
		it_y = it->getValueY();

		if( (int (it_x-3) <= int (dx) ) && ( int (dx) <= int (it_x+3) ) 
		   && ( int (it_y-3) <= int (dy) ) && ( int (dy) <= int (it_y+3) ) )
		{
			setX = it_x;
			setY = it_y;
			mExist = true;
			gdk_window_set_cursor( win, cur );

			std::cout<<"match found"<<std::endl;
		}
		else
			gdk_window_set_cursor( win, NULL );
	}
}