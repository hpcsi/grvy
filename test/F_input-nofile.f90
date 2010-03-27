! *----------------------------------------------
! * GRVY Test Suite
! *
! * $Id: F_input-nofile.f90 8063 2010-03-08 Nick
! *----------------------------------------------

program main
  use grvy
  implicit none

  integer ret_value

  call grvy_log_setlevel(GRVY_NOLOG)
  
  ! Initialize/read the file
  call grvy_input_fopen("./tori-droolz",ret_value)
  
  ! Close the file
  call grvy_input_fclose()

  call exit(ret_value)
end program main
