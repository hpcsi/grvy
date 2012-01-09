!!-----------------------------------------------------------------------bl-
!!--------------------------------------------------------------------------
!! 
!! libGRVY - a utility library for scientific computing.
!!
!! Copyright (C) 2008,2009,2010,2011,2012 The PECOS Development Team
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
!! Regression/Test Suite for libGRVY.
!!
!! $Id$
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

program main
  use grvy
  implicit none
  
  integer(4)     :: iter_max,turbulent,ivar1,ivar2,ivec(4)
  integer        :: string_size
  integer(4)     :: error, flag ! 1 for success, 0 for failure
  character*512  :: example_dir
  integer        :: length, status
  logical        :: lv1,lv2

  real           :: reyn,mach,aoa, A(3)
  real(4)        :: fvar1,fvar2,fvec(3)
  real(8)        :: dvar1,dvar2,dvec(3)

  character      :: key1*100
  character      :: key2*100
  character      :: gridfile*100

  call grvy_log_setlevel(GRVY_NOLOG)

  call get_environment_variable("GRVY_INPUT_EXAMPLE_DIR",example_dir,length,status)

  if(status .ne. 0)then
     example_dir = "./"
  else
     example_dir = trim(example_dir)//"/"
  endif

  error=1 ! start with no error condition
  call grvy_input_fopen(trim(example_dir)//"./input-example.txt",flag)
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
  
  ! ----- INTS -----
  if(ivec(1) .ne. 11 .or. ivec(2) .ne. 12 .or. ivec(3) .ne. 13 .or. ivec(4) .ne. 14) then
     flag = 0;
     error=error*flag  
     write(6,*) "Read int_ivec mismatch"
  endif

  call compare_int(ivar1,10,flag)
  error=error*flag
  call compare_int(ivar2,13,flag)
  error=error*flag

  ! ----- REALS -----
  call grvy_input_fread_real     ("verify/fvar",fvar1,flag)
  error=error*flag
  call grvy_input_fread_real_ivec("verify/fvec",fvar2,2,flag)
  error=error*flag
  call grvy_input_fread_real_vec ("verify/fvec",fvec,3,flag)
  error=error*flag

  if(fvec(1) .ne. 101 .or. fvec(2) .ne. 102 .or. fvec(3) .ne. 103) then
     flag = 1;
     error=error*flag
     write(6,*) "Read float_ivec mismatch"
  endif

  call compare_real(fvar1,100.,flag)
  error=error*flag
  call compare_real(fvar2,102.,flag)
  error=error*flag

  ! ----- DOUBLEs -----  
  call grvy_input_fread_double     ("verify/dvar",dvar1,flag)
  error=error*flag
  call grvy_input_fread_double_ivec("verify/dvec",dvar2,2,flag)
  error=error*flag
  call grvy_input_fread_double_vec ("verify/dvec",dvec,3,flag)
  error=error*flag

  if(dvec(1) .ne. 1.1d20 .or. dvec(2) .ne. 1.2d20 .or. dvec(3) .ne. 1.3d20) then
     flag = 1;
     error=error*flag
     write(6,*) "Read double_ivec mismatch"
  endif
  
  call compare_double(dvar1,1.0d20,flag)
  error=error*flag
  call compare_double(dvar2,1.2d20,flag)
  error=error*flag

  ! ----- Logicals -----

  call grvy_input_fread_logical ("verify/lv1",lv1,.true.,flag)
  error=error*flag

  call grvy_input_fread_logical ("verify/lv2",lv2,.false.,flag)
  error=error*flag

  if(.not. lv1 .or. lv2)then
     error = 0
  endif

  ! Verify we get default value correctly if not available in input file

  call grvy_input_fread_logical ("hello-sweetie/lv1",lv1,.false.,flag)
  error=error*flag

  call grvy_input_fread_logical ("hello-sweetie/lv2",lv2,.true.,flag)
  error=error*flag

  if(lv1 .or. .not. lv2)then
     error = 0
  endif

  ! Verify that we get an error if we try to open a new file prior to
  ! closing the currently open file

  call grvy_input_fopen("./input-example.txt",flag)
  if(flag .ne. 0)then
     call exit(1)
  endif
  
  ! ----- Close File // Error Handling -----  

  call grvy_input_fclose()

  ! Verify that we don't cause an error if we try to close multiple
  ! times (should be a noop)

  call grvy_input_fclose()

  ! Double check that we can open and read the same file again.

  call grvy_input_fopen(trim(example_dir)//"./input-example.txt",flag)
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
     write(6,*) "expected ", var2, " received ", var1
     call exit(1)
  endif
  
  return

end subroutine compare_int

!#########################
!#
!# Compare float (err, reals)
!# 
!#########################

subroutine compare_real(var1, var2)
  implicit none
  real(4), intent(in) :: var1, var2
  
  if(var1 .ne. var2) then
     write(6,*) "FAILED: real variable mismatch"
     write(6,*) "expected ", var2, " received ", var1
     call exit(1)
  endif
  
  return

end subroutine compare_real

!#########################
!#
!# Compare double
!# 
!#########################

subroutine compare_double(var1, var2)
  implicit none
  real(8), intent(in) :: var1, var2
  
  if(var1 .ne. var2) then
     write(6,*) "FAILED: double variable mismatch"
     write(6,*) "expected ", var2, " received ", var1
     call exit(1)
  endif
  
  return

end subroutine compare_double
