#include<grvy.h>
#include<mpi.h>
#include<assert.h>
#include<math.h>

using namespace std;
using namespace GRVY;

void init_data(int offset,int size, double *data)
{
  for(int i=0;i<size;i++)
    data[i] = offset*i;
}

bool verify_data(int offset,int size, double *data)
{
  for(int i=0;i<size;i++)
    {
      if(fabs(data[i] - 1.0*offset*i) > 1.0e-15)
	{
	  printf("verification error: expecting %e, found %e (index=%i)\n",1.0*offset*i,data[i],i);
	  return false;
	}
    }    
  return true;
}

void zero_data(int offset, int size, double *data)
{
  for(int i=0;i<size;i++)
    data[i] = 0.0;
}

int main(int argc, char *argv[])
{

  string inputfile("myfile");
  int num_local;
  const int blocksize = 8192;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&num_local);

  GRVY_MPI_Ocore_Class ocore;

  ocore.Initialize(inputfile,blocksize);

  if(num_local == 0)
    {
      double data[blocksize];

      // Store some known data

      init_data(5,blocksize,data);
      ocore.Write(5,data);

      init_data(12,blocksize,data);
      ocore.Write(12,data);

      init_data(14,blocksize,data);
      ocore.Write(14,data);

      init_data(1,blocksize,data);
      ocore.Write(1,data);

      // Change data locally

      zero_data(5,blocksize,data);
      zero_data(12,blocksize,data);
      zero_data(14,blocksize,data);
      zero_data(1,blocksize,data);

      // Read back known data and verify

      ocore.Read(5,data);
      assert(verify_data(5,blocksize,data) );

      ocore.Read(1,data);
      assert(verify_data(1,blocksize,data) );

      ocore.Read(12,data);
      assert(verify_data(12,blocksize,data) );

      ocore.Read(14,data);
      assert(verify_data(14,blocksize,data) );

      printf("data[0] = %f\n",data[0]);
      printf("data[1] = %f\n\n",data[1]);

    }

  ocore.Finalize();
  MPI_Finalize();

}


