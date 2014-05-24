/* Modification de drdeo_latexdrawable.cc
 *
 * Version 0.0
 * (C) Copyright Frédéric Bonnaud 2000
 * tux.le.pengouin@free.fr
 *
 * This is just a hack of the latex filter
 * Goal : providing an eps export filter
 *        to use drgeo's picture in staroffice
 */


/*
 *  Dr Genius an interactive geometry software
 * (C) Copyright Hilaire Fernandes  1997-2000
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

#include <stdio.h>
#include <cmath>
#include "couleur.h"
#include "drgeo_drgeoStyle.h"
#include "drgeo_postscriptdrawable.h"
#include <time.h>

/* This is an arbitrary value */
static gdouble PostScriptScale = (25.0 * 11 / 9.7);
/* We should ask the user the size he wants his picture to be */

/* This are arbitrary values */
static gdouble PostScriptDotSize[3] = { 4.0, 5.0, 6.0 };

/* This are arbitrary values */
static gchar const *postscriptLineWidth[4] =
  { "%% set line style to Invisible\n0 setlinewidth\n",
  "%% set line style to Dashed\n0.4 setlinewidth [3] 0 setdash\n",
  "%% set line style to Normal\n0.8 setlinewidth [] 0 setdash\n",
  "%% set line style to Thick\n1.4 setlinewidth [] 0 setdash\n"
};

/* We should parse couleur.h to find the right values, but for now
   values are right !
   this should be normalize toward 1 values
*/
static gchar const *postscriptColor[DRGEO_NUMBER_COLOR] =
  { "%% set color to black\n0 0 0 setrgbcolor\n",
  "%% set color to dark gray\n0.35294117647 0.35294117647 0.35294117647 setrgbcolor\n",
  "%% set color to gray\n0.66666666667 0.66666666667 0.66666666667 setrgbcolor\n",
  "%% set color to white\n1 1 1 setrgbcolor\n",
  "%% set color to green\n0 0.92156862745 0 setrgbcolor\n",
  "%% set color to dark green\n0 0.39215686275 0 setrgbcolor\n",
  "%% set color to dark blue\n0.17647058824 0.21960784314 1 setrgbcolor\n",
  "%% set color to blue\n0.3137254902 0.69411764706 1 setrgbcolor\n",
  "%% set color to bordeaux\n0.56862745098 0 0 setrgbcolor\n",
  "%% set color to red\n0.92156862745 0 0 setrgbcolor\n",
  "%% set color to orange\n1 0.6 0.16862745098 setrgbcolor\n",
  "%% set color to yellow\n1 0.94117647059 0.12941176471 setrgbcolor\n"
};

drgeoPostScriptDrawable::
drgeoPostScriptDrawable (const gchar * name, drgeoFigure * figure,
			 FILE * fileHandle, drgeoPoint origin,
			 drgeoPoint size, gdouble scale)
{
  FILE * fileEncode;
  char line[500];

  this->figure = figure;
  this->fileHandle = fileHandle;
  this->origin = origin;
  this->size = size;
  this->scale = scale;

  /* We don't draw frame in eps picture, because most type setter can
     do that very easily */
  fprintf (fileHandle,
	   "%%!PS-Adobe-2.0 EPSF-1.2\n%%%%Title: %s\n%%%%Creator: drgenius (fred's ps exporter)\n",
	   name); 

  /* we need the date, is there a better way to do this : */
  time_t TIME;
  struct tm *LOCAL;
  time (&TIME);
  LOCAL = localtime (&TIME);
  char longdate[255];
  strftime (longdate, 254, "%A %d %B %Y", LOCAL);	/* for french people it's good, and for other ? */

  fprintf (fileHandle,
	   "%%%%CreationDate: %s\n%%%%Pages: 1\n%%%%DocumentFonts:\n%%%%BoundingBox: 0 0 %f %f\n",
	   longdate, size.getX () * PostScriptScale,
	   size.getY () * PostScriptScale);

  /* insert encoding definition */
  if (!(fileEncode = fopen (DRGEO_ENCODEDIR "/latin1.ps", "r")))
    g_printerr ("Can't open encode file "DRGEO_ENCODEDIR "/latin1.ps\n");
 else
   {
     while (fgets (line, 500, fileEncode))
       fwrite ((void *) line, 1, strlen (line), fileHandle);
   }
  fclose (fileEncode);
  fprintf (fileHandle, "\n");

  /* in order to be viewable (in staroffice for example), we need a preview, can drgeo give us
     this preview picture ? */
}

void
drgeoPostScriptDrawable::drawPoint (drgeoStyle & style, drgeoPoint & point)
{
  drgeoPoint p;

  p = areaToPostScript (point);

  /* we're initializing color and line width */
  fprintf (fileHandle, postscriptColor[style.color]);
  fprintf (fileHandle, postscriptLineWidth[drgeoThicknessNormal + 1]);

  switch (style.pointShape)
    {
    case drgeoPointRec:
      fprintf (fileHandle,
	       "%% PointRec\n%f %f %f %f rectfill\n",
	       p.getX () * PostScriptScale -
	       PostScriptDotSize[style.thick] / 2,
	       p.getY () * PostScriptScale -
	       PostScriptDotSize[style.thick] / 2,
	       PostScriptDotSize[style.thick],
	       PostScriptDotSize[style.thick]);
      break;
    case drgeoPointRecEmpty:
      fprintf (fileHandle,
	       "%% PointRec vide\n%f %f %f %f rectstroke\n",
	       p.getX () * PostScriptScale -
	       PostScriptDotSize[style.thick] / 2,
	       p.getY () * PostScriptScale -
	       PostScriptDotSize[style.thick] / 2,
	       PostScriptDotSize[style.thick],
	       PostScriptDotSize[style.thick]);
      break;
    case drgeoPointX:
      fprintf (fileHandle,
	       "%% PointX\n%f %f moveto %f %f lineto stroke\n%f %f moveto %f %f lineto stroke\n",
	       p.getX () * PostScriptScale -
	       PostScriptDotSize[style.thick] / 2,
	       p.getY () * PostScriptScale -
	       PostScriptDotSize[style.thick] / 2,
	       p.getX () * PostScriptScale +
	       PostScriptDotSize[style.thick] / 2,
	       p.getY () * PostScriptScale +
	       PostScriptDotSize[style.thick] / 2,
	       p.getX () * PostScriptScale -
	       PostScriptDotSize[style.thick] / 2,
	       p.getY () * PostScriptScale +
	       PostScriptDotSize[style.thick] / 2,
	       p.getX () * PostScriptScale +
	       PostScriptDotSize[style.thick] / 2,
	       p.getY () * PostScriptScale -
	       PostScriptDotSize[style.thick] / 2);
      break;
    case drgeoPointRound:
      fprintf (fileHandle,
	       "%% point rond\n%f %f moveto %f %f %f 0 360 arc fill\n",
	       p.getX () * PostScriptScale +
	       PostScriptDotSize[style.thick] / 2,
	       p.getY () * PostScriptScale, p.getX () * PostScriptScale,
	       p.getY () * PostScriptScale,
	       PostScriptDotSize[style.thick] / 2);
      break;
    case drgeoPointRoundEmpty:
      fprintf (fileHandle,
	       "%% point rond vide\n%f %f moveto %f %f %f 0 360 arc closepath stroke\n",
	       p.getX () * PostScriptScale +
	       PostScriptDotSize[style.thick] / 2,
	       p.getY () * PostScriptScale, p.getX () * PostScriptScale,
	       p.getY () * PostScriptScale,
	       PostScriptDotSize[style.thick] / 2);
      break;
    default:
      fprintf (fileHandle, "%% Default (this must be a bug in drgeo)\n");
      break;
    }
}

void
drgeoPostScriptDrawable::
drawPolygon (drgeoStyle & style, drgeoPoint * point, gint number)
{
  drgeoPoint p;
  gint nb;

  /* we're initializing color and line width */
  fprintf (fileHandle, postscriptColor[style.color]);
  /* Invisible line. */
  fprintf (fileHandle, postscriptLineWidth[0]);

  fprintf (fileHandle, "%%Polygon\nnewpath\n");

  p = areaToPostScript (point[0]) * PostScriptScale;
  fprintf (fileHandle, "%f %f moveto\n", p.getX (), p.getY ());
  for (nb = 1; nb < number; nb++)
    {
      p = areaToPostScript (point[nb]) * PostScriptScale;
      fprintf (fileHandle, "%f %f lineto\n", p.getX (), p.getY ());
    }
  p = areaToPostScript (point[0]) * PostScriptScale;
  fprintf (fileHandle, "%f %f moveto\n", p.getX (), p.getY ());
  fprintf (fileHandle, "closepath\nfill\n");
  //fprintf (fileHandle, "closepath\ngsave\nfill\ngrestore\n");
}

void
drgeoPostScriptDrawable::
drawSegment (drgeoStyle & style, drgeoPoint & start, drgeoPoint & end)
{
  drgeoPoint p1, p2;

  p2 = areaToPostScript (end);
  p1 = areaToPostScript (start);

  fprintf (fileHandle, postscriptColor[style.color]);
  if (style.mask == yes)
    fprintf (fileHandle, postscriptLineWidth[0]);
  else
    fprintf (fileHandle, postscriptLineWidth[style.thick + 1]);
  fprintf (fileHandle,
	   "%% Segment\n%f %f moveto %f %f lineto stroke\n",
	   p1.getX () * PostScriptScale, p1.getY () * PostScriptScale,
	   p2.getX () * PostScriptScale, p2.getY () * PostScriptScale);
}

void
drgeoPostScriptDrawable::
drawCircle (drgeoStyle & style, drgeoPoint & center, drgeoPoint & point)
{
  double radius;

  radius = (point - center).norm ();
  drawCircle (style, center, radius);
}

void
drgeoPostScriptDrawable::
drawCircle (drgeoStyle & style, drgeoPoint & center, double radius)
{
  drgeoPoint p;

  p = areaToPostScript (center);
  fprintf (fileHandle, postscriptColor[style.color]);
  if (style.mask == yes)
    fprintf (fileHandle, postscriptLineWidth[0]);
  else
    fprintf (fileHandle, postscriptLineWidth[style.thick + 1]);
  fprintf (fileHandle,
	   /* is there a better way to draw a circle in ps ? */
	   "%% Circle\n%f %f moveto %f %f %f 0 360 arc closepath stroke\n",
	   p.getX () * PostScriptScale + radius * PostScriptScale,
	   p.getY () * PostScriptScale, p.getX () * PostScriptScale,
	   p.getY () * PostScriptScale, radius * PostScriptScale);
}

void
drgeoPostScriptDrawable::

drawArc (drgeoStyle & style, drgeoPoint & center, double radius,
	 double start, double length)
{
  drgeoPoint p, startPoint;

  p = areaToPostScript (center);
  fprintf (fileHandle, postscriptColor[style.color]);
  if (style.mask == yes)
    fprintf (fileHandle, postscriptLineWidth[0]);
  else
    fprintf (fileHandle, postscriptLineWidth[style.thick + 1]);



  if (length > 0)
    startPoint.set (radius * cos (start), radius * sin (start));
  else
    startPoint.set (radius * cos (start + length),
		    radius * sin (start + length));

  startPoint += p;
  startPoint *= PostScriptScale;

  if (length > 0)
    fprintf (fileHandle,
	     "%% Arc\n%f %f moveto %f %f %f %f %f arc stroke\n",
	     startPoint.getX (), startPoint.getY (),
	     p.getX () * PostScriptScale, p.getY () * PostScriptScale,
	     radius * PostScriptScale, start * 180.0 / M_PI,
	     (start + length) * 180.0 / M_PI);
  else
    fprintf (fileHandle,
	     "%% Arc\n%f %f moveto %f %f %f %f %f arc stroke\n",
	     startPoint.getX (), startPoint.getY (),
	     p.getX () * PostScriptScale, p.getY () * PostScriptScale,
	     radius * PostScriptScale, (start + length) * 180.0 / M_PI,
	     start * 180.0 / M_PI);

}

void
drgeoPostScriptDrawable::
drawText (drgeoPoint & where, char *text, drgeoColorType fontColor)
{
  drgeoPoint p;
  gchar *convString;

  if (!text)
    return;
  p = areaToPostScript (where);
  if (p.getX () <= 0. || p.getY () <= 0.
      || p.getX () > size.getX () || p.getY () > size.getY ())
    return;

  // FIXME: how to handle this correctly ?
  convString = g_convert (text, -1, "iso8859-1", "utf-8", NULL, NULL, NULL);

  fprintf (fileHandle, postscriptColor[fontColor]);
  /* The size of the font is arbitrary : 10 points */
  fprintf (fileHandle,
	   "%% Text\n/Times findfont 10 scalefont setfont %f %f moveto (%s) show\n",
	   p.getX () * PostScriptScale, p.getY () * PostScriptScale, convString);
  g_free (convString);
}

/* private */
drgeoPoint
drgeoPostScriptDrawable::areaToPostScript (drgeoPoint p)
{
  return p + size / 2 - origin;
}
