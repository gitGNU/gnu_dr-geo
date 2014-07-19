// drgeoTDI.cpp
//
// Copyright (C) 2014 Gurjot Singh <bhattigurjot@gmail.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
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

drgeoTDI:: drgeoTDI()
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
	mainWindow = GTK_WIDGET (gtk_builder_get_object(builder, "drgeoMainWindow"));
	box = GTK_BOX (gtk_builder_get_object(builder, "box1"));

	gtk_widget_show_all (mainWindow); 
}

drgeoTDI:: ~drgeoTDI()
{
}

void drgeoTDI:: newTab()
{
	static gint tabCount = 1;
	//Creates Drawing Area
	drawAreaPointer = new drgeoDrawingArea();
	drawArea = drawAreaPointer->createDrawArea();

	if(!newFlag)
	{
		//Creates Notebook, i.e TDI
		tdiView = GTK_NOTEBOOK(gtk_notebook_new());
		gtk_notebook_set_tab_pos (tdiView, GTK_POS_TOP);

		gtk_box_pack_start(box, GTK_WIDGET(tdiView), TRUE, TRUE, 0);
		gtk_box_reorder_child (box, GTK_WIDGET(tdiView), 1);
		gtk_widget_show (GTK_WIDGET(tdiView));

		newFlag = true;
	}
	//Creates Label
	label = gtk_label_new (g_strdup_printf (("Document %d"), tabCount++));
	//Get Current Page
	cPage = gtk_notebook_get_current_page (tdiView);
	//Insert Page
	gtk_notebook_insert_page (tdiView, drawArea, label, cPage + 1);
	gtk_notebook_next_page (GTK_NOTEBOOK (tdiView));
}

void on_new(GtkWidget *widget, gpointer user_data)
{
	tdiPointer->newTab();
	std::cout << "New menu item was selected." << std::endl;
}

void on_open(GtkWidget *widget, gpointer user_data)
{
	std::cout << "Open menu item was selected." << std::endl;
}

void on_save(GtkWidget *widget, gpointer user_data)
{
	std::cout << "Save menu item was selected." << std::endl;
}

void on_save_as(GtkWidget *widget, gpointer user_data)
{
	std::cout << "Save_As menu item was selected." << std::endl;
}

void on_quit(GtkWidget *widget, gpointer user_data)
{
	std::cout << "Quit menu item was selected." << std::endl;
	gtk_main_quit();
}

void on_undo(GtkWidget *widget, gpointer user_data)
{
	std::cout << "Undo menu item was selected." << std::endl;
}

void on_redo(GtkWidget *widget, gpointer user_data)
{
	std::cout << "Redo menu item was selected." << std::endl;
}

void on_cut(GtkWidget *widget, gpointer user_data)
{
	std::cout << "Cut menu item was selected." << std::endl;
}

void on_copy(GtkWidget *widget, gpointer user_data)
{
	std::cout << "Copy menu item was selected." << std::endl;
}

void on_paste(GtkWidget *widget, gpointer user_data)
{
	std::cout << "Paste menu item was selected." << std::endl;
}

void on_delete(GtkWidget *widget, gpointer user_data)
{
	std::cout << "Delete menu item was selected." << std::endl;
}

void on_about(GtkWidget *widget, gpointer user_data)
{
	std::cout << "About menu item was selected." << std::endl;
}
