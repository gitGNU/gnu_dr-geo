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

#ifndef liste_elem_h
#define liste_elem_h

#include <stdio.h>

// Declaration de la classe liste chainee
// Attention : les objets pointes de la listes doivent etre detruits
// explicitement
// ---------    avant la destruction de la liste
class elem
{
  friend class liste_elem;
public:
    elem * suivant;
  void *fig;
    elem (void)
  {
    fig = NULL;
    suivant = (elem *) NULL;
  }
};
class liste_elem
{
public:
  elem * debut, *fin, *lecture;
  int nb_elem;
    liste_elem (void)
  {
    lecture = debut = fin = new (elem);
    nb_elem = 0;
  }
   ~liste_elem (void)
  {
    elem *p;
    while (debut->suivant != NULL)
      {
	p = debut->suivant;
	delete (debut);
	debut = p;
      }
    delete (debut);
  }
  liste_elem (liste_elem & l);
  int ajoute (void *fig);
  int supprime (void *fig);
  int position (void *fig);
  void init_lire (void);
  void *lire (int pos);
  void vide (void);
};

// Fonctions assocciees a liste_elem

// l1 inclue dans l2 ? (suppose sans repetition)
char contenue (liste_elem & l1, liste_elem & l2);
// concatene l1,l2 dans l1 sans repetition
liste_elem & concatene (liste_elem & l1, liste_elem & l2);
// substract l2 to l1
liste_elem & substract (liste_elem &l1, liste_elem &l2);
// l1 et l2 sont-elles egales ?
char liste_elem_egale (liste_elem & l1, liste_elem & l2);



#endif
