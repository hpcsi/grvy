exampledir              = @prefix@/examples
example_DATA            = input.c inputf.f90 pecos-input.txt \
		       	  timer_sum.c timer_sumf.f90
EXTRA_DIST	        = input.c inputf.f90 pecos-input.txt \
		    	  timer_sum.c timer_sumf.f90 messages.c

INCLUDES                = -I$(top_srcdir)/src -I$(top_builddir)/src -I$(top_builddir)/src/common/getpot
AM_FCFLAGS              = $(INCLUDES)
AM_CFLAGS               = $(INCLUDES)
AM_CPPFLAGS             = $(INCLUDES) $(BOOST_CPPFLAGS)
LIBS                    = -L$(top_builddir)/src -lgrvy $(BOOST_REGEX_LDFLAGS) $(BOOST_REGEX_LIBS) 

BUILT_SOURCES           = .license.stamp
CLEANFILES              = .license.stamp *.gcno *.gcda

example_PROGRAMS        = input timer_sum check_dir inputf timer_sumf message timer cpp-example \
	                  hdf5_access cpp-timer 

input_SOURCES           = input.c
cpp_example_SOURCES     = cpp-example.cpp
hdf5_access_SOURCES     = hdf5_access.cpp

inputf_SOURCES          = inputf.f90
inputf_LIBTOOLFLAGS     = --tag=FC
inputf_LDADD            = ../src/libgrvyf.la

timer_sum_SOURCES       = timer_sum.c
timer_SOURCES           = timer.c
cpp_timer_SOURCES       = cpp-timer.cpp

timer_sumf_SOURCES      = timer_sumf.f90
timer_sumf_LIBTOOLFLAGS = --tag=FC
timer_sumf_LDADD        = ../src/libgrvyf.la

check_dir_SOURCES       = check_dir.c
message_SOURCES         = messages.c

if MPI_ENABLED
   example_PROGRAMS    += cpp-ocore ocore ocore_f
   ocore_SOURCES        = ocore.c
   cpp_ocore_SOURCES    = cpp-ocore.cpp
   ocore_f_SOURCES      = ocore_f.f90
   ocore_f_LDADD        = ../src/libgrvyf.la
endif


#---------------------------------
# Embedded license header support
#---------------------------------

.license.stamp: $(top_srcdir)/LICENSE
	for file in $(srcdir)/*.cpp $(srcdir)/*.c ; do \
		$(top_srcdir)/src/common/lic_utils/update_license.pl \
			$(top_srcdir)/LICENSE $$file | tee -a $@ ;\
	done
	for file in $(srcdir)/*.f90 ; do \
		$(top_srcdir)/src/common/lic_utils/update_license.pl \
			--c2f_comment $(top_srcdir)/LICENSE $$file | tee -a $@ ;\
	done
	touch .license.stamp