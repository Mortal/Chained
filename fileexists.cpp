#include <fstream>

bool fileExists(const char *fileName)
{
  std::fstream fin;
  fin.open(fileName,std::ios::in);
  if( fin.is_open() )
  {
    fin.close();
    return true;
  }
  fin.close();
  return false;
}
