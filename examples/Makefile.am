exampledir        = @prefix@/examples
example_DATA      = input.c inputf.f90 pecos-input.txt \
			timer_sum.c timer_sum.f90
EXTRA_DIST	  = input.c inputf.f90 pecos-input.txt \
			timer_sum.c timer_sum.f90
INCLUDES	  = -I../src
LIBS              = -L../src/.libs -lhpct
example_PROGRAMS  = input timer_sum message
input_SOURCES     = input.c
timer_sum_SOURCES = timer_sum.c
message_SOURCES   = messages.c



