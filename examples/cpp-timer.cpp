//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
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
// cpp-timer.cpp: Example illustrating performance timing via libGRVY.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<grvy.h>
#include<sys/time.h>
#include<time.h>
#include<unistd.h>

// Convenience timer macros for begin/end functions

#define FUNC_BEGIN_TIMER gt.BeginTimer(__func__);
#define FUNC_END_TIMER   gt.EndTimer  (__func__);

// Globals

GRVY::GRVY_Timer_Class gt;		// GRVY Timer

const double Foo_Sleep = 0.3 * 1.e6;
const double Bar_Sleep = 0.05 * 1.e6;
const double Boo_Sleep = 0.1167 * 1.e6;
const double Max_Iters = 10;

// Function proto's

void foo();
void bar();
void boo();

int main()
{

  int i,itest;
  int num_repeat = 2;

  // Initialize the timing library - the global timer will be
  // initialized with this call.

  gt.Init("GRVY Example Timing");

  // Outer Test Loop

  for(itest=0;itest<num_repeat;itest++)
    {

      if(itest > 0)
	gt.Reset();
      
      // Define the beginning of the overall portion to be monitored 

      gt.BeginTimer("Main Program");

      for(i=0;i<Max_Iters;i++)
	{
	  grvy_printf(GRVY_INFO,"Main iteration loop = %i\n",i);
	  foo();
	}

      // Finalize the main program timer

      gt.EndTimer("Main Program");
      gt.Finalize();

      // Print performance summary to stdout 

      gt.Summarize();
    }

  // Query timers directly - results should match those shown in
  // grvy_timer_summarize() routine 

  grvy_printf(GRVY_INFO,"Querying global timer elapsed time:\n");
  grvy_printf(GRVY_INFO,"\tElapsed global time = %10.5e secs\n",gt.ElapsedGlobal());

  grvy_printf(GRVY_INFO,"\nQuerying individual timers (exclusive values):\n");

  grvy_printf(GRVY_INFO,"\tfoo: %10.5e secs\n",gt.ElapsedSeconds("foo"));
  grvy_printf(GRVY_INFO,"\tbar: %10.5e secs\n",gt.ElapsedSeconds("bar"));
  grvy_printf(GRVY_INFO,"\tboo: %10.5e secs\n",gt.ElapsedSeconds("boo"));

  grvy_printf(GRVY_INFO,"\nQuerying individual stats for timer \"bar\" (exclusive values)\n");
  grvy_printf(GRVY_INFO,"\tbar (   count): %i\n",gt.StatsCount   ("bar"));
  grvy_printf(GRVY_INFO,"\tbar (    mean): %e\n",gt.StatsMean    ("bar"));
  grvy_printf(GRVY_INFO,"\tbar (variance): %e\n",gt.StatsVariance("bar"));
  grvy_printf(GRVY_INFO,"\tbar (     min): %e\n",gt.StatsMin     ("bar"));
  grvy_printf(GRVY_INFO,"\tbar (     max): %e\n",gt.StatsMax     ("bar"));

  // The above provides exclusive timing statistics, but you can also
  // query inclusive measurements as well

  grvy_printf(GRVY_INFO,"\nQuerying individual timers (inclusive values):\n");

  grvy_printf(GRVY_INFO,"\tfoo: %10.5e secs\n",gt.ElapsedSeconds_inc("foo"));
  grvy_printf(GRVY_INFO,"\tbar: %10.5e secs\n",gt.ElapsedSeconds_inc("bar"));
  grvy_printf(GRVY_INFO,"\tboo: %10.5e secs\n",gt.ElapsedSeconds_inc("boo"));

  grvy_printf(GRVY_INFO,"\nQuerying individual stats for timer \"foo\" (inclusive values)\n");
  grvy_printf(GRVY_INFO,"\tbar (    mean): %e\n",gt.StatsMean_inc    ("foo"));
  grvy_printf(GRVY_INFO,"\tbar (variance): %e\n",gt.StatsVariance_inc("foo"));
  grvy_printf(GRVY_INFO,"\tbar (     min): %e\n",gt.StatsMin_inc     ("foo"));
  grvy_printf(GRVY_INFO,"\tbar (     max): %e\n",gt.StatsMax_inc     ("foo"));

  return 0;
}


void foo()
{
  FUNC_BEGIN_TIMER

  usleep(Foo_Sleep);
  bar();

  FUNC_END_TIMER
  return;
    
}

void bar()
{
  FUNC_BEGIN_TIMER

  usleep(Bar_Sleep);
  boo();

  FUNC_END_TIMER

  return;
    
}


void boo()
{
  FUNC_BEGIN_TIMER

  usleep(Boo_Sleep);

  FUNC_END_TIMER
  return;
    
}
