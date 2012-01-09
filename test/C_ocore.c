//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011,2012 The PECOS Development Team
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
// ocore.c: Example of MPI out-of-core usage in C
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


#include<grvy.h>
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
  const int blocksize = 8192;
  int i;

  assert( grvy_ocore_init("mpi_ocore.input") == 0);

  if(grvy_ocore_master())
    {
      double data[blocksize];

      const int num_records = 800;

      // Store some known data

      for(i=0;i<num_records;i++)
	{
	  init_data(i,blocksize,data);
	  grvy_ocore_write_double(i,data);
	}

      // Change data locally

      for(i=0;i<num_records;i++)
	zero_data(i,blocksize,data);

      // Read back known data in opposite order and verify

      for(i=num_records-1;i>=0;i--)
	{
	  grvy_ocore_read_double(i,data);
	  assert(verify_data(i,blocksize,data) );
	}

    }

  grvy_ocore_finalize();
  exit(0);
}


