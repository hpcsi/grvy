# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.65)
AC_INIT(grvy, 0.32.0, karl@ices.utexas.edu)
AC_CONFIG_MACRO_DIR([m4])
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

#---------------------------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
#---------------------------------------------------------------

AC_C_CONST
AC_HEADER_STDBOOL
AX_TLS

#-------------------------------------------------------------------
# Check for boost - allow for a header-only build which limits some
# functionality, but avoids pain for folks who don't want to suffer
# through a full Boost build.
#-------------------------------------------------------------------

BOOST_HEADER_ONLY=0
AC_ARG_ENABLE(boost-headers-only, AC_HELP_STRING([--enable-boost-headers-only],
				  [build GRVY with Boost header-only install (disables some functionality)]),
				  [boost_header_only=yes],[boost_header_only=no])
				  
AC_LANG([C++])

if test "x$boost_header_only" != "xyes";then
   BOOST_REQUIRE([1.47])
   BOOST_MATH
   #BOOST_REGEX
   #BOOST_PROGRAM_OPTIONS
   #BOOST_FILESYSTEM('')
   BOOST_FILESYSTEM
   BOOST_SYSTEM
   #BOOST_ASIO
else
   BOOST_REQUIRE([1.46])
   BOOST_HEADER_ONLY=1
   AC_DEFINE(BOOST_HEADER_ONLY,1,[Define if using a header-only build for Boost])
fi

AC_SUBST(BOOST_HEADER_ONLY)
AM_CONDITIONAL(LINK_BOOST_LIBS,test x$BOOST_HEADER_ONLY = x0)
AC_CACHE_SAVE

#----------------------
# Checks for libraries.
#----------------------

AX_PATH_HDF5_NEW(1.8.0,no)

#----------------------------------
# Enable quadrature rule generation
#----------------------------------
AC_ARG_ENABLE(gquad, AC_HELP_STRING([--enable-gquad],[enable Gaussian quadrature rule generation]))

# If the user wants the quadrature capability, then GSL is required
# Note that boost is also required (shared_ptr), but we already check for that.
if test "x$enable_gquad" = "xyes"; then
   # We really need 1.15 for a bugfix in one of the special functions used
   AX_PATH_GSL(1.15)
   AC_DEFINE(USE_GQUAD,1,[Define if users wants gquad capbility.])
   AC_SUBST(USE_GUAD)
fi

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
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)

DX_INIT_DOXYGEN(GRVY, doxygen/grvy.dox, docs)

AC_OUTPUT( Makefile src/Makefile examples/Makefile test/Makefile grvy.pc \
	   grvy.spec doxygen/Makefile src/grvy.h test/F_version.F90      \
	   doxygen/txt_common/about_vpath.page )

# Final summary

AX_SUMMARIZE_CONFIG
