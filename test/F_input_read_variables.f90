!*----------------------------------------------
! * HPCT Test Suite
! *
! * 2010-03-08  Nick
! *----------------------------------------------

program main
  use hpct
  implicit none
  
  real           :: reyn,mach,aoa, A(3)
  integer        :: iter_max,turbulent, ivar1,ivec(4)
  integer(4)     :: ret_value, flag ! 0 for success, 1 for failure
  character      :: key1*100 = ''
  character      :: key2*100 = ''
  character      :: gridfile*100 = ''

  ret_value = 0

  call hpct_input_fopen("./input-example.txt",flag)

  ! ----- read from BASIC section -----

  call hpct_input_fread_real ("reyn", reyn, flag)
  call hpct_input_fread_real ("mach", mach, flag)
  call hpct_input_fread_real ("aoa", aoa, flag)
  call hpct_input_fread_int  ("iter_max",iter_max,flag)
  call hpct_input_fread_char ("gridfile",gridfile,flag)

  ! ----- read from SOLVER section -----

  call hpct_input_fread_int  ("solver/turbulence",turbulent,flag)
  call hpct_input_fread_char_ivec("solver/keywords",key1,1,flag)
  call hpct_input_fread_char_ivec("solver/keywords",key2,2,flag)
  !call hpct_input_fread_float_vec("turbulence/A",A,3,flag)

  ! ----- read from VERIFY section -----

  call hpct_input_fread_int     ("verify/ivar",ivar1,flag)
  !call hpct_input_fread_int_ivec("verify/ivec",ivar2,2,flag)
  call hpct_input_fread_int_vec ("verify/ivec",ivec,4,flag)

  ! ----- Test Results -----
  

  call hpct_input_fclose()
  
  
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
