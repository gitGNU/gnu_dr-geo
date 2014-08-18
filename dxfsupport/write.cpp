#include "xmltodxf.h"

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    std::cout << "Usage ./write <inputfile.xml> <output_file.dxf>";
    return 1;
  }
  writedxf *a = new writedxf(argv[2]);
  a->writehead();
  a->writecode(argv[1]);
  a->writefoot();
  return 0;
}
