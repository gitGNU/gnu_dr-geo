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

#ifndef drgeo_type_h
#define drgeo_type_h

typedef struct vecteur_s
{
  double x, y;
}
vecteur_s;
vecteur_s operator + (vecteur_s v1, vecteur_s v2);	// v1+v2

vecteur_s operator - (vecteur_s v1);	// -v1

vecteur_s operator - (vecteur_s v1, vecteur_s v2);	// v1-v2

double operator * (vecteur_s v1, vecteur_s v2);	// v1*v2

vecteur_s operator * (vecteur_s v1, double k);	// v1*k

vecteur_s operator * (double k, vecteur_s v1);	// k*v1

vecteur_s operator / (vecteur_s v1, double k);	// v1/k

bool operator == (vecteur_s v1, vecteur_s v2);	// v1 == v2

#define point_s vecteur_s

/*#define drgeoCoord vecteur_s
 */

typedef struct droite_s
{
  point_s a, b;
}
droite_s;
typedef struct param_droite
{
  double a, b, c;
}
param_droite;


// Nombre complexe
class Complex
{
public:
  Complex (void)
  {
  }
  Complex (double x, double y)
  {
    r = x;
    i = y;
  }
  double r, i;
};

Complex operator * (Complex z, double k);
Complex operator * (double k, Complex z);
Complex operator / (Complex z, double k);
Complex operator / (double k, Complex z);
Complex operator * (Complex z1, Complex z2);
Complex operator + (Complex z1, Complex z2);
Complex operator - (Complex z1, Complex z2);
Complex operator / (Complex z1, Complex z2);
double imag (Complex z);
double real (Complex z);
Complex conj (Complex z);


#endif
