/*
 *  Dr Genius an interactive geometry software
 * (C) Copyright Hilaire Fernandes  2000
 * hilaire@seul.org 
 * 
 *
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public Licences as by published
 * by the Free Software Foundation; either version 2; or (at your option)
 * any later version
 *
 * This program is distributed in the hope that it will entertaining,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILTY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Publis License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef DRGEO_DRGEOVECTOR_H
#define DRGEO_DRGEOVECTOR_H

#include <glib.h>

#define drgeoPoint drgeoVector
#define drgeoCoord drgeoVector

class drgeoVector
{
public:

  drgeoVector ();
  drgeoVector (double x, double y);
    drgeoVector (drgeoVector & v);
    drgeoVector (const drgeoVector & v);

    drgeoVector & operator - ();
    drgeoVector & operator + (const drgeoVector & v);
    drgeoVector & operator - (const drgeoVector & v);
    drgeoVector & operator * (gdouble k);
    drgeoVector & operator / (gdouble k);
  /* produit scalaire */
  gdouble operator * (const drgeoVector & v);
  /* determinant */
  gdouble operator ^ (const drgeoVector & v);

    drgeoVector & operator = (const drgeoVector & v);

    drgeoVector & operator += (const drgeoVector & v);
    drgeoVector & operator -= (const drgeoVector & v);
    drgeoVector & operator *= (gdouble k);
    drgeoVector & operator /= (gdouble k);

  gboolean operator == (drgeoVector & v);
  gboolean operator != (drgeoVector & v);

  gboolean operator < (drgeoVector & v);
  gboolean operator <= (drgeoVector & v);
  gboolean operator > (drgeoVector & v);
  gboolean operator >= (drgeoVector & v);

  gboolean operator << (drgeoVector & v);
  gboolean operator <<= (drgeoVector & v);
  gboolean operator >> (drgeoVector & v);
  gboolean operator >>= (drgeoVector & v);


  gdouble norm ();
  gdouble squareNorm ();

  void setX (gdouble x);
  void setY (gdouble y);
  void set (gdouble x, gdouble y);
  gdouble getX ();
  gdouble getY ();

    drgeoVector & normal ();
    drgeoVector & rotate (gdouble a);
    drgeoVector & reflexion (drgeoVector & d);

    drgeoPoint & rotatePoint (drgeoPoint & c, gdouble a);
    drgeoPoint & reflexionPoint (drgeoPoint & p, drgeoVector & u);

private:
    gdouble x, y;
};

drgeoVector & operator * (gdouble k, drgeoVector & v);

#endif /* DRGEO_DRGEOVECTOR_H */
