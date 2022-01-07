!!-----------------------------------------------------------------------bl-
!!--------------------------------------------------------------------------
!! 
!! libGRVY - a utility library for scientific computing.
!!
!! Copyright (C) 2008-2013,2018-2022 The PECOS Development Team
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

program main
  use grvy

  implicit none

  integer :: Max_Iters = 1
  real*8  :: work
  real*8  :: do_work1, do_work2
  integer :: i
  integer ierr

  call grvy_log_setlevel(GRVY_INFO)

  call grvy_timer_init('GRVY - a really long string to test that the max width is correctly capped.');

  ! Primary Iteration Loop 

  do i = 1,Max_Iters

     ! Define the beginning of the overall portion to be monitored

     work = do_work1()
     work = do_work2()

  enddo

  call grvy_timer_finalize()
  call grvy_timer_set_summarize_width(140,ierr)

  if(ierr .ne. 0)then
     call exit(1)
  endif

  call grvy_timer_summarize


  call exit(0)
end program main

real*8 function do_work1()
  use grvy
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

real*8 function do_work2()
  use grvy
  integer               :: i
  real*8                :: temp

  temp = 0.0d0

  call grvy_timer_begin('work2')

  do i = 1,7654321
     temp = temp*temp + (3.14*i)/sqrt(1.0d0*i*i)
  enddo

  do_work2 = temp

  call grvy_timer_end('work2')

  return
end function do_work2
    

