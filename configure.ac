#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(hpct, 0.25, karl@ices.utexas.edu)
AC_CONFIG_HEADER(config.h)
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE(hpct,0.25)

######AC_PROG_FC

# Release versioning

GENERIC_MAJOR_VERSION=0
GENERIC_MINOR_VERSION=2
GENERIC_MICRO_VERSION=5

#GENERIC_API_VERSION=1.0
#AC_SUBST(GENERIC_API_VERSION)

#shared library versioning

#AC_SUBST(GENERIC_LIBRARY_VERSION)

#PACKAGE=$GENERIC_LIBRARY_NAME
#AC_SUBST(GENERIC_LIBRARY_NAME)
#GENERIC_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION.$GENERIC_MICRO_VERSION

GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_RELEASE)

# Required for some macros
#AX_AM_MACROS

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_FC
AC_PROG_F77
AC_PROG_LIBTOOL
AM_SANITY_CHECK
#AC_LANG_CPLUSPLUS

# Checks for libraries.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_STDBOOL

# Check for boost

AC_LANG([C++])
AX_BOOST_BASE(1.35)
AX_BOOST_PROGRAM_OPTIONS
AC_CACHE_SAVE

# Checks for library functions.
AC_FUNC_MALLOC

AC_OUTPUT(Makefile src/Makefile examples/Makefile test/Makefile hpct.pc)
