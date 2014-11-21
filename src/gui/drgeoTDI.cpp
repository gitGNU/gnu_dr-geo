// drgeoTDI.cpp
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

#include "drgeoTDI.h"
#include <iostream>

extern drgeoTDI *tdiPointer;
drgeoDrawingArea *drawAreaPointer;

drgeoTDI::drgeoTDI ()
{	
	newFlag = false;
	GError* error = NULL;
	GtkBuilder *builder = gtk_builder_new();
	
	if (!gtk_builder_add_from_file (builder, DRGEO_UI_DIR "/drgeoMain.ui", &error))
	{
		g_warning ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}
	gtk_builder_connect_signals (builder, &error);
	mainWindow = GTK_WIDGET (gtk_builder_get_object (builder, "drgeoMainWindow"));
	box = GTK_BOX (gtk_builder_get_object (builder, "box1"));
	saveMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "save"));
	saveAsMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "save_as"));
	undoMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "undo"));
	redoMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "redo"));
	cutMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "cut"));
	copyMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "copy"));
	pasteMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "paste"));
	deleteMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "delete"));
	pointMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "point"));
	lineMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "line"));
	circleMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "circle"));
	arcMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "arc"));
	closeMenuItem = GTK_WIDGET (gtk_builder_get_object (builder, "close"));
	
    gtk_widget_set_size_request(mainWindow, 800, 500);
	gtk_widget_show_all (mainWindow);
	setSensitivity ();
}

drgeoTDI::~drgeoTDI ()
{
}

void 
drgeoTDI::newTab ()
{
	static gint tabCount = 1;
	/* Creates Drawing Area */
	drawAreaPointer = new drgeoDrawingArea ();
	drawArea = drawAreaPointer->createDrawArea ();

	if(!newFlag)
	{
		/* Creates Notebook, i.e TDI */
		tdiView = GTK_NOTEBOOK (gtk_notebook_new ());
		gtk_notebook_set_tab_pos (tdiView, GTK_POS_TOP);

		gtk_box_pack_start (box, GTK_WIDGET (tdiView), TRUE, TRUE, 0);
		gtk_box_reorder_child (box, GTK_WIDGET (tdiView), 1);
		gtk_widget_show (GTK_WIDGET (tdiView));

		newFlag = true;
	}
	/* Creates Label */
	label = gtk_label_new (g_strdup_printf (("Document %d"), tabCount++));
	/* Get Current Page */
	cPage = gtk_notebook_get_current_page (tdiView);
	/* Insert Page */
	gtk_notebook_insert_page (tdiView, drawArea, label, cPage + 1);
	gtk_notebook_next_page (GTK_NOTEBOOK (tdiView));
}

void 
drgeoTDI::closeTab ()
{
	/* Get Current Page */
	cPage = gtk_notebook_get_current_page (tdiView);
	/* Remove Page */
	gtk_notebook_remove_page (tdiView, cPage);
}

void 
drgeoTDI::setSensitivity ()
{
	if(!newFlag)
	{
		gtk_widget_set_sensitive (saveMenuItem, FALSE);
		gtk_widget_set_sensitive (saveAsMenuItem, FALSE);
		gtk_widget_set_sensitive (undoMenuItem, FALSE);
		gtk_widget_set_sensitive (redoMenuItem, FALSE);
		gtk_widget_set_sensitive (cutMenuItem, FALSE);
		gtk_widget_set_sensitive (copyMenuItem, FALSE);
		gtk_widget_set_sensitive (pasteMenuItem, FALSE);
		gtk_widget_set_sensitive (deleteMenuItem, FALSE);
		gtk_widget_set_sensitive (pointMenuItem, FALSE);
		gtk_widget_set_sensitive (lineMenuItem, FALSE);
		gtk_widget_set_sensitive (circleMenuItem, FALSE);
		gtk_widget_set_sensitive (arcMenuItem, FALSE);
		gtk_widget_set_sensitive (closeMenuItem, FALSE);
	}
	else
	{
		gtk_widget_set_sensitive (saveMenuItem, TRUE);
		gtk_widget_set_sensitive (saveAsMenuItem, TRUE);
		gtk_widget_set_sensitive (undoMenuItem, TRUE);
		gtk_widget_set_sensitive (redoMenuItem, TRUE);
		gtk_widget_set_sensitive (cutMenuItem, TRUE);
		gtk_widget_set_sensitive (copyMenuItem, TRUE);
		gtk_widget_set_sensitive (pasteMenuItem, TRUE);
		gtk_widget_set_sensitive (deleteMenuItem, TRUE);
		gtk_widget_set_sensitive (pointMenuItem, TRUE);
		gtk_widget_set_sensitive (lineMenuItem, TRUE);
		gtk_widget_set_sensitive (circleMenuItem, TRUE);
		gtk_widget_set_sensitive (arcMenuItem, TRUE);
		gtk_widget_set_sensitive (closeMenuItem, TRUE);
	}
}

void 
on_new (GtkWidget *widget, gpointer user_data)
{
	tdiPointer->newTab ();
	tdiPointer->setSensitivity ();
	std::cout << "New menu item was selected." << std::endl;
}

void 
on_open (GtkWidget *widget, gpointer user_data)
{
	std::cout << "Open menu item was selected." << std::endl;
}

void 
on_save (GtkWidget *widget, gpointer user_data)
{
	std::cout << "Save menu item was selected." << std::endl;
}

void 
on_save_as (GtkWidget *widget, gpointer user_data)
{
	std::cout << "Save_As menu item was selected." << std::endl;
}

void 
on_close (GtkWidget *widget, gpointer user_data)
{
    tdiPointer->closeTab ();
    std::cout << "Close menu item was selected." << std::endl;
}

void 
on_quit (GtkWidget *widget, gpointer user_data)
{
	std::cout << "Quit menu item was selected." << std::endl;
	gtk_main_quit ();
}

void 
on_undo (GtkWidget *widget, gpointer user_data)
{
	std::cout << "Undo menu item was selected." << std::endl;
}

void 
on_redo (GtkWidget *widget, gpointer user_data)
{
	std::cout << "Redo menu item was selected." << std::endl;
}

void 
on_cut (GtkWidget *widget, gpointer user_data)
{
	std::cout << "Cut menu item was selected." << std::endl;
}

void 
on_copy (GtkWidget *widget, gpointer user_data)
{
	std::cout << "Copy menu item was selected." << std::endl;
}

void 
on_paste (GtkWidget *widget, gpointer user_data)
{
	std::cout << "Paste menu item was selected." << std::endl;
}

void 
on_delete (GtkWidget *widget, gpointer user_data)
{
	std::cout << "Delete menu item was selected." << std::endl;
}

void 
on_point (GtkWidget *widget, gpointer user_data)
{
    drawAreaPointer->drawEntity (1);
	std::cout << "Point menu item was selected." << std::endl;
}

void 
on_line (GtkWidget *widget, gpointer user_data)
{
    drawAreaPointer->drawEntity (2);
	std::cout << "Line menu item was selected." << std::endl;
}

void 
on_circle (GtkWidget *widget, gpointer user_data)
{
    drawAreaPointer->drawEntity (3);
	std::cout << "Circle menu item was selected." << std::endl;
}

void 
on_arc (GtkWidget *widget, gpointer user_data)
{
    drawAreaPointer->drawEntity (4);
	std::cout << "Arc menu item was selected." << std::endl;
}

void 
on_about (GtkWidget *widget, gpointer user_data)
{
	GtkWidget *aboutDialog;
	GError* error = NULL;
	GtkBuilder *builderAbout = gtk_builder_new();

	if (!gtk_builder_add_from_file (builderAbout, DRGEO_UI_DIR "/drgeoAbout.ui", &error))
	{
		g_warning ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}
	gtk_builder_connect_signals (builderAbout, &error);
	aboutDialog = GTK_WIDGET (gtk_builder_get_object (builderAbout, "drgeoAboutDialog"));

	gtk_widget_show (aboutDialog);
	std::cout << "About menu item was selected." << std::endl;
}
