include $(top_srcdir)/doxygen/aminclude.am

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4
SUBDIRS          = lib src examples test
DISTDIRS         = lib src examples

# Tools in the auxillary directory
AUX_DIST  = build-aux/install-sh
AUX_DIST += build-aux/missing
AUX_DIST += build-aux/link_warning.h

# Support for pkgconfig

pkgconfigdir     = $(libdir)/pkgconfig
pkgconfig_DATA   = hpct.pc

