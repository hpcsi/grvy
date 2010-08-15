exampledir              = @prefix@/examples
example_DATA            = input.c inputf.f90 pecos-input.txt \
		       	  timer_sum.c timer_sumf.f90
EXTRA_DIST	        = input.c inputf.f90 pecos-input.txt \
		    	  timer_sum.c timer_sumf.f90 messages.c

INCLUDES                = -I$(top_srcdir)/src -I$(top_builddir)/src
AM_FCFLAGS              = $(INCLUDES)
AM_CFLAGS               = $(INCLUDES)
AM_CPPFLAGS             = $(INCLUDES)
LIBS                    = -L$(top_builddir)/src -lgrvy

example_PROGRAMS        = input timer_sum check_dir inputf timer_sumf message
input_SOURCES           = input.c

inputf_SOURCES          = inputf.f90
inputf_LIBTOOLFLAGS     = --tag=FC

timer_sum_SOURCES       = timer_sum.c
timer_sumf_SOURCES      = timer_sumf.f90
timer_sumf_LIBTOOLFLAGS = --tag=FC

check_dir_SOURCES       = check_dir.c
message_SOURCES         = messages.c
