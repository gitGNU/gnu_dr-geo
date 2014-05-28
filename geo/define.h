/*
 *  Dr Genius an interactive geometry software
 * (C) Copyright Free Software Foundation  1997-1998
 * hilaire@ofset.org
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

#ifndef define_h
#define define_h

// Gettext macro
#define  _(x)  gettext (x)
#define N_(x)  x


// Epsilon
#define EPSILON (10e-10)

#define PINFINIT (10e20)
#define NINFINIT (-10e20)

// Max size of a macro name ( not the filename)
#define MACRO_NAME_WIDTH 50
// size of the macro text info buffer
#define MACRO_TEXT_INFO 256

// size of nom and nom_type buffers of the object class
#define LONGUEUR_NOM 10
#define LONGUEUR_NOM_TYPE 50

// Here the possible modes form the toolbars
enum toolbarmode
{
  POINT_MODE,
  POINT_MILIEU_MODE,
  POINT_INTER_MODE,
  POINT_REPERE_MODE,
  DROITE_MODE,
  DEMI_DROITE_MODE,
  SEGMENT_MODE,
  VECTEUR_MODE,
  CERCLE_MODE,
  ARC_CERCLE_MODE,
  LIEU_POINT_MODE,
  DROITE_PARALLELE_MODE,
  DROITE_ORTHOGONALE_MODE,
  REFLEXION_MODE,
  SYMETRIE_MODE,
  TRANSLATION_MODE,
  ROTATION_MODE,
  HOMOTHETIE_MODE,
  NUMERIQUE_MODE,
  ANGLE_MODE,
  COORDONNEES_MODE,
  POLYGON_MODE,
  SCRIPT_MODE,
  MACRO_ENREGISTRE_MODE,	// record the input object
  MACRO_ENREGISTREF_MODE,	// record the output objects
  MACRO_PLAY_MODE,
  SOURIS_SELECT_MODE,
  MISE_EN_FORME_MODE,
  PROPERTY_MODE,
  OPTION_DRGEO_MODE,
  EFFACE_OBJET_MODE,
  SAUVEGARDE_MODE,
  OUVERTURE_MODE,
  NOUVEAU_MODE,
  PRINTING_AREA_MODE,
  ANIMATION_MODE
};

/* used to select an object for construction. A non zero value after a
   bitwise AND with the category attribute of an object indicate this
   object belongs to the selected category */

enum objectCategory
{
  NO_OBJECT = 0,
  CONSTRAINED_PT = 1,
  FREE_PT = 2,
  FREE_PT_ON_CURVE = 4,
  LINE = 8,
  HALF_LINE = 16,
  SEGMENT = 32,
  VECTOR = 64,
  CIRCLE = 128,
  ARC_CIRCLE = 256,
  LOCUS = 512,
  REPERE = 1024,
  ANGLE = 2048,
  NUMERIC = 4096,
  EQUATION = 8192,
  POLYGON = 16384,
  SCRIPT = 32768,
  /* Grouped categories */
  POINT = CONSTRAINED_PT + FREE_PT + FREE_PT_ON_CURVE,
  DIRECTION = LINE + HALF_LINE + SEGMENT + VECTOR,
  CURVE = LINE + HALF_LINE + SEGMENT + CIRCLE + ARC_CIRCLE + LOCUS,
  VALUE = ANGLE + NUMERIC + SCRIPT
};

#define NO_TYPE 0;

enum transformationType
{
  ROTATION = 1,
  SYMMETRY = 2,
  REFLEXION = 4,
  TRANSLATION = 8,
  SCALE = 16,

  /* Group transformation */
  TRANSFORMATION = ROTATION + SYMMETRY + REFLEXION + TRANSLATION + SCALE
};

enum pointType
{
  COORDINATE = 32,
  FREE = 64,
  ON_CURVE = 128,
  INTERSECTION = 256,
  MIDDLE_SEGMENT = 512,
  MIDDLE_2PTS = 1024
};

enum repereType
{
  THREE_PTS = 32,
  PT_2VECTORS = 64
};

enum vectorType
{
  VECTOR_2PTS = 32
};

enum lineType
{
  LINE_2PTS = 32,
  LINE_PT_VA = 64,
  LINE_PAR_DIRECTION = 128,
  LINE_PER_DIRECTION = 256
};

enum halfLineType
{
  HALFLINE_2PTS = 32
};

enum segmentType
{
  SEGMENT_2PTS = 32
};

enum circleType
{
  CIRCLE_2PTS = 32,
  CIRCLE_SEGMENT = 64,
  CIRCLE_VALUE = 128
};

enum arcCircleType
{
  ARCCIRCLE_3PTS = 32
};

enum angleType
{
  ANGLE_3PTS = 1,
  ANGLE_2VEC = 2
};

enum numericType
{
  SEGMENT_LENGTH = 1,
  VECTOR_NORM = 2,
  DISTANCE_2PTS = 4,
  DISTANCE_PT_CIRCLE = 8,
  DISTANCE_PT_LINE = 16,
  CIRCLE_PERIMETER = 32,
  LINE_SLOPE = 64,
  ARC_CIRCLE_LENGTH = 128,
  FREE_VALUE = 256,
  POINT_ABSCISSA = 512,
  POINT_ORDINATE = 1024,
  VECTOR_ABSCISSA = 2048,
  VECTOR_ORDINATE = 4096
};

enum equationType
{
  LINE_EQUATION = 1,
  CIRCLE_EQUATION = 2
};

enum polygonType
{
  POLYGON_NPTS = 32
};

enum scriptType
{
  SCRIPT_NITEMS = 1
};

//Flag pour cas_possible
#define NOT_VALID 0
#define VALID 1

// type d'unite des classe angle
#define DEGRES 1
#define GRADE 2
#define RADIAN 3


#define LOCUS_SAMPLE 200


#endif
