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
// Regression/Test Suite for libGRVY.
//
// $Id: hdf5_access.cpp 16240 2010-12-10 17:00:12Z karl $
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
  
  gt.Init("mytimer");
  gt.BeginTimer("Main Program");
  
  grvy_log_setlevel(GRVY_NOLOG);

  // do some busy work

  double foo = 0;

  gt.BeginTimer("Work");

  for(int i=0;i<1000000;i++)
    foo = foo + i*foo*3.1415*sqrt(foo)*exp(foo)/foo;

  gt.EndTimer("Work");

  // Finalize the performance timer

  gt.EndTimer("Main Program");
  gt.Finalize();

  gt.SaveHistTiming("Example1","",1,-1,"r"GRVY_BUILD_VERSION,0,"test.h5");

  // Query the historical performance logs

  gt.SummarizeHistTiming("test.h5","#","ghist");

  // cleanup

  if(unlink("test.h5"))
    return(1);

  return 0;
}
