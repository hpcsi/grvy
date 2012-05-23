exampledir              = @prefix@/examples
example_DATA            = input.c inputf.f90 pecos-input.txt \
		       	  timer.c timerf.f90
EXTRA_DIST	        = input.c inputf.f90 pecos-input.txt \
		    	  timer.c timerf.f90 messages.c .license.stamp

#INCLUDES                = -I$(top_srcdir)/src -I$(top_builddir)/src -I$(top_builddir)/src/common/getpot
INCLUDES                = -I$(top_srcdir)/src -I$(top_builddir)/src 
AM_FCFLAGS              = $(INCLUDES)
AM_CFLAGS               = $(INCLUDES)
#AM_CPPFLAGS             = $(INCLUDES) $(BOOST_CPPFLAGS)
AM_CPPFLAGS             = $(INCLUDES) 
LIBS                    = -L$(top_builddir)/src -lgrvy 

#if LINK_BOOST_LIBS
#   LIBS                += $(BOOST_REGEX_LDFLAGS) $(BOOST_REGEX_LIBS) 
#endif


BUILT_SOURCES           = .license.stamp
CLEANFILES              = .license.stamp *.gcno *.gcda

example_PROGRAMS        = input timer check_dir inputf timerf messages timer cpp-input \
	                  hdf5_access cpp-timer 

input_SOURCES           = input.c
cpp_input_SOURCES       = cpp-input.cpp
hdf5_access_SOURCES     = hdf5_access.cpp

inputf_SOURCES          = inputf.f90
inputf_LIBTOOLFLAGS     = --tag=FC
inputf_LDADD            = ../src/libgrvyf.la

timer_SOURCES           = timer.c
cpp_timer_SOURCES       = cpp-timer.cpp

timerf_SOURCES          = timerf.f90
timerf_LIBTOOLFLAGS     = --tag=FC
timerf_LDADD            = ../src/libgrvyf.la

check_dir_SOURCES       = check_dir.c
messages_SOURCES        = messages.c

if MPI_ENABLED
   example_PROGRAMS    += cpp-ocore ocore ocore_f ocore_perf
   ocore_SOURCES        = ocore.c
   cpp_ocore_SOURCES    = cpp-ocore.cpp
   ocore_f_SOURCES      = ocore_f.f90
   ocore_perf_SOURCES   = ocore_performance.cpp
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