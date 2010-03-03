program F_unique_dir
  use hpct;
  implicit none;

  character*12 :: program_name = "F_unique_dir"

  integer :: flag
  integer :: retval = 0

  character*34 :: invalid_template = "TempDir_doesNotHaveSixTrailingExes"
  character*34 :: valid_template   = "TempDir_itHasSixTrailingExesXXXXXX"
  character*34 :: backup_template

  call hpct_log_setlevel(HPCT_NOLOG)

  backup_template = invalid_template
  call hpct_create_unique_dir(invalid_template, flag);
  if (flag.eq.0) then
    write (*,*) "Expected nonzero return for invalid template"
    retval = 1
  end if
  if (backup_template.ne.invalid_template) then
    write (*,*) "Expected template to not change on failed invocation"
    retval = 1
  end if

  backup_template = valid_template
  call hpct_create_unique_dir(valid_template, flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for valid template"
    retval = 1
  end if
  if (backup_template.eq.invalid_template) then
    write (*,*) "Expected template to change on successful invocation"
    retval = 1
  end if

  call exit(retval)
end program F_unique_dir
