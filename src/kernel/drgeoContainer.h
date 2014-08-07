/*
 * drgeoContainer.h
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

#ifndef DRGEOCONTAINER_H
#define DRGEOCONTAINER_H

class drgeoContainer
{
  double _x;
  double _y;
  
  public:
    drgeoContainer ();
    ~drgeoContainer ();
    void setValue (double x, double y);
    double getValueX ();
    double getValueY ();
};

#endif /* DRGEOCONTAINER_H */