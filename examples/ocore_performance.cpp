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
// Simple utility for benchmarking GRVY Ocore
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
  const int num_iterations = 4096*128; // 32 GB worth of data to write

  GRVY_MPI_Ocore_Class ocore;
  GRVY_Timer_Class gt;

  gt.Init("Ocore Performance");

  assert(ocore.Initialize(inputfile,1,MPI_COMM_WORLD) == 0); 

  if( ocore.isMaster() )
    {
      double data[blocksize];

      const int num_records = 1;

      // Store some data 

      init_data(100,blocksize,data);

      gt.BeginTimer("Write");
      for(int iter=0;iter<num_iterations;iter++)
	for(int i=0;i<num_records;i++)
	  ocore.Write(i,data);
      gt.EndTimer("Write");

      // Read back data

      gt.BeginTimer("Read");
      for(int iter=0;iter<num_iterations;iter++)
	for(int i=0;i<num_records;i++)
	  ocore.Read(i,data);
      gt.EndTimer("Read");

    }

  gt.Finalize();

  if(ocore.isMaster())
    gt.Summarize();

  ocore.Finalize();

}


