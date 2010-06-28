!*----------------------------------------------
! * GRVY Test Suite
! *
! * 2010-03-08  Nick
! *
! * Contrasting to C_input.c
! *
! *----------------------------------------------

program main
  use grvy
  implicit none
  
  integer(4)     :: iter_max,turbulent,ivar1,ivar2,ivec(4)
  integer        :: string_size
  integer(4)     :: error, flag ! 1 for success, 0 for failure

  real           :: reyn,mach,aoa, A(3)
  real(4)        :: fvar1,fvar2,fvec(3)
  real(8)        :: dvar1,dvar2,dvec(3)

  character      :: key1*100
  character      :: key2*100
  character      :: gridfile*100

  error=1 ! start with no error condition
  call grvy_input_fopen("./input-example.txt",flag)
  error=error*flag

  ! ----- read from BASIC section -----
  call grvy_input_fread_real ("reyn", reyn, flag)
  error=error*flag
  call grvy_input_fread_real ("mach", mach, flag)
  error=error*flag
  call grvy_input_fread_real ("aoa", aoa, flag)
  error=error*flag
  call grvy_input_fread_int  ("iter_max",iter_max,flag)
  error=error*flag
  call grvy_input_fread_char ("gridfile",gridfile,flag)
  error=error*flag

  ! verify that len_trim works as desired on parsed strings
  string_size = len_trim(gridfile)
  if(string_size .ne. 15) then
     write(6,*) "Failed len_trim test"     
     call exit(1)
  endif
  ! ----- read from SOLVER section -----

  call grvy_input_fread_int      ("solver/turbulence",turbulent,flag)
  error=error*flag
  call grvy_input_fread_char_ivec("solver/keywords",key1,1,flag)
  error=error*flag
  call grvy_input_fread_char_ivec("solver/keywords",key2,2,flag)
  error=error*flag
  call grvy_input_fread_real_vec ("turbulence/A",A,3,flag)
  error=error*flag

  ! verify that len_trim works as desired on parsed string arrays
  string_size = len_trim(key2)
  if(string_size .ne. 4) then
     write(6,*) "Failed len_trim test"
     call exit(1)
  endif

  ! ----- read from VERIFY section -----
  
  call grvy_input_fread_int     ("verify/ivar",ivar1,flag)
  error=error*flag
  call grvy_input_fread_int_ivec("verify/ivec",ivar2,3,flag)
  error=error*flag
  call grvy_input_fread_int_vec ("verify/ivec",ivec,4,flag)
  error=error*flag

  ! ----- Verify Results -----
  
  if(ivec(1) .ne. 11 .or. ivec(2) .ne. 12 .or. ivec(3) .ne. 13 .or. ivec(4) .ne. 14) then
     flag = 0;
     error=error*flag  
     write(6,*) "Read int_ivec mismatch"
  endif

  call compare_int(ivar1,10,flag)
  error=error*flag
  call compare_int(ivar2,13,flag)
  error=error*flag

  call grvy_input_fread_real     ("verify/fvar",fvar1,flag)
  error=error*flag
  call grvy_input_fread_real_ivec("verify/fvec",fvar2,1,flag)
  error=error*flag
  call grvy_input_fread_real_vec ("verify/fvec",fvec,3,flag)
  error=error*flag

  if(fvec(1) .ne. 101 .or. fvec(2) .ne. 102 .or. fvec(3) .ne. 103) then
     flag = 1;
     error=error*flag
     write(6,*) "Read float_ivec mismatch"
  endif

  call grvy_input_fclose()
  
  if(error .eq. 0) then ! signal error
     call exit(1)
  endif

  call exit(0)

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
