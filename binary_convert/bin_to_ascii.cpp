#include <iostream>

int main(int argc,char**argv)
{
  double vec[3];
  while (std::cin.read(reinterpret_cast<char*>(vec),3*sizeof(double)))
    std::cout<<vec[0]<<" "<<vec[1]<<" "<<vec[2]<<std::endl;

  return 0;
}
