// drgeoDrawingArea.cpp
//
// Copyright (C) 2014 - Gurjot Singh <bhattigurjot@gmail.com>
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

#include "drgeoDrawingArea.h"

drgeoDrawingArea:: drgeoDrawingArea()
{
#ifndef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
	//Connect the signal handler if it isn't already a virtual method override:
	signal_draw().connect(sigc::mem_fun(*this, &drgeoDrawingArea::on_draw), false);
#endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
}

drgeoDrawingArea:: ~drgeoDrawingArea()
{
}

bool drgeoDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	// demo drawing
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	// coordinates for the center of the window
	int xc, yc;
	xc = width / 2;
	yc = height / 2;

	cr->set_source_rgb(1.0, 1.0, 1.0);   // white background
	cr->paint();

	cr->set_line_width(2.0);

	// draw red lines out from the center of the window
	cr->set_source_rgb(0.0, 0.26, 0.26);
	cr->move_to(0, 0);
	cr->line_to(xc, yc);
	cr->line_to(0, height);
	cr->move_to(xc, yc);
	cr->line_to(width, yc);
	cr->stroke();

	return true;
}
