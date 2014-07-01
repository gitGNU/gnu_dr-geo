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

	//Load the Glade file and instantiate its widgets
	Glib::RefPtr<Gtk::Builder> builder;
	builder = Gtk::Builder::create_from_file(UI_FILE);

	drgeoMDI *main_win = 0;
	builder->get_widget_derived("main_window", main_win);

	return app->run(*main_win);
}

