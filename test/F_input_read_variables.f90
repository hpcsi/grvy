!*----------------------------------------------
! * GRVY Test Suite
! *
! * 2010-03-08  Nick
! *----------------------------------------------

program main
  use grvy
  implicit none
  
  real           :: reyn,mach,aoa, A(3)
  integer        :: iter_max,turbulent, ivar1,ivec(4),ivar2(2)
  integer        :: string_size
  integer(4)     :: ret_value, flag ! 0 for success, 1 for failure
  character      :: key1*100
  character      :: key2*100
  character      :: gridfile*100

  ret_value = 0

  call grvy_input_fopen("./input-example.txt",flag)

  ! ----- read from BASIC section -----

  call grvy_input_fread_real ("reyn", reyn, flag)
  call grvy_input_fread_real ("mach", mach, flag)
  call grvy_input_fread_real ("aoa", aoa, flag)
  call grvy_input_fread_int  ("iter_max",iter_max,flag)
  call grvy_input_fread_char ("gridfile",gridfile,flag)

  ! verify that len_trim works as desired on parsed strings

  string_size = len_trim(gridfile)

  if(string_size .ne. 15) then
     call exit(1)
  endif

  ! ----- read from SOLVER section -----

  call grvy_input_fread_int  ("solver/turbulence",turbulent,flag)
  call grvy_input_fread_char_ivec("solver/keywords",key1,1,flag)
  call grvy_input_fread_char_ivec("solver/keywords",key2,2,flag)
  call grvy_input_fread_real_vec("turbulence/A",A,3,flag)

  ! verify that len_trim works as desired on parsed string arrays

  string_size = len_trim(key2)

  if(string_size .ne. 4) then
     call exit(1)
  endif

  ! ----- read from VERIFY section -----

  call grvy_input_fread_int     ("verify/ivar",ivar1,flag)
  !call grvy_input_fread_int_ivec("verify/ivec",ivar2,2,flag)
  call grvy_input_fread_int_vec ("verify/ivec",ivec,4,flag)

  ! ----- Test Results -----
  

  call grvy_input_fclose()
  
  call exit(ret_value)

end program main

!#########################
!#
!# Compare Integers
!# 
!#########################

subroutine compare_int(var1, var2)
  implicit none
  integer(4), intent(in) :: var1, var2
  
  if(var1 .ne. var2) then
     write(6,*) "FAILED: integer variable mismatch"
     call exit(1)
  endif
  
  return

end subroutine compare_int
