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
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<math.h>
#include<grvy.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv)
{

  int i;
  int flag         = 1;
  double foo       = 0;
  char *input_dir  = getenv("GRVY_INPUT_EXAMPLE_DIR");
  //char tmpdir[]    = ".tmpdir";
  char tmpdir[]    = "tmpdir-XXXXXX";
  char tmp_string[1024];

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

  grvy_timer_save_hist    ("Example1","comment",1,"test.h5");
  grvy_timer_save_hist_exp("Example2","comment",2,825,"r2345",250,"test.h5");

  grvy_timer_save_hist_ext    (230.5768,"mymachine","example1","comment",1,"test2.h5");
  grvy_timer_save_hist_ext_exp(4.4678e3,"mymachine","example1","comment",2,825,
  			       "r2345",250,"test2.h5");

  // Query the historical performance logs

  grvy_summarize_hist_timing("test2.h5","#",tmpdir);

  // Verify against gold standard

  sprintf(tmp_string,"tail -2 %s/mymachine/example1 | "
	  "awk '{print $1\" \"$5\" \"$6\" \"$7\" \"$8\" \"$9}' > %s/data",tmpdir,tmpdir);

  flag *= (0 == system(tmp_string));

  sprintf(tmp_string,"diff %s/data %s/ref_files/hdf_timer.txt 2>&1 >/dev/null",tmpdir,input_dir);
  flag *= (0 == system(tmp_string));

  // cleanup

  if(unlink("test.h5"))
    return(1);

  if(unlink("test2.h5"))
    return(1);

  if(flag == 0)
    exit(1);
  else
    exit(0);

  return 0;
}
