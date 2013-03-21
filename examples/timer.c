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
// Simple example illustrating performance timing via libGRVY.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<stdio.h>
#include<unistd.h>
#include<grvy.h>
#include<sys/time.h>
#include<time.h>

const double Foo_Sleep = 0.3 * 1.e6;
const double Bar_Sleep = 0.05 * 1.e6;
const double Boo_Sleep = 0.1167 * 1.e6;
const    int Max_Iters = 10;

void foo();
void bar();
void boo();

int main()
{

  int i,itest;
  const int num_repeat = 2;

  /* Initialize the timing library - the global timer will be
     initialized with this call */

  grvy_timer_init("GRVY Example Timing");

  /* Outer Test Loop */

  for(itest=0;itest<num_repeat;itest++)
    {

      /* Reset the timer to mearure performance timing for each test
	 loop iteration  */

      if(itest > 0)
	grvy_timer_reset();

      /* Define the beginning of the overall code portion to be monitored */

      grvy_timer_begin("Main Program");

      /* Primary Iteration Work Loop */

      for(i=0;i<Max_Iters;i++)
	{
	  printf("Main iteration loop = %i\n",i);
	  foo();
	}

      /* Finalize the main program timer */

      grvy_timer_end("Main Program");
      grvy_timer_finalize();

      /* Print performance summary to stdout */

      grvy_timer_summarize();

    }

  /* Query timers directly - results should match those
     shown in grvy_timer_summarize() routine */

  printf("Querying global timer elapsed time:\n");
  printf("\tElapsed global time = %10.5e secs\n",grvy_timer_elapsed_global());

  printf("\nQuerying individual timers (exclusive values):\n");

  printf("\tfoo: %10.5e secs\n",grvy_timer_elapsedseconds("foo"));
  printf("\tbar: %10.5e secs\n",grvy_timer_elapsedseconds("bar"));
  printf("\tboo: %10.5e secs\n",grvy_timer_elapsedseconds("boo"));

  printf("\nQuerying individual stats for timer \"bar\" (exclusive values)\n");
  printf("\tbar (   count): %i\n",grvy_timer_stats_count   ("bar"));
  printf("\tbar (    mean): %e\n",grvy_timer_stats_mean    ("bar"));
  printf("\tbar (variance): %e\n",grvy_timer_stats_variance("bar"));
  printf("\tbar (     min): %e\n",grvy_timer_stats_min     ("bar"));
  printf("\tbar (     max): %e\n",grvy_timer_stats_max     ("bar"));

  /* The above provides exclusive timing statistics, but you can also
     query inclusive measurements as well */

  printf("\nQuerying individual timers (inclusive values):\n");

  printf("\tfoo: %10.5e secs\n",grvy_timer_elapsedseconds_inc("foo"));
  printf("\tbar: %10.5e secs\n",grvy_timer_elapsedseconds_inc("bar"));
  printf("\tboo: %10.5e secs\n",grvy_timer_elapsedseconds_inc("boo"));

  printf("\nQuerying individual stats for timer \"foo\" (inclusive values)\n");
  printf("\tbar (    mean): %e\n",grvy_timer_stats_mean_inc    ("foo"));
  printf("\tbar (variance): %e\n",grvy_timer_stats_variance_inc("foo"));
  printf("\tbar (     min): %e\n",grvy_timer_stats_min_inc     ("foo"));
  printf("\tbar (     max): %e\n",grvy_timer_stats_max_inc     ("foo"));

  /* save measured timing to historical performance DB (required
     optional HDF library linkage) */

  const int num_procs = 1;

  grvy_timer_save_hist("C-Example1","My clever comment",num_procs,"hist.h5");

  return 0;
}


void foo()
{
  /* Mark the beginning of new timer */

  grvy_timer_begin("foo");

  usleep(Foo_Sleep);
  bar();

  /* Mark the end of the timer */

  grvy_timer_end("foo");

  return;
    
}

void bar()
{
  /* Mark the beginning of new timer (use macro for function name) */

  grvy_timer_begin(__func__);

  usleep(Bar_Sleep);
  boo();

  /* Mark the end of the timer */

  grvy_timer_end(__func__);

  return;
    
}


void boo()
{
  /* Mark the beginning of new timer */

  grvy_timer_begin(__func__);

  usleep(Boo_Sleep);

  /* Mark the end of the timer */

  grvy_timer_end(__func__);

  return;
    
}
