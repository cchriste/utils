#include <iostream>
#include <cstdlib>

int main(int argc,char**argv)
{
  if (argc < 3)
  {
    std::cout<<"Usage: gen_vecs <nvecs> <maxval> [random_seed]\n";
    std::cout<<"Example: gen_vecs 512 2000 `od -vAn -N4 -tu4 < /dev/urandom`";
    return -1;
  }
  if (argc > 3)
    srand48(atol(argv[3]));

  unsigned nvecs=atol(argv[1]);
  double maxv=atof(argv[2]);
  double vec[3];
  for (unsigned i=0;i<nvecs;i++)
  {
    vec[0]=maxv*(-0.5+drand48());
    vec[1]=maxv*(-0.5+drand48());
    vec[2]=maxv*(-0.5+drand48());
    std::cout.write(reinterpret_cast<const char*>(vec),3*sizeof(double));
  }
  
  return 0;
}
