!!-----------------------------------------------------------------------bl-
!!--------------------------------------------------------------------------
!! 
!! libGRVY - a utility library for scientific computing.
!!
!! Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
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
!! input.c: Fortran example of basic input file parsing via libGRVY.
!!
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

program main
  use grvy
  implicit none

  integer              :: flag
  integer              :: error
  real                 :: reyn
  real*8               :: magicval         
  integer              :: iter_max
  integer              :: switches(4)
  integer, allocatable :: switches2(:)
  character            :: filename*100
  character            :: key1*100
  character            :: key2*100
  logical              :: restart

  ! Initialize/read the file

  call grvy_input_fopen("./pecos-input.txt     ",flag);

  ! Dump the whole file to stdout

  print*,' ------ Full Dump ------'
  call grvy_input_fdump(flag);
  print*,' ---- End Full Dump ----'

  print*,' ------ Full Dump (delimited) ------'
  call grvy_input_fdump_delim('# ',flag);

  ! Read specific variables

  call grvy_input_fread_real ("reyn",reyn,flag)
  call grvy_input_fread_int  ("iter_max",iter_max,flag)
  call grvy_input_fread_char ("gridfile",filename,flag)

  print*,'reyn                = ',reyn
  print*,'iter_max            = ',iter_max
  print*,'gridfile            = ',trim(filename)

  call grvy_input_fread_int_vec ("solver/switches",switches,4,flag)
  print*,'aa:switches            = ',switches(1),switches(2),switches(3),switches(4)

  allocate(switches2(4))
  call grvy_input_fread_int_vec ("solver/switches",switches2,4,flag)
  print*,'bb:switches2           = ',switches2(1),switches2(2),switches2(3),switches2(4)

  call grvy_input_fread_char_ivec("solver/keywords",key1,1,flag)
  call grvy_input_fread_char_ivec("solver/keywords",key2,2,flag)

  print*,'solver/keyword 1    = ',trim(key1)
  print*,'solver/keyword 2    = ',trim(key2)

  ! Read a logical (you need to provide a default value which is used if the
  ! desired keyword is not available in the input file)

  call grvy_input_fread_logical  ("restart",restart,.false.,flag)

  print*,'restart = ',restart

  ! Attempt to read undefined variable (should get error message)

  call grvy_input_fread_int("koomie",iter_max,flag)

  ! Register a default value and read variable which is not in input
  ! file (allows for backwards compatability of input files)

  call grvy_input_register_double  ("solver/magicval",0.03d0,flag)
  error = error*flag

  call grvy_input_fread_double ("solver/magicval",magicval,flag)
  print*,'solver/magicval     = ',magicval
  
  ! Disable error messages if you want to control them yourself
  ! If you query the unknown variable again it should be quiet

  call grvy_log_setlevel(GRVY_NOLOG)
  call grvy_input_fread_int("koomie",iter_max,flag)


stop
end program main
