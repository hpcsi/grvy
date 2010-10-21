h_sources               = grvy.h grvy_classes.h grvy_int.h fortran_string_order.h grvy_env.h
cc_sources              = grvy_input.cpp grvy_classes.cpp grvy_timer.cpp grvy_timer_interface.cpp \
		          grvy_utils.cpp grvy_math.cpp grvy_log.cpp \
			  grvy_version.cpp grvy_env.cpp grvy_hdf5.cpp

getpot_sources          = common/getpot/getpot.h

INCLUDES                = -I$(top_builddir)/src/common/getpot
AM_FCFLAGS              = $(INCLUDES)
AM_CFLAGS               = $(INCLUDES)
AM_CPPFLAGS             = $(INCLUDES) $(BOOST_CPPFLAGS)
bin_PROGRAMS            = # Append below
lib_LTLIBRARIES         = # Append below

BUILT_SOURCES = .license.stamp


LIBS                    = $(BOOST_REGEX_LDFLAGS) $(BOOST_REGEX_LIBS)

library_includedir      = $(includedir)
library_include_HEADERS = grvy.h grvy_classes.h

# Public C library
lib_LTLIBRARIES    += libgrvy.la
libgrvy_la_LDFLAGS  = $(all_libraries) -release $(GENERIC_RELEASE)
libgrvy_la_SOURCES  = $(getpot_sources) $(cc_sources) $(h_sources)

if HDF5_ENABLED
  AM_CPPFLAGS          += $(HDF5_CFLAGS)
  libgrvy_la_LDFLAGS   += $(HDF5_LIBS) -lhdf5_hl
endif

# Public Fortran library is a superset of libgrvy.la
lib_LTLIBRARIES          += libgrvyf.la
libgrvyf_la_LIBTOOLFLAGS  = --tag=FC
libgrvyf_la_SOURCES       = grvy.f90
libgrvyf_la_LIBADD        = libgrvy.la

# Compiling grvy.f90 for libgrvyf.la produces grvy.mod
moduledir               = @libdir@
module_DATA             = grvy.mod

standalonedir           = @prefix@/bin
standalone_PROGRAMS     = version

bin_PROGRAMS           += version
version_SOURCES         = version.cpp grvy.h libgrvy.la
version_LDADD           = -L$(top_builddir)/src -lgrvy
version_DEPENDENCIES    = libgrvy.la

bin_PROGRAMS           += hostinfo
hostinfo_SOURCES        = hostinfo.cpp
hostinfo_LDADD          = -L$(top_builddir)/src -lgrvy
hostinfo_DEPENDENCIES   = libgrvy.la

#---------------------------------
# Embedded license header support
#---------------------------------

.license.stamp: $(top_srcdir)/LICENSE
	$(top_srcdir)/src/common/lic_utils/update_license.pl $(top_srcdir)/LICENSE $(cc_sources) $(h_sources)
	echo 'updated source license headers' >$@



EXTRA_DIST              = common/lic_utils common/lcov

dist-hook:
	rm -rf `find $(distdir)/ -name .svn`


CLEANFILES              = grvy.mod .license.stamp *.gcno *.gcda

METASOURCES = AUTO
