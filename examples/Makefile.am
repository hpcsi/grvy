exampledir        = @prefix@/examples
example_DATA      = input.c inputf.f90 pecos-input.txt \
			timer_sum.c timer_sum.f90
EXTRA_DIST	  = input.c inputf.f90 pecos-input.txt \
			timer_sum.c timer_sum.f90

INCLUDES          = -I$(top_srcdir)/src -I$(top_builddir)/src
AM_FCFLAGS        = $(INCLUDES)
AM_CFLAGS         = $(INCLUDES)
AM_CPPFLAGS       = $(INCLUDES)
LIBS              = -L$(top_builddir)/src -lhpct

example_PROGRAMS  = input timer_sum check_dir inputf #message
input_SOURCES     = input.c
inputf_SOURCES    = inputf.f90
timer_sum_SOURCES = timer_sum.c
check_dir_SOURCES = check_dir.c
#message_SOURCES   = messages.c
