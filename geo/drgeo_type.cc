
/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-1999
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

#include "drgeo_type.h"

// Definition des operateurs
vecteur_s
operator + (vecteur_s v1, vecteur_s v2)
{
// v1+v2
  vecteur_s r;
  r.x = v1.x + v2.x;
  r.y = v1.y + v2.y;
  return r;
}

vecteur_s
operator - (vecteur_s v1)
{
// -v1
  vecteur_s r;
  r.x = -v1.x;
  r.y = -v1.y;
  return r;
}

vecteur_s
operator - (vecteur_s v1, vecteur_s v2)
{
// v1-v2
  vecteur_s r;
  r.x = v1.x - v2.x;
  r.y = v1.y - v2.y;
  return r;
}

double
operator * (vecteur_s v1, vecteur_s v2)
{
// v1*v2
  double r;
  r = v1.x * v2.x + v1.y * v2.y;
  return r;
}

vecteur_s
operator * (vecteur_s v1, double k)
{
// v1*k
  vecteur_s r;
  r.x = k * v1.x;
  r.y = k * v1.y;
  return r;
}

vecteur_s
operator * (double k, vecteur_s v1)
{
// k*v1
  vecteur_s r;
  r.x = k * v1.x;
  r.y = k * v1.y;
  return r;
}

vecteur_s
operator / (vecteur_s v1, double k)
{
// v1/k
  vecteur_s r;
  r.x = v1.x / k;
  r.y = v1.y / k;
  return r;
}

bool
operator == (vecteur_s v1, vecteur_s v2)
{
  return (v1.x == v2.x && v1.y == v2.y);
}


// provide facilities to manipulate complexe numbers
Complex
operator * (Complex z, double k)
{
  z.r *= k;
  z.i *= k;
  return z;
}

Complex
operator * (double k, Complex z)
{
  z.r *= k;
  z.i *= k;
  return z;
}

Complex
operator / (Complex z, double k)
{
  z.r /= k;
  z.i /= k;
  return z;
}

Complex
operator / (double k, Complex z)
{
  Complex zz;
  zz.r = k * z.r / (z.r * z.r - z.i * z.i);
  zz.r = k * z.i / (z.r * z.r - z.i * z.i);
  return zz;
}

Complex
operator * (Complex z1, Complex z2)
{
  Complex z;
  z.r = z1.r * z2.r - z1.i * z2.i;
  z.i = z1.r * z2.i + z1.i * z2.r;
  return z;
}

Complex
operator + (Complex z1, Complex z2)
{
  Complex z;
  z.r = z1.r + z2.r;
  z.i = z1.i + z2.i;
  return z;
}

Complex
operator - (Complex z1, Complex z2)
{
  Complex z;
  z.r = z1.r - z2.r;
  z.i = z1.i - z2.i;
  return z;
}

Complex
operator / (Complex z1, Complex z2)
{
  Complex z;
  double r;
  r = z2.r * z2.r + z2.i * z2.i;
  z.r = (z1.r * z2.r + z1.i * z2.i) / r;
  z.i = (z2.r * z1.i - z1.r * z2.i) / r;
  return z;
}

double
imag (Complex z)
{
  return z.i;
}

double
real (Complex z)
{
  return z.r;
}

Complex
conj (Complex z)
{
  z.i = -z.i;
  return z;
}
