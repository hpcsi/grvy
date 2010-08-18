# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(grvy, 0.29.0, karl@ices.utexas.edu)
AC_CONFIG_HEADER(config_grvy.h)
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE(grvy,0.29.0)

# Release versioning

GENERIC_MAJOR_VERSION=0
GENERIC_MINOR_VERSION=29
GENERIC_MICRO_VERSION=0

GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION

AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_MAJOR_VERSION)
AC_SUBST(GENERIC_MINOR_VERSION)
AC_SUBST(GENERIC_MICRO_VERSION)

# Required for some macros

#AX_AM_MACROS

#------------------------------
# Checks for required programs
#------------------------------

AC_PROG_CC
AC_PROG_CXX
AC_PROG_FC
AC_PROG_F77
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_SED
AM_SANITY_CHECK

#----------------------
# Checks for libraries.
#----------------------

#---------------------------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
#---------------------------------------------------------------

AC_C_CONST
AC_HEADER_STDBOOL
AX_TLS

#----------------
# Check for boost
#----------------

AC_LANG([C++])
BOOST_REQUIRE([1.36])
BOOST_MATH
#BOOST_PROGRAM_OPTIONS
AC_CACHE_SAVE

#------------------------------
# Checks for library functions.
#------------------------------

AC_FUNC_MALLOC

# Query configuration environment

AX_SUMMARIZE_ENV

#----------------
# Doxygen support
#----------------

DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(ON)
DX_PS_FEATURE(OFF)

DX_INIT_DOXYGEN(GRVY, doxygen/grvy.dox, docs)

AC_OUTPUT( Makefile src/Makefile examples/Makefile test/Makefile test/hpct_api/Makefile grvy.pc \
	   grvy.spec doxygen/Makefile src/grvy.h )

# Final summary

AX_SUMMARIZE_CONFIG
