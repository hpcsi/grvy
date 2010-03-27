#include <stdio.h>
#include <stdlib.h>
#include <grvy.h>

/*----------------------------------------------
 * GRVY Test Suite
 *
 * $Id:  $
 *----------------------------------------------*/

const char *test_name = "C math: test isinf functionality";

int main(int argc, char **argv)
{

  float  fone = 1.0;
  double done = 1.0;

  float  fzero = 0.0;
  double dzero = 0.0;

  float should_be_f_inf = fone/fzero;
  float should_not_be_f_inf = 5.0;

  double should_be_d_inf = done/dzero;
  double should_not_be_d_inf = 6.0 ;

  float *fval;
  double *dval;

  int is_my_val_inf = 0;

  int flag = 0;

  /* Make sure we get float inf */
  fval = &should_be_f_inf;
  is_my_val_inf = grvy_float_isinf( fval );
  if( is_my_val_inf == 1) printf("Infinity detected. \n");
  if( is_my_val_inf == 0 )
    flag = 1;

  /* Make sure we don't get a false positive */
  fval = &should_not_be_f_inf;
  is_my_val_inf = grvy_float_isinf( fval );
  if( is_my_val_inf == 1) printf("Infinity detected. \n");
  if( is_my_val_inf == 1 )
    flag = 1;

  /* Make sure we get double isnan */
  dval = &should_be_d_inf;
  is_my_val_inf = grvy_double_isinf( dval );
  if( is_my_val_inf == 1) printf("Infinity detected. \n");
  if( is_my_val_inf == 0 )
    flag = 1;

  /* Make sure we don't get a false positive */
  dval = &should_not_be_d_inf;
  is_my_val_inf = grvy_double_isinf( dval );
  if( is_my_val_inf == 1) printf("Infinity detected. \n");
  if( is_my_val_inf == 1 )
    flag = 1;

  if(flag == 0){
      printf("It's all good. \n");
      exit (0);}
  else {
    printf("Karl's mom dropped the ball. \n");
    exit (1);}

}
