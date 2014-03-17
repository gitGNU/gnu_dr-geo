/*
 *  Dr Geo an interactive geometry software
 * (C) Copyright Hilaire Fernandes  2000
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

#include "config.h"
#include "drgeo_locus.h"
#include "drgeo_point.h"
#include "drgeo_drawable.h"
#include "drgeo_gtkdrawable.h"
#include "traite.h"		/* FIXME : move the method updateItems as a static method in drgeo_locus.* */


enum locusState
{
  onMax,
  onMin,
  normal
};

locus::locus (liste_elem & parents, gboolean createdFromMacro, liste_elem * figureList):
geometricObject (createdFromMacro, figureList)
{
  style.color = preferedColor (":locusColor");
  style.thick = preferedThickness (":locusStyle");

  category = LOCUS;
  /* FREE_POINT_ON_CURVE - CONSTRAINED_POINT */
  parents.init_lire ();
  parentList = g_list_append (parentList,
			      (gpointer) searchForCategory (parents,
							    FREE_PT_ON_CURVE));
  parents.init_lire ();
  parentList = g_list_append (parentList,
			      (gpointer) searchForCategory (parents,
							    CONSTRAINED_PT));

  if (getFirst == NULL || getLast == NULL)
    exist = false;

  initName ();
}

locus::locus (xmlNodePtr tree, GHashTable * itemIdToAdress, liste_elem * figureList):
geometricObject (tree, figureList)
{
  xmlNodePtr item;
  void *obj;
  category = LOCUS;

  /* get the parent ref */
  item = xml_search_next (tree->xmlChildrenNode, "parent");
  while (item)
    {
      if ((obj = xmlInsertParent (item, itemIdToAdress)) == NULL)
	exit (1);		/* FIXME: implement a nicer way */
      parentList = g_list_append (parentList, obj);
      item = xml_search_next (item->next, "parent");
    }
  initName ();
}

void
locus::draw (drgeoDrawable & area, char force)
{
  gint n;
  if ((style.mask == alway) || (style.mask == yes && force == FALSE)
      || !exist)
    return;

  // if two points are two far from each other, do not connect
  // them
  for (n = 0; n < sampleNumber - 1; n++)
    {
      if ((p[n] - p[n + 1]).squareNorm () < 4)
	area.drawSegment (style, p[n], p[n + 1]);
    }
}

void
locus::update (drgeoDrawable & area)
{
  static drgeoPoint unity (1, 1), areaCenter, areaSize;;
  drgeoPoint Pmax, Pmin;
  gint n, locusIndex, freePointIndex;
  gdouble x, x0, step, step0, stepMin, stepMax, d;
  locusState state;
  point *free, *constrained;
  gboolean *update;


  free = (point *) getFirst;
  constrained = (point *) getLast;

  if (free == NULL || constrained == NULL)
    {
      exist = false;
      return;
    }

  exist = parentExist ();
  if (!exist)
    return;


  x0 = free->getAbscissa ();
  step0 = 0.001;  // 0.005
  stepMin = step0 / (2 * 2 * 2 * 2);
  stepMax = step0 * 2 * 2 * 2 * 2;
  step = stepMin;

  locusIndex = figureList->position ((void *) constrained);
  freePointIndex = figureList->position ((void *) free);
  
  if (locusIndex < freePointIndex)
    {
      exist = false;
      return;
    }
  update = new gboolean [locusIndex - freePointIndex + 1];
  for (n = 0; n <= locusIndex - freePointIndex; n++)
    update[n] = false;

  needUpdate (constrained, freePointIndex, update, figureList);

  areaSize = area.getAreaSize ();
  areaCenter = area.getAreaCenter ();
  Pmax = areaCenter + areaSize / 2 + unity;
  Pmin = areaCenter - areaSize / 2 - unity;

//      printf ("Pmax (%f ; %f)\n",Pmax.getX(), Pmax.getY());
//      printf ("Pmin (%f ; %f)\n",Pmin.getX(), Pmin.getY());

  // search for the first point on the screen
  x = 0;
  free->setAbscissa (x);
  updateItems (area, figureList, freePointIndex, locusIndex, FALSE, update);
  p[0] = constrained->getCoordinate ();

  while ((x < 1.) &&
	 (!constrained->objectExist () || (p[0] << Pmin || p[0] >> Pmax)))
    {
      x += stepMax;
      free->setAbscissa (x);
      updateItems (area, figureList, freePointIndex, locusIndex, FALSE, update);
      p[0] = constrained->getCoordinate ();
    };


  if (x > 1.)
    {
      /* the locus doesn't exist */
      exist = FALSE;
      free->setAbscissa (x0);
      updateItems (area, figureList, freePointIndex, locusIndex, FALSE, update);
      return;
    }

  // We got a first sample, is there another one closer, let's go back ?
  if (x > 0.)
    {
      step = stepMin;
      p[1] = p[0];
      do
	{
	  p[2] = p[1];
	  x -= step;
	  free->setAbscissa (x);
	  updateItems (area, figureList, freePointIndex, locusIndex, FALSE, update);
	  p[1] = constrained->getCoordinate ();

	}
      while (constrained->objectExist () && p[1] > Pmin && p[1] < Pmax);
      p[0] = p[2];
    }

  for (n = 1, state = normal; n < LOCUS_SAMPLE && x <= 1.; n++, x += step)
    {      
      free->setAbscissa (x);
      updateItems (area, figureList, freePointIndex, locusIndex, FALSE, update);
      if (!constrained->objectExist ())
	{
	  n--;
	  if (step > stepMin)
	    {
	      x -= step;
	      step /= 2;
	      state = onMax;
	    }
	  else
	    state = normal;
	  continue;
	}
      p[n] = constrained->getCoordinate ();

      // g_print ("p(%f ; %f), sample: %d, abscisse: %f",p[n].getX(), p[n].getY(), n, x);

      if (p[n] << Pmin || p[n] >> Pmax)
	{
	  // we are totaly off screen, skeep it
	  n--;
	  step = stepMax;
	  state = normal;
	}
      else if ((p[n - 1] - p[n]).squareNorm () > .2 && step > stepMin
	       && state != onMin)
	{
	  x -= step;
	  n--;
	  step /= 2;
	  state = onMax;
	}
      else if ((p[n - 1] - p[n]).squareNorm () < .04 && step < stepMax
	       && state != onMax)
	{
	  x -= step;
	  n--;
	  step *= 2;
	  state = onMin;
	}
      else
	{
	  /* not rejected so we take it */
	  state = normal;
	  step = step0;
	}
    }
/*
        // Take last sample
	if (n < LOCUS_SAMPLE) 
	{
		free->setAbscissa (1);
		updateItems (figureList, freePointIndex, locusIndex, FALSE,update);
		p[n + 1] = constrained->getCoordinate ();
		if (p[n + 1] > Pmin && p[n + 1] < Pmax)
			n ++;
	}
*/

  // g_print ("Echantillons : %d  ", n); 
  sampleNumber = n;
  free->setAbscissa (x0);
  updateItems (area, figureList, freePointIndex, locusIndex, FALSE, update);

  delete [] update;
}

gboolean
locus::overObject (drgeoPoint & mouse, gdouble range)
{
  gint n;
  if (!exist)
    return FALSE;
  for (n = 0; n < sampleNumber; n++)
    if ((p[n] - mouse).norm () < range)
      return TRUE;
  return FALSE;
}

void
locus::move (drgeoVector & t)
{
  /* FIXME: we can eventually move the curve to which
     beong the FREE_POINT_ON_CURVE point.
   */
}

void
locus::initName ()
{
  gchar vide[1] = { 0 };
  point *free, *constrained;
  geometricObject *curve;
  int l;
  if (style.mask == alway || !exist)
    return;

  g_free (typeName);

  free = (point *) getFirst;
  constrained = (point *) getLast;
  curve = (geometricObject *) (g_list_first (free->getParentList ())->data);

  /* compute the string length */
  l = strlen (_("%1's locus when %2 describes %3")) + 1;
  if (constrained->getName ()? strlen (constrained->getName ()) : 0)
    l += strlen (constrained->getName ());
  if (free->getName ()? strlen (free->getName ()) : 0)
    l += strlen (free->getName ());
  if (curve->getName ()? strlen (curve->getName ()) : 0)
    l += strlen (curve->getName ());

  typeName = (gchar *) g_malloc (l);
  strcpy (typeName, _("%1's locus when %2 describes %3"));
  /* insert the names when they exist */
  if (constrained->getName ())
    strinsmsg (typeName, constrained->getName (), "%1");
  else
    strinsmsg (typeName, vide, "%1");
  if (free->getName ())
    strinsmsg (typeName, free->getName (), "%2");
  else
    strinsmsg (typeName, vide, "%2");
  if (curve->getName ())
    strinsmsg (typeName, curve->getName (), "%3");
  else
    strinsmsg (typeName, vide, "%3");
}

void
locus::save (xmlNodePtr tree, liste_elem & figureList)
{
  xmlNodePtr item;
  GList *parent;

  parent = g_list_first (parentList);
  item = xmlNewChild (tree, NULL, BAD_CAST "locus", NULL);
  save_attribute (item, this, "None");

  while (parent)
    {
      xmlAddParent (item, (geometricObject *) parent->data);
      parent = g_list_next (parent);
    }
}
void
locus::updateDescription ()
{
  geometricObject::updateDescription ();

  description = new gchar *[3];
  description[0] = g_strdup_printf (_("Locus:: %s"), name);
  description[1] = g_strdup_printf (_("Free point on curve %s"),
				    getNthName (0));
  description[2] = g_strdup_printf (_("Constrained point %s"),
				    getNthName (1));
}

drgeoPoint & locus::getPointAt (gdouble abscissa)
{
  static drgeoPoint
    m;

  if (abscissa == 1.0)
    m = p[sampleNumber - 1];
  else
    m = p[(gint) (abscissa * sampleNumber)];
  return m;
}

drgeoPoint & locus::getClosestPoint (drgeoPoint & p)
{
  gint n;
  gdouble distance, d;
  static drgeoPoint m;

  m = this->p[0];
  distance = (p - m).squareNorm ();

  for (n = 1; n < sampleNumber; n++)
    {
      d = (this->p[n] - p).squareNorm ();
      if (d < distance)
	{
	  m = this->p[n];
	  distance = d;
	}
    }
  return m;
}

gdouble
locus::getAbscissa (drgeoPoint & p)
{
  gint n, i;
  gdouble distance;

  distance = (p - this->p[0]).squareNorm ();
  i = 0;

  for (n = 1; n < sampleNumber; n++)
    if ((this->p[n] - p).squareNorm () < distance)
      {
	distance = (p - this->p[n]).squareNorm ();
	i = n;
      }
  return ((gdouble) i) / ((gdouble) sampleNumber);
}

gboolean
locus::getIntersection (geometricObject & c, drgeoPoint & p, gint k)
{
  gint n;
  gdouble distance, d;

  p = this->p[0];
  distance = (this->p[0] - c.getClosestPoint (this->p[0])).squareNorm ();
  
  for (n = 1; n < sampleNumber; n++) 
    {
      d = (c.getClosestPoint (this->p[n]) - this->p[n]).squareNorm ();
      if (d < distance)
	{
	  p = this->p[n];
	  distance = d;
	}
    }
  if ((p - c.getClosestPoint (p)).squareNorm () < 0.1)
    return true;
  else return false;
}
