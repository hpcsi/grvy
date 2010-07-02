#include<grvy.h>
#include<stdio.h>
#include<unistd.h>

#include<sys/time.h>
#include<time.h>

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

      grvy_timer_reset();

      for(i=0;i<Max_Iters;i++)
	{

	  grvy_timer_init("GRVY");

	  /* Define the beginning of the overall portion to be monitored */
	  
	  printf("Main iteration loop = %i\n",i);
	  
	  foo();
	  bar();
	  poo();

	  grvy_timer_finalize();
	  grvy_timer_summarize();

	}

      printf("\nExpecting %10.5e secs\n",Max_Iters*(Foo_Sleep+Poo_Sleep+Bar_Sleep)/1.e6);

      printf("\n");
      printf("Summary data should match grvy_timer_elapsedseconds data points:\n");
      printf("\tbar: %10.5e secs\n",grvy_timer_elapsedseconds("bar"));
      printf("\tfoo: %10.5e secs\n",grvy_timer_elapsedseconds("foo"));
      printf("\tpoo: %10.5e secs\n",grvy_timer_elapsedseconds("poo"));

      printf("\nElapsed global time = %10.5e\n",grvy_timer_elapsed_global());
      printf("\n");

      printf("\n");
      printf("Query stats individually\n");
      printf("\tbar (   count): %i\n",grvy_timer_stats_count   ("bar"));
      printf("\tbar (    mean): %e\n",grvy_timer_stats_mean    ("bar"));
      printf("\tbar (variance): %e\n",grvy_timer_stats_variance("bar"));
    }

  return 0;
}


void foo()
{

  char *id = "koomie is the coolest cranes are better";

  grvy_timer_begin(__func__);
  //grvy_timer_begin(id);

  usleep(Foo_Sleep);

  grvy_timer_end(__func__);
  //grvy_timer_end(id);

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
