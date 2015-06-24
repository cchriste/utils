#include <iostream>

int main(int argc,char**argv)
{
  float vec[3];
  while (std::cin.read(reinterpret_cast<char*>(vec),3*sizeof(float)))
    std::cout<<vec[0]<<" "<<vec[1]<<" "<<vec[2]<<std::endl;

  return 0;
}
