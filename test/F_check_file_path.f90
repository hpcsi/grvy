program F_check_file_path
  use hpct;
  implicit none;

  character*17 :: program_name = "F_check_file_path"

  integer :: flag
  integer :: retval = 0

  call hpct_check_file_path("F_check_file_path/foo", flag);
  if (flag.ne.(-1)) then
    write (*,*) "Expected nonzero return on attempt to clobber this binary"
    retval = 1
  end if

  call hpct_check_file_path("////", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for universally existent file path"
    retval = 1
  end if
  call hpct_check_file_path(".", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for universally existent file path"
    retval = 1
  end if
  call hpct_check_file_path("..", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for universally existent file path"
    retval = 1
  end if

  call hpct_check_file_path("./CheckFilePathString", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for paths relative to current directory"
    retval = 1
  end if
  call hpct_check_file_path("../CheckFilePathString", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for paths relative to current directory"
    retval = 1
  end if

  call hpct_check_file_path("CheckFilePathDir/foo", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for a single directory"
    retval = 1
  end if

  call hpct_check_file_path("CheckFilePathDir/A/B/C/foo", flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for a multiple directories"
    retval = 1
  end if

  call exit(retval)
end program F_check_file_path
