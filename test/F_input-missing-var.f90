! *----------------------------------------------
! * HPCT Test Suite
! *
! * F_input-missing-var.f90 8063 2010-03-08 nick
! *----------------------------------------------

program main
  use hpct
  implicit none

  integer           :: ret_value = 0 ! 0 for success, 1 for failure
  real              :: reyn
  character(len=60) :: filename

  ! Silence warning/error/info message 
  call hpct_log_setlevel(HPCT_NOLOG)
  
  !filename = build_example_file_path()

  ! Initialize/read the file
  !call hpct_input_fopen(filename,ret_value)
  
  ! Read specific variables and echo locally
  !hpct_input_fread_float("strangeways", reyn,ret_value)

  ! Close the file
  !call hpct_input_fclose()

  call exit(ret_value)
end program main
