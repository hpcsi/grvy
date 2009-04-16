program main
  integer flag
  real reyn
  integer iter_max
  integer switches(3)
  character :: filename*100 = ''
  character :: key1*100 = ''
  character :: key2*100 = ''

!------------------------------------------------
! Simple C example to illustre basic input 
! file parsing via HPCT.
!
! $Id$
!------------------------------------------------

  ! Initialize/read the file

  call hpct_input_fopen("./pecos-input.txt",flag);

  ! Dump the whole file to stdout

  print*,' ------ Full Dump ------'
  call hpct_input_fdump(flag);
  print*,' ---- End Full Dump ----'

  print*,' ------ Full Dump (delimited) ------'
  call hpct_input_fdump_delim('# ',flag);

  ! Read specific variables

  call hpct_input_fread_real ("reyn",reyn,flag)
  call hpct_input_fread_int  ("iter_max",iter_max,flag)
  call hpct_input_fread_char ("gridfile",filename,flag)

  print*,'reyn                = ',reyn
  print*,'iter_max            = ',iter_max
  print*,'gridfile            = ',trim(filename)

  call hpct_input_fread_int_vec ("solver/switches",switches,4,flag)

  print*,'switches            = ',switches(1),switches(2),switches(3),switches(4)

  call hpct_input_fread_char_ivec("solver/keywords",key1,1,flag)
  call hpct_input_fread_char_ivec("solver/keywords",key2,2,flag)

  print*,'solver/keyword 1    = ',trim(key1)
  print*,'solver/keyword 2    = ',trim(key2)

  ! Attempt to read undefined variable (should get error message)

  call hpct_input_fread_int("koomie",iter_max,flag)

  ! Disable error messages if you want to control them yourself
  ! If you query the unknown variable again it should be quiet

  call hpct_input_toggle_messages(0)
  call hpct_input_fread_int("koomie",iter_max,flag)



stop
end program main
