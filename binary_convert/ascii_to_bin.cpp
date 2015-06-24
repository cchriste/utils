#include <iostream>

int main(int argc,char**argv)
{
  float vec[3];
  while (std::cin>>vec[0]>>vec[1]>>vec[2])
  {
    //std::cout<<vec[0]<<" "<<vec[1]<<" "<<vec[2]<<std::endl;
    std::cout.write(reinterpret_cast<const char*>(vec),3*sizeof(float));
  }

  return 0;
}
