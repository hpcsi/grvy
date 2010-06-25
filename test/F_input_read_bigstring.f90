!*----------------------------------------------
! * GRVY Test Suite
! *
! * 2010-03-08  Nick
! *
! * $Id:$
! *----------------------------------------------

program main
  use grvy
  implicit none
  
  integer        :: flag
  character      :: key1*2
  character      :: gridfile*10

  call grvy_log_setlevel(GRVY_NOLOG);

  call grvy_input_fopen("./input-example.txt",flag)

  if(flag .eq. 0)then
     call exit(1)
  endif

  ! Verify that we get an error if user does not provide enough
  ! storage space for a parsed string

  call grvy_input_fread_char ("gridfile",gridfile,flag)

  if(flag .ne. 0)then
     call exit(1)
  endif

  call grvy_input_fread_char_ivec("solver/keywords",key1,1,flag)

  if(flag .ne. 0)then
     call exit(1)
  endif

  call exit(0)

end program main

