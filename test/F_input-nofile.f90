! *----------------------------------------------
! * HPCT Test Suite
! *
! * $Id: F_input-nofile.f90 8063 2010-03-08 Nick
! *----------------------------------------------

program main
  use hpct
  implicit none

  integer ret_value

  call hpct_log_setlevel(HPCT_NOLOG)
  
  ! Initialize/read the file
  call hpct_input_fopen("./tori-droolz",ret_value)
  
  ! Close the file
  call hpct_input_fclose()

  call exit(ret_value)
end program main
