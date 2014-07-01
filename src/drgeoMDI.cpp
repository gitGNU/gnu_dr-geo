// drgeoMDI.cpp
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

#include <drgeoMDI.h>
#include <iostream>

drgeoMDI:: drgeoMDI(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade):
Gtk::Window(cobject), builder(refGlade)
{
	Gtk::Window* main_win = 0;
	builder->get_widget("main_window", main_win);
	main_win->signal_delete_event().connect( sigc::mem_fun(*this, &drgeoMDI::on_drgeo_delete_event) );
}

drgeoMDI::~drgeoMDI()
{
}

bool 
drgeoMDI::on_drgeo_delete_event(GdkEventAny* event)
{
	std::cout<<"Dr. Geo quits!\n";
	true;

}
