#include<grvy.h>
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
  const int blocksize = 8192;

  GRVY_MPI_Ocore_Class ocore;

  assert(ocore.Initialize(inputfile,blocksize) == 0); 

  if( ocore.isMaster() )
    {
      double data[blocksize];

      const int num_records = 800;

      // Store some known data

      for(int i=0;i<num_records;i++)
	{
	  init_data(i,blocksize,data);
	  ocore.Write(i,data);
	}

      // Change data locally

      for(int i=0;i<num_records;i++)
	zero_data(i,blocksize,data);

      // Read back known data in opposite order and verify

      for(int i=num_records-1;i>=0;i--)
	{
	  ocore.Read(i,data);
	  assert(verify_data(i,blocksize,data) );
	}

      // Loop over all active data (say if we want to save to disk)

      size_t pool_size = ocore.NumActive();

      for(size_t i=0;i<pool_size;i++)
	{
	  int index = ocore.PopRecord(data);
	  assert(verify_data(index,blocksize,data) );
	}

    }

  ocore.Finalize();

}


