include $(top_srcdir)/doxygen/aminclude.am

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS  = -I m4
SUBDIRS          = src examples test doxygen
DISTDIRS         = src examples

# Tools in the auxillary directory

AUX_DIST         = build-aux/install-sh
AUX_DIST        += build-aux/missing

# Support for pkgconfig

pkgconfigdir     = $(libdir)/pkgconfig
pkgconfig_DATA   = grvy.pc
