#include "xmltodxf.h"
using namespace std;

writedxf::writedxf(const char * _filename)
{
  dxf = new DL_Dxf();
  DL_Codes::version exportVersion = DL_Codes::AC1015;
  dw = dxf->out((const char *)_filename, exportVersion);
  if (dw==NULL)
    {
      printf("Cannot open file 'myfile.dxf' for writing.");
    }

}

void writedxf::parsePoint(xmlNode * node)
{
  char* id;
  point_ _point;

  if ((!xmlStrcmp(node->name, (const xmlChar *)"point")))
    {
      id = (char*)xmlGetProp(node, (const xmlChar *)"id");
    }
  if ((!xmlStrcmp(node->children->name, (const xmlChar *)"x")))
    {
      _point.x = atof((const char *)xmlNodeGetContent(node->children));
    }
  if ((!xmlStrcmp(node->children->next->name, (const xmlChar *)"y")))
    {
      _point.y = atof((const char *)xmlNodeGetContent(node->children->next));
    }
  _points.insert(std::pair<std::string, point_>((string)id, _point));

  dxf->writePoint(
        *dw,
        DL_PointData(_point.x,
                     _point.y,
                     0.0),
        DL_Attributes("mainlayer", 256, -1, "BYLAYER"));
}

void writedxf::parseLine(xmlNode * node)
{
  if ((!xmlStrcmp(node->children->name, (const xmlChar *)"parent")))
    {
      auto ref = (char*)xmlGetProp(node->children, (const xmlChar *)"ref");
      auto startpoint = _points[ref];
      ref = (char*)xmlGetProp(node->children->next, (const xmlChar *)"ref");
      auto endpoint = _points[ref];
      dxf->writeLine(
            *dw,
            DL_LineData(startpoint.x,   // start point
                        startpoint.y,
                        0.0,
                        endpoint.x,   // end point
                        endpoint.y,
                        0.0),
            DL_Attributes("mainlayer", 256, -1, "BYLAYER"));
    }
}


void writedxf::parseCircle(xmlNode * node)
{
  if ((!xmlStrcmp(node->children->name, (const xmlChar *)"parent")))
    {
      auto ref = (char*)xmlGetProp(node->children, (const xmlChar *)"ref");
      auto startpoint = _points[ref];
      ref = (char*)xmlGetProp(node->children->next, (const xmlChar *)"ref");
      auto endpoint = _points[ref];
      auto radius = sqrt(fabs((startpoint.x - endpoint.x) *(startpoint.x - endpoint.x) - (startpoint.y - endpoint.y) *(startpoint.y - endpoint.y)));

      dxf->writeCircle(
            *dw,
            DL_CircleData(startpoint.x,   // start point
                          startpoint.y,
                          0.0,
                          radius),
            DL_Attributes("mainlayer", 256, -1, "BYLAYER"));
    }
}

void writedxf::parseArc(xmlNode * node)
{
  if ((!xmlStrcmp(node->children->name, (const xmlChar *)"parent")))
    {
      auto ref = (char*)xmlGetProp(node->children, (const xmlChar *)"ref");
      auto startpoint = _points[ref];
      ref = (char*)xmlGetProp(node->children->next, (const xmlChar *)"ref");
      auto midpoint = _points[ref];
      ref = (char*)xmlGetProp(node->children->next, (const xmlChar *)"ref");
      auto endpoint = _points[ref];

      double cx, cy, radius, startA, endA;
      double mid12x, mid12y,mid23x,mid23y,slope12,slope23,slopePerp12,slopePerp23, co1,co2;

      mid12x = ((startpoint.x+midpoint.x)/2);
      mid12y = ((startpoint.y+midpoint.y)/2);

      mid23x = ((midpoint.x+endpoint.x)/2);
      mid23y = ((midpoint.y+endpoint.y)/2);

      slope12 = (midpoint.y-startpoint.y)/(midpoint.x-startpoint.x); /* Slope of line between point 1 and 2 */
      slope23 = (endpoint.y-midpoint.y)/(endpoint.x-midpoint.x); /* Slope of line between point 2 and 3 */

      slopePerp12 = -(1/slope12); /* Slope of perpendicular 1 */
      slopePerp23 = -(1/slope23); /* Slope of perpendicular 1 */

      co1 = mid12y -(slopePerp12*mid12x); /* Constant 1 */
      co2 = mid23y -(slopePerp23*mid23x); /* Constant 2 */

      cx = (co1 - co2)/(slopePerp23 - slopePerp12); /* X-coordinate of Center point */
      cy = (slopePerp12 * cx) + co1; /* Y-coordinate of Center point */

      radius = sqrt(pow((cx-startpoint.x), 2) + pow((cy-startpoint.y), 2)); /* Radius of an Arc */

      startA = (180/M_PI) *atan2((startpoint.y-cy),(startpoint.x-cx)); /* Starting Angle */
      endA = (180/M_PI) *  atan2 ((endpoint.y-cy),(endpoint.x-cx)) ; /* Ending Angle */


      dxf->writeArc(
            *dw,
            DL_ArcData(startpoint.x,
                       startpoint.y,
                       0.0,
                       radius,
                       startA,
                       endA),
            DL_Attributes("mainlayer", 256, -1, "BYLAYER"));


    }
}


void writedxf::writehead()
{
  dxf->writeHeader(*dw);
  // int variable:
  dw->dxfString(9, "$INSUNITS");
  dw->dxfInt(70, 4);
  // real (double, float) variable:
  dw->dxfString(9, "$DIMEXE");
  dw->dxfReal(40, 1.25);
  // string variable:
  dw->dxfString(9, "$TEXTSTYLE");
  dw->dxfString(7, "Standard");
  // vector variable:
  dw->dxfString(9, "$LIMMIN");
  dw->dxfReal(10, 0.0);
  dw->dxfReal(20, 0.0);
  dw->sectionEnd();
  dw->sectionTables();
  dxf->writeVPort(*dw);
  dw->tableLineTypes(25);
  dxf->writeLineType(*dw, DL_LineTypeData("BYBLOCK", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("BYLAYER", 0));
  dxf->writeLineType(*dw,
                     DL_LineTypeData("CONTINUOUS", 0));
  dxf->writeLineType(*dw,
                     DL_LineTypeData("ACAD_ISO02W100", 0));
  dxf->writeLineType(*dw,
                     DL_LineTypeData("ACAD_ISO03W100", 0));
  dxf->writeLineType(*dw,
                     DL_LineTypeData("ACAD_ISO04W100", 0));
  dxf->writeLineType(*dw,
                     DL_LineTypeData("ACAD_ISO05W100", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("BORDER", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("BORDER2", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("BORDERX2", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("CENTER", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("CENTER2", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("CENTERX2", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("DASHDOT", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("DASHDOT2", 0));
  dxf->writeLineType(*dw,
                     DL_LineTypeData("DASHDOTX2", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("DASHED", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("DASHED2", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("DASHEDX2", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("DIVIDE", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("DIVIDE2", 0));
  dxf->writeLineType(*dw,
                     DL_LineTypeData("DIVIDEX2", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("DOT", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("DOT2", 0));
  dxf->writeLineType(*dw, DL_LineTypeData("DOTX2", 0));
  dw->tableEnd();
  int numberOfLayers = 3;
  dw->tableLayers(numberOfLayers);

  dxf->writeLayer(*dw,
                  DL_LayerData("0", 0),
                  DL_Attributes(
                    std::string(""),      // leave empty
                    DL_Codes::black,        // default color
                    100,                  // default width
                    "CONTINUOUS"));       // default line style

  dxf->writeLayer(*dw,
                  DL_LayerData("mainlayer", 0),
                  DL_Attributes(
                    std::string(""),
                    DL_Codes::red,
                    100,
                    "CONTINUOUS"));

  dxf->writeLayer(*dw,
                  DL_LayerData("anotherlayer", 0),
                  DL_Attributes(
                    std::string(""),
                    DL_Codes::black,
                    100,
                    "CONTINUOUS"));

  dw->tableEnd();
  dxf->writeStyle(*dw, DL_StyleData("standard", 0, 2.5, 1.0, 0.0, 0, 2.5, "standard", ""));
  dxf->writeView(*dw);
  dxf->writeUcs(*dw);

  dw->tableAppid(1);
  dw->tableAppidEntry(0x12);
  dw->dxfString(2, "ACAD");
  dw->dxfInt(70, 0);
  dw->tableEnd();
  dxf->writeDimStyle(*dw,
                     1,
                     1,
                     1,
                     1,
                     1);
  dxf->writeBlockRecord(*dw);
  dxf->writeBlockRecord(*dw, "myblock1");
  dxf->writeBlockRecord(*dw, "myblock2");
  dw->tableEnd();
  dw->sectionEnd();
  dw->sectionBlocks();

  dxf->writeBlock(*dw,
                  DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
  dxf->writeEndBlock(*dw, "*Model_Space");

  dxf->writeBlock(*dw,
                  DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
  dxf->writeEndBlock(*dw, "*Paper_Space");

  dxf->writeBlock(*dw,
                  DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
  dxf->writeEndBlock(*dw, "*Paper_Space0");

  dxf->writeBlock(*dw,
                  DL_BlockData("myblock1", 0, 0.0, 0.0, 0.0));
  // ...
  // write block entities e.g. with dxf->writeLine(), ..
  // ...
  dxf->writeEndBlock(*dw, "myblock1");

  dxf->writeBlock(*dw,
                  DL_BlockData("myblock2", 0, 0.0, 0.0, 0.0));
  // ...
  // write block entities e.g. with dxf->writeLine(), ..
  // ...
  dxf->writeEndBlock(*dw, "myblock2");

  dw->sectionEnd();
  dw->sectionEntities();
}

void writedxf::writecode(char *xmlfilename)
{
  xmlDoc *document;
  xmlNode *root, *first_child, *node;
  char *filename;
  filename = xmlfilename;

  document = xmlReadFile(filename, NULL, 0);
  root = xmlDocGetRootElement(document);

  cout << root->name;

  first_child = root->children;

  for (node = first_child->children; node; node = node->next)
    {
      if ((!xmlStrcmp(node->name, (const xmlChar *)"point")))
        {
          this->parsePoint(node);
        }
    }

  for (node = first_child->children; node; node = node->next)
    {
      if ((!xmlStrcmp(node->name, (const xmlChar *)"segment")))
        {
          this->parseLine(node);
        }
      if ((!xmlStrcmp(node->name, (const xmlChar *)"circle")))
        {
          this->parseCircle(node);
        }
      if ((!xmlStrcmp(node->name, (const xmlChar *)"arc")))
        {
          this->parseArc(node);
        }
      if ((!xmlStrcmp(node->name, (const xmlChar *)"line")))
        {
          this->parseLine(node);
        }
    }
}

void writedxf::writefoot()
{
  dw->sectionEnd();
  dxf->writeObjects(*dw);
  dxf->writeObjectsEnd(*dw);
  dw->dxfEOF();
  dw->close();
  delete dw;
  delete dxf;
}