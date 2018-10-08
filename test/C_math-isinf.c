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
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <grvy.h>

const char *test_name = "C math: test isinf functionality";

int main(int argc, char **argv)
{

  float  fone = 1.0;
  double done = 1.0;

  float  fzero = 0.0;
  double dzero = 0.0;

  float should_be_f_pos_inf = fone/fzero;
  float should_be_f_neg_inf = (-1 *fone)/fzero;
  float should_not_be_f_inf = 5.0;
  float should_be_f_nan     = sqrt( -1 *fone );


  double should_be_d_pos_inf = done/dzero;
  double should_be_d_neg_inf = (-1 *done)/dzero;
  double should_not_be_d_inf = 6.0;
  double should_be_d_nan     = sqrt( -1 *done );

  float *fval;
  double *dval;

  int is_my_val_inf = 0;

  int flag = 0;

  /* Make sure we got separate float positive and negative infinities */
  if (should_be_f_pos_inf == should_be_f_neg_inf)
    flag = 1;

  /* Make sure we get float positive inf */
  fval = &should_be_f_pos_inf;
  is_my_val_inf = grvy_float_isinf( fval );
  if( is_my_val_inf == 0 )
    flag = 1;

  /* Make sure we get float negative inf */
  fval = &should_be_f_neg_inf;
  is_my_val_inf = grvy_float_isinf( fval );
  if( is_my_val_inf == 0 )
    flag = 1;

  /* Make sure we don't get a false positive for finite values */
  fval = &should_not_be_f_inf;
  is_my_val_inf = grvy_float_isinf( fval );
  if( is_my_val_inf == 1 )
    flag = 1;

  /* Make sure we don't get a false positive for NaN */
  fval = &should_be_f_nan;
  is_my_val_inf = grvy_float_isinf( fval );
  if( is_my_val_inf == 1 )
    flag = 1;

  /* Make sure we got separate double positive and negative infinities */
  if (should_be_d_pos_inf == should_be_d_neg_inf)
    flag = 1;

  /* Make sure we get double positive inf */
  dval = &should_be_d_pos_inf;
  is_my_val_inf = grvy_double_isinf( dval );
  if( is_my_val_inf == 0 )
    flag = 1;

  /* Make sure we get double negative inf */
  dval = &should_be_d_neg_inf;
  is_my_val_inf = grvy_double_isinf( dval );
  if( is_my_val_inf == 0 )
    flag = 1;

  /* Make sure we don't get a false positive for finite values */
  dval = &should_not_be_d_inf;
  is_my_val_inf = grvy_double_isinf( dval );
  if( is_my_val_inf == 1 )
    flag = 1;

  /* Make sure we don't get a false positive for NaN */
  dval = &should_be_d_nan;
  is_my_val_inf = grvy_double_isinf( dval );
  if( is_my_val_inf == 1 )
    flag = 1;


  if(flag == 0){
      exit (0);}
  else {
    exit (1);}

}
