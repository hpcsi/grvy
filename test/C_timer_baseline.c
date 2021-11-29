//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008-2013,2018-2021 The PECOS Development Team
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
#include<stdlib.h>
#include<math.h>

const double Foo_Sleep = 0.03 * 1.e6;
const double Bar_Sleep = 0.05 * 1.e6;
const double Boo_Sleep = 0.1  * 1.e6;
const double Max_Iters = 3;

const double Rel_TOL   = 0.1;	// tolerance (coarse-grained for potentially busy test hosts)

void   foo();
void   bar();
void   boo();
void   vette_relative_diff(char *id, double expected);


int main()
{

  int i,itest;
  int num_repeat = 2;

  grvy_timer_init("GRVY");

  /* Do some work - note that foo() includes calls to other
   * routines to test embedded timer capability.  */
  
  for(itest=0;itest<num_repeat;itest++)
    {
      for(i=0;i<Max_Iters;i++)
	foo();
    }

  grvy_timer_finalize();
  //grvy_timer_summarize();

  double Boo_expected = Boo_Sleep*Max_Iters*num_repeat/1.e6;
  double Bar_expected = Bar_Sleep*Max_Iters*num_repeat/1.e6;
  double Foo_expected = Foo_Sleep*Max_Iters*num_repeat/1.e6;
  double total_expected = Boo_expected + Bar_expected + Foo_expected;

  grvy_printf(GRVY_DEBUG,"Expected time for boo = %e secs\n",Boo_expected);
  grvy_printf(GRVY_DEBUG,"Expected time for bar = %e secs\n",Bar_expected);
  grvy_printf(GRVY_DEBUG,"Expected time for foo = %e secs\n",Foo_expected);

  /* Timer values reasonable? */

  vette_relative_diff("boo",Boo_expected);
  vette_relative_diff("bar",Bar_expected);
  vette_relative_diff("foo",Foo_expected);

  /* Total time reasonable? */

  double total_time = grvy_timer_elapsed_global();

  if(total_time <= 0.0)
    exit(1);

  if(fabs(total_time - total_expected)/total_expected > Rel_TOL)
    {
      grvy_printf(GRVY_ERROR,"Global timer mismatch - test host may be overloaded\n");
      exit(1);
    }

  /* Unassigned timer reasonable? */

  double unassigned = total_time - ( grvy_timer_elapsedseconds("foo") + 
				     grvy_timer_elapsedseconds("boo") + 
				     grvy_timer_elapsedseconds("bar") );

  if(unassigned <= 0.0)
    exit(1);
  
  if(unassigned > 1e-3)
    {
      grvy_printf(GRVY_ERROR,"Unassigned timer mismatch - test host may be overloaded\n");
      grvy_printf(GRVY_ERROR,"--> Unaccounted time = %e\n",unassigned);
      exit(1);
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
  grvy_timer_begin(__func__);

  usleep(Boo_Sleep);

  grvy_timer_end(__func__);

  return;
    
}

void vette_relative_diff(char *id,double expected)
{
  double measured = grvy_timer_elapsedseconds(id);

  if(measured <= 0.0)
    {
      grvy_printf(GRVY_ERROR,"Elapsed time should be > 0 (%s)\n",id);
      exit(1);
    }

  if(fabs(measured-expected)/expected > Rel_TOL)
    {
      grvy_printf(GRVY_INFO,"Timer mismatch - test host may be overloaded\n");
      grvy_printf(GRVY_INFO,"  --> Measured      = %e\n",measured);
      grvy_printf(GRVY_INFO,"  --> Expected      = %e\n",expected);
      grvy_printf(GRVY_INFO,"  --> Relative Diff = %e (%)\n",fabs(measured-expected)/expected);
    }

}
