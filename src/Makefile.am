h_sources               = grvy.h grvy_classes.h grvy_int.h fortran_string_order.h hpct.h grvy_env.h
cc_sources              = grvy_input.cpp grvy_classes.cpp grvy_timer.cpp \
		          grvy_utils.cpp grvy_math.cpp grvy_log.cpp grvy_hpct_compat.cpp \
			  grvy_version.cpp grvy_env.cpp grvy_hdf5.cpp

getpot_sources          = common/getpot/getpot.h

INCLUDES                = -I$(top_builddir)/src/common/getpot
AM_FCFLAGS              = $(INCLUDES)
AM_CFLAGS               = $(INCLUDES)
AM_CPPFLAGS             = $(INCLUDES) $(BOOST_CPPFLAGS)

BUILT_SOURCES = .license.stamp

# Note: the use of hpct_ api syntax and linkage to -lhpct is still
# supported in libGRVY but will be deprecated in the future. 

lib_LTLIBRARIES         = libgrvy.la libhpct.la
LIBS                    = $(BOOST_REGEX_LDFLAGS) $(BOOST_REGEX_LIBS) 



library_includedir      = $(includedir)
library_include_HEADERS = grvy.h hpct.h grvy_classes.h 
libgrvy_la_LDFLAGS      = $(all_libraries) -release $(GENERIC_RELEASE) 
libgrvy_la_SOURCES      = $(getpot_sources) $(cc_sources) $(h_sources)

if HDF5_ENABLED
  AM_CPPFLAGS          += $(HDF5_CFLAGS)
  libgrvy_la_LDFLAGS   += $(HDF5_LIBS) -lhdf5_hl
endif

libhpct_la_LDFLAGS      = $(all_libraries) -release $(GENERIC_RELEASE)
libhpct_la_SOURCES      = $(getpot_sources) $(cc_sources) $(h_sources)

bin_PROGRAMS            = version hostinfo # foo
version_SOURCES         = version.cpp grvy.h libgrvy.la
version_LDADD           = -L$(top_builddir)/src -lgrvy
version_DEPENDENCIES    = libgrvy.la

hostinfo_SOURCES        = hostinfo.cpp
hostinfo_LDADD          = -L$(top_builddir)/src -lgrvy
hostinfo_DEPENDENCIES   = libgrvy.la

#--------------------------
# Fortran Module Interface
#--------------------------

noinst_PROGRAMS         = test test2
standalonedir           = @prefix@/bin
standalone_PROGRAMS     = version

test_SOURCES            = grvy.f90 main.f90
test_LIBTOOLFLAGS       = --tag=FC
test_DEPENDENCIES       = grvy.o

test2_SOURCES           = grvy_hpct.f90 main_hpct.f90
test2_LIBTOOLFLAGS      = --tag=FC
test2_DEPENDENCIES      = grvy_hpct.o

main.f90: grvy.f90
main_hpct.f90: grvy_hpct.f90

#---------------------------------
# Embedded license header support
#---------------------------------

.license.stamp: $(top_srcdir)/LICENSE
	$(top_srcdir)/src/common/lic_utils/update_license.pl $(top_srcdir)/LICENSE $(cc_sources) $(h_sources)
	echo 'updated source license headers' >$@


moduledir               = @libdir@
module_DATA             = grvy.mod hpct.mod

EXTRA_DIST              = common/lic_utils common/lcov

dist-hook:
	rm -rf `find $(distdir)/ -name .svn`


CLEANFILES              = grvy.mod hpct.mod .license.stamp *.gcno

METASOURCES = AUTO
