# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(grvy, 0.31.0, karl@ices.utexas.edu)
AC_CONFIG_HEADER(config_grvy.h)
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE

# Release versioning

AX_SPLIT_VERSION
GENERIC_MAJOR_VERSION=$AX_MAJOR_VERSION
GENERIC_MINOR_VERSION=$AX_MINOR_VERSION
GENERIC_MICRO_VERSION=$AX_POINT_VERSION

GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION

AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_MAJOR_VERSION)
AC_SUBST(GENERIC_MINOR_VERSION)
AC_SUBST(GENERIC_MICRO_VERSION)

# Required for some macros

#------------------------------
# Checks for required programs
#------------------------------

AC_PROG_CC
AC_PROG_CXX
AC_PROG_FC
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_SED
AM_SANITY_CHECK

# ---------------------
# Enable MPI utilities?
# ---------------------

AC_ARG_ENABLE(mpi, AC_HELP_STRING([--enable-mpi],[enable MPI associated utilities]))

HAVE_MPI=0

if test "x$enable_mpi" = "xyes"; then
   AC_LANG([C])
   ACX_MPI([CC="$MPICC"], AC_MSG_ERROR([Could not find MPI C compiler support]))
   AC_LANG([Fortran])
   ACX_MPI([FC="$MPIFC"], AC_MSG_ERROR([Could not find MPI F90 compiler support]))
   AC_LANG([C++])
   ACX_MPI([HAVE_MPI=1 CXX="$MPICXX" AC_DEFINE(HAVE_MPI,1,[Define if MPI is available])], 
           AC_MSG_ERROR([Could not find MPI C++ compiler support]))
fi

AM_CONDITIONAL(MPI_ENABLED,test x$HAVE_MPI = x1)

# Conditionals

dnl AM_CONDITIONAL(CODE_COVERAGE_ENABLED,FALSE)
dnl AM_CONDITIONAL(SVN_CHECKOUT,FALSE)
dnl AM_CONDITIONAL(DX_COND_doc,FALSE)
dnl AM_CONDITIONAL(DX_COND_html,lFALSE)
dnl AM_CONDITIONAL(DX_COND_chm,FALSE)
dnl AM_CONDITIONAL(DX_COND_chi,FALSE)
dnl AM_CONDITIONAL(DX_COND_man,FALSE)
dnl AM_CONDITIONAL(DX_COND_rtf,FALSE)
dnl AM_CONDITIONAL(DX_COND_xml,FALSE)
dnl AM_CONDITIONAL(DX_COND_ps,FALSE) 
dnl AM_CONDITIONAL(DX_COND_pdf,FALSE) 
dnl AM_CONDITIONAL(DX_COND_latex,FALSE)

#----------------------
# Checks for libraries.
#----------------------

AX_PATH_HDF5_NEW(1.8.0,no)

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
BOOST_REGEX
BOOST_PROGRAM_OPTIONS
#BOOST_ASIO
AC_CACHE_SAVE



#--------------------------
# Checks for code coverage
#-------------------------

AX_CODE_COVERAGE

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

AC_OUTPUT( Makefile src/Makefile examples/Makefile test/Makefile grvy.pc \
	   grvy.spec doxygen/Makefile src/grvy.h test/F_version.F90      \
	   doxygen/txt_common/about_vpath.page )

# Final summary

AX_SUMMARIZE_CONFIG
