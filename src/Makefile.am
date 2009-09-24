h_sources               = hpct.h hpct_classes.h hpct_int.h
cc_sources              = hpct_input.cpp hpct_classes.cpp hpct_timer.cpp \
			  hpct_utils.cpp hpct_messages.cpp
getpot_sources          = GetPot

AM_CPPFLAGS             = $(BOOST_CPPFLAGS)
lib_LTLIBRARIES         = libhpct.la
library_includedir      = $(includedir)
library_include_HEADERS = hpct.h
libhpct_la_LDFLAGS      = $(all_libraries) -release $(GENERIC_RELEASE)
libhpct_la_SOURCES      = $(getpot_sources) $(cc_sources) $(h_sources)

#--------------------------
# Fortran Module Interface
#--------------------------

noinst_PROGRAMS         = test
test_SOURCES            = hpct.f90 main.f90
test_LIBTOOLFLAGS       = --tag=FC
moduledir               = @libdir@
module_DATA             = hpct.mod
CLEANFILES              = hpct.mod

METASOURCES = AUTO
