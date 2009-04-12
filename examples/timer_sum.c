#include<hpct.h>
#include<stdio.h>
#include<unistd.h>

double Foo_Sleep = 0.3 * 1.e6;
double Bar_Sleep = 0.05 * 1.e6;
double Poo_Sleep = 0.1167 * 1.e6;
double Max_Iters = 10;

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
      for(i=0;i<Max_Iters;i++)
	{
	  
	  /* Define the beginning of the overall portion to be monitored */
	  
	  hpct_timer_init();
	  
	  printf("Main iteration loop = %i\n",i);
	  
	  foo();
	  bar();
	  poo();
	  
	  hpct_timer_finalize();
	}
      
      hpct_timer_summarize();
      
      printf("\nExpecting %10.5e secs\n",Max_Iters*(Foo_Sleep+Poo_Sleep+Bar_Sleep)/1.e6);
    }

  return 0;
}


void foo()
{
  
  hpct_timer_begin(__func__);
  usleep(Foo_Sleep);
  hpct_timer_end(__func__);

  return;
    
}

void bar()
{
  hpct_timer_begin(__func__);
  usleep(Bar_Sleep);
  hpct_timer_end(__func__);

  return;
    
}


void poo()
{
  hpct_timer_begin(__func__);
  usleep(Poo_Sleep);
  hpct_timer_end(__func__);

  return;
    
}
