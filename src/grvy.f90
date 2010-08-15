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
     
     ! ----------------------
     ! Input Parsing Support
     ! ----------------------
     
     !> \page apiF 
     !! \section input Input Parsing Routines
     !! <b>Open/close a libGRVY style input file:</b>
     !! \code
     !! subroutine grvy_input_fopen
     !! subroutine grvy_input_fclose
     !! \endcode
     
     subroutine grvy_input_fopen(filename,flag)
       character :: filename    !< libGRVY style input filename
       integer   :: flag        !< return flag
     end subroutine grvy_input_fopen

     subroutine grvy_input_fclose()
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
     
     subroutine grvy_input_fdump(flag)
       integer   :: flag        !< return flag
     end subroutine grvy_input_fdump
     
     subroutine grvy_input_fdump_delim(prefix,flag)
       character :: prefix      !< delimiter
       integer   :: flag        !< return flag
     end subroutine grvy_input_fdump_delim
     
     subroutine grvy_input_fdump_file(prefix,filename,flag)
       character :: prefix      !< delimiter
       character :: filename    !< libGRVY style input filename
       integer   :: flag        !< return flag
     end subroutine grvy_input_fdump_file

     subroutine grvy_input_fread_real(var,value,flag)
       character :: var
       real      :: value
       integer   :: flag        !< libGRVY return flag
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

     subroutine grvy_input_register_get_int(var,value,flag)
       character :: var
       integer   :: value
       integer   :: flag
     end subroutine grvy_input_register_get_int

     subroutine grvy_input_register_float(var,value,flag)
       character :: var
       real      :: value
       integer   :: flag
     end subroutine grvy_input_register_float

     subroutine grvy_input_register_get_float(var,value,flag)
       character :: var
       real      :: value
       integer   :: flag
     end subroutine grvy_input_register_get_float

     subroutine grvy_input_register_double(var,value,flag)
       character :: var
       real*8    :: value
       integer   :: flag
     end subroutine grvy_input_register_double

     subroutine grvy_input_register_get_double(var,value,flag)
       character :: var
       real*8    :: value
       integer   :: flag
     end subroutine grvy_input_register_get_double

     subroutine grvy_input_register_char(var,value,flag)
       character :: var
       character :: value
       integer   :: flag
     end subroutine grvy_input_register_char

     subroutine grvy_input_register_get_char(var,value,flag)
       character :: var
       character :: value
       integer   :: flag
     end subroutine grvy_input_register_get_char

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

     real*8 function grvy_timer_elapsedseconds(id)
       character :: id
     end function grvy_timer_elapsedseconds

     integer function grvy_timer_stats_count(id)
       character :: id
     end function grvy_timer_stats_count

     real*8 function grvy_timer_stats_mean(id)
       character :: id
     end function grvy_timer_stats_mean

     real*8 function grvy_timer_stats_variance(id)
       character :: id
     end function grvy_timer_stats_variance

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
