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

      grvy_timer_init("GRVY Example Timing");
      grvy_timer_begin("Main Program");

      for(i=0;i<Max_Iters;i++)
	{

	  /* Define the beginning of the overall portion to be monitored */
	  
	  printf("Main iteration loop = %i\n",i);

	  foo();

	}

      grvy_timer_end("Main Program");
      grvy_timer_finalize();
      grvy_timer_summarize();

    }


  //  grvy_timer_finalize();
  //  grvy_timer_summarize();

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
  grvy_timer_begin(__func__);

  usleep(Boo_Sleep);

  grvy_timer_end(__func__);

  return;
    
}
