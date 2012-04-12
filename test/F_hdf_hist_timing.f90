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

  real*8 :: do_work1
  integer :: Max_Iters = 3
  real*8  :: work1
  integer :: i
  integer        :: status
  
  call grvy_log_setlevel(GRVY_INFO)

  call grvy_timer_init('Fortran is the best!');

  ! Primary Iteration Loop 

  do i = 1,Max_Iters

     ! Define the beginning of the overall portion to be monitored

     work1 = do_work1()
     call do_work2

  enddo

  call grvy_timer_finalize()

  call grvy_timer_save_hist    ("Example1","comment",1,".test.h5");
  call grvy_timer_save_hist_exp("Example2","comment",1,-1,"r2345",350.0d0,".test.h5");

  status = system ("rm .test.h5 2>&1 > /dev/null")

  if(status .ne. 0)then
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
    

