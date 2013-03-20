!! -*-f90-*-
!!-----------------------------------------------------------------------bl-
!!--------------------------------------------------------------------------
!! 
!! libGRVY - a utility library for scientific computing.
!!
!! Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
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

program F_check_copy_dir
  use grvy
#ifdef __INTEL_COMPILER
  use ifport
#endif
  implicit none

  character*14  :: template1  = "tmpdirf_XXXXXX"
  character*14  :: template2  = "tmpdirf_XXXXXX"
  character*512 :: example_dir

  integer :: SUCCESS = 0
  integer :: FAILURE = 1

  integer :: flag
  integer :: status
  integer :: length

  call grvy_log_setlevel(GRVY_NOLOG)
  call get_environment_variable("GRVY_INPUT_EXAMPLE_DIR",example_dir,length,status)

  if(status .ne. 0)then
     example_dir = "./"
  else
     example_dir = trim(example_dir)//"/"
  endif

  call grvy_create_scratch_dir(template1, flag)
  if(flag .ne. SUCCESS)then
     write (*,*) "Create unique_dir unsuccessful ",template1
     call exit(1)
  endif

  call grvy_create_scratch_dir(template2, flag)
  if(flag .ne. SUCCESS)then
     write (*,*) "Create unique_dir unsuccessful",template2
     call exit(1)
  endif

  ! Failure should occur if we attempt to copy a file 

  call grvy_copy_dir("F_check_file_path",template1,flag)
  if(flag .ne. FAILURE)then
     write (*,*) "Expected failure for file copy"
     call exit(1)
  endif

  ! Failure should occur if input/destination dirs are identical 

  call grvy_copy_dir(template1,template1,flag)
  if(flag .ne. FAILURE)then
     write (*,*) "Expected failure for identical in/out dirs"
     call exit(1)
  endif

  ! Success should occur for existing directory copied to empty existing destination 

  call grvy_copy_dir(trim(example_dir)//"ref_files/adir",template1,flag);
  if(flag .ne. SUCCESS)then
     write (*,*) "copy_dir to empty directory unsuccesful"
     call exit(1)
  endif

  ! verify result

  status = system (trim(example_dir)//"diff_dir.sh "//trim(example_dir)//"ref_files/adir "//template1)
  if(status .ne. 0)then
     write (*,*) "copy_dir to empty directory does not verify"
     call exit(1)
  endif

  ! Success should occur for existing directory copied to non-existing destination dir

  status = system ("rmdir "//template2)
  if(status .ne. 0)then
     write (*,*) "unable to remove existing dir ",template2
     call exit(1)
  endif

  call grvy_copy_dir(trim(example_dir)//"ref_files/adir",template2,flag);
  if(flag .ne. SUCCESS)then
     write (*,*) "copy_dir to non-existent directory unsuccesful"
     call exit(1)
  endif

  ! verify result

  status = system (trim(example_dir)//"diff_dir.sh "//trim(example_dir)//"ref_files/adir "//template2)
  if(status .ne. 0)then
     write (*,*) "copy_dir to non-existent directory does not verify"
     call exit(1)
  endif

  call exit(0)
end program F_check_copy_dir
