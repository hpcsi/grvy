//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010 The PECOS Development Team
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <grvy.h>

const char *test_name = "C math: test isnan functionality";

int main(int argc, char **argv)
{

  float  fminus_one = -1.0;
  double dminus_one = -1.0;

  float should_be_f_nan = sqrt( fminus_one );
  float should_not_be_f_nan = sqrt( 5.0 );

  double should_be_d_nan = sqrt( dminus_one );
  double should_not_be_d_nan = sqrt( 6.0 );

  float *fval;
  double *dval;

  int is_my_val_nan = 0;

  int flag = 0;

  /* Make sure we get float isnan */
  fval = &should_be_f_nan;
  is_my_val_nan = grvy_float_isnan( fval );
  if( is_my_val_nan == 0 )
    flag = 1;

  /* Make sure we don't get a false positive */
  fval = &should_not_be_f_nan;
  is_my_val_nan = grvy_float_isnan( fval );
  if( is_my_val_nan == 1 )
    flag = 1;

  /* Make sure we get double isnan */
  dval = &should_be_d_nan;
  is_my_val_nan = grvy_double_isnan( dval );
  if( is_my_val_nan == 0 )
    flag = 1;

  /* Make sure we don't get a false positive */
  dval = &should_not_be_d_nan;
  is_my_val_nan = grvy_double_isnan( dval );
  if( is_my_val_nan == 1 )
      flag = 1;

  if(flag == 0){
      exit (0);}
  else{ 
    exit (1);}

}
