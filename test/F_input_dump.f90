!!-----------------------------------------------------------------------bl-
!!--------------------------------------------------------------------------
!! 
!! libGRVY - a utility library for scientific computing.
!!
!! Copyright (C) 2008,2009,2010 The PECOS Development Team
!!
!! This library is free software; you can redistribute it and/or
!! modify it under the terms of the Version 2.1 GNU Lesser General
!! Public License as published by the Free Software Foundation.
!!
!! This library is distributed in the hope that it will be useful,
!! but WITHOUT ANY WARRANTY; without even the implied warranty of
!! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
!! Lesser General Public License for more details.
!!
!! You should have received a copy of the GNU Lesser General Public
!! License along with this library; if not, write to the Free Software
!! Foundation, Inc. 51 Franklin Street, Fifth Floor, 
!! Boston, MA  02110-1301  USA
!!
!!-----------------------------------------------------------------------el-
!!
!! Regression/Test Suite for libGRVY.
!!
!! $Id$
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

program main
  use grvy
  implicit none
  
  integer(4)     :: error, flag ! 1 for success, 0 for failure
  character*19   :: dir_template = "grvy-scratch-XXXXXX"
  integer(4)     :: status

  call grvy_log_setlevel(GRVY_WARN)

  error=1 ! start with no error condition

  call grvy_input_fopen("./input-example.txt",flag)
  error=error*flag



  ! Define registered variable which may not be present in input file 

  call grvy_input_register_int   ("sec1/aint",   1,          flag);
  error=error*flag
  call grvy_input_register_float ("sec1/afloat", 2.2,        flag);
  error=error*flag
  call grvy_input_register_double("sec1/adouble",3.3d3,      flag);
  error=error*flag
  call grvy_input_register_char  ("sec1/astring","plankton", flag);

  ! Dump the input contents to a temporary file 

!  call grvy_create_unique_dir(dir_template,flag);
  call grvy_create_scratch_dir(dir_template,flag);
  if(flag .ne. 0)then
     call grvy_log(GRVY_INFO,"Unable to create unique directory");
     call exit(1)
  endif

  call grvy_input_fdump_file("# ",dir_template//"/solution.txt",flag)
  error=error*flag

  status = system ("diff "//dir_template//"/solution.txt file2 2>&1 > /dev/null");
  if(status .ne. 0)then
     call exit(1)
  endif

  ! Verify stdout dump doesn't fail.
  
!   open(6,file=dir_template//"/stdout")

!   call grvy_input_fdump(flag)
!   error=error*flag
!   call grvy_input_fdump_delim("# ",flag)
!   error=error*flag

  ! ----- Close File // Error Handling -----  
  call grvy_input_fclose()
  
  if(error .eq. 0) then ! signal error
     call exit(1)
  endif

  call exit(0)

end program main

