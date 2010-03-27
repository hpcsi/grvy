!--------------------------------------------------------------------------
! -------------------------------------------------------------------------
!
! Copyright (C) 2008,2009,2010 The PECOS Development Team
!
! Please see http:!pecos.ices.utexas.edu for more information.
!
! This file is part of the PECOS GRVY Toolkit
!
! GRVY is free software: you can redistribute it and/or modify
! it under the terms of the GNU General Public License as published by
! the Free Software Foundation, either version 3 of the License, or
! (at your option) any later version.
!
! GRVY is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU General Public License for more details.
!
! You should have received a copy of the GNU General Public License
! along with GRVY.  If not, see <http:!www.gnu.org/licenses/>.
!
! -------------------------------------------------------------------------
!
! grvy.f90: Fortran module interface definition for GRVY routines.
!
! $Id$
! -------------------------------------------------------------------------
! -------------------------------------------------------------------------
module grvy
  interface

     ! ----------------------
     ! Input Parsing Support
     ! ----------------------
     
     subroutine grvy_input_fopen(filename,flag)
       character :: filename
       integer   :: flag
     end subroutine grvy_input_fopen

     subroutine grvy_input_fclose()
     end subroutine grvy_input_fclose

     subroutine grvy_input_fdump(flag)
       integer   :: flag
     end subroutine grvy_input_fdump

     subroutine grvy_input_fdump_delim(prefix,flag)
       character :: prefix
       integer   :: flag
     end subroutine grvy_input_fdump_delim

     subroutine grvy_input_fdump_file(prefix,filename,flag)
       character :: prefix
       character :: filename
       integer   :: flag
     end subroutine grvy_input_fdump_file

     subroutine grvy_input_fread_real(var,value,flag)
       character :: var
       real      :: value
       integer   :: flag
     end subroutine grvy_input_fread_real

     subroutine grvy_input_fread_double(var,value,flag)
       character :: var
       real*8    :: value
       integer   :: flag
     end subroutine grvy_input_fread_double

     subroutine grvy_input_fread_int(var,value,flag)
       character :: var
       integer   :: value
       integer   :: flag
     end subroutine grvy_input_fread_int

     subroutine grvy_input_fread_int_vec(var,value,nelems,flag)
       character :: var
       integer   :: value(*)
       integer   :: nelems
       integer   :: flag
     end subroutine grvy_input_fread_int_vec

     subroutine grvy_input_fread_int_ivec(var,value,elem,flag)
       character :: var
       integer   :: value
       integer   :: elem
       integer   :: flag
     end subroutine grvy_input_fread_int_ivec

     subroutine grvy_input_fread_real_vec(var,value,nelems,flag)
       character :: var
       real      :: value(*)
       integer   :: nelems
       integer   :: flag
     end subroutine grvy_input_fread_real_vec

     subroutine grvy_input_fread_real_ivec(var,value,elem,flag)
       character :: var
       real      :: value
       integer   :: elem
       integer   :: flag
     end subroutine grvy_input_fread_real_ivec

     subroutine grvy_input_fread_double_vec(var,value,nelems,flag)
       character :: var
       real*8    :: value(*)
       integer   :: nelems
       integer   :: flag
     end subroutine grvy_input_fread_double_vec

     subroutine grvy_input_fread_double_ivec(var,value,elem,flag)
       character :: var
       real*8    :: value
       integer   :: elem
       integer   :: flag
     end subroutine grvy_input_fread_double_ivec

     subroutine grvy_input_fread_char(var,value,flag)
       character :: var
       character :: value
       integer   :: flag
     end subroutine grvy_input_fread_char

     subroutine grvy_input_fread_char_ivec(var,value,elem,flag)
       character :: var
       character :: value
       integer   :: elem
       integer   :: flag
     end subroutine grvy_input_fread_char_ivec

     subroutine grvy_input_toggle_messages(flag)
       integer   :: flag
     end subroutine grvy_input_toggle_messages

     ! ---------------------------------
     ! Default Input Value Registrations
     ! ---------------------------------

     subroutine grvy_input_register_int(var,value,flag)
       character :: var
       integer   :: value
       integer   :: flag
     end subroutine grvy_input_register_int

     subroutine grvy_input_register_float(var,value,flag)
       character :: var
       real      :: value
       integer   :: flag
     end subroutine grvy_input_register_float

     subroutine grvy_input_register_double(var,value,flag)
       character :: var
       real*8    :: value
       integer   :: flag
     end subroutine grvy_input_register_double

     subroutine grvy_input_register_char(var,value,flag)
       character :: var
       character :: value
       integer   :: flag
     end subroutine grvy_input_register_char

     ! ---------------
     ! Timing Routines
     ! ---------------

     subroutine grvy_asci_time(timestring)
       character :: timestring
     end subroutine grvy_asci_time

     subroutine grvy_timer_init(id)
       character :: id
     end subroutine grvy_timer_init

     subroutine grvy_timer_reset()
     end subroutine grvy_timer_reset

     subroutine grvy_timer_finalize()
     end subroutine grvy_timer_finalize

     subroutine grvy_timer_summarize()
     end subroutine grvy_timer_summarize

     subroutine grvy_timer_begin(id)
       character :: id
     end subroutine grvy_timer_begin

     subroutine grvy_timer_end(id)
       character :: id
     end subroutine grvy_timer_end

     subroutine grvy_timer(value)
       real*8 :: value
     end subroutine grvy_timer

     subroutine grvy_timer_elapsed_global(value)
       real*8 :: value
     end subroutine grvy_timer_elapsed_global

     ! ---------------
     ! Math
     ! ---------------

     integer function grvy_double_isnan(value)
       real*8 :: value
     end function grvy_double_isnan

     integer function grvy_double_isinf(value)
       real*8 :: value
     end function grvy_double_isinf

     ! ---------------
     ! Logging
     ! ---------------

     subroutine grvy_log_setlevel(priority)
       integer :: priority
     end subroutine grvy_log_setlevel

     subroutine grvy_log(loglevel,mesg)
       integer   :: loglevel
       character :: mesg
     end subroutine grvy_log

     ! ---------------
     ! Miscellaneous
     ! ---------------

     subroutine grvy_check_file_path(pathname,flag)
       character :: pathname
       integer   :: flag
     end subroutine grvy_check_file_path

     subroutine grvy_create_unique_dir(name_template,flag)
       character :: name_template
       integer   :: flag
     end subroutine grvy_create_unique_dir

     subroutine grvy_create_scratch_dir(name_template,flag)
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

end module grvy
