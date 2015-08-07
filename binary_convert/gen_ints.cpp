#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <cstdio>
#include <mpi.h>
#include <string>
#include <sstream>

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
						       ( std::ostringstream() << std::dec << x ) ).str()

int main(int argc,char**argv)
{
  if (argc < 3)
  {
    std::cout<<"Generates pseudorandom 64-bit unsigned ints within a given range [min,max).\n";
    std::cout<<"Usage: gen_ints <nints> <outfilename> [random_seed]\n";
    std::cout<<"Example: gen_ints 512 ints.bin `od -vAn -N4 -tu4 < /dev/urandom`\n";
    return -1;
  }
  if (argc > 3)
    srand48(atol(argv[3]));

  int rank,size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  char *str_end;
  uint64_t tot=strtoull(SSTR(argv[1]).c_str(),&str_end,10);

  uint64_t num_per_rank=tot/size;
  if (num_per_rank*size != tot)
  {
    std::cout<<"error: total must be evenly divisible by number of ranks\n";
    MPI_Finalize();
    return -1;
  }

  const unsigned BUFSZ=2*1048576; //16mb buffer
  unsigned bufsize=tot<BUFSZ?tot:BUFSZ;
  uint64_t *buf=new uint64_t[bufsize];

  MPI_File out_fh;
  MPI_Info info;
  MPI_Status status;
  MPI_Info_create(&info);
  MPI_File_open(MPI_COMM_WORLD,argv[2],MPI_MODE_WRONLY|MPI_MODE_CREATE,info,&out_fh );
  MPI_File_set_size( out_fh, (MPI_Offset)(tot*sizeof(uint64_t)) );
  uint64_t k, start, end, wr_head, wr_start;
  start=num_per_rank*rank;
  end=start+num_per_rank-1;
  end = (end > tot)?tot : end;
  wr_start = wr_head = start * sizeof(uint64_t);

  std::cout<<"tot("<<tot<<") start("<<start<<") end("<<end<<") num_per_rank("<<num_per_rank<<") wr_start("<<wr_start<<")\n";

  unsigned j=0;
  uint64_t num=num_per_rank-1;
  for (uint64_t i=0;i<num_per_rank;i++,j++)
  {
    buf[j]=start+num*drand48();
    if (j>=bufsize) {
      std::cout<<"("<<rank<<") writing "<<j*sizeof(uint64_t)<<" bytes.\n";
      MPI_File_write_at( out_fh, wr_head, (void*)buf, j*sizeof(uint64_t), MPI_CHAR, &status);
      j=0;
      wr_head+=j*sizeof(uint64_t);
    }
  }
  //write remainder
  if (j>0)
    MPI_File_write_at( out_fh, wr_head, (void*)buf, j*sizeof(uint64_t), MPI_CHAR, &status);
  
  std::cout<<"("<<rank<<") writing "<<j*sizeof(uint64_t)<<" bytes.\n";

  MPI_Finalize();
  return 0;
}
