#include<grvy.h>
#include<mpi.h>
#include<assert.h>
#include<math.h>
#include<stdio.h>

void init_data(int offset,int size, double *data)
{
  int i;
  for(i=0;i<size;i++)
    data[i] = offset*i;
}

int verify_data(int offset,int size, double *data)
{
  int i;
  for(i=0;i<size;i++)
    {
      if(fabs(data[i] - 1.0*offset*i) > 1.0e-15)
	{
	  printf("verification error: expecting %e, found %e (index=%i)\n",1.0*offset*i,data[i],i);
	  return 0;
	}
    }    
  return 1;
}

void zero_data(int offset, int size, double *data)
{
  int i;
  for(i=0;i<size;i++)
    data[i] = 0.0;
}

int main(int argc, char *argv[])
{

  int num_local;
  const int blocksize = 8192;
  int i;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&num_local);

  grvy_ocore_init("myfile",blocksize);

  if(num_local == 0)
    {
      double data[blocksize];

      const int num_records = 800;

      // Store some known data

      for(i=0;i<num_records;i++)
	{
	  init_data(i,blocksize,data);
	  grvy_ocore_write(i,data);
	}

      // Change data locally

      for(i=0;i<num_records;i++)
	zero_data(i,blocksize,data);

      // Read back known data in opposite order and verify

      for(i=num_records-1;i>=0;i--)
	{
	  grvy_ocore_read(i,data);
	  assert(verify_data(i,blocksize,data) );
	}

    }

  grvy_ocore_finalize();
  MPI_Finalize();

}


