program main
  use hpct

  character :: filepath1*100 = './afile'
  character :: filepath2*100 = 'bfile'
  character :: filepath3*100 = 'adir/bdir/cfile'
  character :: filepath4*100 = '/tmp/moontower'
  character :: filepath5*100 = '../sneaky/adir/dfile'


  print*,'Fortran Rules'
  print*,' '
  print*,'Possibly creating parent directories for the following files'

  print*,'--> ',filepath1
  print*,'--> ',filepath2
  print*,'--> ',filepath3
  print*,'--> ',filepath4
  print*,'--> ',filepath5

  ! Create parent directories if necessary

  call hpct_check_file_path(filepath1);
  call hpct_check_file_path(filepath2);
  call hpct_check_file_path(filepath3);
  call hpct_check_file_path(filepath4);
  call hpct_check_file_path(filepath5);

  ! Write to the files

  open(unit=13,file=filepath1,status='unknown')
  write(13,*) '42'
  close(13)

  open(unit=13,file=filepath2,status='unknown')
  write(13,*) 'is'
  close(13)

  open(unit=13,file=filepath3,status='unknown')
  write(13,*) 'all'
  close(13)

  open(unit=13,file=filepath4,status='unknown')
  write(13,*) 'you'
  close(13)

  open(unit=13,file=filepath5,status='unknown')
  write(13,*) 'need'
  close(13)
  
  print*,' '
  print*,'SUCCESS'

stop
end program main
