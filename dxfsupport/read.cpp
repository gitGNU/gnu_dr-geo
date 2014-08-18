#include <iostream>
#include "dxftoxml.h"
#include "dl_dxf.h"
#include "dl_creationadapter.h"

using namespace std;

void readfile(string file, string output_file)
{
  // Load DXF file into memory:
  std::cout << "Reading file " << file << "...\n";
  std::cout << "Writing file " << output_file << "...\n";
  dxftoxml* creationClass = new dxftoxml( output_file);
  DL_Dxf* dxf = new DL_Dxf();
  if (!dxf->in(file, creationClass)) { // if file open failed
      std::cerr << file << " could not be opened.\n";
      return;
  }
  delete dxf;
  delete creationClass;
}

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    std::cout << "Usage ./read <inputfile.dxf> <output_file.xml>";
    return 1;
  }
  readfile(argv[1], argv[2]);
}
