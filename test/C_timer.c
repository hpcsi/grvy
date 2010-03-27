#include<grvy.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<time.h>
#include<math.h>

/*----------------------------------------------
 * GRVY Test Suite
 *
 * $Id$
 *----------------------------------------------*/

double Foo_Sleep = 0.098 * 1.e6;
double Bar_Sleep = 0.075 * 1.e6;
double Poo_Sleep = 0.23  * 1.e6;
double Max_Iters = 3;
double Tolerance = 5e-3;	/* a coarse-grained check only to catch major issues */

void foo();
void bar();
void poo();

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
	  
	  foo();
	  bar();
	  poo();

	  grvy_timer_finalize();
	  //	  grvy_timer_summarize();
	}


      if(i=Max_Iters-1)
	{
	  double expecting = Max_Iters*(Foo_Sleep+Poo_Sleep+Bar_Sleep)/1.e6;
	  double measured  = grvy_timer_elapsedseconds("bar") +
	                     grvy_timer_elapsedseconds("foo") + 
	                     grvy_timer_elapsedseconds("poo");

	  if( fabs(measured-expecting) > Tolerance)
	    {
	      printf("--> Timing Difference = %10.5e \n",fabs(measured-expecting));
	      printf("--> The test host could be overloaded or the timer may be incorrect\n");
	      return(1);
	    }

	  else
	    return(0);

	}
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


void poo()
{
  grvy_timer_begin(__func__);
  usleep(Poo_Sleep);
  grvy_timer_end(__func__);

  return;
    
}
