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
echo C compiler.................... : $CC
echo C compiler flags.............. : $CFLAGS
echo Fortran compiler ............. : $FC
echo Fortran compiler flags........ : $FCFLAGS
echo Install dir................... : $prefix 
echo Boost C++ flags............... : $BOOST_CPPFLAGS
echo Build user.................... : $USER
echo Build host.................... : $BUILD_HOST
echo Configure date................ : $BUILD_DATE
echo Build architecture............ : $BUILD_ARCH
echo SVN revision number........... : $BUILD_VERSION
echo
if test "$BOOST_HEADER_ONLY" = "1"; then
     echo Boost Configuration - Header only build requested
     echo '   grvy_copy_dir()'............ : disabled
else
     echo Boost Configuration - Library linkage included for the following:
     echo '   'Boost LDFLAGS.............. : $BOOST_FILESYSTEM_LDFLAGS
#     echo '   'Program options lib........ : $BOOST_PROGRAM_OPTIONS_LIBS
     echo '   'Filesystem lib............. : $BOOST_FILESYSTEM_LIBS
     echo '   'System lib................. : $BOOST_SYSTEM_LIBS
fi
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
