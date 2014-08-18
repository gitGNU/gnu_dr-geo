#ifndef xmltodxf
#define xmltodxf

#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "dl_dxf.h"
#include <iostream>
#include <map>
#include "dl_creationadapter.h"
#include <vector>
#include <math.h>
using namespace std;

class point_
{
public:
  double x,y;
};


class writedxf
{
public:
  std::map<string, point_> _points;
  std::vector<DL_PointData> P;
  DL_Dxf * dxf;
  DL_WriterA *dw;

  writedxf(const char * _filename);
  void parsePoint(xmlNode * node);
  void parseLine(xmlNode * node);
  void parseCircle(xmlNode * node);
  void parseArc(xmlNode * node);
  void writehead();
  void writecode(char *xmlfilename);
  void writefoot();
};
#endif