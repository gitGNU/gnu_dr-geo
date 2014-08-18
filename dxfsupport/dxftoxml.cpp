#include "dxftoxml.h"

dxftoxml::dxftoxml(string _file)
{
  myfile.open (_file);
  myfile << "<?xml version=\"1.0\"?>";
  myfile << "<drgenius>";
  myfile << "<drgeo name=\"a\" scale=\"30.000000\" origin_x=\"0.000000\" origin_y=\"0.000000\" grid=\"False\">";
  myfile << "\n";
}

void dxftoxml::addPoint(const DL_PointData& data)
{
  createpoint(data.x, data.y);
}

void dxftoxml::addLine(const DL_LineData& data)
{
  long refa = createpoint(data.x1, data.y1);
  long refb = createpoint(data.x2, data.y2);
  myfile << "<segment id=\"" << counter << "\" type=\"2pts\" color=\"Black\" thickness=\"Normal\" style=\"Cross\" filled=\"False\" masked=\"False\" name=\"\">";
  myfile << "<parent ref=\"" << refa << "\"/><parent ref=\"" << refb << "\"/>";
  myfile << "</segment>\n";
  counter++;
}

void dxftoxml::addArc(const DL_ArcData& data)
{
  double ang = data.angle1 + data.angle2 / 2;
  long refa = createpoint(cos(data.angle1) * data.radius, sin(data.angle1) * data.radius);
  long refb = createpoint(cos(ang) * data.radius, sin(ang) * data.radius);
  long refc = createpoint(cos(data.angle2) * data.radius, sin(data.angle2) * data.radius);
  myfile << "<arcCircle id=\"" << counter << "\" type=\"3pts\" ";
  myfile << "color=\"Black\" thickness=\"Normal\" style=\"Cross\" ";
  myfile << "filled=\"False\" masked=\"False\" ";
  myfile << "name=\"\">";

  myfile << "<parent ref=\"" << refa << "\"/><parent ref=\"" << refb << "\"/><parent ref=\"" << refc << "\"/>";
  myfile << "</arcCircle>\n";

  counter++;
}

void dxftoxml::addCircle(const DL_CircleData& data)
{
  long refa = createpoint(data.cx, data.cy);
  long refb = createpoint(data.cx + data.radius, data.cy + data.radius);
  myfile << "<circle id=\"" << counter << "\" type=\"2pts\" color=\"Black\" thickness=\"Normal\" style=\"Cross\" filled=\"False\" masked=\"False\" name=\"\">";

  myfile << "<parent ref=\"" << refa << "\"/><parent ref=\"" << refb << "\"/>";
  myfile << "</circle>\n";

  counter++;
}

long dxftoxml::createpoint(double x, double y)
{
  myfile << "<point id=\"" << counter << "\" type=\"Free\" ";
  myfile << "color=\"Red\" thickness=\"Dashed\" style=\"Cross\" ";
  myfile << "filled=\"False\" masked=\"False\" name=\"\">";
  myfile << "<x>"<< x << "</x><y>"<< y <<"</y>";
  myfile << "</point>\n";
  int a = counter;
  counter++;
  return a;
}

dxftoxml::~dxftoxml()
{
  myfile << "</drgeo></drgenius>";
}
