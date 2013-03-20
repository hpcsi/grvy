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

double Foo_Sleep = 0.098 * 1.e6;
double Bar_Sleep = 0.075 * 1.e6;
double boo_Sleep = 0.23  * 1.e6;
double Max_Iters = 3;
double Tolerance = 5e-4;	/* a coarse-grained check to compare against gettimeofday() */

void foo();
void bar();
void boo();

double gtod_total_timing = 0.0;
double boo_gtod_timing   = 0.0;

struct timeval tv;

int main()
{

  int i,itest;
  int num_repeat = 2;
  double t1;
  int flag;

  // Verify error if no initialization

  /* Primary Iteration Loop */

  gettimeofday(&tv,NULL);
  t1 =  ((double) tv.tv_sec) + 1.e-6*((double) tv.tv_usec);

  grvy_timer_init("GRVY");
  raw_grvy_timer = grvy_timer();
  
  /* Do some work - note that foo() includes calls to other
   * routines to test embedded timer capability.  */
  
  for(itest=0;itest<num_repeat;itest++)
    {
      for(i=0;i<Max_Iters;i++)
	foo();
    }

  grvy_total_timing = grvy_timer_elapsed_global();
  grvy_boo_timing   = grvy_timer_elapsedseconds("boo");

  gettimeofday(&tv,NULL);
  gtod_total_timing +=   ((double) tv.tv_sec) + 1.e-6*((double) tv.tv_usec) - t1;

  grvy_timer_finalize();
  raw_grvy_timer = grvy_timer() - raw_grvy_timer;


  double diff = fabs(grvy_boo_timing-boo_gtod_timing);

  if( diff > Tolerance)
    {
      grvy_printf(GRVY_ERROR,"Function timing mismatch -> diff = %e (secs)\n",diff);
      grvy_printf(GRVY_ERROR,"The test host could be overloaded or the timer may be incorrect\n");
      return(1);
    }

  diff = fabs(grvy_total_timing-gtod_total_timing);

  if( diff > Tolerance)
    {
      grvy_printf(GRVY_ERROR,"Global timing mismatch -> diff = %e (secs)\n",diff);
      grvy_printf(GRVY_ERROR,"The test host could be overloaded or the timer may be incorrect\n");
      return(1);
    }

  diff = fabs(grvy_total_timing-raw_grvy_timer);

  if( diff > Tolerance)
    {
      grvy_printf(GRVY_ERROR,"Internal global timing mismatch -> diff = %e (secs)\n",diff);
      grvy_printf(GRVY_ERROR,"The test host could be overloaded or the timer may be incorrect\n");
      return(1);
    }

  // Test a reset()

  grvy_timer_reset();

  grvy_total_timing = grvy_timer_elapsed_global();
  grvy_boo_timing   = grvy_timer_elapsedseconds("boo");

  if( (grvy_total_timing > Tolerance) || (grvy_boo_timing != 0.0) )
    {
      grvy_printf(GRVY_ERROR,"Problem with timer reset\n");
      grvy_printf(GRVY_ERROR," --> total timing = %f\n",grvy_total_timing);
      grvy_printf(GRVY_ERROR," -->   boo timing = %f\n",grvy_boo_timing);
      return(1);
    }

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

  gettimeofday(&tv,NULL);
  t1 =  ((double) tv.tv_sec) + 1.e-6*((double) tv.tv_usec);

  grvy_timer_begin(__func__);

  usleep(boo_Sleep);

  grvy_timer_end(__func__);

  gettimeofday(&tv,NULL);
  boo_gtod_timing +=   ((double) tv.tv_sec) + 1.e-6*((double) tv.tv_usec) - t1;

  return;
    
}
