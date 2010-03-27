! *----------------------------------------------
! * GRVY Test Suite
! *
! * F_input-missing-var.f90 8063 2010-03-08 nick
! *----------------------------------------------

program main
  use grvy
  implicit none

  integer           :: ret_value = 0 ! 0 for success, 1 for failure
  real              :: reyn
  character(len=60) :: filename

  ! Silence warning/error/info message 
  call grvy_log_setlevel(GRVY_NOLOG)
  
  !filename = build_example_file_path()

  ! Initialize/read the file
  !call grvy_input_fopen(filename,ret_value)
  
  ! Read specific variables and echo locally
  !grvy_input_fread_float("strangeways", reyn,ret_value)

  ! Close the file
  !call grvy_input_fclose()

  call exit(ret_value)
end program main
