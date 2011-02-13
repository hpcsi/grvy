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

program F_check_file_path
  use grvy
  implicit none;

  character*17 :: program_name = "F_check_file_path"

  integer :: flag
  integer :: retval = 0

  call grvy_log_setlevel(GRVY_NOLOG)

  call grvy_check_file_path("F_check_file_path/foo", flag)
  if (flag.ne.(-1)) then
    write (*,*) "Expected nonzero return on attempt to clobber this binary"
    retval = 1
  end if

  call grvy_check_file_path("////", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for universally existent file path"
    retval = 1
  end if
  call grvy_check_file_path(".", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for universally existent file path"
    retval = 1
  end if
  call grvy_check_file_path("..", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for universally existent file path"
    retval = 1
  end if

  call grvy_check_file_path("./CheckFilePathString", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for paths relative to current directory"
    retval = 1
  end if
  call grvy_check_file_path("../CheckFilePathString", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for paths relative to current directory"
    retval = 1
  end if

  call grvy_check_file_path("CheckFilePathDir/foo", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for a single directory"
    retval = 1
  end if

  call grvy_check_file_path("CheckFilePathDir/A/B/C/foo", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for a multiple directories"
    retval = 1
  end if

  call exit(retval)
end program F_check_file_path
