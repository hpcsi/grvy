check_PROGRAMS      = C_input_read_variables C_input_nofile_available
INCLUDES	  = -I$(top_srcdir)/src
LIBS              = -L$(top_builddir)/src/.libs -lhpct


# C Tests


C_input_read_variables_SOURCES        = input.c 
C_input_nofile_available_SOURCES      = input-nofile.c


# Fortran Tests

#timer_sum_SOURCES = timer_sum.c
#message_SOURCES   = messages.c

TESTS            =  C_input_read_variables \
		    C_input_nofile_available

