#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <mpi.h>
#include <string>
#include <sstream>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
						       ( std::ostringstream() << std::dec << x ) ).str()

int main(int argc,char**argv)
{
  if (argc < 5)
  {
    std::cout<<"Generates pseudorandom 64-bit unsigned ints within a given range [min,max).\n";
    std::cout<<"Usage: gen_ints <nints> <min> <max> <outfilename> [random_seed]\n";
    std::cout<<"Example: gen_ints 512 ints.bin `od -vAn -N4 -tu4 < /dev/urandom`\n";
    return -1;
  }
  if (argc > 5)
    srand48(atol(argv[5]));

  int rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::ostringstream filename;
  filename << argv[4] << rank << ".bin";
  FILE *file=fopen(filename.str().c_str(),"wb");
  unsigned long nints=atol(argv[1]);

  char *end;
  unsigned long long _min=strtoull(argv[2],&end,10);
  unsigned long long _max=strtoull(argv[3],&end,10);

  const unsigned BUFSZ=1048576; //8mb buffer
  unsigned bufsize=nints<BUFSZ?nints:BUFSZ;
  unsigned long long *buf=new unsigned long long[bufsize];

  unsigned j=0;
  for (unsigned long i=0;i<nints;i++,j++)
  {
    buf[j]=(((unsigned long long)lrand48() << 32) | (unsigned long long)lrand48());
    if (j>=bufsize) {
      fwrite(buf,sizeof(unsigned long long),j,file);
      j=0;
    }
  }
  //write remainder
  if (j>0)
    fwrite(buf,sizeof(unsigned long long),j,file);
  
  MPI_Finalize();
  return 0;
}
