!!-----------------------------------------------------------------------bl-
!!--------------------------------------------------------------------------
!! 
!! libGRVY - a utility library for scientific computing.
!!
!! Copyright (C) 2008,2009,2010,2011 The PECOS Development Team
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

program F_check_copy_dir
  use grvy
  implicit none

!  character*14 :: template1  = "tmpdirf_XXXXXX"
!  character*14 :: template2  = "tmpdirf_XXXXXX"

  integer :: flag = 1
  integer :: retval

  character*34 :: valid_template   = "TempDir_itHasSixTrailingExesXXXXXX"

!!!  call grvy_log_setlevel(GRVY_NOLOG)

  print*,'flag = ',flag

  call grvy_create_unique_dir(valid_template, retval)
  print*,'retval = ',retval
  flag = flag * retval

  print*,'flag = ',flag

!  call grvy_create_unique_dir(template1,  retval)
!  flag = flag * retval

!  print*,'retval = ',retval
!  print*,'flag = ',flag

!  call grvy_create_unique_dir(template2, retval)
!  flag = flag * retval

  ! Failure should occur if we attempt to copy a file 

!   call grvy_copy_dir("F_check_file_path",template,retval)
!   if(retval .ne. 1)then
!      flag = 0
!   endif

!   ! Failure should occur if input/destination dirs are identical 

!   call grvy_copy_dir(template,template,retval)
!   if(retval .ne. 1)then
!      flag = 0
!   endif

  if(flag .eq. 0)then
     retval = 1
  else
     retval = 0
  endif

  call exit(retval)
end program F_check_copy_dir
