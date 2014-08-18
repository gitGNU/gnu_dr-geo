#ifndef DXFTOXML_H
#define DXFTOXML_H

#include "dl_creationadapter.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class dxftoxml : public DL_CreationAdapter {
public:
    dxftoxml(string _file);
    ~dxftoxml();
    virtual void addPoint(const DL_PointData& data);
    virtual void addLine(const DL_LineData& data);
    virtual void addArc(const DL_ArcData& data);
    virtual void addCircle(const DL_CircleData& data);
    long createpoint(double, double);
    long counter = 3000000;
    ofstream myfile;

};

#endif // DXFTOXML_H
