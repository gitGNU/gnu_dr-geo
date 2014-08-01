/*
 * drgeoTDI.h
 *
 * Copyright (C) 2014 Gurjot Singh <bhattigurjot@gmail.com>
	 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
	 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DRGEOTDI_H
#define DRGEOTDI_H

#include <gtk/gtk.h>
#include "drgeoDrawingArea.h"

class drgeoTDI
{
	public:
		drgeoTDI ();
		virtual ~drgeoTDI ();

		void newTab ();
		void closeTab ();
		void setSensitivity ();

	protected:
		GtkWidget *mainWindow;
		GtkBox *box;
		GtkNotebook *tdiView;
		GtkWidget *drawArea;
		GtkWidget *label;
		drgeoDrawingArea *drawAreaPointer;

		/**Menu Items**/
		GtkWidget *closeMenuItem;

		bool newFlag;
		gint cPage;
};

/**Signal handlers**/
//Define C-style functions as callbacks
extern "C" 
{
	void on_new (GtkWidget *widget, gpointer user_data);
	void on_open (GtkWidget *widget, gpointer user_data);
	void on_save (GtkWidget *widget, gpointer user_data);
	void on_save_as (GtkWidget *widget, gpointer user_data);
    void on_close (GtkWidget *widget, gpointer user_data);
	void on_quit (GtkWidget *widget, gpointer user_data);
	void on_undo (GtkWidget *widget, gpointer user_data);
	void on_redo (GtkWidget *widget, gpointer user_data);
	void on_cut (GtkWidget *widget, gpointer user_data);
	void on_copy (GtkWidget *widget, gpointer user_data);
	void on_paste (GtkWidget *widget, gpointer user_data);
	void on_delete (GtkWidget *widget, gpointer user_data);
	void on_about (GtkWidget *widget, gpointer user_data);	
}

#endif /* DRGEOTDI_H */
