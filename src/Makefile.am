h_sources               = grvy.h grvy_classes.h grvy_int.h fortran_string_order.h hpct.h
cc_sources              = grvy_input.cpp grvy_classes.cpp grvy_timer.cpp \
		          grvy_utils.cpp grvy_math.cpp grvy_log.cpp grvy_hpct_compat.cpp \
			  grvy_version.cpp

getpot_sources          = common/getpot/getpot.h

INCLUDES                = 
AM_FCFLAGS              = $(INCLUDES)
AM_CFLAGS               = $(INCLUDES)
AM_CPPFLAGS             = $(INCLUDES) $(BOOST_CPPFLAGS)

# Note: the use of hpct_ api syntax and linkage to -lhpct is still
# supported in libGRVY but will be deprecated in the future. 

lib_LTLIBRARIES         = libgrvy.la libhpct.la
library_includedir      = $(includedir)
library_include_HEADERS = $(top_srcdir)/config_grvy.h grvy.h hpct.h
libgrvy_la_LDFLAGS      = $(all_libraries) -release $(GENERIC_RELEASE)
libgrvy_la_SOURCES      = $(getpot_sources) $(cc_sources) $(h_sources)

libhpct_la_LDFLAGS      = $(all_libraries) -release $(GENERIC_RELEASE)
libhpct_la_SOURCES      = $(getpot_sources) $(cc_sources) $(h_sources)

version_SOURCES         = version.cpp grvy.h
version_LDADD           = -L$(top_builddir)/src -lgrvy

#install-exec-hook:
#	cd $(DESTDIR)$(libdir) && \
#	for file in libgrvy*; do $(LN_S) $$file `echo $$file | $(SED) s/grvy/hpct/`; done

#--------------------------
# Fortran Module Interface
#--------------------------

noinst_PROGRAMS         = test test2
standalonedir           = @prefix@/bin
standalone_PROGRAMS     = version

test_SOURCES            = grvy.f90 main.f90
test_LIBTOOLFLAGS       = --tag=FC

test2_SOURCES           = grvy_hpct.f90 main_hpct.f90
test2_LIBTOOLFLAGS      = --tag=FC



moduledir               = @libdir@
module_DATA             = grvy.mod hpct.mod
CLEANFILES              = grvy.mod hpct.mod

METASOURCES = AUTO
