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
!! Fortran example illustrating performance timing via libGRVY..
!!
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

program main
  use grvy
  implicit none

  integer   :: Foo_Sleep = 1
  integer   :: Bar_Sleep = 2
  integer   :: Boo_Sleep = 3
  integer   :: Max_Iters = 2
  integer   :: i
  real*8    :: igot,igot2
  character :: timestring*50 = ''

  call grvy_asci_time(timestring)

  write(*,'(a,a26)') 'Run on: ',trim(timestring)

  ! Initialize the timing library - the global timer will be
  ! initialized with this call

  call grvy_timer_init('Fortran is the best!');

  ! Outer Test Loop

  do i = 1,Max_Iters

     ! Define the beginning of the overall portion to be monitored

     print*,'Main iteration loop = ',i
     
     call foo(Foo_Sleep)

     igot = grvy_timer_elapsed_global()
     print*,'Elapsed time since global init is: ',igot

     call bar(Bar_Sleep)
     call boo(Boo_Sleep)

  enddo

  ! Finalize the main program timer 

  call grvy_timer_finalize()     

  ! Print performance summary to stdout 

  call grvy_timer_summarize()

  print*,' '
  print*,'Expecting ',Max_Iters*(Foo_Sleep+Boo_Sleep+Bar_Sleep),' secs '

  ! Example use of timer directly

  igot = grvy_timer()
  call sleep(1)
  igot2 = grvy_timer()

  print*,' '
  print*,'Measured ',igot2-igot,' secs (expected approximately 1.0)'

  igot = grvy_timer_elapsed_global()
  print*,' '
  print*,'The total elapsed time since init is: ',igot

  print*,' '
  print*,'Query individual timers  - should match summary'

  write(*,'(1x,"boo: ",es12.5," secs")') grvy_timer_elapsedseconds("boo")
  write(*,'(1x,"bar: ",es12.5," secs")') grvy_timer_elapsedseconds("bar")
  write(*,'(1x,"foo: ",es12.5," secs")') grvy_timer_elapsedseconds("foo")

  print*,' '
  print*,'Query individual timer stats - should match summary'

  write(*,'(1x,"foo-count:    ",i3)')     grvy_timer_stats_count    ("foo")
  write(*,'(1x,"foo-mean:     ",es12.5)') grvy_timer_stats_mean     ("foo")
  write(*,'(1x,"foo-variance: ",es12.5)') grvy_timer_stats_variance ("foo")
  write(*,'(1x,"foo-min:      ",es12.5)') grvy_timer_stats_min      ("foo")
  write(*,'(1x,"foo-max:      ",es12.5)') grvy_timer_stats_max      ("foo")

  ! The above queries provide eclusive timing statistics, but you can
  ! also query inclusive measurements as well

  print*,' '
  print*,'Query individual timers (inclusive values)'

  write(*,'(1x,"boo: ",es12.5," secs")') grvy_timer_elapsedseconds_inc("boo")
  write(*,'(1x,"bar: ",es12.5," secs")') grvy_timer_elapsedseconds_inc("bar")
  write(*,'(1x,"foo: ",es12.5," secs")') grvy_timer_elapsedseconds_inc("foo")

  print*,' '
  print*,'Query individual timer stats (inclusive values for boo)'

  write(*,'(1x,"boo-mean:     ",es12.5)') grvy_timer_stats_mean_inc    ("boo")
  write(*,'(1x,"boo-variance: ",es12.5)') grvy_timer_stats_variance_inc("boo")
  write(*,'(1x,"boo-min:      ",es12.5)') grvy_timer_stats_min_inc     ("boo")
  write(*,'(1x,"boo-max:      ",es12.5)') grvy_timer_stats_max_inc     ("boo")



  ! Dump current results to a historical performance logfile

  call grvy_timer_save_hist("F90-Example1","",1,"hist.h5")

  stop
end program main


subroutine foo(isleep)
  use grvy
  integer isleep
  
  call grvy_timer_begin('foo');
  call sleep(isleep);
  call grvy_timer_end('foo');

  return
end subroutine foo
    
subroutine bar(isleep)
  use grvy
  integer isleep

  call grvy_timer_begin('bar');
  call sleep(isleep);
  call grvy_timer_end('bar');
  return

end subroutine bar

subroutine boo(isleep)
  use grvy
  integer isleep

  call grvy_timer_begin('boo');
  call sleep(isleep);
  call grvy_timer_end('boo');
  return
end subroutine boo

