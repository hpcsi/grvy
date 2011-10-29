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

#include<math.h>
#include<grvy.h>

int main(int argc, char **argv)
{

  int i;
  double foo    = 0;
  char tmpdir[] = "tmpdir-XXXXXX";

  grvy_timer_init("mytimer");
  grvy_timer_begin("Main Program");
  
  grvy_log_setlevel(GRVY_NOLOG);

  if(grvy_create_scratch_dir(tmpdir))
    return(1);

  // do some busy work

  grvy_timer_begin("Work");

  for(i=0;i<1000000;i++)
    foo = foo + i*foo*3.1415*sqrt(foo)*exp(foo)/foo;

  grvy_timer_end("Work");

  // Finalize the performance timer

  grvy_timer_end("Main Program");
  grvy_timer_finalize();

  grvy_timer_save_hist("Example1","comment",1,"test.h5");

  // Query the historical performance logs

  grvy_summarize_hist_timing("test.h5","#",tmpdir);

  // cleanup

  if(unlink("test.h5"))
    return(1);

  return 0;
}
