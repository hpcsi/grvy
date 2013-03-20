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
// Regression/Test Suite for libGRVY.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<grvy.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<time.h>
#include<math.h>
#include <sys/time.h>

double Foo_Sleep = 0.058 * 1.e6;
double Bar_Sleep = 0.035 * 1.e6;
double boo_Sleep = 0.023  * 1.e6;
double Max_Iters = 1;

void foo();
void bar();
void boo();

int main(int argc, char*argv[])
{

  int i,itest;
  int num_repeat = 1;
  double t1;
  int flag;

  /* Primary Iteration Loop */

  grvy_timer_init("GRVY - a really long string to test that the max width is correctly capped.");
  
  /* Do some work - note that foo() includes calls to other
   * routines to test embedded timer capability.  */
  
  for(itest=0;itest<num_repeat;itest++)
    {
      for(i=0;i<Max_Iters;i++)
	foo();
    }

  grvy_timer_finalize();
  grvy_timer_summarize();

  /* make sure we get an error if we try to set the width too small */

  grvy_log_setlevel(GRVY_NOLOG); // silence expected error message

  if(grvy_timer_set_summarize_width(119) != 1)
    return 1;

  return 0;
}


void foo()
{
  grvy_timer_begin(__func__);

  usleep(Foo_Sleep);
  bar();

  grvy_timer_end(__func__);
  return;
}

void bar()
{
  grvy_timer_begin(__func__);

  usleep(Bar_Sleep);
  boo();

  grvy_timer_end(__func__);

  return;
}


void boo()
{
  static double t1;

  grvy_timer_begin(__func__);

  usleep(boo_Sleep);

  grvy_timer_end(__func__);

  return;
    
}
