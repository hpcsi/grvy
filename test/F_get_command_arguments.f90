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

program F_get_command_arguments
  use grvy
  implicit none

  character*17 :: program_name = "F_get_command_arguments"

  integer :: length, flag
  integer :: retval = 0
  character(len=99) :: expected
  character(len=20) :: buf20
  character(len=10) :: buf10
  character(len=5)  :: buf5

! Wrapper F_get_command_arguments.sh provides us with 'a b c d e f'
! TODO Check length vs expected
  buf20 = ''
  expected = 'a b c d e f'
  call grvy_get_command_arguments(buf20)
  if (trim(expected).ne.trim(buf20)) then
    write (*,*) 'Did not receive expected arguments!'
    call exit(1) ! All bets are off for remaining tests.
  end if

  flag  = -1
  buf20 = ''
  call grvy_get_command_arguments(buf20, status=flag)
  if (flag.ne.0) then
    retval = 1; write (*,*) 'Expected success on trivial retrieval'
  end if

! Prefix tests
! TODO Check length vs expected
! TODO Check flag vs expected

  buf20 = ''
  expected = 'pre a b c d e f'
  call grvy_get_command_arguments(buf20,'pre')
  if (trim(expected).ne.trim(buf20)) then
    retval = 1; write (*,*) 'Incorrect prefix observed'
  end if

  buf20 = ''
  expected = 'a b c d e f'
  call grvy_get_command_arguments(buf20,'')
  if (trim(expected).ne.trim(buf20)) then
    retval = 1; write (*,*) 'Incorrect trivial prefix observed'
  end if

! Suffix tests
! TODO Check length vs expected
! TODO Check flag vs expected

  buf20 = ''
  expected = 'a b c d e f post'
  call grvy_get_command_arguments(buf20,'', 'post')
  if (trim(expected).ne.trim(buf20)) then
    retval = 1; write (*,*) 'Incorrect suffix observed'
  end if

  buf20 = ''
  expected = 'a b c d e f'
  call grvy_get_command_arguments(buf20,'', '')
  if (trim(expected).ne.trim(buf20)) then
    retval = 1; write (*,*) 'Incorrect trivial suffix observed'
  end if

! First tests
! TODO Check length vs expected
! TODO Check flag vs expected

  buf20 = ''
  expected = 'a b c d e f'
  call grvy_get_command_arguments(buf20, '', '', 1)
  if (trim(expected).ne.trim(buf20)) then
    retval = 1; write (*,*) 'Incorrect trivial first observed'
  end if

  buf20 = ''
  expected = 'b c d e f'
  call grvy_get_command_arguments(buf20, '', '', 2)
  if (trim(expected).ne.trim(buf20)) then
    retval = 1; write (*,*) 'Incorrect trivial first=2 observed'
  end if

  buf20 = ''
  expected = 'c d e f'
  call grvy_get_command_arguments(buf20, '', '', 3)
  if (trim(expected).ne.trim(buf20)) then
    retval = 1; write (*,*) 'Incorrect trivial first=3 observed'
  end if

! Last tests
! TODO Check length vs expected
! TODO Check flag vs expected

  buf20 = ''
  expected = 'a'
  call grvy_get_command_arguments(buf20, '', '', 1, 1)
  if (trim(expected).ne.trim(buf20)) then
    retval = 1; write (*,*) 'Incorrect last=1 observed'
  end if

  buf20 = ''
  expected = 'a b'
  call grvy_get_command_arguments(buf20, '', '', 1, 2)
  if (trim(expected).ne.trim(buf20)) then
    retval = 1; write (*,*) 'Incorrect last=2 observed'
  end if

  buf20 = ''
  expected = 'b c'
  call grvy_get_command_arguments(buf20, '', '', 2, 3)
  if (trim(expected).ne.trim(buf20)) then
    retval = 1; write (*,*) 'Incorrect first=2,last=3 observed'
  end if

  buf20 = ''
  expected = ''
  call grvy_get_command_arguments(buf20, '', '', 3, 2)
  if (trim(expected).ne.trim(buf20)) then
    retval = 1; write (*,*) 'Incorrect trivial last observed'
  end if

! TODO Check insufficient length (e.g. buf5, buf10) causes flag
! TODO Check insufficient length returns correct length=length

  call exit(retval)
end program F_get_command_arguments
