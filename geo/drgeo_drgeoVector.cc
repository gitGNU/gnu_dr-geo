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

#include <math.h>
#include "drgeo_drgeoVector.h"

drgeoVector::drgeoVector ()
{
  x = 0;
  y = 0;
}

drgeoVector::drgeoVector (double x, double y)
{
  this->x = x;
  this->y = y;
}

drgeoVector::drgeoVector (drgeoVector & v)
{
  *this = v;
}

drgeoVector & drgeoVector::operator - ()
{
  static drgeoVector
    ve;

  ve.x = -x;
  ve.y = -y;
  return ve;
}

drgeoVector & drgeoVector::operator + (const drgeoVector & v)
{
  static drgeoVector
    ve;

  ve.x = v.x + x;
  ve.y = v.y + y;
  return ve;
}

drgeoVector & drgeoVector::operator - (const drgeoVector & v)
{
  static drgeoVector
    ve;

  ve.x = x - v.x;
  ve.y = y - v.y;
  return ve;
}

drgeoVector & drgeoVector::operator * (gdouble k)
{
  static drgeoVector
    ve;

  ve.x = x * k;
  ve.y = y * k;
  return ve;
}

drgeoVector & drgeoVector::operator / (gdouble k)
{
  static drgeoVector
    ve;
  if (k == 0)
    k = 1;
  ve.x = x / k;
  ve.y = y / k;
  return ve;
}

gdouble
drgeoVector::operator * (const drgeoVector & v)
{
  return (x * v.x + y * v.y);
}

gdouble
drgeoVector::operator ^ (const drgeoVector & v)
{
  return (x * v.y - y * v.x);
}

drgeoVector & drgeoVector::operator = (const drgeoVector & v)
{
  x = v.x;
  y = v.y;
  return *this;
}

drgeoVector & drgeoVector::operator += (const drgeoVector & v)
{
  x += v.x;
  y += v.y;
  return *this;
}

drgeoVector & drgeoVector::operator -= (const drgeoVector & v)
{
  x -= v.x;
  y -= v.y;
  return *this;
}

drgeoVector & drgeoVector::operator *= (gdouble k)
{
  x *= k;
  y *= k;
  return *this;
}

drgeoVector & drgeoVector::operator /= (gdouble k)
{
  x /= k;
  y /= k;
  return *this;
}

gboolean
drgeoVector::operator == (drgeoVector & v)
{
  return (x == v.x && y == v.y);
}

gboolean
drgeoVector::operator < (drgeoVector & v)
{
  return (x < v.x && y < v.y);
}

gboolean
drgeoVector::operator <= (drgeoVector & v)
{
  return (x <= v.x && y <= v.y);
}

gboolean
drgeoVector::operator > (drgeoVector & v)
{
  return (x > v.x && y > v.y);
}

gboolean
drgeoVector::operator >= (drgeoVector & v)
{
  return (x >= v.x && y >= v.y);
}

gboolean
drgeoVector::operator << (drgeoVector & v)
{
  return (x < v.x || y < v.y);
}

gboolean
drgeoVector::operator <<= (drgeoVector & v)
{
  return (x <= v.x || y <= v.y);
}

gboolean
drgeoVector::operator >> (drgeoVector & v)
{
  return (x > v.x || y > v.y);
}

gboolean
drgeoVector::operator >>= (drgeoVector & v)
{
  return (x >= v.x || y >= v.y);
}


gboolean
drgeoVector::operator != (drgeoVector & v)
{
  return (x != v.x || y != v.y);
}

drgeoVector & operator * (gdouble k, drgeoVector & v)
{
  return v * k;
}

gdouble
drgeoVector::squareNorm ()
{
  return (x * x + y * y);
}

gdouble
drgeoVector::norm ()
{
  return sqrt (x * x + y * y);
}

void
drgeoVector::set (gdouble x, gdouble y)
{
  this->x = x;
  this->y = y;
}

void
drgeoVector::setX (gdouble x)
{
  this->x = x;
}

void
drgeoVector::setY (gdouble y)
{
  this->y = y;
}

gdouble
drgeoVector::getX ()
{
  return x;
}

gdouble
drgeoVector::getY ()
{
  return y;
}

drgeoVector & drgeoVector::normal ()
{
  static drgeoVector
    ve;
  ve.x = -y;
  ve.y = x;
  return ve;
}

drgeoVector & drgeoVector::rotate (gdouble a)
{
  static drgeoVector
    v;
  v.x = x * cos (a) - sin (a) * y;
  v.y = x * sin (a) + cos (a) * y;
  return v;
}

drgeoVector & drgeoVector::reflexion (drgeoVector & d)
{
  static drgeoVector
    v,
    d1,
    n;

  d1 = d / d.norm ();
  n = d1.normal ();
  v = (*this * d1) * d1;
  v -= (*this * n) * n;
  return v;
}



drgeoPoint & drgeoVector::rotatePoint (drgeoPoint & c, gdouble a)
{
  static drgeoPoint
    r;

  r = (*this - c).rotate (a) + c;
  return r;
}

drgeoPoint & drgeoVector::reflexionPoint (drgeoPoint & p, drgeoVector & u)
{
  static drgeoPoint
    r;

  r = (*this - p).reflexion (u) + p;
  return r;
}
