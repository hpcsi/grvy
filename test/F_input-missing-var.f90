!!-----------------------------------------------------------------------bl-
!!--------------------------------------------------------------------------
!! 
!! libGRVY - a utility library for scientific computing.
!!
!! Copyright (C) 2008,2009,2010,2011,2012 The PECOS Development Team
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

  integer           :: ret_value = 0 ! 0 for success, 1 for failure
  real              :: reyn
  character(len=60) :: filename

  ! Silence warning/error/info message 
  call grvy_log_setlevel(GRVY_NOLOG)
  
  !filename = build_example_file_path()

  ! Initialize/read the file
  !call grvy_input_fopen(filename,ret_value)
  
  ! Read specific variables and echo locally
  !grvy_input_fread_float("strangeways", reyn,ret_value)

  ! Close the file
  !call grvy_input_fclose()

  call exit(ret_value)
end program main
