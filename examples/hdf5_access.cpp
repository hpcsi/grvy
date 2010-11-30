//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010 The PECOS Development Team
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
//--------------------------------------------------------------------------
//
// cpp-example.cpp: Simple usage examples in C++.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<cstdio>
#include<cmath>
#include<cstdlib>
#include<iostream>
#include<grvy.h>

using namespace GRVY;
using namespace std;

int main(int argc, char **argv)
{
  GRVY_Timer_Class gt;	// GRVY performance timer
  GRVY_HDF5_Class h5;	// GRVY object for interacting with HDF5 files
  
  gt.Init("mytimer");
  gt.BeginTimer("Main Program");
  
  grvy_log_setlevel(GRVY_INFO);
  //  grvy_log_setlevel(GRVY_DEBUG);

  // Create a new hdf file

  grvy_printf(GRVY_INFO,"About to create hdf file\n");

  if(h5.Create("koomie.h5",true))
    exit(1);

  // Create a new group

  h5.GroupCreate("solution_data");

  // Close the file
  h5.Close();

  // do some busy work

  double foo = 0;

  gt.BeginTimer("Work");

  for(int i=0;i<1000000;i++)
    foo = foo + i*foo*3.1415*sqrt(foo)*exp(foo)/foo;

  grvy_printf(GRVY_DEBUG,"foo is %f\n",foo);

  gt.EndTimer("Work");

  // Finalize the performance timer

  gt.EndTimer("Main Program");
  gt.Finalize();
  gt.Summarize();

  gt.SaveHistTiming("Example1","",1,"koomie2.h5");

  // Query the historical performance logs

  gt.SummarizeHistTiming("koomie2.h5","ghist");

  return 0;
}
