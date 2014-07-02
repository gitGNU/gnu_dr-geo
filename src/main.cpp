/*
 * main.cc
 * Copyright (C) 2014 Gurjot Singh <bhattigurjot@gmail.com>
 * 
 * drgeo is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * drgeo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtkmm.h>
#include <drgeoMDI.h>
/*#include "config.h"

#ifdef ENABLE_NLS
#include <libintl.h>
#endif*/

#define UI_FILE "src/mainWindow.ui"
   
int
main (int argc, char *argv[])
{
	Glib::RefPtr<Gtk::Application> app =
    Gtk::Application::create(argc, argv,
      "dr.geo");

	drgeoMDI win;
	Gtk::Window *appWindow = win.get_window();	
	app->run(*appWindow);
}

