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
// cpp-timer.cpp: Example illustrating performance timing via libGRVY.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<grvy.h>
#include<sys/time.h>
#include<time.h>

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

  // Primary Iteration Loop 

  for(itest=0;itest<num_repeat;itest++)
    {

      gt.Init("GRVY Example Timing");
      gt.BeginTimer("Main Program");

      for(i=0;i<Max_Iters;i++)
	{

	  /* Define the beginning of the overall portion to be monitored */
	  
	  grvy_printf(GRVY_INFO,"Main iteration loop = %i\n",i);
	  foo();

	}

      gt.EndTimer("Main Program");
      gt.Finalize();
      gt.Summarize();
      gt.Reset();
    }

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
