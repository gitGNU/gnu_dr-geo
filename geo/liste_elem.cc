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

#include "liste_elem.h"

#define FALSE 0
#define TRUE 1

//Definition des fonctions membres de la classe liste chainee
liste_elem::liste_elem (liste_elem & l)
{
  int nb;
  lecture = debut = fin = new (elem);
  nb_elem = 0;
  l.init_lire ();
  nb = l.nb_elem;
  while (nb != 0)
    {
      ajoute (l.lire (0));
      nb--;
    }
}
int
liste_elem::ajoute (void *fig)
{
  fin->fig = fig;
  fin->suivant = new (elem);
  fin = fin->suivant;
  nb_elem++;
  return (nb_elem);
}

int
liste_elem::supprime (void *fig)
{
  elem *p, *prec;
  int position = 0;
  p = prec = debut;
  while (p->suivant != NULL)
    {
      position++;
      if (p->fig == fig)
	break;
      prec = p;
      p = p->suivant;
    }
  if (p->fig == fig)
    {
      if (p == debut)
	debut = p->suivant;
      else
	prec->suivant = p->suivant;
      delete (p);
      nb_elem--;
      return (position);
    }
  return (0);
}

int
liste_elem::position (void *fig)
{
  int position = 1;
  elem *p;
  p = debut;
  while (p->suivant != NULL)
    {
      if (p->fig == fig)
	break;
      position++;
      p = p->suivant;
    }
  if (p->fig == fig)
    return (position);
  return (0);
}

void
liste_elem::init_lire (void)
{
  lecture = debut;
}

void *
liste_elem::lire (int pos)
{
  void *fig;
  elem *p;
  if (pos == 0)
    {
      if (lecture != NULL)
	{
	  fig = lecture->fig;
	  lecture = lecture->suivant;
	  return (fig);
	}
      else
	{
	  lecture = debut;
	  return (NULL);
	}
    }
  else
    {
      p = debut;
      if (pos > nb_elem)
	return NULL;
      for (int a = 1; a != pos; a++)
	{
	  p = p->suivant;
	  if (p == NULL)
	    return (NULL);
	}
      return (p->fig);
    }
}
void
liste_elem::vide (void)
{
//      vide la liste chain‚e
  elem *p, *q;
  p = debut->suivant;
  while (p != NULL)
    {
      q = p->suivant;
      delete (p);
      p = q;
    }
  lecture = fin = debut;
  debut->fig = NULL;
  debut->suivant = NULL;
  nb_elem = 0;
}

// Fonctions assocciees a liste_elem
char
contenue (liste_elem & l1, liste_elem & l2)
{
  char rep = TRUE;
  int nb1 = l1.nb_elem, nb2 = l2.nb_elem;
  if (nb1 > nb2)
    return FALSE;
  l1.init_lire ();
  while (nb1 != 0 && rep)
    {
      if (l2.position (l1.lire (0)) == 0)
	rep = FALSE;
      nb1--;
    }
  return rep;
}

liste_elem & concatene (liste_elem & l1, liste_elem & l2)
{
  int nb = l2.nb_elem;
  void *fig;
  l2.init_lire ();
  while (nb != 0)
    {
      fig = l2.lire (0);
      if (l1.position (fig) == 0)
	l1.ajoute (fig);
      nb--;
    }
  return l1;
}


liste_elem & substract (liste_elem & l1, liste_elem & l2)
{
  int nb = l2.nb_elem;
  void *fig;
  l2.init_lire ();
  while (nb != 0)
    {
      fig = l2.lire (0);
      if (l1.position (fig))
	l1.supprime (fig);
      nb--;
    }
  return l1;
}

char
liste_elem_egale (liste_elem & l1, liste_elem & l2)
{
  int nb1 = l1.nb_elem, nb2 = l2.nb_elem;
  void *fig1, *fig2;
  if (nb1 != nb2)
    return FALSE;
  l1.init_lire ();
  l2.init_lire ();
  while (nb1-- != 0)
    {
      fig1 = l1.lire (0);
      fig2 = l2.lire (0);
      if (fig1 != fig2)
	return FALSE;
    }
  return TRUE;
}
