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
!! grvy_hpct.f90: Fortran module interface definition for HPCT routines.
!!
!! $Id$
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

module hpct
  interface

     ! ----------------------
     ! Input Parsing Support
     ! ----------------------
     
     subroutine hpct_input_fopen(filename,flag)
       character :: filename
       integer   :: flag
     end subroutine hpct_input_fopen

     subroutine hpct_input_fclose()
     end subroutine hpct_input_fclose

     subroutine hpct_input_fdump(flag)
       integer   :: flag
     end subroutine hpct_input_fdump

     subroutine hpct_input_fdump_delim(prefix,flag)
       character :: prefix
       integer   :: flag
     end subroutine hpct_input_fdump_delim

     subroutine hpct_input_fdump_file(prefix,filename,flag)
       character :: prefix
       character :: filename
       integer   :: flag
     end subroutine hpct_input_fdump_file

     subroutine hpct_input_fread_real(var,value,flag)
       character :: var
       real      :: value
       integer   :: flag
     end subroutine hpct_input_fread_real

     subroutine hpct_input_fread_double(var,value,flag)
       character :: var
       real*8    :: value
       integer   :: flag
     end subroutine hpct_input_fread_double

     subroutine hpct_input_fread_int(var,value,flag)
       character :: var
       integer   :: value
       integer   :: flag
     end subroutine hpct_input_fread_int

     subroutine hpct_input_fread_int_vec(var,value,nelems,flag)
       character :: var
       integer   :: value(*)
       integer   :: nelems
       integer   :: flag
     end subroutine hpct_input_fread_int_vec

     subroutine hpct_input_fread_int_ivec(var,value,elem,flag)
       character :: var
       integer   :: value
       integer   :: elem
       integer   :: flag
     end subroutine hpct_input_fread_int_ivec

     subroutine hpct_input_fread_real_vec(var,value,nelems,flag)
       character :: var
       real      :: value(*)
       integer   :: nelems
       integer   :: flag
     end subroutine hpct_input_fread_real_vec

     subroutine hpct_input_fread_real_ivec(var,value,elem,flag)
       character :: var
       real      :: value
       integer   :: elem
       integer   :: flag
     end subroutine hpct_input_fread_real_ivec

     subroutine hpct_input_fread_double_vec(var,value,nelems,flag)
       character :: var
       real*8    :: value(*)
       integer   :: nelems
       integer   :: flag
     end subroutine hpct_input_fread_double_vec

     subroutine hpct_input_fread_double_ivec(var,value,elem,flag)
       character :: var
       real*8    :: value
       integer   :: elem
       integer   :: flag
     end subroutine hpct_input_fread_double_ivec

     subroutine hpct_input_fread_char(var,value,flag)
       character :: var
       character :: value
       integer   :: flag
     end subroutine hpct_input_fread_char

     subroutine hpct_input_fread_char_ivec(var,value,elem,flag)
       character :: var
       character :: value
       integer   :: elem
       integer   :: flag
     end subroutine hpct_input_fread_char_ivec

     subroutine hpct_input_toggle_messages(flag)
       integer   :: flag
     end subroutine hpct_input_toggle_messages

     ! ---------------------------------
     ! Default Input Value Registrations
     ! ---------------------------------

     subroutine hpct_input_register_int(var,value,flag)
       character :: var
       integer   :: value
       integer   :: flag
     end subroutine hpct_input_register_int

     subroutine hpct_input_register_float(var,value,flag)
       character :: var
       real      :: value
       integer   :: flag
     end subroutine hpct_input_register_float

     subroutine hpct_input_register_double(var,value,flag)
       character :: var
       real*8    :: value
       integer   :: flag
     end subroutine hpct_input_register_double

     subroutine hpct_input_register_char(var,value,flag)
       character :: var
       character :: value
       integer   :: flag
     end subroutine hpct_input_register_char

     ! ---------------
     ! Timing Routines
     ! ---------------

     subroutine hpct_asci_time(timestring)
       character :: timestring
     end subroutine hpct_asci_time

     subroutine hpct_timer_init(id)
       character :: id
     end subroutine hpct_timer_init

     subroutine hpct_timer_reset()
     end subroutine hpct_timer_reset

     subroutine hpct_timer_finalize()
     end subroutine hpct_timer_finalize

     subroutine hpct_timer_summarize()
     end subroutine hpct_timer_summarize

     subroutine hpct_timer_begin(id)
       character :: id
     end subroutine hpct_timer_begin

     subroutine hpct_timer_end(id)
       character :: id
     end subroutine hpct_timer_end

     subroutine hpct_timer(value)
       real*8 :: value
     end subroutine hpct_timer

     subroutine hpct_timer_elapsed_global(value)
       real*8 :: value
     end subroutine hpct_timer_elapsed_global

     ! ---------------
     ! Math
     ! ---------------

     integer function hpct_double_isnan(value)
       real*8 :: value
     end function hpct_double_isnan

     integer function hpct_double_isinf(value)
       real*8 :: value
     end function hpct_double_isinf

     ! ---------------
     ! Logging
     ! ---------------

     subroutine hpct_log_setlevel(priority)
       integer :: priority
     end subroutine hpct_log_setlevel

     subroutine hpct_log(loglevel,mesg)
       integer   :: loglevel
       character :: mesg
     end subroutine hpct_log

     ! ---------------
     ! Miscellaneous
     ! ---------------

     subroutine hpct_check_file_path(pathname,flag)
       character :: pathname
       integer   :: flag
     end subroutine hpct_check_file_path

     subroutine hpct_create_unique_dir(name_template,flag)
       character :: name_template
       integer   :: flag
     end subroutine hpct_create_unique_dir

     subroutine hpct_create_scratch_dir(name_template,flag)
       character :: name_template
       integer   :: flag
     end subroutine hpct_create_scratch_dir

  end interface

  ! ---------------------------
  ! Logging Priority Constants
  ! ---------------------------

  integer, parameter :: HPCT_NOLOG =  -1
  integer, parameter :: HPCT_FATAL =   0
  integer, parameter :: HPCT_ERROR = 100
  integer, parameter :: HPCT_WARN  = 200
  integer, parameter :: HPCT_INFO  = 300
  integer, parameter :: HPCT_DEBUG = 400
  integer, parameter :: HPCT_ALL   = 500

end module hpct
