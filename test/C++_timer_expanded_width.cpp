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
// Regression/Test Suite for libGRVY.
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <grvy.h>
#include <unistd.h>
#include "examplefile.h"

using namespace GRVY;
using namespace std;

double Foo_Sleep = 0.058 * 1.e6;
double Bar_Sleep = 0.035 * 1.e6;
double boo_Sleep = 0.023  * 1.e6;
double Max_Iters = 1;

GRVY_Timer_Class gt;	// Input parsing object

// Convenience timer macros for begin/end functions

#define FUNC_BEGIN_TIMER gt.BeginTimer(__func__);
#define FUNC_END_TIMER   gt.EndTimer  (__func__);


void foo();
void bar();
void boo();

int main(int argc, char *argv[])
{

  int i,itest;
  int num_repeat = 1;

  gt.Init("GRVY - a really long string to test that the max width is correctly capped.");
  
  /* Do some work - note that foo() includes calls to other
   * routines to test embedded timer capability.  */
  
  for(itest=0;itest<num_repeat;itest++)
    {
      for(i=0;i<Max_Iters;i++)
	foo();
    }

  gt.Finalize();

  if(gt.SetSummarizeWidth(140) != 0)
    return 1;

  gt.Summarize();

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

  usleep(boo_Sleep);

  FUNC_END_TIMER
  return;
    
}
