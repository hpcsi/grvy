# SYNOPSIS
#
#   Summarizes configuration settings.
#
#   AX_SUMMARIZE_CONFIG([, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
#
# DESCRIPTION
#
#   Outputs a summary of relevant configuration settings.
#
# LAST MODIFICATION
#
#   2009-07-16
#

AC_DEFUN([AX_SUMMARIZE_CONFIG],
[

echo
echo '----------------------------------- SUMMARY -----------------------------------'
echo
echo Package version............... : $PACKAGE-$VERSION
echo
echo C++ compiler.................. : $CXX
echo C++ compiler flags............ : $CXXFLAGS
echo Fortran compiler ............. : $FC
echo Fortran compiler flags........ : $FCFLAGS
echo Install dir................... : $prefix 
echo Boost dir..................... : $BOOST_ROOT
echo Boost regex lib............... : $BOOST_REGEX_LDFLAGS $BOOST_REGEX_LIBS
echo Build user.................... : $USER
echo Build host.................... : $BUILD_HOST
echo Configure date................ : $BUILD_DATE
echo Build architecture............ : $BUILD_ARCH
echo SVN revision number........... : $BUILD_VERSION
echo
echo Optional Features:
   if test "$HAVE_HDF5" = "0"; then
     echo '   'Link with HDF5............. : no
   else
     echo '   'Link with HDF5............. : yes
   fi
   if test "$HAVE_MPI" = "0"; then
     echo '   'Link with MPI.............. : no
   else
     echo '   'Link with MPI.............. : yes
   fi
   if test "$HAVE_GCOV_TOOLS" = "0"; then
     echo '   'Enable gcov code coverage.. : no
   else
     echo '   'Enable gcov code coverage.. : yes
   fi
echo
echo '-------------------------------------------------------------------------------'

echo
echo Configure complete, now type \'make\' and then \'make install\'. 
echo 
echo To verify functionality of your local build, type \'make check\' to run a suite 
echo of available regression tests.
echo

])
