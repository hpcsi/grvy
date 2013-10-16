//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor, 
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
//
// cpp-ocore.cpp: Example of MPI out-of-core usage in C++
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

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
	  grvy_printf(GRVY_ERROR,"verification error: expecting %e, found %e (index=%i)\n",1.0*offset*i,data[i],i);
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
  string inputfile("mpi_ocore.input");
  const int blocksize = 8192;

  GRVY_MPI_Ocore_Class ocore;
  MPI_Comm WORK_COMM;
  MPI_Comm OCORE_COMM;

  int globRank, globProcs;
  const int numWorkers = 1;

  grvy_log_setlevel(GRVY_DEBUG);

  MPI_Init(&argc,NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &globProcs);
  MPI_Comm_rank(MPI_COMM_WORLD, &globRank);

  if(globProcs <= 1)
    {
      grvy_printf(GRVY_ERROR,"\nThis example requires at least 2 MPI tasks...exiting\n\n");
      MPI_Finalize();
      return 0;
    }

  MPI_Barrier(MPI_COMM_WORLD);

  assert(ocore.Initialize(inputfile,2,MPI_COMM_WORLD) == 0); 

  //  if( ocore.isMaster() )
  if(globRank == 0)
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
  MPI_Finalize();

  return(0);

}


