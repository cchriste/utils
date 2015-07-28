#include <iostream>

int main(int argc,char**argv)
{
  unsigned long long vec;
  while (std::cin.read(reinterpret_cast<char*>(&vec),sizeof(unsigned long long)))
    std::cout<<vec<<std::endl;

  return 0;
}
