/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-1999
 * hilaire.fernandes@iname.com 
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

#ifndef macro_h
#define macro_h

#include <string.h>
#include <glib.h>
#include "define.h"
#include "liste_elem.h"
#include "drgeo_geometricObject.h"

class noyau_macro;

// Declaration des types de bases noyau_macro et macro
// structure de liste_parent
// 0            nb_parent
// 4            numero parent 1
// 8            numero parent 2
// ..     ..
class noyau_macro
{
  friend class macro;
public:
    noyau_macro * suivant;

  objectCategory category;
  int type, *liste_parent;
  // type is a subType from category, not used for input parameters

  void *fig;
  // adresse de la figure correspondant au noyau lors de la creation
  // de la macro utile lors de la creation de la macro et de son
  // fonctionnement pour stocker l'@ de l'objet cree a partir de ce
  // noyau

  int param_f;
  // Flag pour signifier si ce noyau represente  une figure finale

  void * extra;
  // parametre supplementaire pour pts intersection cercle et xxx

    noyau_macro (void)
  {
    suivant = (noyau_macro *) NULL;
    liste_parent = (int *) NULL;
  }
};

class macro
{
public:
  macro (char *name, char *description, int pi, int pf);
   ~macro (void);

  int ajoute (int *parent, int type, objectCategory category,
	      void *objet, int flag, void * extra);
  // retourne la position d'insertion ou l'emplacement si le noyau existe deja

  int position (void *fig);

  // cherche la position de fig dans les noyaux (utile lors de la
  // creation de la macro) 

  void init_lire (void);
  // retourne l'@ du noyau de la pos eme
  noyau_macro *lire (int pos);
  void vide (void);

  bool play (class drgeoFigure * aFigure);
  char *getName ();
  char *getDescription ();
  void setDescription (char *description);
  int getInputParameterNumber ()
  {
    return param_i;
  }
  int getOutputParameterNumber ()
  {
    return param_f;
  }
  int getKernelNumber ()
  {
    return nb_noyau;
  }

private:
  geometricObject * createItem (liste_elem & l_p, noyau_macro * n,
				gboolean createdFromMacro,
				liste_elem * figureList);
  liste_elem & findParent (int *l, class liste_elem & l_p);

  // Private data structures.

  noyau_macro *debut, *courant, *lecture;

  // Version number for the macro.
  int version;

  // Number of input parameters, number of output parameters for the
  // macro.  And total number of items involved in the macro.
  int param_i, param_f, nb_noyau;

  // Name and text information about the macro.
  char nom[MACRO_NAME_WIDTH];
  char info[MACRO_TEXT_INFO];

};

class drgeoMacroRegistry
{
public:
  ~drgeoMacroRegistry ();

  // This method is used to retrieve the unique drgeoMacroRegistry
  // instance.
  static drgeoMacroRegistry *get ();
  macro *find (const gchar *aName);
  void add (macro * aMacro);
  bool load (xmlNodePtr macroXml);
  bool save (const gchar *macroName, xmlNodePtr macroXml);
  macro *first ();
  bool isDone ();
  macro *next ();

private:
  // This constructor is private to restrict instanciation, and force
  // the use of the get() method to retrieve the unique instance fo
  // this class.
    drgeoMacroRegistry ();

  static drgeoMacroRegistry *registry;
  liste_elem list;
  int index;			// Used for first(), isDone() and next().

};

inline
macro::macro (char *name, char *description, int pi, int pf)
{
  int l;
  lecture = debut = courant = new (noyau_macro);
  param_i = pi;
  param_f = pf;
  nb_noyau = 0;
  // Check string size
  l = strlen (name);
  if (l >= MACRO_NAME_WIDTH)
    l = MACRO_NAME_WIDTH - 1;
  if (name)
    {
      strncpy (nom, name, l);
      nom[l] = 0;
    }
  l = strlen (description);
  if (l >= MACRO_TEXT_INFO)
    l = MACRO_TEXT_INFO - 1;
  if (description)
    {
      strncpy (info, description, l);
      info[l] = 0;
    }

// FXME:        version = MAC_VERSION;
}

inline
macro::~
macro (void)
{
  noyau_macro *p;
  while (debut->suivant != NULL)
    {
      p = debut->suivant;
      delete (debut);
      debut = p;
    }
  delete (debut);
}

inline char *
macro::getName ()
{
  return (nom);
}

inline char *
macro::getDescription ()
{
  return (info);
}

inline void
macro::setDescription (char *description)
{
  int l;

  l = strlen (description);
  if (l >= MACRO_TEXT_INFO)
    l = MACRO_TEXT_INFO - 1;
  if (description)
    {
      strncpy (info, description, l);
      info[l] = 0;
    }
}

#endif
