h_sources               = grvy.h grvy_classes.h grvy_int.h fortran_string_order.h
cc_sources              = grvy_input.cpp grvy_classes.cpp grvy_timer.cpp \
		          grvy_utils.cpp grvy_math.cpp grvy_log.cpp

getpot_sources          = common/getpot/getpot.h

INCLUDES                = 
AM_FCFLAGS              = $(INCLUDES)
AM_CFLAGS               = $(INCLUDES)
AM_CPPFLAGS             = $(INCLUDES) $(BOOST_CPPFLAGS)

lib_LTLIBRARIES         = libgrvy.la
library_includedir      = $(includedir)
library_include_HEADERS = grvy.h
libgrvy_la_LDFLAGS      = $(all_libraries) -release $(GENERIC_RELEASE)
libgrvy_la_SOURCES      = $(getpot_sources) $(cc_sources) $(h_sources)

#--------------------------
# Fortran Module Interface
#--------------------------

noinst_PROGRAMS         = test
test_SOURCES            = grvy.f90 main.f90
test_LIBTOOLFLAGS       = --tag=FC
moduledir               = @libdir@
module_DATA             = grvy.mod
CLEANFILES              = grvy.mod

METASOURCES = AUTO
