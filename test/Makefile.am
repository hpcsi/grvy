check_PROGRAMS    = C_input_read_variables C_input_nofile_available \
	            C_input_read_missing_variable C_input_register_variables \
	            C_input_register_read_missing_variable C_timer_sum
INCLUDES	  = -I$(top_srcdir)/src
LIBS              = -L$(top_builddir)/src/.libs -lhpct

# C Tests

C_input_read_variables_SOURCES                 = input.c 
C_input_nofile_available_SOURCES               = input-nofile.c
C_input_read_missing_variable_SOURCES          = input-missing-var.c 
C_input_register_variables_SOURCES             = input-register.c
C_input_register_read_missing_variable_SOURCES = input-reg-missing-var.c
C_timer_sum_SOURCES                            = timer.c

EXTRA_DIST       = init.sh finalize.sh input-example.txt

# Fortran Tests

#timer_sum_SOURCES = timer_sum.c
#message_SOURCES   = messages.c

TESTS            =  init.sh C_input_read_variables         \
		    C_input_nofile_available               \
		    C_input_read_missing_variable          \
		    C_input_register_variables             \
		    C_input_register_read_missing_variable \
		    C_timer_sum \
		    finalize.sh

