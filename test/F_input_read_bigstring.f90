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
  
  integer        :: flag
  character      :: key1*2
  character      :: gridfile*10

  call grvy_log_setlevel(GRVY_NOLOG);

  call grvy_input_fopen("./input-example.txt",flag)

  if(flag .eq. 0)then
     call exit(1)
  endif

  ! Verify that we get an error if user does not provide enough
  ! storage space for a parsed string

  call grvy_input_fread_char ("gridfile",gridfile,flag)

  if(flag .ne. 0)then
     call exit(1)
  endif

  call grvy_input_fread_char_ivec("solver/keywords",key1,1,flag)

  if(flag .ne. 0)then
     call exit(1)
  endif

  call exit(0)

end program main

