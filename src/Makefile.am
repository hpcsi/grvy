

h_sources                 = grvy.h grvy_classes.h grvy_int.h fortran_string_order.h \
	                    grvy_env.h grvy_ptable.h grvy_mpi.h
cc_sources                = grvy_input.cpp grvy_input_interface.cpp grvy_classes.cpp \
	                    grvy_timer.cpp grvy_timer_interface.cpp                  \
		            grvy_utils.cpp grvy_math.cpp grvy_log.cpp                \
			    grvy_version.cpp grvy_env.cpp grvy_hdf5.cpp

cc_sources               += grvy_ramdisk.cpp grvy_ramdisk_interface.cpp


AM_CPPFLAGS               =  $(BOOST_CPPFLAGS) -DBOOST_ALL_DYN_LINK
BUILT_SOURCES             = .license.stamp

bin_PROGRAMS              = # Appended below
lib_LTLIBRARIES           = # Appended below

library_includedir        = $(includedir)
library_include_HEADERS   = grvy.h grvy_classes.h

#-----------------------
# Public C/C++ library
#-----------------------

lib_LTLIBRARIES          += libgrvy.la
libgrvy_la_LDFLAGS        = $(all_libraries) -release $(GENERIC_RELEASE) 

if LINK_BOOST_LIBS
  libgrvy_la_LDFLAGS     += $(BOOST_FILESYSTEM_LDFLAGS)  \
			    $(BOOST_FILESYSTEM_LIBS) $(BOOST_SYSTEM_LIBS) $(BOOST_LOG_LIBS) -lboost_log_setup $(BOOST_THREAD_LIBS)
endif


libgrvy_la_SOURCES        = $(getpot_sources) $(cc_sources) $(h_sources)

if HDF5_ENABLED	         
  AM_CPPFLAGS            += $(HDF5_CFLAGS)
  libgrvy_la_LDFLAGS     += $(HDF5_LIBS) -lhdf5_hl

  bin_PROGRAMS           += gdump 
  gdump_SOURCES           = gdump.cpp
  gdump_LDADD             = -L$(top_builddir)/src -lgrvy
  gdump_DEPENDENCIES      = libgrvy.la

  bin_PROGRAMS           += gadd
  gadd_SOURCES            = gadd.cpp
  gadd_LDADD              = -L$(top_builddir)/src -lgrvy
  gadd_DEPENDENCIES       = libgrvy.la

endif

#-----------------------
# Public Fortran library
#-----------------------

lib_LTLIBRARIES          += libgrvyf.la
libgrvyf_la_LIBTOOLFLAGS  = --tag=FC
libgrvyf_la_SOURCES       = grvy.f90
libgrvyf_la_LIBADD        = libgrvy.la

#-----------------------
# Additional Binaries
#-----------------------

bin_PROGRAMS             += grvy_version
grvy_version_SOURCES      = version.cpp grvy.h
grvy_version_LDADD        = -L$(top_builddir)/src -lgrvy
grvy_version_DEPENDENCIES = libgrvy.la

bin_PROGRAMS             += hostinfo
hostinfo_SOURCES          = hostinfo.cpp
hostinfo_LDADD            = -L$(top_builddir)/src -lgrvy
hostinfo_DEPENDENCIES     = libgrvy.la



#---------------------------------
# Embedded license header support
#---------------------------------

.license.stamp: $(top_srcdir)/LICENSE
	$(top_srcdir)/src/common/lic_utils/update_license.pl -S=$(top_srcdir)/src \
		$(top_srcdir)/LICENSE $(cc_sources) $(h_sources)                  \
		$(version_SOURCES) $(hostinfo_SOURCES) $(gdump_SOURCES)
	$(top_srcdir)/src/common/lic_utils/update_license.pl -S=$(top_srcdir)/src \
		--c2f_comment $(top_srcdir)/LICENSE grvy.f90
	echo 'updated source license headers' >$@

#-------------------------
# Install Fortran modules
#-------------------------

install-data-local: libgrvyf.la
	mkdir -p $(DESTDIR)$(libdir)
	$(INSTALL_DATA) $(top_builddir)/src/grvy.mod $(DESTDIR)$(libdir)/grvy.mod

uninstall-local: 
	rm -rf $(DESTDIR)$(libdir)/grvy.mod

dist-hook:
	rm -rf `find $(distdir)/ -name .svn`

EXTRA_DIST              = common/lic_utils common/lcov .license.stamp
CLEANFILES              = grvy.mod .license.stamp *.gcno *.gcda

METASOURCES = AUTO
