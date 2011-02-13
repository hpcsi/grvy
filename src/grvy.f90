!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------
!!
!! libGRVY - a utility library for scientific computing.
!!
!! Copyright (C) 2008,2009,2010 The PECOS Development Team
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
!!--------------------------------------------------------------------------
!!
!! grvy.f90: Fortran module interface definition for GRVY routines.
!!
!! $Id$
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

!> \page apiF Fortran Library Interface
!!
!! This section outlines the available GRVY functions for Fortran.
!! Note that library access is provided thru a Fortran90 module
!! which requires the external calling application to include the
!! following within the Fortran90 source code:
!!
!! \code
!! use grvy
!! \endcode
!!
!! Several simple examples using these functions are provided in the
!! examples directory. Functions which have an integer return value
!! return "0" upon success. To illustrate the organizational
!! flexibility of the input file parsing, a \ref inputFile
!! "sample input file" is also provided.

module grvy

  interface

     ! -------------------
     ! Versioning routines
     ! -------------------

     subroutine grvy_version_stdout() bind (C)
       use iso_c_binding
       implicit none
     end subroutine grvy_version_stdout

     integer (C_int) function grvy_get_numeric_version() bind (C)
       use iso_c_binding
       implicit none
     end function grvy_get_numeric_version

     ! ----------------------
     ! Input Parsing Support
     ! ----------------------

     !> \page apiF
     !! \section input Input Parsing Routines
     !! <b>Open/close a libGRVY style input file:</b>
     !! \code
     !! subroutine grvy_input_fopen()
     !! subroutine grvy_input_fclose()
     !! \endcode

     integer (C_int) function grvy_input_fopen_passthrough(filename) bind (C,name='grvy_input_fopen')
       use iso_c_binding
       implicit none

       character (C_char),intent(in) :: filename(*) !< libGRVY style input filename

     end function grvy_input_fopen_passthrough

     subroutine grvy_input_fclose() bind (C)
       use iso_c_binding
       implicit none
     end subroutine grvy_input_fclose

     !> \page apiF
     !!
     !! <b> Dump the contents of a libGRVY style input file :</b>
     !!
     !! The following routines can be used to dump the entire contents of
     !! a parsed libGRVY style input file to \e stdout or to an ASCII
     !! file (note that all comments within the original input file are
     !! stripped). The prefix variable can be used to include an
     !! additional delimiter which prepends each line of the output.
     !! These routines are normally used to save all relevant input
     !! parameters in the application output for future traceability and
     !! repeatability.
     !! int read(int fd,char *buf,size_t count)
     !!
     !! - subroutine grvy_input_fdump()
     !! - subroutine grvy_input_fdump_delim()
     !! - subroutine grvy_input_fdump_file()

     integer (C_int) function grvy_input_fdump_passthrough() bind (C,name='grvy_input_fdump')
       use iso_c_binding
       implicit none
     end function grvy_input_fdump_passthrough

     integer (C_int) function grvy_input_fdump_delim_passthrough(prefix) bind (C,name='grvy_input_fdump_delim')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: prefix(*)     
     end function grvy_input_fdump_delim_passthrough

     integer (C_int) function grvy_input_fdump_file_passthrough(prefix,filename) bind (C,name='grvy_input_fdump_file')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: prefix(*)    
       character (C_char),intent(in) :: filename(*)  
     end function grvy_input_fdump_file_passthrough

     integer (C_int) function grvy_input_fread_real_passthrough(var,value) bind (C,name='grvy_input_fread_float')
       use iso_c_binding
       implicit none
       character (C_char), intent(in)   :: var(*)
       real      (C_float),intent(out)  :: value
     end function grvy_input_fread_real_passthrough

     integer (C_int) function grvy_input_fread_double_passthrough(var,value) bind (C,name='grvy_input_fread_double')
       use iso_c_binding
       implicit none
       character (C_char),  intent(in)  :: var(*)
       real      (C_double),intent(out) :: value
     end function grvy_input_fread_double_passthrough

     integer (C_int) function grvy_input_fread_int_passthrough(var,value) bind (C,name='grvy_input_fread_int')
       use iso_c_binding
       implicit none
       character (C_char),intent(in)    :: var(*)
       integer   (C_int), intent(out)   :: value
     end function grvy_input_fread_int_passthrough

     ! ----------------------------
     ! vec bindings to C routines
     ! ----------------------------

     integer (C_int) function grvy_input_fread_int_vec_passthrough(var,value,nelems)  &
          bind (C,name='grvy_input_fread_int_vec')
       use iso_c_binding
       implicit none
       character (C_char),       intent(in)  :: var(*)
       integer   (C_int),        intent(out) :: value(*)
       integer   (C_int), value, intent(in)  :: nelems
     end function grvy_input_fread_int_vec_passthrough

     integer (C_int) function grvy_input_fread_real_vec_passthrough(var,value,nelems)  &
          bind (C,name='grvy_input_fread_float_vec')
       use iso_c_binding
       implicit none
       character (C_char),       intent(in)  :: var(*)
       real      (C_float),      intent(out) :: value(*)
       integer   (C_int), value, intent(in)  :: nelems
     end function grvy_input_fread_real_vec_passthrough

     integer (C_int) function grvy_input_fread_double_vec_passthrough(var,value,nelems)  &
          bind (C,name='grvy_input_fread_double_vec')
       use iso_c_binding
       implicit none
       character (C_char),       intent(in)  :: var(*)
       real      (C_double),     intent(out) :: value(*)
       integer   (C_int), value, intent(in)  :: nelems
     end function grvy_input_fread_double_vec_passthrough

     ! ----------------------------
     ! ivec bindings to C routines
     ! ----------------------------

     integer (C_int) function grvy_input_fread_int_ivec_passthrough(var,value,elem)  &
          bind (C,name='grvy_input_fread_int_ivec')
       use iso_c_binding
       implicit none
       character (C_char),       intent(in)  :: var(*)
       integer   (C_int),        intent(out) :: value
       integer   (C_int), value, intent(in)  :: elem
     end function grvy_input_fread_int_ivec_passthrough

     integer (C_int) function grvy_input_fread_real_ivec_passthrough(var,value,elem)  &
          bind (C,name='grvy_input_fread_float_ivec')
       use iso_c_binding
       implicit none
       character (C_char),       intent(in)  :: var(*)
       real      (C_float),      intent(out) :: value
       integer   (C_int), value, intent(in)  :: elem
     end function grvy_input_fread_real_ivec_passthrough

     integer (C_int) function grvy_input_fread_double_ivec_passthrough(var,value,elem)  &
          bind (C,name='grvy_input_fread_double_ivec')
       use iso_c_binding
       implicit none
       character (C_char),       intent(in)  :: var(*)
       real      (C_double),     intent(out) :: value
       integer   (C_int), value, intent(in)  :: elem
     end function grvy_input_fread_double_ivec_passthrough

     subroutine grvy_input_fread_char(var,value,flag)
       implicit none
       character :: var
       character :: value
       integer   :: flag
     end subroutine grvy_input_fread_char

     subroutine grvy_input_fread_char_ivec(var,value,elem,flag)
       implicit none
       character :: var
       character :: value
       integer   :: elem
       integer   :: flag
     end subroutine grvy_input_fread_char_ivec

     ! ---------------------------------
     ! Default Input Value Registrations
     ! ---------------------------------

     subroutine grvy_input_register_int(var,value,flag)
       implicit none
       character :: var
       integer   :: value
       integer   :: flag
     end subroutine grvy_input_register_int

     subroutine grvy_input_register_get_int(var,value,flag)
       implicit none
       character :: var
       integer   :: value
       integer   :: flag
     end subroutine grvy_input_register_get_int

     subroutine grvy_input_register_float(var,value,flag)
       implicit none
       character :: var
       real      :: value
       integer   :: flag
     end subroutine grvy_input_register_float

     subroutine grvy_input_register_get_float(var,value,flag)
       implicit none
       character :: var
       real      :: value
       integer   :: flag
     end subroutine grvy_input_register_get_float

     subroutine grvy_input_register_double(var,value,flag)
       implicit none
       character :: var
       real*8    :: value
       integer   :: flag
     end subroutine grvy_input_register_double

     subroutine grvy_input_register_get_double(var,value,flag)
       implicit none
       character :: var
       real*8    :: value
       integer   :: flag
     end subroutine grvy_input_register_get_double

     subroutine grvy_input_register_char(var,value,flag)
       implicit none
       character :: var
       character :: value
       integer   :: flag
     end subroutine grvy_input_register_char

     subroutine grvy_input_register_get_char(var,value,flag)
       implicit none
       character :: var
       character :: value
       integer   :: flag
     end subroutine grvy_input_register_get_char

     ! ---------------
     ! Timing Routines
     ! ---------------

     subroutine grvy_timer_summarize() bind (C)
       use iso_c_binding
       implicit none
     end subroutine grvy_timer_summarize

     integer (C_int) function grvy_timer_set_summarize_width(value) bind(C)
       use iso_c_binding
       implicit none
       integer   (C_int), value, intent(in)  :: value
     end function grvy_timer_set_summarize_width

     subroutine grvy_asci_time(timestring)
       implicit none
       character :: timestring
     end subroutine grvy_asci_time

     subroutine grvy_timer_init(id)
       implicit none
       character :: id
     end subroutine grvy_timer_init

     subroutine grvy_timer_reset()
     end subroutine grvy_timer_reset

     subroutine grvy_timer_finalize()
     end subroutine grvy_timer_finalize

!     subroutine grvy_timer_summarize()
!     end subroutine grvy_timer_summarize

     subroutine grvy_timer_begin(id)
       implicit none
       character :: id
     end subroutine grvy_timer_begin

     subroutine grvy_timer_end(id)
       implicit none
       character :: id
     end subroutine grvy_timer_end

     subroutine grvy_timer(value)
       implicit none
       real*8 :: value
     end subroutine grvy_timer

     subroutine grvy_timer_elapsed_global(value)
       implicit none
       real*8 :: value
     end subroutine grvy_timer_elapsed_global

     real*8 function grvy_timer_elapsedseconds(id)
       implicit none
       character :: id
     end function grvy_timer_elapsedseconds

     integer function grvy_timer_stats_count(id)
       implicit none
       character :: id
     end function grvy_timer_stats_count

     real*8 function grvy_timer_stats_mean(id)
       implicit none
       character :: id
     end function grvy_timer_stats_mean

     real*8 function grvy_timer_stats_variance(id)
       implicit none
       character :: id
     end function grvy_timer_stats_variance

     subroutine  grvy_timer_save_hist_passthrough(experiment,comment,num_procs, &
                                                  jobId,code_revision,filename) bind (C,name='grvy_timer_save_hist')
       use iso_c_binding
       implicit none
       character(C_char),intent(in)         :: experiment(*)
       character(C_char),intent(in)         :: comment(*)
       integer  (C_int), intent(in), value  :: num_procs
       integer  (C_int), intent(in), value  :: jobId
       integer  (C_int), intent(in), value  :: code_revision
       character(C_char),intent(in)         :: filename(*)
     end subroutine grvy_timer_save_hist_passthrough

     ! ---------------
     ! Math
     ! ---------------

     integer function grvy_double_isnan(value)
       implicit none
       real*8 :: value
     end function grvy_double_isnan

     integer function grvy_double_isinf(value)
       implicit none
       real*8 :: value
     end function grvy_double_isinf

     ! ---------------
     ! Logging
     ! ---------------

     subroutine grvy_log_setlevel(priority)
       implicit none
       integer :: priority
     end subroutine grvy_log_setlevel

     subroutine grvy_log_setmask(loglevel,mesg)
       implicit none
       integer   :: loglevel
       character :: mesg
     end subroutine grvy_log_setmask

     subroutine grvy_log(loglevel,mesg)
       implicit none
       integer   :: loglevel
       character :: mesg
     end subroutine grvy_log

     ! ---------------
     ! Miscellaneous
     ! ---------------

     subroutine grvy_check_file_path(pathname,flag)
       implicit none
       character :: pathname
       integer   :: flag
     end subroutine grvy_check_file_path

     subroutine grvy_create_unique_dir(name_template,flag)
       implicit none
       character :: name_template
       integer   :: flag
     end subroutine grvy_create_unique_dir

     subroutine grvy_create_scratch_dir(name_template,flag)
       implicit none
       character :: name_template
       integer   :: flag
     end subroutine grvy_create_scratch_dir

  end interface

  ! ---------------------------
  ! Logging Priority Constants
  ! ---------------------------

  integer, parameter :: GRVY_NOLOG =  -1
  integer, parameter :: GRVY_FATAL =   0
  integer, parameter :: GRVY_ERROR = 100
  integer, parameter :: GRVY_WARN  = 200
  integer, parameter :: GRVY_INFO  = 300
  integer, parameter :: GRVY_DEBUG = 400
  integer, parameter :: GRVY_ALL   = 500

  ! ---------------------------
  ! Fortran-only utilities
  ! ---------------------------

  !> \page apiF
  !! \section fortranonly Fortran-only utilities
  !! The following routines provide Fortran-specific functionality:
  !!
  !! \code
  !! subroutine grvy_get_command_arguments()
  !! \endcode

contains

!> Gather one or more command arguments into a single string.  Optionally,
!! surround \c string with the given \c prefix and \c suffix.
!!
!! @param[out] string String containing all arguments.
!! @param[in]  prefix Prefix to prepend to \c string.
!! @param[in]  suffix Suffix to append to \c string.
!! @param[in]  first  First argument to include using intrinsic
!!                    <tt>get_command_argument</tt>'s numbering scheme.
!!                    Defaults to 1 if not provided.
!! @param[in]  last   Last argument to include (inclusive) using
!!                    intrinsic <tt>get_command_argument</tt>'s numbering
!!                    scheme.  Defaults to intrinsic
!!                    <tt>command_argument_count</tt>'s value if not provided.
!! @param[out] length Last character position set in \c args.
!! @param[out] status Zero on success.  Nonzero otherwise.
!!             If not present, any error causes <tt>call abort</tt>.
subroutine grvy_get_command_arguments (string, prefix, suffix,      &
                                       first, last, length, status)
  implicit none
  character(len = *), intent(out)           :: string
  character(len = *), intent(in),  optional :: prefix
  character(len = *), intent(in),  optional :: suffix
  integer,            intent(in),  optional :: first, last
  integer,            intent(out), optional :: length, status

  ! Dynamically allocatable character arrays without fixed buffer limits
  ! only seem possible in newer Fortran standards.

  integer, parameter      :: buflen = 9999
  character(len = buflen) :: buf
  integer                 :: bufpos, arglen, i, ifirst, ilast, ilength, istatus
  
  ! Provide default values and checks on first and last

  if (present(last)) then
    ilast = min(last, command_argument_count())
  else
    ilast = command_argument_count()
  endif
  if (present(first)) then
    ifirst = first
  else
    ifirst = 1
  endif

  ! Determine the total storage necessary to hold the returned string

  if (present(prefix) .and. len_trim(prefix) > 0) then
    ilength = len_trim(prefix) + 1
  else
    ilength = 0
  end if

  do i = ifirst, ilast
    call get_command_argument (i, buf, arglen, istatus)
    if (istatus /= 0) goto 5                  ! unrecoverable
    ilength = len_trim(buf)                   ! accumulate length
    if (i /= ilast) ilength = ilength + 1     ! accumulate padding
  end do

  if (present(suffix) .and. len_trim(suffix) > 0) then
    ilength = 1 + len_trim(suffix)
  else
    ilength = 0
  end if

  if (present(length)) length = ilength

  ! Ensure caller has the storage necessary to house the result

  if (len(string) < ilength) then
    status = -1
    goto 5
  end if

  ! Accumulate the desired string tracking length in bufpos

  if (present(prefix) .and. len_trim(prefix) > 0) then
    string = trim(prefix) // " "
    bufpos = len_trim(prefix) + 1
  else
    string = ''
    bufpos = 0
  endif

  do i = ifirst, ilast
    call get_command_argument (i, buf, arglen, istatus)
    if (istatus /= 0) goto 5
    arglen = len_trim(buf)
    if (i == ifirst) then
      string = string(:bufpos) // trim(buf)
      bufpos = bufpos + arglen
    else
      string = string(:bufpos) // " " // trim(buf)
      bufpos = bufpos + 1 + arglen
    end if
  end do

  if (present(suffix) .and. len_trim(suffix) > 0) then
      string = string(:bufpos) // " " // trim(suffix)
  end if

  if (present(status)) status = 0 ! Success

  return

  ! Common error handling code for handling subroutine failure

5 if (present(status)) then
     status = istatus
     return
  end if
  write (*,*) "grvy_get_command_arguments: " // &
       "error encountered when status not used"
  call abort
  
end subroutine grvy_get_command_arguments

! --------------------------------------------------------------------
! Wrapper routines for functions which include character strings (or
! require extra return flag arguments); the string wrappers insert
! necessary null terminators for subsequent use with C/C++.
! 
! It is an extra jump, but it's significantly less painful than dealing
! with all the various name mangling permutations and hidden argument
! order betwixt C/Fortran. Kudos to the iso_c_binding folks
! -------------------------------------------------------------------

 subroutine grvy_input_fopen(filename,return_flag)
   use iso_c_binding
    implicit none
    character(len=*),intent(in)         :: filename
    integer  (C_int),intent(inout)      :: return_flag

    return_flag = grvy_input_fopen_passthrough(filename//C_NULL_CHAR)
    return
  end subroutine grvy_input_fopen

  subroutine grvy_input_fdump(return_flag)
    use iso_c_binding
    implicit none
    integer  (C_int),intent(inout)      :: return_flag

    return_flag = grvy_input_fdump_passthrough()
  end subroutine grvy_input_fdump

 subroutine grvy_input_fdump_delim(prefix,return_flag)
   use iso_c_binding
    implicit none
    character(len=*),intent(in)         :: prefix
    integer  (C_int),intent(inout)      :: return_flag

    return_flag = grvy_input_fdump_delim_passthrough(prefix//C_NULL_CHAR)
    return
  end subroutine grvy_input_fdump_delim

  subroutine grvy_input_fdump_file(prefix,filename,return_flag)
   use iso_c_binding
    implicit none
    character(len=*),intent(in)         :: prefix      !< delimiter
    character(len=*),intent(in)         :: filename    !< input filename
    integer  (C_int),intent(inout)      :: return_flag !< error return flag

    return_flag = grvy_input_fdump_file_passthrough(prefix//C_NULL_CHAR,filename//C_NULL_CHAR)
    return
  end subroutine grvy_input_fdump_file

  subroutine grvy_input_fread_real(var,value,return_flag)
   use iso_c_binding
    implicit none
    character(len=*),  intent(in)       :: var         !< variable keyword
    real     (C_float),intent(out)      :: value       !< keyword value from input value
    integer  (C_int),  intent(out)      :: return_flag !< error return flag

    return_flag = grvy_input_fread_real_passthrough(var//C_NULL_CHAR,value)
    return
  end subroutine grvy_input_fread_real

  subroutine grvy_input_fread_double(var,value,return_flag)
   use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: var         !< variable keyword
    real     (C_double),intent(out)     :: value       !< keyword value from input value
    integer  (C_int),   intent(out)     :: return_flag !< error return flag

    return_flag = grvy_input_fread_double_passthrough(var//C_NULL_CHAR,value)
    return
  end subroutine grvy_input_fread_double

  subroutine grvy_input_fread_int(var,value,return_flag)
   use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: var         !< variable keyword
    integer  (C_int),   intent(out)     :: value       !< keyword value from input
    integer  (C_int),   intent(out)     :: return_flag !< error return flag

    return_flag = grvy_input_fread_int_passthrough(var//C_NULL_CHAR,value)
    return
  end subroutine grvy_input_fread_int

  ! ----------------
  ! vec wrappers
  ! ----------------

  subroutine grvy_input_fread_int_vec(var,value,nelems,return_flag)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: var         !< variable keyword
    integer  (C_int),   intent(out)     :: value(*)    !< keyword vector values from input
    integer  (C_int),   intent(in)      :: nelems      !< size of vector to read 
    integer  (C_int),   intent(out)     :: return_flag !< error return flag

    return_flag = grvy_input_fread_int_vec_passthrough(var//C_NULL_CHAR,value,nelems)
    return
  end subroutine grvy_input_fread_int_vec

  subroutine grvy_input_fread_real_vec(var,value,nelems,return_flag)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: var         !< variable keyword
    real     (C_float), intent(out)     :: value(*)    !< keyword vector values from input
    integer  (C_int),   intent(in)      :: nelems      !< size of vector to read 
    integer  (C_int),   intent(out)     :: return_flag !< error return flag

    return_flag = grvy_input_fread_real_vec_passthrough(var//C_NULL_CHAR,value,nelems)
    return
  end subroutine grvy_input_fread_real_vec

  subroutine grvy_input_fread_double_vec(var,value,nelems,return_flag)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: var         !< variable keyword
    real     (C_double),intent(out)     :: value(*)    !< keyword vector values from input
    integer  (C_int),   intent(in)      :: nelems      !< size of vector to read 
    integer  (C_int),   intent(out)     :: return_flag !< error return flag

    return_flag = grvy_input_fread_double_vec_passthrough(var//C_NULL_CHAR,value,nelems)
    return
  end subroutine grvy_input_fread_double_vec

  ! ----------------
  ! ivec wrappers
  ! ----------------

  subroutine grvy_input_fread_int_ivec(var,value,elem,return_flag)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: var         !< variable keyword
    integer  (C_int),   intent(out)     :: value       !< ith (elem) vector value from input
    integer  (C_int),   intent(in)      :: elem        !< size of vector to read 
    integer  (C_int),   intent(out)     :: return_flag !< error return flag

    integer :: c_index

    ! Convert from Fortran to C style indexing

    c_index = elem - 1

    return_flag = grvy_input_fread_int_ivec_passthrough(var//C_NULL_CHAR,value,c_index)
    return
  end subroutine grvy_input_fread_int_ivec

  subroutine grvy_input_fread_real_ivec(var,value,elem,return_flag)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: var         !< variable keyword
    real     (C_float), intent(out)     :: value       !< ith (elem) vector value from input
    integer  (C_int),   intent(in)      :: elem        !< size of vector to read 
    integer  (C_int),   intent(out)     :: return_flag !< error return flag

    integer :: c_index

    ! Convert from Fortran to C style indexing

    c_index = elem - 1

    return_flag = grvy_input_fread_real_ivec_passthrough(var//C_NULL_CHAR,value,c_index)
    return
  end subroutine grvy_input_fread_real_ivec

  subroutine grvy_input_fread_double_ivec(var,value,elem,return_flag)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: var         !< variable keyword
    real     (C_double),intent(out)     :: value       !< ith (elem) vector value from input
    integer  (C_int),   intent(in)      :: elem        !< size of vector to read 
    integer  (C_int),   intent(out)     :: return_flag !< error return flag

    integer :: c_index

    ! Convert from Fortran to C style indexing

    c_index = elem - 1

    return_flag = grvy_input_fread_double_ivec_passthrough(var//C_NULL_CHAR,value,c_index)
    return
  end subroutine grvy_input_fread_double_ivec

  subroutine grvy_timer_save_hist(experiment,comment,num_procs,jobId,code_revision,filename)
    use iso_c_binding
    implicit none
    character(len=*),intent(in)         :: experiment
    character(len=*),intent(in)         :: comment
    integer  (C_int),intent(in), value  :: num_procs
    integer  (C_int),intent(in), value  :: jobId
    integer  (C_int),intent(in), value  :: code_revision
    character(len=*),intent(in)         :: filename
    
    call grvy_timer_save_hist_passthrough(experiment//C_NULL_CHAR,comment//C_NULL_CHAR, &
         num_procs,jobId,code_revision,filename//C_NULL_CHAR)
    return
  end subroutine grvy_timer_save_hist
  
end module grvy
