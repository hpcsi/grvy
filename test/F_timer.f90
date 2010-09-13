!!--------------------------------------------------------------------------
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
!!--------------------------------------------------------------------------
!!
!! Regression/Test Suite for libGRVY.
!!
!! $Id: grvy.h.in 12360 2010-08-15 14:02:03Z karl $
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

program main
  use grvy

  integer :: Max_Iters = 3
  real*8  :: grvy_total_timing
  real*8  :: dtime_global
!  real*8  :: Tolerance = 1.0d-1 ! resolution of dtime is terrible, be very generous here.
  real*8  :: Tolerance = 1.0d-3 ! resolution of dtime is terrible, be very generous here.
  real    :: timearray(2)
  real*8  :: work1
  real*8  :: raw_timer
  real*8  :: raw_grvy_timer1,raw_grvy_timer2
  integer :: call_count
  real*8  :: call_mean, call_variance
  real*8  :: diff
  character :: timestring*50 = ''
  integer :: i

  call grvy_log_setlevel(GRVY_INFO)

  dtime_global = dtime(timearray)
  
  call grvy_timer_init('Fortran is the best!');
  call grvy_timer(raw_grvy_timer1)

  ! Primary Iteration Loop 

  do i = 1,Max_Iters

     ! Define the beginning of the overall portion to be monitored

     work1 = do_work1()
     call do_work2

  enddo

  call grvy_timer_finalize()

  call grvy_timer(raw_grvy_timer2)

  call grvy_timer_elapsed_global(grvy_total_timing);
  dtime_global = dtime(timearray)

  grvy_work1_timing = grvy_timer_elapsedseconds("work1")

  ! Compare raw timer to total elapsed

  raw_timer = raw_grvy_timer2 - raw_grvy_timer1

  if( abs(grvy_total_timing - raw_timer) > Tolerance )then
     call exit(1)
  endif

  call grvy_asci_time(timestring)

  ! Verify timer call count

  call_count = grvy_timer_stats_count   ("work1")
  call_mean  = grvy_timer_stats_mean    ("work1")
  call_mean  = grvy_timer_stats_variance("work1")

  if (call_count .ne. Max_Iters)then
     call exit(1)
  endif

  ! Verify reset nullifies timer

  call grvy_timer_reset()

  grvy_work1_timing = grvy_timer_elapsedseconds("work1")

  if( abs(grvy_work1_timing) > 1e-15 )then
     call exit(1)
  endif
  
  !print*,'Fortran timer = ',dtime_global
  !print*,'GRVY timer = ',grvy_total_timing

!   diff = abs(grvy_total_timing-dtime_global)

!   if( diff > Tolerance) then
!      print*,'Error: Global timing mismatch -> diff = ',diff,' (secs)'
!      print*,'       The test host could be overloaded or the timer may be incorrect'
!      call exit(1)
!   endif

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
    

