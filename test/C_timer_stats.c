//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008-2013,2018-2022 The PECOS Development Team
// Additional Copyright (C) 2018 individual authors
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
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<grvy.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<time.h>
#include<math.h>
#include<sys/time.h>
#include<assert.h>

double Foo_Sleep = 0.098 * 1.e6;
double Bar_Sleep = 0.075 * 1.e6;
double boo_Sleep = 0.23 * 1.e6;

const double Tolerance = 5e-4;	/* a coarse-grained abs tolerance to compare against gettimeofday() */
const double Rtol = .05;	/* a coarse-grained relatlve percentage tolerace                    */

void foo();
void foo_fast();
void bar();
void boo();

struct timeval tv;
double boo_gtod_timing   = 0.0;

int main()
{

  int i,itest;
  const int num_repeat   = 2;
  const int max_iters = 3;
  const int max_iters_fast = 313;
  double t1;

  int call_count;
  double call_mean     = 0.0;
  double call_variance = 0.0;
  double call_min      = 0.0;
  double call_max      = 0.0;

  grvy_log_setlevel(GRVY_ERROR);

  /* Basic test (no embedded timers) */

  grvy_timer_init("GRVY");

  for(itest=0;itest<max_iters_fast;itest++)
    foo_fast();

  grvy_timer_finalize();

  call_count = grvy_timer_stats_count("foo_fast");

  if( call_count != max_iters_fast )
    {
      grvy_printf(GRVY_ERROR,"Timer stat count mismatch for foo_fast\n");
      grvy_printf(GRVY_ERROR,"  --> Found %i calls, expecting %i",call_count,max_iters_fast);
      return(1);
    }

  /* Test with embedded timers */

  boo_gtod_timing   = 0.0;

  grvy_timer_reset();
  grvy_timer_init("GRVY");
  
  for(itest=0;itest<num_repeat;itest++)
    {
      for(i=0;i<max_iters;i++)
	foo();
    }

  grvy_timer_finalize();

  call_count    = grvy_timer_stats_count("foo");
  call_mean     = grvy_timer_stats_mean("boo");
  call_variance = grvy_timer_stats_variance("boo");
  call_min      = grvy_timer_stats_min("boo");
  call_max      = grvy_timer_stats_max("boo");

  if( call_count != num_repeat*max_iters )
    {
      grvy_printf(GRVY_ERROR,"Timer stat count mismatch for foo(%i calls)\n",call_count);
      grvy_printf(GRVY_ERROR,"  --> Found %i calls, expecting %i",call_count,num_repeat*max_iters);
      return(1);
    }

  /* sanity check on min/max */

  assert(call_min <= call_mean);
  assert(call_max >= call_mean);

  double boo_gtod_mean = boo_gtod_timing/grvy_timer_stats_count("boo");

  grvy_printf(GRVY_DEBUG,"call mean = %f\n",call_mean);
  grvy_printf(GRVY_DEBUG,"gtod_mean = %f\n",boo_gtod_mean);

  double diff = fabs(boo_gtod_mean - call_mean);

  if( diff > Tolerance)
    {
      grvy_printf(GRVY_ERROR,"Potential mean value mismatch -> diff = %e (secs).\n",diff);
      grvy_printf(GRVY_ERROR,"The test host could be overloaded or the timer stats may be incorrect.\n");
      return(1);
    }

  /* sanity check on mean value */

  double approx_elapsed_foo = grvy_timer_stats_mean("foo")*grvy_timer_stats_count("foo");
  double approx_elapsed_boo = grvy_timer_stats_mean("boo")*grvy_timer_stats_count("boo");
  double approx_elapsed_bar = grvy_timer_stats_mean("bar")*grvy_timer_stats_count("bar");

  if(fabs(approx_elapsed_foo - grvy_timer_elapsedseconds("foo"))/approx_elapsed_foo > Rtol)
    {
      grvy_printf(GRVY_ERROR,"Potential mean value statistic inconsistency\n");
      grvy_printf(GRVY_ERROR,"The test host could be overloaded or the timer stats may be incorrect\n");
      return(1);
    }

  if(fabs(approx_elapsed_boo - grvy_timer_elapsedseconds("boo"))/approx_elapsed_boo > Rtol)
    {
      grvy_printf(GRVY_ERROR,"Potential mean value statistic inconsistency\n");
      grvy_printf(GRVY_ERROR,"The test host could be overloaded or the timer stats may be incorrect\n");
      return(1);
    }
  
  if(fabs(approx_elapsed_bar - grvy_timer_elapsedseconds("bar"))/approx_elapsed_bar > Rtol)
    {
      grvy_printf(GRVY_ERROR,"Potential mean value statistic inconsistency\n");
      grvy_printf(GRVY_ERROR,"The test host could be overloaded or the timer stats may be incorrect\n");
      return(1);
    }
  

  // make sure summarize function is accessible

  grvy_timer_summarize();

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

void foo_fast()
{
  static int i=0;

  grvy_timer_begin(__func__);

  i++;

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
