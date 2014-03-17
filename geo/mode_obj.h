/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-1998
 * hilaire.fernandes@iname.com 
 * 
 *
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Geoeral Public Licences as by published
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

#ifndef mode_obj_h
#define mode_obj_h

#include "liste_elem.h"
#include "drgeo_geometricObject.h"

//Caracteristiques du tableau de codage des modes de creation d'une figure
#define NB_MODE_MAX 10
#define NB_OBJET_PAR_MODE_MAX 5


// Description des modes de creations
extern objectCategory mode1[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode2[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode3[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode4[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode5[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode6[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode7[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode8[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode9[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode10[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode11[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode12[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode13[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode14[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode15[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode16[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode17[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode18[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode19[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

extern objectCategory mode20[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];

// Dummy mode for polygon and script
extern objectCategory mode21[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];


// Declarations des classes de creation d'objets
// Classe de base de creation de figure
class buildParametersListForGeometricObject
{
public:
  buildParametersListForGeometricObject
    (objectCategory tmode[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX])
  {
    init_class (tmode);
  }
  void init_class (objectCategory tmode[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX])
  {
    int a, b;
    buildMode = tmode;
    for (a = 0; a < NB_MODE_MAX; a++)
      {
	for (b = 0; b < NB_OBJET_PAR_MODE_MAX; b++)
	  mode[a][b] = NO_OBJECT;
	cas_possible[a] = VALID;
      }
  }
  virtual int inserer_figure (liste_elem & selection,
			      geometricObject * new_fig);

  // Check if fig can be used
  virtual int utilisable (geometricObject * fig);

  // Test fi fig can be used and is not in the selection list
  virtual int utilisable1 (geometricObject * fig, liste_elem & selection);

  virtual void reset (void) = 0;

  unsigned int mode[NB_MODE_MAX][NB_OBJET_PAR_MODE_MAX];
  objectCategory (*buildMode)[NB_OBJET_PAR_MODE_MAX];
  char cas_possible[NB_MODE_MAX];
  char nb_mode;
};

// Classe de creation de point libre sur objet
#define NB_MOD 1
class buildParametersListForPoint:public buildParametersListForGeometricObject
{
public:
  buildParametersListForPoint ():buildParametersListForGeometricObject (mode1)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    nb_mode = NB_MOD;
    init_class (mode1);
  }
};

// Classe de creation de droite
#undef NB_MOD
#define NB_MOD 3
class buildParametersListForLine:public buildParametersListForGeometricObject
{
public:
  buildParametersListForLine ():buildParametersListForGeometricObject (mode2)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode2);
    nb_mode = NB_MOD;
  }
};

// Classe de creation de demi-droite
#undef NB_MOD
#define NB_MOD 1
class buildParametersListForHalfLine:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForHalfLine ():buildParametersListForGeometricObject
    (mode3)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode3);
    nb_mode = NB_MOD;
  }
};

// Classe de creation de segment
#undef NB_MOD
#define NB_MOD 1
class buildParametersListForSegment:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForSegment ():buildParametersListForGeometricObject
    (mode4)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode4);
    nb_mode = NB_MOD;
  }
};

// Classe de creation de cercle
#undef NB_MOD
#define NB_MOD 3
class buildParametersListForCircle:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForCircle ():buildParametersListForGeometricObject
    (mode5)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode5);
    nb_mode = NB_MOD;
  }
};

// Classe de creation de point_inter
#undef NB_MOD
#define NB_MOD 1
class buildParametersListForIntersectionPoint:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForIntersectionPoint
    ():buildParametersListForGeometricObject (mode6)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode6);
    nb_mode = NB_MOD;
  }
};

// Classe de creation de droite_parallele
#undef NB_MOD
#define NB_MOD 1
class buildParametersListForParallelLine:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForParallelLine ():buildParametersListForGeometricObject
    (mode7)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode7);
    nb_mode = NB_MOD;
  }
};

// Classe de creation de droite_orthogonale
#undef NB_MOD
#define NB_MOD 1
class buildParametersListForOrthogonalLine:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForOrthogonalLine
    ():buildParametersListForGeometricObject (mode8)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode8);
    nb_mode = NB_MOD;
  }
};

// Classe de creation de transforme par reflexion
#undef NB_MOD
#define NB_MOD 8
class buildParametersListForReflexion:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForReflexion ():buildParametersListForGeometricObject
    (mode9)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode9);
    nb_mode = NB_MOD;
  }
};

// Classe de creation de transforme par symetrie
#undef NB_MOD
#define NB_MOD 8
class buildParametersListForSymmetry:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForSymmetry ():buildParametersListForGeometricObject
    (mode10)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode10);
    nb_mode = NB_MOD;
  }
};

// Classe de creation de transforme par translation
#undef NB_MOD
#define NB_MOD 8
class buildParametersListForTranslation:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForTranslation ():buildParametersListForGeometricObject
    (mode11)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode11);
    nb_mode = NB_MOD;
  }
};

// Classe de creation de transforme par rotation
#undef NB_MOD
#define NB_MOD 8
class buildParametersListForRotation:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForRotation ():buildParametersListForGeometricObject
    (mode12)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode12);
    nb_mode = NB_MOD;
  }
};

// Classe de creation de valeur numerique
#undef NB_MOD
#define NB_MOD 8
class buildParametersListForNumeric:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForNumeric ():buildParametersListForGeometricObject
    (mode13)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode13);
    nb_mode = NB_MOD;
  }
};

// Classe de creation d'angle
#undef NB_MOD
#define NB_MOD 2
class buildParametersListForAngle:public buildParametersListForGeometricObject
{
public:
  buildParametersListForAngle ():buildParametersListForGeometricObject
    (mode14)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode14);
    nb_mode = NB_MOD;
  }
};

// Classe de creation de coordonnees de points,vecteur, equation de droite et cercle
#undef NB_MOD
#define NB_MOD 4
class buildParametersListForCoordinate:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForCoordinate ():buildParametersListForGeometricObject
    (mode15)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode15);
    nb_mode = NB_MOD;
  }
};

// Classe de creation d'un point a partir de ces coordonnees
#undef NB_MOD
#define NB_MOD 1
class buildParametersListForCoordinatePoint:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForCoordinatePoint
    ():buildParametersListForGeometricObject (mode16)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode16);
    nb_mode = NB_MOD;
  }
};

// Classe de creation d'un point milieu d'un segemnt
#undef NB_MOD
#define NB_MOD 2
class buildParametersListForMiddlePoint:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForMiddlePoint ():buildParametersListForGeometricObject
    (mode17)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode17);
    nb_mode = NB_MOD;
  }
};

// Classe de creation d'un lieu geometrique d'un point
#undef NB_MOD
#define NB_MOD 1
class buildParametersListForLocus:public buildParametersListForGeometricObject
{
public:
  buildParametersListForLocus ():buildParametersListForGeometricObject
    (mode18)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode18);
    nb_mode = NB_MOD;
  }
};

// Classe de creation d'un arc de cercle
#undef NB_MOD
#define NB_MOD 1
class buildParametersListForArcCircle:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForArcCircle ():buildParametersListForGeometricObject
    (mode19)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode19);
    nb_mode = NB_MOD;
  }
};

// Class of creation of scaled object
#undef NB_MOD
#define NB_MOD 8
class buildParametersListForScale:public buildParametersListForGeometricObject
{
public:
  buildParametersListForScale ():buildParametersListForGeometricObject
    (mode20)
  {
    nb_mode = NB_MOD;
  }
  void reset (void)
  {
    init_class (mode20);
    nb_mode = NB_MOD;
  }
};


// Class of creation of polygon object
// For polygon object we do not use the traditionnal
// mechanism
#undef NB_MOD
#define NB_MOD 1
class buildParametersListForPolygon:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForPolygon ():buildParametersListForGeometricObject
    (mode21)
  {
    nb_mode = NB_MOD;
  }
  int inserer_figure (liste_elem & selection, geometricObject * new_fig);

  // Check if fig can be used
  int utilisable (geometricObject * fig);

  // Test fi fig can be used and is not in the selection list
  int utilisable1 (geometricObject * fig, liste_elem & selection);

  void reset (void)
  {
  }
};

// Class of creation of script object
// For script object we do not use the traditionnal
// mechanism
#undef NB_MOD
#define NB_MOD 1
class buildParametersListForScript:public
  buildParametersListForGeometricObject
{
public:
  buildParametersListForScript ():buildParametersListForGeometricObject
    (mode21)
  {
    nb_mode = NB_MOD;
  }
  int inserer_figure (liste_elem & selection, geometricObject * new_fig);

  // Check if fig can be used
  int utilisable (geometricObject * fig);

  // Test fi fig can be used and is not in the selection list
  int utilisable1 (geometricObject * fig, liste_elem & selection);

  void reset (void)
  {
  }
};

#endif
