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

#include "drgeoMDI.h"
#include "drgeoDrawingArea.h"
#include <iostream>

drgeoMDI:: drgeoMDI()
: m_vBox1(Gtk::ORIENTATION_VERTICAL)
{	
	set_title("Dr. Geo");
	set_default_size(700, 500);

	add(m_vBox1); // put a MenuBar at the top of the box and other stuff below it.

	//Create actions for menus and toolbars:
	m_refActionGroup = Gtk::ActionGroup::create();

	//File menu:
	m_refActionGroup->add(Gtk::Action::create("FileMenu", "File"));
	m_refActionGroup->add(Gtk::Action::create("FileNew", Gtk::Stock::NEW),
	                      sigc::mem_fun(*this, &drgeoMDI::on_new));
	m_refActionGroup->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN),
	                      sigc::mem_fun(*this, &drgeoMDI::on_open));
	m_refActionGroup->add(Gtk::Action::create("FileSave", Gtk::Stock::SAVE),
	                      sigc::mem_fun(*this, &drgeoMDI::on_save));
	m_refActionGroup->add(Gtk::Action::create("FileSaveAs", Gtk::Stock::SAVE_AS),
	                      sigc::mem_fun(*this, &drgeoMDI::on_save_as));
	m_refActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
	                      sigc::mem_fun(*this, &drgeoMDI::on_quit));

	//Edit menu:
	m_refActionGroup->add(Gtk::Action::create("EditMenu", "Edit"));
	m_refActionGroup->add(Gtk::Action::create("EditUndo", Gtk::Stock::UNDO),
	                      sigc::mem_fun(*this, &drgeoMDI::on_undo));
	m_refActionGroup->add(Gtk::Action::create("EditRedo", Gtk::Stock::REDO),
	                      sigc::mem_fun(*this, &drgeoMDI::on_redo));
	m_refActionGroup->add(Gtk::Action::create("EditCut", Gtk::Stock::CUT),
	                      sigc::mem_fun(*this, &drgeoMDI::on_cut));
	m_refActionGroup->add(Gtk::Action::create("EditCopy", Gtk::Stock::COPY),
	                      sigc::mem_fun(*this, &drgeoMDI::on_copy));
	m_refActionGroup->add(Gtk::Action::create("EditPaste", Gtk::Stock::PASTE),
	                      sigc::mem_fun(*this, &drgeoMDI::on_paste));
	m_refActionGroup->add(Gtk::Action::create("EditDelete", Gtk::Stock::DELETE),
	                      sigc::mem_fun(*this, &drgeoMDI::on_delete));

	//View menu:
	m_refActionGroup->add( Gtk::Action::create("ViewMenu", "View") );

	//Draw menu:
	m_refActionGroup->add( Gtk::Action::create("DrawMenu", "Draw") );

	//Help menu:
	m_refActionGroup->add( Gtk::Action::create("HelpMenu", "Help") );
	m_refActionGroup->add( Gtk::Action::create("HelpAbout", Gtk::Stock::HELP),
	                      sigc::mem_fun(*this, &drgeoMDI::on_about));


	m_refUIManager = Gtk::UIManager::create();
	m_refUIManager->insert_action_group(m_refActionGroup);

	add_accel_group(m_refUIManager->get_accel_group());

	//Layout the actions in a menubar and toolbar:
	Glib::ustring ui_info = 
		"<ui>"
		"  <menubar name='MenuBar'>"
		"    <menu action='FileMenu'>"
		"      <menuitem action='FileNew'/>"
		"      <menuitem action='FileOpen'/>"
		"      <menuitem action='FileSave'/>"
		"      <menuitem action='FileSaveAs'/>"
		"      <separator/>"
		"      <menuitem action='FileQuit'/>"
		"    </menu>"
		"    <menu action='EditMenu'>"
		"      <menuitem action='EditUndo'/>"
		"      <menuitem action='EditRedo'/>"
		"      <separator/>"
		"      <menuitem action='EditCut'/>"
		"      <menuitem action='EditCopy'/>"
		"      <menuitem action='EditPaste'/>"
		"      <menuitem action='EditDelete'/>"
		"    </menu>"
		"    <menu action='ViewMenu'>"
		"    </menu>"
		"    <menu action='DrawMenu'>"
		"    </menu>"
		"    <menu action='HelpMenu'>"
		"      <menuitem action='HelpAbout'/>"
		"    </menu>"
		"  </menubar>"
		"</ui>";

	try
	{
		m_refUIManager->add_ui_from_string(ui_info);
	}
	catch(const Glib::Error& ex)
	{
		std::cerr << "building menus failed: " <<  ex.what();
	}

	//Get the menubar and add it to a container widget:
	Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
	if(pMenubar)
		m_vBox1.pack_start(*pMenubar, Gtk::PACK_SHRINK);

	drgeoDrawingArea *area = new drgeoDrawingArea;
	m_vBox1.pack_start(*area, Gtk::PACK_EXPAND_WIDGET);

	show_all_children();
}

drgeoMDI:: ~drgeoMDI()
{
}

void drgeoMDI::on_new()
{
	std::cout << "New menu item was selected." << std::endl;
}

void drgeoMDI::on_open()
{
	std::cout << "Open menu item was selected." << std::endl;
}

void drgeoMDI::on_save()
{
	std::cout << "Save menu item was selected." << std::endl;
}

void drgeoMDI::on_save_as()
{
	std::cout << "Save_As menu item was selected." << std::endl;
}

void drgeoMDI::on_quit()
{
	std::cout << "Quit menu item was selected." << std::endl;
	hide();
}

void drgeoMDI::on_undo()
{
	std::cout << "Undo menu item was selected." << std::endl;
}

void drgeoMDI::on_redo()
{
	std::cout << "Redo menu item was selected." << std::endl;
}

void drgeoMDI::on_cut()
{
	std::cout << "Cut menu item was selected." << std::endl;
}

void drgeoMDI::on_copy()
{
	std::cout << "Copy menu item was selected." << std::endl;
}

void drgeoMDI::on_paste()
{
	std::cout << "Paste menu item was selected." << std::endl;
}

void drgeoMDI::on_delete()
{
	std::cout << "Delete menu item was selected." << std::endl;
}

void drgeoMDI::on_about()
{
	std::cout << "About menu item was selected." << std::endl;
}

