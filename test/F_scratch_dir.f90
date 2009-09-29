program F_scratch_dir
  use hpct;
  implicit none;

  character*13 :: program_name = "F_scratch_dir"

  integer :: flag
  integer :: retval = 0

  character*37 :: invalid_template = "ScratchDir_doesNotHaveSixTrailingExes"
  character*37 :: valid_template   = "ScratchDir_itHasSixTrailingExesXXXXXX"
  character*37 :: backup_template

  backup_template = invalid_template
  call hpct_create_scratch_dir(invalid_template, flag);
  if (flag.eq.0) then
    write (*,*) "Expected nonzero return for invalid template"
    retval = 1
  end if
  if (backup_template.ne.invalid_template) then
    write (*,*) "Expected template to not change on failed invocation"
    retval = 1
  end if

  backup_template = valid_template
  call hpct_create_scratch_dir(valid_template, flag);
  if (flag.ne.0) then
    write (*,*) "Expected zero return for valid template"
    retval = 1
  end if
  if (backup_template.eq.invalid_template) then
    write (*,*) "Expected template to change on successful invocation"
    retval = 1
  end if

  call exit(retval)
end program F_scratch_dir
