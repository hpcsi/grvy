include $(top_srcdir)/doxygen/aminclude.am

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS  = -I m4
SUBDIRS          = src examples test doxygen
DISTDIRS         = src examples
EXTRA_DIST       = CHANGES AUTHORS dist_version

# Tools in the auxiliary directory

AUX_DIST         = build-aux/install-sh
AUX_DIST        += build-aux/missing

# Support for pkgconfig

pkgconfigdir     = $(libdir)/pkgconfig
pkgconfig_DATA   = grvy.pc

# Revision control support for external distribution

if SVN_CHECKOUT
dist_version: FORCE
	@SVN_REVISION@ > $(top_srcdir)/dist_version

FORCE:
endif

