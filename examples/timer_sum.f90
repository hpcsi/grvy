
program main

  integer :: Foo_Sleep = 1
  integer :: Bar_Sleep = 2
  integer :: Poo_Sleep = 3
  integer :: Max_Iters = 2
  real*8  :: igot,igot2
  character :: timestring*50 = ''

  integer i

  call hpct_asci_time(timestring)

  write(*,'(a,a26)') 'Run on: ',trim(timestring)

  ! Primary Iteration Loop 

  do i = 1,Max_Iters

     ! Define the beginning of the overall portion to be monitored

     call hpct_timer_init();

     print*,'Main iteration loop = ',i
     
     call foo(Foo_Sleep);
     call bar(Bar_Sleep);
     call poo(Poo_Sleep);
     
     call hpct_timer_finalize();
     
  enddo

  call hpct_timer_summarize();

  print*,' '
  print*,'Expecting ',Max_Iters*(Foo_Sleep+Poo_Sleep+Bar_Sleep),' secs '

  ! example use of timer directly
  
  call hpct_timer(igot)
  call sleep(1)
  call hpct_timer(igot2)

  print*,' '
  print*,'Measured ',igot2-igot,' secs (expected 1.0)'

  stop
end program main


subroutine foo(isleep)
  integer isleep
  
  call hpct_timer_begin('foo');
  call sleep(isleep);
  call hpct_timer_end('foo');

  return
end subroutine foo
    
subroutine bar(isleep)
  integer isleep

  call hpct_timer_begin('bar');
  call sleep(isleep);
  call hpct_timer_end('bar');
  return

end subroutine bar

subroutine poo(isleep)
  integer isleep

  call hpct_timer_begin('poo');
  call sleep(isleep);
  call hpct_timer_end('poo');
  return
end subroutine poo

