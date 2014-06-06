/*
 *  Dr Genius interactive geometry software
 * (C) 2001,2002 Copyright FSF 
 * Author: Hilaire Fernandes  2001
 * 
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

#include <libintl.h>
#include "define.h"
#include "drgenius_config.h"
#include "drgeo_gtkhelpers.h"
#include "drgeo_drgeoStyle.h"
#include "drgenius_mdi.h"

extern int numericPrecision;
extern drgeniusMDI *mdi;
extern const char* drgeniusHelp[];
static GtkDialog *propertyBox;
static GtkBuilder *xmlPropertyBox;

/*
  Helper functions
*/
static void 
drgeoConfigSetGen(gchar *attr, gchar *value, gchar *conf)
{
  gchar *scm;

  scm = g_strconcat ("(set! ",conf, " (drgeoConfigSet '",
		     attr, " \"", value, "\" ", conf, "))",
		     NULL);
  scm_c_eval_string (scm);
  g_free (scm);

}

static void 
drgeoConfigSet(gchar *attr, gchar *value)
{
  drgeoConfigSetGen (attr, value, "userConf");
}

static void 
drgeoConfigSetDialog(gchar *attr, gchar *value)
{
  drgeoConfigSetGen (attr, value, "dialogConf");
}

static void
cachePreferedValue ();


gchar * 
drgeoConfigGet (gchar *attr)
{
  gchar *scm;
  static gchar* value = NULL;
  SCM ret;
  
  scm = g_strconcat ("(drgeoConfigGet '",
		     attr, "  userConf)",
		     NULL);
  ret = scm_c_eval_string (scm);
  g_free (scm);
  g_free(value);
  value = scm_to_locale_string (ret);
  return value;
}

static GtkWidget * drgeoConfigWidget (gchar *item, 
				      gchar *attr, GtkBuilder *xml)
{
  gchar *str;
  GtkWidget *w;

  str = g_strconcat (item, drgeoConfigGet (attr), NULL);
  w = glade_xml_get_widget (xml, str);
  if (w == NULL)
    printf ("drgenius_config::drgeoConfigWidget, can't find widget  %s\n", 
	    str);
  g_free (str);
  return w;
}


/* 
   Static callback for the property dialog 
*/
/* on point */
static void on_pointColor_toggled (GtkToggleButton * b, gpointer colorIndex);
static void on_pointShape_toggled (GtkToggleButton * b, gpointer shapeIndex);
static void on_pointSize_toggled (GtkToggleButton * b, gpointer sizeIndex);
/* polygon */
static void on_polygonColor_toggled (GtkToggleButton * b,
				     gpointer colorIndex);
/* on segment */
static void on_segmentColor_toggled (GtkToggleButton * b,
				     gpointer colorIndex);
static void on_segmentStyle_toggled (GtkToggleButton * b,
				     gpointer styleIndex);
/* on halfline */
static void on_halflineColor_toggled (GtkToggleButton * b,
				      gpointer colorIndex);
static void on_halflineStyle_toggled (GtkToggleButton * b,
				      gpointer styleIndex);
/* on line */
static void on_lineColor_toggled (GtkToggleButton * b, gpointer colorIndex);
static void on_lineStyle_toggled (GtkToggleButton * b, gpointer styleIndex);
/* on vector */
static void on_vectorColor_toggled (GtkToggleButton * b, gpointer colorIndex);
static void on_vectorStyle_toggled (GtkToggleButton * b, gpointer styleIndex);
/* on circle */
static void on_circleColor_toggled (GtkToggleButton * b, gpointer colorIndex);
static void on_circleStyle_toggled (GtkToggleButton * b, gpointer styleIndex);
/* on arc circle */
static void on_arccircleColor_toggled (GtkToggleButton * b,
				       gpointer colorIndex);
static void on_arccircleStyle_toggled (GtkToggleButton * b,
				       gpointer styleIndex);
/* on locus */
static void on_locusColor_toggled (GtkToggleButton * b, gpointer colorIndex);
static void on_locusStyle_toggled (GtkToggleButton * b, gpointer styleIndex);
/* on angle */
static void on_angleColor_toggled (GtkToggleButton * b, gpointer colorIndex);
static void on_angleStyle_toggled (GtkToggleButton * b, gpointer styleIndex);
/* on scalar */
static void on_scalarColor_toggled (GtkToggleButton * b, gpointer colorIndex);


static void
connectWithData (GtkBuilder * tree, gchar * widgetName,
		 GtkSignalFunc f, gint data)
{
  GtkWidget *widget;

  widget = glade_xml_get_widget (tree, widgetName);
  if (widget == NULL)
    g_print ("Cannot find widget %s\n", widgetName);
  else
    gtk_signal_connect (GTK_OBJECT (widget),
			"toggled", f, GINT_TO_POINTER (data));
}

static void
cachePreferedValue ()
{
  // Cache the numeric precision in an integer, this speed up the 
  // read of the value when used in the numeric object
  numericPrecision = atoi (drgeoConfigGet (":numericPrecision"));
}



void
loadUserPreferences ()
/* Load User preference from the config file */
{
  gchar * filename;
  gchar *scm;

  filename = g_strconcat( g_get_home_dir (), "/.drgeo", NULL);

  
  if (!g_file_test (filename, G_FILE_TEST_EXISTS))
    {
      gchar *str;

      str = g_strconcat ("cp ", DRGEO_SCMDIR, "/preferences.scm ",
			 filename, NULL);
      g_spawn_command_line_async (str, NULL);
      g_free (str);
  
      // load the defaut one (usefull to upgrade)
      scm = g_strconcat ("(define userConf (drgeoConfigLoad \"", DRGEO_SCMDIR, "/preferences.scm\"))", NULL);
    }
  else    
    scm = g_strconcat ("(define userConf (drgeoConfigLoadAndUpdate \"", DRGEO_SCMDIR, 
		       "/preferences.scm\" \"", filename, "\"))", NULL);
    
  
  scm_c_eval_string (scm);
  g_free (scm);

  g_free (filename);
  cachePreferedValue ();
}

void
saveUserPreferences ()
{
  gchar * filename;
  gchar *scm;

  filename = g_strconcat( g_get_home_dir (), "/.drgeo", NULL);
  
  scm = g_strconcat ("(drgeoConfigSave \"", filename, "\" userConf)", NULL);

  scm_c_eval_string (scm);
  g_free (scm);
  g_free (filename);
}

void
updateDialogFromUserPreferences (GtkObject * d)
{
  GtkWidget *w;

  /* First set a copy of the user preferences */
  scm_c_eval_string ("(define dialogConf userConf)");
  

  /***********/
  /*         */
  /*  POINT  */
  /*         */
  /***********/

  /* color */
  w = drgeoConfigWidget ("point", ":pointColor", xmlPropertyBox);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /* shape */
  w = drgeoConfigWidget ("point", ":pointShape", xmlPropertyBox);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /* size */
  w = drgeoConfigWidget ("point", ":pointSize", xmlPropertyBox);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /*************/
  /*           */
  /*  POLYGON  */
  /*           */
  /*************/

  /* color */
  w = drgeoConfigWidget ("polygon", ":polygonColor", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /*************/
  /*           */
  /*  SEGMENT  */
  /*           */
  /*************/

  /* color */
  w = drgeoConfigWidget ("segment", ":segmentColor", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /* style */
  w = drgeoConfigWidget ("segment", ":segmentStyle", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /***************/
  /*             */
  /*  HALF-LINE  */
  /*             */
  /***************/

  /* halfLine */
  w = drgeoConfigWidget ("halfline", ":halfLineColor", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /* style */
  w = drgeoConfigWidget ("halfline", ":halfLineStyle", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /**********/
  /*        */
  /*  LINE  */
  /*        */
  /**********/

  /* color */
  w = drgeoConfigWidget ("line", ":lineColor", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /* style */
  w = drgeoConfigWidget ("line", ":lineStyle", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /**********/
  /* VECTOR */
  /**********/

  /* color */
  w = drgeoConfigWidget ("vector", ":vectorColor", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /* style */
  w = drgeoConfigWidget ("vector", ":vectorStyle", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /**********/
  /* CIRCLE */
  /**********/

  /* color */
  w = drgeoConfigWidget ("circle", ":circleColor", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /* style */
  w = drgeoConfigWidget ("circle", ":circleStyle", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /*************/
  /* arcCircle */
  /*************/
  
  /* color */
  w = drgeoConfigWidget ("arccircle", ":arcCircleColor", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /* style */
  w = drgeoConfigWidget ("arccircle", ":arcCircleStyle", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /*********/
  /* LOCUS */
  /*********/

  /* color */
  w = drgeoConfigWidget ("locus", ":locusColor", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /* style */
  w = drgeoConfigWidget ("locus", ":locusStyle", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /*********/
  /* ANGLE */
  /*********/

  /* color */
  w = drgeoConfigWidget ("angle", ":angleColor", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /* style */
  w = drgeoConfigWidget ("angle", ":angleStyle", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /**********/
  /* SCALAR */
  /**********/

  /* color */
  w = drgeoConfigWidget ("scalar", ":scalarColor", xmlPropertyBox);  
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w), TRUE);

  /* numeric precision */
  w = glade_xml_get_widget (xmlPropertyBox, "drgeoNumericSpinbutton");
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (w), 
			     atof (drgeoConfigGet (":numericPrecision")));



  w = glade_xml_get_widget (xmlPropertyBox, "drgeoUndoSpinbutton");
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (w), 
			     atof (drgeoConfigGet (":undoLevel")));
  
  w = glade_xml_get_widget (xmlPropertyBox, "drgeoFigureNameEntry");
  gtk_entry_set_text (GTK_ENTRY (w), _(drgeoConfigGet (":figureName")));

  w = glade_xml_get_widget (xmlPropertyBox, "drgeoFileNameEntry");
  gtk_entry_set_text (GTK_ENTRY (w), drgeoConfigGet (":figureFile"));

  w = glade_xml_get_widget (xmlPropertyBox, "drgeoFlydrawFileNameEntry");
  gtk_entry_set_text (GTK_ENTRY (w), drgeoConfigGet (":flydrawFile"));

  w = glade_xml_get_widget (xmlPropertyBox, "drgeoLatexFileNameEntry");
  gtk_entry_set_text (GTK_ENTRY (w), drgeoConfigGet (":latexFile"));

  w = glade_xml_get_widget (xmlPropertyBox, "drgeoPostscriptFileNameEntry");
  gtk_entry_set_text (GTK_ENTRY (w), drgeoConfigGet (":psFile"));

  w = glade_xml_get_widget (xmlPropertyBox, "globalSessionFileNameEntry");
  gtk_entry_set_text (GTK_ENTRY (w), drgeoConfigGet (":sessionFile"));

  w = glade_xml_get_widget (xmlPropertyBox, "drgeoHtmlViewerEntry");
  gtk_entry_set_text (GTK_ENTRY (w), drgeoConfigGet (":htmlViewer"));

}

void
updateUserPreferencesFromDialog (GtkObject * d)
{
  GtkWidget *w;

  scm_c_eval_string ("(define userConf dialogConf)");

  cachePreferedValue ();

  saveUserPreferences ();

}

void
initPreferencesBox ()
{
  if (propertyBox == NULL)
    {

      xmlPropertyBox = glade_xml_new (DRGEO_GLADEDIR "/drgenius2.glade",
				      "propertybox", NULL);
      propertyBox = (GtkDialog *) glade_xml_get_widget
	(xmlPropertyBox, "propertybox");
      
      //set transient
      mdi->setTransientDialog (GTK_WINDOW (propertyBox));
      

      glade_xml_signal_autoconnect (xmlPropertyBox);


      /* Connect with userdata (LibGlade is not very helpfull there. */


      /* Point */
      connectWithData (xmlPropertyBox, "pointBlack",
		       GTK_SIGNAL_FUNC (on_pointColor_toggled), 0);
      connectWithData (xmlPropertyBox, "pointDarkgrey",
		       GTK_SIGNAL_FUNC (on_pointColor_toggled), 1);
      connectWithData (xmlPropertyBox, "pointGrey",
		       GTK_SIGNAL_FUNC (on_pointColor_toggled), 2);
      connectWithData (xmlPropertyBox, "pointWhite",
		       GTK_SIGNAL_FUNC (on_pointColor_toggled), 3);
      connectWithData (xmlPropertyBox, "pointDarkgreen",
		       GTK_SIGNAL_FUNC (on_pointColor_toggled), 4);
      connectWithData (xmlPropertyBox, "pointGreen",
		       GTK_SIGNAL_FUNC (on_pointColor_toggled), 5);
      connectWithData (xmlPropertyBox, "pointDarkblue",
		       GTK_SIGNAL_FUNC (on_pointColor_toggled), 6);
      connectWithData (xmlPropertyBox, "pointBlue",
		       GTK_SIGNAL_FUNC (on_pointColor_toggled), 7);
      connectWithData (xmlPropertyBox, "pointBordeaux",
		       GTK_SIGNAL_FUNC (on_pointColor_toggled), 8);
      connectWithData (xmlPropertyBox, "pointRed",
		       GTK_SIGNAL_FUNC (on_pointColor_toggled), 9);
      connectWithData (xmlPropertyBox, "pointOrange",
		       GTK_SIGNAL_FUNC (on_pointColor_toggled), 10);
      connectWithData (xmlPropertyBox, "pointYellow",
		       GTK_SIGNAL_FUNC (on_pointColor_toggled), 11);

      connectWithData (xmlPropertyBox, "pointRound",
		       GTK_SIGNAL_FUNC (on_pointShape_toggled), 0);
      connectWithData (xmlPropertyBox, "pointSquare",
		       GTK_SIGNAL_FUNC (on_pointShape_toggled), 2);
      connectWithData (xmlPropertyBox, "pointCross",
		       GTK_SIGNAL_FUNC (on_pointShape_toggled), 1);
      connectWithData (xmlPropertyBox, "pointRoundEmpty",
		       GTK_SIGNAL_FUNC (on_pointShape_toggled), 3);
      connectWithData (xmlPropertyBox, "pointSquareEmpty",
		       GTK_SIGNAL_FUNC (on_pointShape_toggled), 4);

      connectWithData (xmlPropertyBox, "pointSmall",
		       GTK_SIGNAL_FUNC (on_pointSize_toggled), 0);
      connectWithData (xmlPropertyBox, "pointNormal",
		       GTK_SIGNAL_FUNC (on_pointSize_toggled), 1);
      connectWithData (xmlPropertyBox, "pointLarge",
		       GTK_SIGNAL_FUNC (on_pointSize_toggled), 2);

      /* Polygon */
      connectWithData (xmlPropertyBox, "polygonBlack",
		       GTK_SIGNAL_FUNC (on_polygonColor_toggled), 0);
      connectWithData (xmlPropertyBox, "polygonDarkgrey",
		       GTK_SIGNAL_FUNC (on_polygonColor_toggled), 1);
      connectWithData (xmlPropertyBox, "polygonGrey",
		       GTK_SIGNAL_FUNC (on_polygonColor_toggled), 2);
      connectWithData (xmlPropertyBox, "polygonWhite",
		       GTK_SIGNAL_FUNC (on_polygonColor_toggled), 3);
      connectWithData (xmlPropertyBox, "polygonDarkgreen",
		       GTK_SIGNAL_FUNC (on_polygonColor_toggled), 4);
      connectWithData (xmlPropertyBox, "polygonGreen",
		       GTK_SIGNAL_FUNC (on_polygonColor_toggled), 5);
      connectWithData (xmlPropertyBox, "polygonDarkblue",
		       GTK_SIGNAL_FUNC (on_polygonColor_toggled), 6);
      connectWithData (xmlPropertyBox, "polygonBlue",
		       GTK_SIGNAL_FUNC (on_polygonColor_toggled), 7);
      connectWithData (xmlPropertyBox, "polygonBordeaux",
		       GTK_SIGNAL_FUNC (on_polygonColor_toggled), 8);
      connectWithData (xmlPropertyBox, "polygonRed",
		       GTK_SIGNAL_FUNC (on_polygonColor_toggled), 9);
      connectWithData (xmlPropertyBox, "polygonOrange",
		       GTK_SIGNAL_FUNC (on_polygonColor_toggled), 10);
      connectWithData (xmlPropertyBox, "polygonYellow",
		       GTK_SIGNAL_FUNC (on_polygonColor_toggled), 11);

      /* Segment */
      connectWithData (xmlPropertyBox, "segmentBlack",
		       GTK_SIGNAL_FUNC (on_segmentColor_toggled), 0);
      connectWithData (xmlPropertyBox, "segmentDarkgrey",
		       GTK_SIGNAL_FUNC (on_segmentColor_toggled), 1);
      connectWithData (xmlPropertyBox, "segmentGrey",
		       GTK_SIGNAL_FUNC (on_segmentColor_toggled), 2);
      connectWithData (xmlPropertyBox, "segmentWhite",
		       GTK_SIGNAL_FUNC (on_segmentColor_toggled), 3);
      connectWithData (xmlPropertyBox, "segmentDarkgreen",
		       GTK_SIGNAL_FUNC (on_segmentColor_toggled), 4);
      connectWithData (xmlPropertyBox, "segmentGreen",
		       GTK_SIGNAL_FUNC (on_segmentColor_toggled), 5);
      connectWithData (xmlPropertyBox, "segmentDarkblue",
		       GTK_SIGNAL_FUNC (on_segmentColor_toggled), 6);
      connectWithData (xmlPropertyBox, "segmentBlue",
		       GTK_SIGNAL_FUNC (on_segmentColor_toggled), 7);
      connectWithData (xmlPropertyBox, "segmentBordeaux",
		       GTK_SIGNAL_FUNC (on_segmentColor_toggled), 8);
      connectWithData (xmlPropertyBox, "segmentRed",
		       GTK_SIGNAL_FUNC (on_segmentColor_toggled), 9);
      connectWithData (xmlPropertyBox, "segmentOrange",
		       GTK_SIGNAL_FUNC (on_segmentColor_toggled), 10);
      connectWithData (xmlPropertyBox, "segmentYellow",
		       GTK_SIGNAL_FUNC (on_segmentColor_toggled), 11);

      connectWithData (xmlPropertyBox, "segmentSmall",
		       GTK_SIGNAL_FUNC (on_segmentStyle_toggled), 0);
      connectWithData (xmlPropertyBox, "segmentNormal",
		       GTK_SIGNAL_FUNC (on_segmentStyle_toggled), 1);
      connectWithData (xmlPropertyBox, "segmentLarge",
		       GTK_SIGNAL_FUNC (on_segmentStyle_toggled), 2);
      /* Halfline */
      connectWithData (xmlPropertyBox, "halflineBlack",
		       GTK_SIGNAL_FUNC (on_halflineColor_toggled), 0);
      connectWithData (xmlPropertyBox, "halflineDarkgrey",
		       GTK_SIGNAL_FUNC (on_halflineColor_toggled), 1);
      connectWithData (xmlPropertyBox, "halflineGrey",
		       GTK_SIGNAL_FUNC (on_halflineColor_toggled), 2);
      connectWithData (xmlPropertyBox, "halflineWhite",
		       GTK_SIGNAL_FUNC (on_halflineColor_toggled), 3);
      connectWithData (xmlPropertyBox, "halflineDarkgreen",
		       GTK_SIGNAL_FUNC (on_halflineColor_toggled), 4);
      connectWithData (xmlPropertyBox, "halflineGreen",
		       GTK_SIGNAL_FUNC (on_halflineColor_toggled), 5);
      connectWithData (xmlPropertyBox, "halflineDarkblue",
		       GTK_SIGNAL_FUNC (on_halflineColor_toggled), 6);
      connectWithData (xmlPropertyBox, "halflineBlue",
		       GTK_SIGNAL_FUNC (on_halflineColor_toggled), 7);
      connectWithData (xmlPropertyBox, "halflineBordeaux",
		       GTK_SIGNAL_FUNC (on_halflineColor_toggled), 8);
      connectWithData (xmlPropertyBox, "halflineRed",
		       GTK_SIGNAL_FUNC (on_halflineColor_toggled), 9);
      connectWithData (xmlPropertyBox, "halflineOrange",
		       GTK_SIGNAL_FUNC (on_halflineColor_toggled), 10);
      connectWithData (xmlPropertyBox, "halflineYellow",
		       GTK_SIGNAL_FUNC (on_halflineColor_toggled), 11);

      connectWithData (xmlPropertyBox, "halflineSmall",
		       GTK_SIGNAL_FUNC (on_halflineStyle_toggled), 0);
      connectWithData (xmlPropertyBox, "halflineNormal",
		       GTK_SIGNAL_FUNC (on_halflineStyle_toggled), 1);
      connectWithData (xmlPropertyBox, "halflineLarge",
		       GTK_SIGNAL_FUNC (on_halflineStyle_toggled), 2);
      /* Line */
      connectWithData (xmlPropertyBox, "lineBlack",
		       GTK_SIGNAL_FUNC (on_lineColor_toggled), 0);
      connectWithData (xmlPropertyBox, "lineDarkgrey",
		       GTK_SIGNAL_FUNC (on_lineColor_toggled), 1);
      connectWithData (xmlPropertyBox, "lineGrey",
		       GTK_SIGNAL_FUNC (on_lineColor_toggled), 2);
      connectWithData (xmlPropertyBox, "lineWhite",
		       GTK_SIGNAL_FUNC (on_lineColor_toggled), 3);
      connectWithData (xmlPropertyBox, "lineDarkgreen",
		       GTK_SIGNAL_FUNC (on_lineColor_toggled), 4);
      connectWithData (xmlPropertyBox, "lineGreen",
		       GTK_SIGNAL_FUNC (on_lineColor_toggled), 5);
      connectWithData (xmlPropertyBox, "lineDarkblue",
		       GTK_SIGNAL_FUNC (on_lineColor_toggled), 6);
      connectWithData (xmlPropertyBox, "lineBlue",
		       GTK_SIGNAL_FUNC (on_lineColor_toggled), 7);
      connectWithData (xmlPropertyBox, "lineBordeaux",
		       GTK_SIGNAL_FUNC (on_lineColor_toggled), 8);
      connectWithData (xmlPropertyBox, "lineRed",
		       GTK_SIGNAL_FUNC (on_lineColor_toggled), 9);
      connectWithData (xmlPropertyBox, "lineOrange",
		       GTK_SIGNAL_FUNC (on_lineColor_toggled), 10);
      connectWithData (xmlPropertyBox, "lineYellow",
		       GTK_SIGNAL_FUNC (on_lineColor_toggled), 11);

      connectWithData (xmlPropertyBox, "lineSmall",
		       GTK_SIGNAL_FUNC (on_lineStyle_toggled), 0);
      connectWithData (xmlPropertyBox, "lineNormal",
		       GTK_SIGNAL_FUNC (on_lineStyle_toggled), 1);
      connectWithData (xmlPropertyBox, "lineLarge",
		       GTK_SIGNAL_FUNC (on_lineStyle_toggled), 2);
      /* Vector */
      connectWithData (xmlPropertyBox, "vectorBlack",
		       GTK_SIGNAL_FUNC (on_vectorColor_toggled), 0);
      connectWithData (xmlPropertyBox, "vectorDarkgrey",
		       GTK_SIGNAL_FUNC (on_vectorColor_toggled), 1);
      connectWithData (xmlPropertyBox, "vectorGrey",
		       GTK_SIGNAL_FUNC (on_vectorColor_toggled), 2);
      connectWithData (xmlPropertyBox, "vectorWhite",
		       GTK_SIGNAL_FUNC (on_vectorColor_toggled), 3);
      connectWithData (xmlPropertyBox, "vectorDarkgreen",
		       GTK_SIGNAL_FUNC (on_vectorColor_toggled), 4);
      connectWithData (xmlPropertyBox, "vectorGreen",
		       GTK_SIGNAL_FUNC (on_vectorColor_toggled), 5);
      connectWithData (xmlPropertyBox, "vectorDarkblue",
		       GTK_SIGNAL_FUNC (on_vectorColor_toggled), 6);
      connectWithData (xmlPropertyBox, "vectorBlue",
		       GTK_SIGNAL_FUNC (on_vectorColor_toggled), 7);
      connectWithData (xmlPropertyBox, "vectorBordeaux",
		       GTK_SIGNAL_FUNC (on_vectorColor_toggled), 8);
      connectWithData (xmlPropertyBox, "vectorRed",
		       GTK_SIGNAL_FUNC (on_vectorColor_toggled), 9);
      connectWithData (xmlPropertyBox, "vectorOrange",
		       GTK_SIGNAL_FUNC (on_vectorColor_toggled), 10);
      connectWithData (xmlPropertyBox, "vectorYellow",
		       GTK_SIGNAL_FUNC (on_vectorColor_toggled), 11);

      connectWithData (xmlPropertyBox, "vectorSmall",
		       GTK_SIGNAL_FUNC (on_vectorStyle_toggled), 0);
      connectWithData (xmlPropertyBox, "vectorNormal",
		       GTK_SIGNAL_FUNC (on_vectorStyle_toggled), 1);
      connectWithData (xmlPropertyBox, "vectorLarge",
		       GTK_SIGNAL_FUNC (on_vectorStyle_toggled), 2);
      /* Circle */
      connectWithData (xmlPropertyBox, "circleBlack",
		       GTK_SIGNAL_FUNC (on_circleColor_toggled), 0);
      connectWithData (xmlPropertyBox, "circleDarkgrey",
		       GTK_SIGNAL_FUNC (on_circleColor_toggled), 1);
      connectWithData (xmlPropertyBox, "circleGrey",
		       GTK_SIGNAL_FUNC (on_circleColor_toggled), 2);
      connectWithData (xmlPropertyBox, "circleWhite",
		       GTK_SIGNAL_FUNC (on_circleColor_toggled), 3);
      connectWithData (xmlPropertyBox, "circleDarkgreen",
		       GTK_SIGNAL_FUNC (on_circleColor_toggled), 4);
      connectWithData (xmlPropertyBox, "circleGreen",
		       GTK_SIGNAL_FUNC (on_circleColor_toggled), 5);
      connectWithData (xmlPropertyBox, "circleDarkblue",
		       GTK_SIGNAL_FUNC (on_circleColor_toggled), 6);
      connectWithData (xmlPropertyBox, "circleBlue",
		       GTK_SIGNAL_FUNC (on_circleColor_toggled), 7);
      connectWithData (xmlPropertyBox, "circleBordeaux",
		       GTK_SIGNAL_FUNC (on_circleColor_toggled), 8);
      connectWithData (xmlPropertyBox, "circleRed",
		       GTK_SIGNAL_FUNC (on_circleColor_toggled), 9);
      connectWithData (xmlPropertyBox, "circleOrange",
		       GTK_SIGNAL_FUNC (on_circleColor_toggled), 10);
      connectWithData (xmlPropertyBox, "circleYellow",
		       GTK_SIGNAL_FUNC (on_circleColor_toggled), 11);

      connectWithData (xmlPropertyBox, "circleSmall",
		       GTK_SIGNAL_FUNC (on_circleStyle_toggled), 0);
      connectWithData (xmlPropertyBox, "circleNormal",
		       GTK_SIGNAL_FUNC (on_circleStyle_toggled), 1);
      connectWithData (xmlPropertyBox, "circleLarge",
		       GTK_SIGNAL_FUNC (on_circleStyle_toggled), 2);
      /* Arc circle */
      connectWithData (xmlPropertyBox, "arccircleBlack",
		       GTK_SIGNAL_FUNC (on_arccircleColor_toggled), 0);
      connectWithData (xmlPropertyBox, "arccircleDarkgrey",
		       GTK_SIGNAL_FUNC (on_arccircleColor_toggled), 1);
      connectWithData (xmlPropertyBox, "arccircleGrey",
		       GTK_SIGNAL_FUNC (on_arccircleColor_toggled), 2);
      connectWithData (xmlPropertyBox, "arccircleWhite",
		       GTK_SIGNAL_FUNC (on_arccircleColor_toggled), 3);
      connectWithData (xmlPropertyBox, "arccircleDarkgreen",
		       GTK_SIGNAL_FUNC (on_arccircleColor_toggled), 4);
      connectWithData (xmlPropertyBox, "arccircleGreen",
		       GTK_SIGNAL_FUNC (on_arccircleColor_toggled), 5);
      connectWithData (xmlPropertyBox, "arccircleDarkblue",
		       GTK_SIGNAL_FUNC (on_arccircleColor_toggled), 6);
      connectWithData (xmlPropertyBox, "arccircleBlue",
		       GTK_SIGNAL_FUNC (on_arccircleColor_toggled), 7);
      connectWithData (xmlPropertyBox, "arccircleBordeaux",
		       GTK_SIGNAL_FUNC (on_arccircleColor_toggled), 8);
      connectWithData (xmlPropertyBox, "arccircleRed",
		       GTK_SIGNAL_FUNC (on_arccircleColor_toggled), 9);
      connectWithData (xmlPropertyBox, "arccircleOrange",
		       GTK_SIGNAL_FUNC (on_arccircleColor_toggled), 10);
      connectWithData (xmlPropertyBox, "arccircleYellow",
		       GTK_SIGNAL_FUNC (on_arccircleColor_toggled), 11);

      connectWithData (xmlPropertyBox, "arccircleSmall",
		       GTK_SIGNAL_FUNC (on_arccircleStyle_toggled), 0);
      connectWithData (xmlPropertyBox, "arccircleNormal",
		       GTK_SIGNAL_FUNC (on_arccircleStyle_toggled), 1);
      connectWithData (xmlPropertyBox, "arccircleLarge",
		       GTK_SIGNAL_FUNC (on_arccircleStyle_toggled), 2);
      /* Locus */
      connectWithData (xmlPropertyBox, "locusBlack",
		       GTK_SIGNAL_FUNC (on_locusColor_toggled), 0);
      connectWithData (xmlPropertyBox, "locusDarkgrey",
		       GTK_SIGNAL_FUNC (on_locusColor_toggled), 1);
      connectWithData (xmlPropertyBox, "locusGrey",
		       GTK_SIGNAL_FUNC (on_locusColor_toggled), 2);
      connectWithData (xmlPropertyBox, "locusWhite",
		       GTK_SIGNAL_FUNC (on_locusColor_toggled), 3);
      connectWithData (xmlPropertyBox, "locusDarkgreen",
		       GTK_SIGNAL_FUNC (on_locusColor_toggled), 4);
      connectWithData (xmlPropertyBox, "locusGreen",
		       GTK_SIGNAL_FUNC (on_locusColor_toggled), 5);
      connectWithData (xmlPropertyBox, "locusDarkblue",
		       GTK_SIGNAL_FUNC (on_locusColor_toggled), 6);
      connectWithData (xmlPropertyBox, "locusBlue",
		       GTK_SIGNAL_FUNC (on_locusColor_toggled), 7);
      connectWithData (xmlPropertyBox, "locusBordeaux",
		       GTK_SIGNAL_FUNC (on_locusColor_toggled), 8);
      connectWithData (xmlPropertyBox, "locusRed",
		       GTK_SIGNAL_FUNC (on_locusColor_toggled), 9);
      connectWithData (xmlPropertyBox, "locusOrange",
		       GTK_SIGNAL_FUNC (on_locusColor_toggled), 10);
      connectWithData (xmlPropertyBox, "locusYellow",
		       GTK_SIGNAL_FUNC (on_locusColor_toggled), 11);

      connectWithData (xmlPropertyBox, "locusSmall",
		       GTK_SIGNAL_FUNC (on_locusStyle_toggled), 0);
      connectWithData (xmlPropertyBox, "locusNormal",
		       GTK_SIGNAL_FUNC (on_locusStyle_toggled), 1);
      connectWithData (xmlPropertyBox, "locusLarge",
		       GTK_SIGNAL_FUNC (on_locusStyle_toggled), 2);
      /* Angle */
      connectWithData (xmlPropertyBox, "angleBlack",
		       GTK_SIGNAL_FUNC (on_angleColor_toggled), 0);
      connectWithData (xmlPropertyBox, "angleDarkgrey",
		       GTK_SIGNAL_FUNC (on_angleColor_toggled), 1);
      connectWithData (xmlPropertyBox, "angleGrey",
		       GTK_SIGNAL_FUNC (on_angleColor_toggled), 2);
      connectWithData (xmlPropertyBox, "angleWhite",
		       GTK_SIGNAL_FUNC (on_angleColor_toggled), 3);
      connectWithData (xmlPropertyBox, "angleDarkgreen",
		       GTK_SIGNAL_FUNC (on_angleColor_toggled), 4);
      connectWithData (xmlPropertyBox, "angleGreen",
		       GTK_SIGNAL_FUNC (on_angleColor_toggled), 5);
      connectWithData (xmlPropertyBox, "angleDarkblue",
		       GTK_SIGNAL_FUNC (on_angleColor_toggled), 6);
      connectWithData (xmlPropertyBox, "angleBlue",
		       GTK_SIGNAL_FUNC (on_angleColor_toggled), 7);
      connectWithData (xmlPropertyBox, "angleBordeaux",
		       GTK_SIGNAL_FUNC (on_angleColor_toggled), 8);
      connectWithData (xmlPropertyBox, "angleRed",
		       GTK_SIGNAL_FUNC (on_angleColor_toggled), 9);
      connectWithData (xmlPropertyBox, "angleOrange",
		       GTK_SIGNAL_FUNC (on_angleColor_toggled), 10);
      connectWithData (xmlPropertyBox, "angleYellow",
		       GTK_SIGNAL_FUNC (on_angleColor_toggled), 11);

      connectWithData (xmlPropertyBox, "angleSmall",
		       GTK_SIGNAL_FUNC (on_angleStyle_toggled), 0);
      connectWithData (xmlPropertyBox, "angleNormal",
		       GTK_SIGNAL_FUNC (on_angleStyle_toggled), 1);
      connectWithData (xmlPropertyBox, "angleLarge",
		       GTK_SIGNAL_FUNC (on_angleStyle_toggled), 2);
      /* Scalar */
      connectWithData (xmlPropertyBox, "scalarBlack",
		       GTK_SIGNAL_FUNC (on_scalarColor_toggled), 0);
      connectWithData (xmlPropertyBox, "scalarDarkgrey",
		       GTK_SIGNAL_FUNC (on_scalarColor_toggled), 1);
      connectWithData (xmlPropertyBox, "scalarGrey",
		       GTK_SIGNAL_FUNC (on_scalarColor_toggled), 2);
      connectWithData (xmlPropertyBox, "scalarWhite",
		       GTK_SIGNAL_FUNC (on_scalarColor_toggled), 3);
      connectWithData (xmlPropertyBox, "scalarDarkgreen",
		       GTK_SIGNAL_FUNC (on_scalarColor_toggled), 4);
      connectWithData (xmlPropertyBox, "scalarGreen",
		       GTK_SIGNAL_FUNC (on_scalarColor_toggled), 5);
      connectWithData (xmlPropertyBox, "scalarDarkblue",
		       GTK_SIGNAL_FUNC (on_scalarColor_toggled), 6);
      connectWithData (xmlPropertyBox, "scalarBlue",
		       GTK_SIGNAL_FUNC (on_scalarColor_toggled), 7);
      connectWithData (xmlPropertyBox, "scalarBordeaux",
		       GTK_SIGNAL_FUNC (on_scalarColor_toggled), 8);
      connectWithData (xmlPropertyBox, "scalarRed",
		       GTK_SIGNAL_FUNC (on_scalarColor_toggled), 9);
      connectWithData (xmlPropertyBox, "scalarOrange",
		       GTK_SIGNAL_FUNC (on_scalarColor_toggled), 10);
      connectWithData (xmlPropertyBox, "scalarYellow",
		       GTK_SIGNAL_FUNC (on_scalarColor_toggled), 11);

      /* we keep the GladeXml object to retrieve widgets references */
      updateDialogFromUserPreferences (GTK_OBJECT (propertyBox));
    }
  gtk_widget_show_all (GTK_WIDGET (propertyBox));
}


/*
  Helpers for the callback 
*/
static void 
setDialogColor (gchar *attr, drgeoColorType color)
{
      switch (color)
	{
	case drgeoColorBlack:
	  drgeoConfigSetDialog (attr, "Black");
	  break;
	case drgeoColorDarkGrey:
	  drgeoConfigSetDialog (attr, "Darkgrey");
	  break;
	case drgeoColorGrey:
	  drgeoConfigSetDialog (attr, "Grey");
	  break;
	case drgeoColorWhite:
	  drgeoConfigSetDialog (attr, "White");
	  break;
	case drgeoColorDarkGreen:
	  drgeoConfigSetDialog (attr, "Darkgreen");
	  break;
	case drgeoColorGreen:
	  drgeoConfigSetDialog (attr, "Green");
	  break;
	case drgeoColorDarkBlue:
	  drgeoConfigSetDialog (attr, "Darkblue");
	  break;
	case drgeoColorBlue:
	  drgeoConfigSetDialog (attr, "Blue");
	  break;
	case drgeoColorBordeaux:
	  drgeoConfigSetDialog (attr, "Bordeaux");
	  break;
	case drgeoColorRed:
	  drgeoConfigSetDialog (attr, "Red");
	  break;
	case drgeoColorOrange:
	  drgeoConfigSetDialog (attr, "Orange");
	  break;
	case drgeoColorYellow:
	  drgeoConfigSetDialog (attr, "Yellow");
	  break;
	}
}

static void
setDialogStyle (gchar *attr, drgeoThicknessType style)
{
      switch (style)
	{
	case drgeoThicknessDashed:
	  drgeoConfigSetDialog (attr, "Small");
	  break;
	case drgeoThicknessNormal:
	  drgeoConfigSetDialog (attr, "Normal");
	  break;
	case drgeoThicknessLarge:
	  drgeoConfigSetDialog (attr, "Large");
	  break;
	}
}

/*
  Callback for the property box
*/
/* on point */
static void
on_pointColor_toggled (GtkToggleButton * b, gpointer colorIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogColor (":pointColor", 
		    (drgeoColorType) GPOINTER_TO_INT(colorIndex));
}
static void
on_pointShape_toggled (GtkToggleButton * b, gpointer shapeIndex)
{
  drgeoPointType type;

  type = (drgeoPointType) GPOINTER_TO_INT (shapeIndex);

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    {
      switch (type)
	{
	case drgeoPointRound:
	  drgeoConfigSetDialog (":pointShape", "Round");
	  break;
	case drgeoPointX:
	  drgeoConfigSetDialog (":pointShape", "Cross");
	  break;
	case drgeoPointRec:
	  drgeoConfigSetDialog (":pointShape", "Squarre");
	  break;
	case drgeoPointRoundEmpty:
	  drgeoConfigSetDialog (":pointShape", "RoundEmpty");
	  break;
	case drgeoPointRecEmpty:
	  drgeoConfigSetDialog (":pointShape", "SquareEmpty");
	  break;
	}
    }
}
static void
on_pointSize_toggled (GtkToggleButton * b, gpointer sizeIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogStyle (":pointSize", 
		    (drgeoThicknessType) GPOINTER_TO_INT(sizeIndex));
}

/* on polygon */
static void
on_polygonColor_toggled (GtkToggleButton * b, gpointer colorIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogColor (":polygonColor", 
		    (drgeoColorType) GPOINTER_TO_INT(colorIndex));
}

/* on segment */
static void
on_segmentColor_toggled (GtkToggleButton * b, gpointer colorIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogColor (":segmentColor", 
		    (drgeoColorType) GPOINTER_TO_INT(colorIndex));
}
static void
on_segmentStyle_toggled (GtkToggleButton * b, gpointer styleIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogStyle (":segmentStyle", 
		    (drgeoThicknessType) GPOINTER_TO_INT(styleIndex));
}

/* on halfline */
static void
on_halflineColor_toggled (GtkToggleButton * b, gpointer colorIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogColor (":halfLineColor", 
		    (drgeoColorType) GPOINTER_TO_INT(colorIndex));
}
static void
on_halflineStyle_toggled (GtkToggleButton * b, gpointer styleIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogStyle (":halfLineStyle", 
		    (drgeoThicknessType) GPOINTER_TO_INT(styleIndex));
}

/* on line */
static void
on_lineColor_toggled (GtkToggleButton * b, gpointer colorIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogColor (":lineColor", 
		    (drgeoColorType) GPOINTER_TO_INT(colorIndex));
}
static void
on_lineStyle_toggled (GtkToggleButton * b, gpointer styleIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogStyle (":lineStyle", 
		    (drgeoThicknessType) GPOINTER_TO_INT(styleIndex));
}

/* on vector */
static void
on_vectorColor_toggled (GtkToggleButton * b, gpointer colorIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogColor (":vectorColor", 
		    (drgeoColorType) GPOINTER_TO_INT(colorIndex));
}
static void
on_vectorStyle_toggled (GtkToggleButton * b, gpointer styleIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogStyle (":vectorStyle", 
		    (drgeoThicknessType) GPOINTER_TO_INT(styleIndex));
}

/* on circle */
static void
on_circleColor_toggled (GtkToggleButton * b, gpointer colorIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogColor (":circleColor", 
		    (drgeoColorType) GPOINTER_TO_INT(colorIndex));
}
static void
on_circleStyle_toggled (GtkToggleButton * b, gpointer styleIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogStyle (":circleStyle", 
		    (drgeoThicknessType) GPOINTER_TO_INT(styleIndex));
}

/* on arccircle */
static void
on_arccircleColor_toggled (GtkToggleButton * b, gpointer colorIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogColor (":arcCircleColor", 
		    (drgeoColorType) GPOINTER_TO_INT(colorIndex));
}
static void
on_arccircleStyle_toggled (GtkToggleButton * b, gpointer styleIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogStyle (":arcCircleStyle", 
		    (drgeoThicknessType) GPOINTER_TO_INT(styleIndex));
}

/* on locus */
static void
on_locusColor_toggled (GtkToggleButton * b, gpointer colorIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogColor (":locusColor", 
		    (drgeoColorType) GPOINTER_TO_INT(colorIndex));
}
static void
on_locusStyle_toggled (GtkToggleButton * b, gpointer styleIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogStyle (":locusStyle", 
		    (drgeoThicknessType) GPOINTER_TO_INT(styleIndex));
}

/* on angle */
static void
on_angleColor_toggled (GtkToggleButton * b, gpointer colorIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogColor (":angleColor", 
		    (drgeoColorType) GPOINTER_TO_INT(colorIndex));
}
static void
on_angleStyle_toggled (GtkToggleButton * b, gpointer styleIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogStyle (":angleStyle", 
		    (drgeoThicknessType) GPOINTER_TO_INT(styleIndex));
}

/* on scalar */
static void
on_scalarColor_toggled (GtkToggleButton * b, gpointer colorIndex)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (b)))
    setDialogColor (":scalarColor", 
		    (drgeoColorType) GPOINTER_TO_INT(colorIndex));
}
void on_numeric_changed (GtkWidget * widget, gpointer data)
{
  gint numericPrecision;
  gchar *str;

  numericPrecision = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (widget));
  str = g_strdup_printf ("%d", numericPrecision);
  drgeoConfigSetDialog (":numericPrecision", str);
  g_free (str);
}

void on_undo_changed (GtkWidget * widget, gpointer data)
{
  gint undoLevel;
  gchar *str;

  undoLevel = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (widget));
  str = g_strdup_printf ("%d", undoLevel);
  drgeoConfigSetDialog (":undoLevel", str);
  g_free (str);
}
void on_figureName_changed (GtkWidget * widget, gpointer data)
{
  drgeoConfigSetDialog (":figureName", (gchar *)
			gtk_entry_get_text (GTK_ENTRY (widget)));
}
void on_htmlViewer_changed (GtkWidget * widget, gpointer data)
{
  drgeoConfigSetDialog (":htmlViewer", (gchar *)
			gtk_entry_get_text (GTK_ENTRY (widget)));
}
void on_figureFile_changed (GtkWidget * widget, gpointer data)
{
  drgeoConfigSetDialog (":figureFile", (gchar *)
			gtk_entry_get_text (GTK_ENTRY (widget)));
}
void on_sessionFile_changed (GtkWidget * widget, gpointer data)
{
  drgeoConfigSetDialog (":sessionFile", (gchar *)
			gtk_entry_get_text (GTK_ENTRY (widget)));
}
void on_flydrawFile_changed (GtkWidget * widget, gpointer data)
{
  drgeoConfigSetDialog (":flydrawFile", (gchar *)
			gtk_entry_get_text (GTK_ENTRY (widget)));
}
void on_latexFile_changed (GtkWidget * widget, gpointer data)
{
  drgeoConfigSetDialog (":latexFile", (gchar *)
			gtk_entry_get_text (GTK_ENTRY (widget)));
}
void on_psFile_changed (GtkWidget * widget, gpointer data)
{
  drgeoConfigSetDialog (":psFile", (gchar *)
			gtk_entry_get_text (GTK_ENTRY (widget)));
}


void 
on_propertyBox_response (GtkWidget *d, gint r, gpointer data)
{

  switch (r)
    {
    case GTK_RESPONSE_OK:
      updateUserPreferencesFromDialog (GTK_OBJECT (d));
      gtk_widget_hide (d);
      break;
    case GTK_RESPONSE_APPLY:
      updateUserPreferencesFromDialog (GTK_OBJECT (d));
      break;
    case GTK_RESPONSE_CANCEL:
      gtk_widget_hide (d);
      break;
    case GTK_RESPONSE_HELP:
      open_help_cb (NULL, drgeniusHelp[1]);
      break;
    }
}
