// drgeoMDI.h
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

#ifndef DRGEOMDI_H
#define DRGEOMDI_H

#include <gtkmm.h>

class drgeoMDI: public Gtk::Window
{
	public:
		drgeoMDI();
		virtual ~drgeoMDI();
		virtual Gtk::Window* get_window();

	protected:
		Gtk::Box m_Box;
		Glib::RefPtr<Gtk::UIManager> m_refUIManager;
		Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
		Gtk::Window* main_win;

	//Signal handlers:
		void on_new();
		void on_open();
		void on_save();
		void on_save_as();
		void on_quit();
		void on_undo();
		void on_redo();
		void on_cut();
		void on_copy();
		void on_paste();
		void on_delete();
		void on_about();		
};

#endif /* DRGEOMDI_H */
