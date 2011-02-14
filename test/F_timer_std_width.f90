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
!! $Id: grvy.h.in 12360 2010-08-15 14:02:03Z karl $
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

program main
  use grvy

  integer :: Max_Iters = 1
  real*8  :: work1
  integer :: i
  integer :: ierr

  call grvy_log_setlevel(GRVY_INFO)

  call grvy_timer_init('GRVY - a really long string to test that the max width is correctly capped.');

  ! Primary Iteration Loop 

  do i = 1,Max_Iters

     ! Define the beginning of the overall portion to be monitored

     work1 = do_work1()
     call do_work2

  enddo

  call grvy_timer_finalize()
  call grvy_timer_summarize

  ! make sure we get an error if we try to set the width too small

  call grvy_log_setlevel(GRVY_NOLOG) ! silence expected error message
  call grvy_timer_set_summarize_width(72,ierr) 

  if(ierr .ne. 1)then
     call exit(1)
  endif

  call exit(0)
end program main

real*8 function do_work1()
  integer :: i
  real*8  :: temp

  call grvy_timer_begin('work1')

  temp = 0.0d0

  do i = 1,12345678
     temp = temp + 3.0d0*temp + 5.d0*i/(2.0d0*i)
  enddo

  do_work1 = temp

  call grvy_timer_end('work1')

  return
end function do_work1

subroutine do_work2()

  integer :: i
  real*8  :: temp

  call grvy_timer_begin('work2')

  do i = 1,7654321
     temp = temp*temp + (3.14*i)/sqrt(1.0d0*i*i)
  enddo

  call grvy_timer_end('work2')

  return
end subroutine do_work2
    

