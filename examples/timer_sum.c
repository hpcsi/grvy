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
// timer_sum.c: Example illustrating performance timing via libGRVY.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<stdio.h>
#include<unistd.h>
#include<grvy.h>
#include<sys/time.h>
#include<time.h>

double Foo_Sleep = 0.3 * 1.e6;
double Bar_Sleep = 0.05 * 1.e6;
double Boo_Sleep = 0.1167 * 1.e6;
double Max_Iters = 10;

void foo();
void bar();
void boo();

int main()
{

  int i,itest;
  int num_repeat = 2;

  /* Primary Iteration Loop */

  for(itest=0;itest<num_repeat;itest++)
    {

      grvy_timer_reset();

      for(i=0;i<Max_Iters;i++)
	{

	  grvy_timer_init("GRVY");

	  /* Define the beginning of the overall portion to be monitored */
	  
	  printf("Main iteration loop = %i\n",i);
	  
	  foo();
	  bar();
	  boo();

	  grvy_timer_finalize();
	  grvy_timer_summarize();

	}

      printf("\nExpecting %10.5e secs\n",Max_Iters*(Foo_Sleep+Boo_Sleep+Bar_Sleep)/1.e6);

      printf("\n");
      printf("Summary data should match grvy_timer_elapsedseconds data points:\n");
      printf("\tbar: %10.5e secs\n",grvy_timer_elapsedseconds("bar"));
      printf("\tfoo: %10.5e secs\n",grvy_timer_elapsedseconds("foo"));
      printf("\tboo: %10.5e secs\n",grvy_timer_elapsedseconds("boo"));

      printf("\nElapsed global time = %10.5e\n",grvy_timer_elapsed_global());
      printf("\n");

      printf("\n");
      printf("Query stats individually\n");
      printf("\tbar (   count): %i\n",grvy_timer_stats_count   ("bar"));
      printf("\tbar (    mean): %e\n",grvy_timer_stats_mean    ("bar"));
      printf("\tbar (variance): %e\n",grvy_timer_stats_variance("bar"));

      /* save timing to historical performance DB */

      grvy_timer_save_hist_exp("C-Example1","comment",1,-1,"revision",0,"hist.h5");

    }

  return 0;
}


void foo()
{

  grvy_timer_begin(__func__);
  usleep(Foo_Sleep);
  grvy_timer_end(__func__);

  return;
    
}

void bar()
{
  grvy_timer_begin(__func__);
  usleep(Bar_Sleep);
  grvy_timer_end(__func__);

  return;
    
}


void boo()
{
  grvy_timer_begin(__func__);
  usleep(Boo_Sleep);
  grvy_timer_end(__func__);

  return;
    
}
