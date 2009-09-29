#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hpct.h"

/*----------------------------------------------
 * HPCT Test Suite
 *
 * $Id:$
 *----------------------------------------------*/

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
  is_my_val_nan = hpct_float_isnan( fval );
  if( is_my_val_nan == 0 )
    flag = 1;

  /* Make sure we don't get a false positive */
  fval = &should_not_be_f_nan;
  is_my_val_nan = hpct_float_isnan( fval );
  if( is_my_val_nan == 1 )
    flag = 1;

  /* Make sure we get double isnan */
  dval = &should_be_d_nan;
  is_my_val_nan = hpct_double_isnan( dval );
  if( is_my_val_nan == 0 )
    flag = 1;

  /* Make sure we don't get a false positive */
  dval = &should_not_be_d_nan;
  is_my_val_nan = hpct_double_isnan( dval );
  if( is_my_val_nan == 1 )
      flag = 1;

  if(flag == 0){
      exit (0);}
  else{ 
    exit (1);}

}
