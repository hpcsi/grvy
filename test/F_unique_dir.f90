!!-----------------------------------------------------------------------bl-
!!--------------------------------------------------------------------------
!! 
!! libGRVY - a utility library for scientific computing.
!!
!! Copyright (C) 2008-2013,2018-2021 The PECOS Development Team
!! Additional Copyright (C) 2018 individual authors
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
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

program F_unique_dir
  use grvy
  implicit none

  character*12 :: program_name = "F_unique_dir"

  integer :: flag
  integer :: retval = 0

  character*34 :: invalid_template = "TempDir_doesNotHaveSixTrailingExes"
  character*34 :: valid_template   = "TempDir_itHasSixTrailingExesXXXXXX"
  character*34 :: backup_template

  call grvy_log_setlevel(GRVY_NOLOG)

  backup_template = invalid_template
  call grvy_create_unique_dir(invalid_template, flag);
  if (flag.eq.0) then
    write (*,*) "Expected nonzero return for invalid template"
    retval = 1
  end if
  if (backup_template.ne.invalid_template) then
    write (*,*) "Expected template to not change on failed invocation"
    retval = 1
  end if

  backup_template = valid_template
  call grvy_create_unique_dir(valid_template, flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for valid template"
    retval = 1
  end if
  if (backup_template.eq.invalid_template) then
    write (*,*) "Expected template to change on successful invocation"
    retval = 1
  end if

  call exit(retval)
end program F_unique_dir
