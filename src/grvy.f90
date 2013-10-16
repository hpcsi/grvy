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
!! grvy.f90: Fortran module interface definition for GRVY routines.
!!
!! $Id$
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

!> \file

!> @defgroup Fapi Fortran API
!!
!! \brief This page provides a link to all of the Fortran functions which
!! are available in libGRVY (grouped by functionality). 
!! Library access is provided thru a Fortran90 module
!! which requires the external calling application to include the
!! following within the Fortran90 source code:
!!
!! \code
!! use grvy
!! \endcode
!!
!! Unless specified otherwise, functions which have an integer return
!! value return "0" upon success (and the primary exception to this
!! rule is with a subset of the Input Parsing routines).
!!
!! Note: the F90 auto-documentation is currently undergoing a
!! refactoriztion. The majority of the interface is provided using the
!! iso_c_binding capability in the newer Fortran standerd and thus,
!! the API is similar to the C API.
!! 
!! During the transition, please consult the Fortran examples provided in
!! the top-level \e examples directory. For convenience, several
!! of these examples are accessible below:
!!
!! \li \ref F90inputExample "inputf.f90" - input parsing example
!! \li \ref F90timerExample "timerf.f90" - performance timing example
!! \li \ref F90mathExample  "math.f90"   - floating point test example

!
! Click on one of the links to see more details on the routines
! available for each utility. 
! 
! \li \ref Fversioning
!
! Alternatively, if you prefer to see all
! of the available routines as part of the Fortran API on a single
! page, refer to the F90 interface definitions in grvy.f90 directly.

module grvy

  interface

     ! -------------------
     ! Versioning routines
     ! -------------------

     !> 
     !! @defgroup Fversioning Runtime Versioning Information
     !! @ingroup Fapi
     !! 
     !! \brief Obtain libGRVY runtime versioning information.

     !>
     !! \ingroup Fversioning 
     !! @{
     
     !> \brief Echo library versioning and configuration information to stdout222

     subroutine grvy_version_stdout() bind (C)
       use iso_c_binding
       implicit none
     end subroutine grvy_version_stdout

     !> \brief Obtain numeric library version 

     ! \return running version

     integer (C_int) function grvy_get_numeric_version() bind (C)
       use iso_c_binding
       implicit none
     end function grvy_get_numeric_version

     !> @}

     ! ----------------------
     ! Input Parsing Support
     ! ----------------------

     !>
     !! \defgroup Finput Input Parsing
     !! @ingroup Fapi
     !! 
     !! \brief Routines for accessing input parameters from a libGRVY
     !! style input file. See the \ref inputFile "example input file" for
     !! more information on the file format.

     !>
     !! \ingroup Finput
     !! \name Input Parsing - File open/close:
     !! 
     !! Routines for accessing input parameters from a libGRVY style
     !! input file. Note that an input file must be open prior to reading
     !! any keyword/value pairs or performing input file dump
     !! operations. See the \ref inputFile "example input file" for more
     !! information on the file format.

     !>
     !! \ingroup Finput
     !! @{ 
     !! 
     
     !! \page apiF
     !! \section input Input Parsing Routines
     !! <b>Open/close a libGRVY style input file:</b>
     !! \code
     !! subroutine grvy_input_fopen()
     !! subroutine grvy_input_fclose()
     !! \endcode

     !>
     !! \brief Open a libGRVY style input file
     !! \param filename desired input filename

     integer (C_int) function grvy_input_fopen_passthrough(filename) bind (C,name='grvy_input_fopen')
       use iso_c_binding
       implicit none

       character (C_char),intent(in) :: filename(*) !< libGRVY style input filename
     end function grvy_input_fopen_passthrough

     !> \brief Close the currently open libGRVY style input file

     subroutine grvy_input_fclose() bind (C)
       use iso_c_binding
       implicit none
     end subroutine grvy_input_fclose

     !> @} 

     !! \page apiF
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

     integer (C_int) function grvy_input_fread_logical_passthrough(var,value,default_value) &
          bind (C,name='grvy_input_fread_logical_from_int')
       use iso_c_binding
       implicit none
       character (C_char),intent(in)         :: var(*)
       integer   (C_int), intent(out)        :: value
       integer   (C_int), value, intent(in)  :: default_value
     end function grvy_input_fread_logical_passthrough

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

     real (C_double) function grvy_timer_elapsedseconds_passthrough(id) bind (C,name='grvy_timer_elapsedseconds')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end function grvy_timer_elapsedseconds_passthrough

     real (C_double) function grvy_timer_elapsedseconds_inc_passthrough(id) bind (C,name='grvy_timer_elapsedseconds_inc')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end function grvy_timer_elapsedseconds_inc_passthrough

     subroutine grvy_timer_begin_passthrough(id) bind (C,name='grvy_timer_begin')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end subroutine grvy_timer_begin_passthrough

     subroutine grvy_timer_end_passthrough(id) bind (C,name='grvy_timer_end')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end subroutine grvy_timer_end_passthrough

     subroutine grvy_timer_init_passthrough(description) bind (C,name='grvy_timer_init')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: description(*) !< description
     end subroutine grvy_timer_init_passthrough

     subroutine grvy_timer_finalize() bind (C)
       use iso_c_binding
       implicit none
     end subroutine grvy_timer_finalize

     subroutine grvy_timer_reset() bind (C)
       use iso_c_binding
       implicit none
     end subroutine grvy_timer_reset

     subroutine grvy_timer_summarize() bind (C)
       use iso_c_binding
       implicit none
     end subroutine grvy_timer_summarize

     integer (C_int) function grvy_timer_set_summarize_width_passthrough(value) &
          bind(C,name='grvy_timer_set_summarize_width')
       use iso_c_binding
       implicit none
       integer   (C_int), value, intent(in)  :: value
     end function grvy_timer_set_summarize_width_passthrough

     real (C_double) function grvy_timer() bind(C,name='grvy_timer')
       use iso_c_binding
       implicit none
     end function grvy_timer

     real (C_double) function grvy_timer_elapsed_global() bind(C,name='grvy_timer_elapsed_global')
       use iso_c_binding
       implicit none
     end function grvy_timer_elapsed_global

     subroutine grvy_asci_time(timestring) bind(C,name='grvy_asci_time_fortran')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: timestring(*) 
     end subroutine grvy_asci_time

     integer (C_int) function grvy_timer_stats_count_passthrough(id) bind(C,name='grvy_timer_stats_count')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end function grvy_timer_stats_count_passthrough

     real (C_double) function grvy_timer_stats_mean_passthrough(id) bind(C,name='grvy_timer_stats_mean')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end function grvy_timer_stats_mean_passthrough

     real (C_double) function grvy_timer_stats_variance_passthrough(id) bind(C,name='grvy_timer_stats_variance')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end function grvy_timer_stats_variance_passthrough

     real (C_double) function grvy_timer_stats_min_passthrough(id) bind(C,name='grvy_timer_stats_min')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end function grvy_timer_stats_min_passthrough

     real (C_double) function grvy_timer_stats_max_passthrough(id) bind(C,name='grvy_timer_stats_max')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end function grvy_timer_stats_max_passthrough

     real (C_double) function grvy_timer_stats_mean_inc_passthrough(id) bind(C,name='grvy_timer_stats_mean_inc')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end function grvy_timer_stats_mean_inc_passthrough

     real (C_double) function grvy_timer_stats_variance_inc_passthrough(id) bind(C,name='grvy_timer_stats_variance_inc')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end function grvy_timer_stats_variance_inc_passthrough

     real (C_double) function grvy_timer_stats_min_inc_passthrough(id) bind(C,name='grvy_timer_stats_min_inc')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end function grvy_timer_stats_min_inc_passthrough

     real (C_double) function grvy_timer_stats_max_inc_passthrough(id) bind(C,name='grvy_timer_stats_max_inc')
       use iso_c_binding
       implicit none
       character (C_char),intent(in) :: id(*) !< timer name
     end function grvy_timer_stats_max_inc_passthrough

     subroutine  grvy_timer_save_hist_passthrough(experiment,comment,num_procs, &
                                                  filename) bind (C,name='grvy_timer_save_hist')
       use iso_c_binding
       implicit none
       character(C_char),  intent(in)         :: experiment(*)
       character(C_char),  intent(in)         :: comment(*)
       integer  (C_int),   intent(in), value  :: num_procs
       character(C_char),  intent(in)         :: filename(*)
     end subroutine grvy_timer_save_hist_passthrough

     subroutine  grvy_timer_save_hist_passthrough_exp(experiment,comment,num_procs, jobId, code_revision, &
                                                      flops,filename) bind (C,name='grvy_timer_save_hist_exp')
       use iso_c_binding
       implicit none
       character(C_char),  intent(in)         :: experiment(*)
       character(C_char),  intent(in)         :: comment(*)
       integer  (C_int),   intent(in), value  :: num_procs
       integer  (C_int),   intent(in), value  :: jobId
       character(C_char),  intent(in)         :: code_revision(*)
       real     (C_double),intent(in), value  :: flops
       character(C_char),  intent(in)         :: filename(*)
     end subroutine grvy_timer_save_hist_passthrough_exp

     ! -------------------
     ! MPI_Ocore Routines
     ! -------------------

!!!     integer (C_int) function grvy_ocore_init_passthrough(filename,blocksize) bind (C,name='grvy_ocore_init')
     integer (C_int) function grvy_ocore_init_passthrough(filename,num_ocore_tasks,COMM_GLOBAL) &
          bind (C,name='grvy_ocore_init_fortran')
       use iso_c_binding
       implicit none

       character (C_char),intent(in)        :: filename(*)     !< Ocore input filename (parsed using libGRVY)
       integer   (C_int), intent(in),value  :: num_ocore_tasks !< Number of Ocore tasks
       integer   (C_int), intent(in),value  :: COMM_GLOBAL     !< Global communicator
!!!       integer   (C_int), intent(in),value  :: blocksize   !< Number of elements per each record block

     end function grvy_ocore_init_passthrough

     integer (C_int) function grvy_ocore_read_real8(record_id,data) bind (C,name='grvy_ocore_read_double')
       use iso_c_binding
       implicit none

       integer  (C_SIZE_T),intent(in),value :: record_id !< Record identifier
       real     (C_double),intent(out)      :: data(*)   !< Block of data to store (number of elements = blocksize)

     end function grvy_ocore_read_real8

     integer (C_int) function grvy_ocore_read_real4(record_id,data) bind (C,name='grvy_ocore_read_float')
       use iso_c_binding
       implicit none

       integer  (C_SIZE_T),intent(in),value   :: record_id !< Record identifier
       real     (C_float), intent(out)        :: data(*)   !< Block of data to store (number of elements = blocksize)

     end function grvy_ocore_read_real4

     integer (C_int) function grvy_ocore_read_int8(record_id,data) bind (C,name='grvy_ocore_read_int64')
       use iso_c_binding
       implicit none

       integer  (C_SIZE_T),   intent(in),value :: record_id !< Record identifier
       integer  (C_LONG_LONG),intent(out)      :: data(*)   !< Block of data to store (number of elements = blocksize)

     end function grvy_ocore_read_int8

     integer (C_int) function grvy_ocore_read_int4(record_id,data) bind (C,name='grvy_ocore_read_int')
       use iso_c_binding
       implicit none

       integer  (C_SIZE_T),   intent(in),value :: record_id !< Record identifier
       integer  (C_INT),      intent(out)      :: data(*)   !< Block of data to store (number of elements = blocksize)

     end function grvy_ocore_read_int4

     integer (C_int) function grvy_ocore_write_real8(record_id,data) bind (C,name='grvy_ocore_write_double')
       use iso_c_binding
       implicit none

       integer  (C_SIZE_T),intent(in),value :: record_id !< Record identifier
       real     (C_DOUBLE),intent(out)      :: data(*)   !< Block of data to store (number of elements = blocksize)

     end function grvy_ocore_write_real8

     integer (C_int) function grvy_ocore_write_real4(record_id,data) bind (C,name='grvy_ocore_write_float')
       use iso_c_binding
       implicit none

       integer  (C_SIZE_T),intent(in),value :: record_id !< Record identifier
       real     (C_FLOAT), intent(out)      :: data(*)   !< Block of data to store (number of elements = blocksize)

     end function grvy_ocore_write_real4

     integer (C_int) function grvy_ocore_write_int4(record_id,data) bind (C,name='grvy_ocore_write_int')
       use iso_c_binding
       implicit none

       integer  (C_SIZE_T),intent(in),value :: record_id !< Record identifier
       integer  (C_INT),   intent(out)      :: data(*)   !< Block of data to store (number of elements = blocksize)

     end function grvy_ocore_write_int4

     integer (C_int) function grvy_ocore_write_int8(record_id,data) bind (C,name='grvy_ocore_write_int64')
       use iso_c_binding
       implicit none

       integer  (C_SIZE_T),   intent(in),value :: record_id !< Record identifier
       integer  (C_LONG_LONG),intent(inout)    :: data(*)   !< Block of data to store (number of elements = blocksize)

     end function grvy_ocore_write_int8

     integer (C_int) function grvy_ocore_master() bind (C)
       use iso_c_binding
       implicit none

     end function grvy_ocore_master

     integer (C_int) function grvy_ocore_enabled() bind (C)
       use iso_c_binding
       implicit none

     end function grvy_ocore_enabled

     subroutine grvy_ocore_finalize() bind (C)
       use iso_c_binding
       implicit none
     end subroutine grvy_ocore_finalize

     integer (C_SIZE_T) function grvy_ocore_num_active() bind (C)
       use iso_c_binding
       implicit none
     end function grvy_ocore_num_active

     integer (C_SIZE_T) function grvy_ocore_pop_record_real4(data) bind (C,name='grvy_ocore_pop_record_float')
       use iso_c_binding
       implicit none
       real  (C_FLOAT),  intent(inout)        :: data(*)   !< Block of data to return
     end function grvy_ocore_pop_record_real4

     integer (C_SIZE_T) function grvy_ocore_pop_record_real8(data) bind (C,name='grvy_ocore_pop_record_double')
       use iso_c_binding
       implicit none
       real  (C_DOUBLE),  intent(inout)       :: data(*)   !< Block of data to return
     end function grvy_ocore_pop_record_real8

     integer (C_SIZE_T) function grvy_ocore_pop_record_int8(data) bind (C,name='grvy_ocore_pop_record_int64')
       use iso_c_binding
       implicit none
       integer  (C_LONG_LONG),  intent(inout) :: data(*)   !< Block of data to return
     end function grvy_ocore_pop_record_int8

     integer (C_SIZE_T) function grvy_ocore_pop_record_int4(data) bind (C,name='grvy_ocore_int')
       use iso_c_binding
       implicit none
       integer  (C_LONG_LONG),  intent(inout) :: data(*)   !< Block of data to return
     end function grvy_ocore_pop_record_int4

     ! -------------------
     ! File I/O Utilities
     ! -------------------

     integer (C_int) function grvy_copy_dir_passthrough(from_dir,to_dir) bind (C,name='grvy_copy_dir')
       use iso_c_binding
       implicit none

       character (C_char),intent(in)        :: from_dir(*) 
       character (C_char),intent(in)        :: to_dir(*)   
     end function grvy_copy_dir_passthrough

     integer (C_int) function grvy_check_file_path_passthrough(pathname) bind (C,name='grvy_check_file_path')
       use iso_c_binding
       implicit none
       character (C_char),intent(in)        :: pathname(*) 
     end function grvy_check_file_path_passthrough

     integer (C_int) function grvy_create_unique_dir_passthrough(name_template) bind (C,name='grvy_create_unique_dir')
       use iso_c_binding
       implicit none
       character (C_char),intent(in)     :: name_template(*) 
     end function grvy_create_unique_dir_passthrough

     integer (C_int) function grvy_create_scratch_dir_passthrough(name_template) bind (C,name='grvy_create_scratch_dir')
       use iso_c_binding
       implicit none
       character (C_char),intent(in)     :: name_template(*) 
     end function grvy_create_scratch_dir_passthrough

     ! ---------------
     ! Math Utilities
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

!!!     subroutine grvy_create_unique_dir(name_template,flag)
!!!       implicit none
!!!       character :: name_template
!!!       integer   :: flag
!!!     end subroutine grvy_create_unique_dir
!!!
!!!     subroutine grvy_create_scratch_dir(name_template,flag)
!!!       implicit none
!!!       character :: name_template
!!!       integer   :: flag
!!!     end subroutine grvy_create_scratch_dir

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
! order betwixt C/Fortran. Kudos to the iso_c_binding folks.
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

  subroutine grvy_input_fread_logical(var,value,default_value,return_flag)
   use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: var           !< variable keyword
    logical,            intent(out)     :: value         !< keyword logical value from input
    logical,            intent(in)      :: default_value !< default value (if not specified in input file)
    integer  (C_int),   intent(out)     :: return_flag   !< error return flag

    ! Local

    integer  (C_int)                    :: value_int       !< keyword value from input
    integer  (C_int)                    :: value_def_int   !< keyword value from input

    if(default_value)then
       value_def_int = 1
    else
       value_def_int = 0
    endif

    return_flag = grvy_input_fread_logical_passthrough(var//C_NULL_CHAR,value_int,value_def_int)
    
    if(value_int .eq. 1) then
       value = .true.
    else
       value = .false.
    endif

    return
  end subroutine grvy_input_fread_logical

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

  ! ----------------
  ! Timer wrappers
  ! ----------------

  real*8 function grvy_timer_elapsedseconds(id) 
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    grvy_timer_elapsedseconds = grvy_timer_elapsedseconds_passthrough(id//C_NULL_CHAR)
    return
  end function grvy_timer_elapsedseconds

  real*8 function grvy_timer_elapsedseconds_inc(id) 
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    grvy_timer_elapsedseconds_inc = grvy_timer_elapsedseconds_inc_passthrough(id//C_NULL_CHAR)
    return
  end function grvy_timer_elapsedseconds_inc

  subroutine grvy_timer_begin(id) 
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    call grvy_timer_begin_passthrough(id//C_NULL_CHAR)
    return
  end subroutine grvy_timer_begin
  
  subroutine grvy_timer_end(id) 
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    call grvy_timer_end_passthrough(id//C_NULL_CHAR)
    return
  end subroutine grvy_timer_end
  
  subroutine grvy_timer_init(description) 
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: description !< description
    
    call grvy_timer_init_passthrough(description//C_NULL_CHAR)
    return
  end subroutine grvy_timer_init
  
  integer function grvy_timer_stats_count(id)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    grvy_timer_stats_count = grvy_timer_stats_count_passthrough(id//C_NULL_CHAR)
  end function grvy_timer_stats_count
  
  real*8 function grvy_timer_stats_mean(id)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    grvy_timer_stats_mean = grvy_timer_stats_mean_passthrough(id//C_NULL_CHAR)
    return
  end function grvy_timer_stats_mean
  
  real*8 function grvy_timer_stats_variance(id)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    grvy_timer_stats_variance = grvy_timer_stats_variance_passthrough(id//C_NULL_CHAR)
    return
  end function grvy_timer_stats_variance

  real*8 function grvy_timer_stats_min(id)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    grvy_timer_stats_min = grvy_timer_stats_min_passthrough(id//C_NULL_CHAR)
    return
  end function grvy_timer_stats_min

  real*8 function grvy_timer_stats_max(id)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    grvy_timer_stats_max = grvy_timer_stats_max_passthrough(id//C_NULL_CHAR)
    return
  end function grvy_timer_stats_max

  real*8 function grvy_timer_stats_mean_inc(id)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    grvy_timer_stats_mean_inc = grvy_timer_stats_mean_inc_passthrough(id//C_NULL_CHAR)
    return
  end function grvy_timer_stats_mean_inc
  
  real*8 function grvy_timer_stats_variance_inc(id)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    grvy_timer_stats_variance_inc = grvy_timer_stats_variance_inc_passthrough(id//C_NULL_CHAR)
    return
  end function grvy_timer_stats_variance_inc

  real*8 function grvy_timer_stats_min_inc(id)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    grvy_timer_stats_min_inc = grvy_timer_stats_min_inc_passthrough(id//C_NULL_CHAR)
    return
  end function grvy_timer_stats_min_inc

  real*8 function grvy_timer_stats_max_inc(id)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: id         !< timer keyword
    
    grvy_timer_stats_max_inc = grvy_timer_stats_max_inc_passthrough(id//C_NULL_CHAR)
    return
  end function grvy_timer_stats_max_inc

  subroutine grvy_timer_save_hist(experiment,comment,num_procs,filename)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: experiment
    character(len=*),   intent(in)      :: comment
    integer  (C_int),   intent(in)      :: num_procs
    character(len=*),   intent(in)      :: filename
    
    call grvy_timer_save_hist_passthrough(experiment//C_NULL_CHAR,comment//C_NULL_CHAR, &
         num_procs,filename//C_NULL_CHAR)
    return
  end subroutine grvy_timer_save_hist

  subroutine grvy_timer_save_hist_exp(experiment,comment,num_procs,jobId,code_revision,flops,filename)
    use iso_c_binding
    implicit none
    character(len=*),   intent(in)      :: experiment
    character(len=*),   intent(in)      :: comment
    integer  (C_int),   intent(in)      :: num_procs
    integer  (C_int),   intent(in)      :: jobId
    character(len=*),   intent(in)      :: code_revision
    real     (C_double),intent(in)      :: flops
    character(len=*),   intent(in)      :: filename
    
    call grvy_timer_save_hist_passthrough_exp(experiment//C_NULL_CHAR,comment//C_NULL_CHAR, &
         num_procs,jobId,code_revision//C_NULL_CHAR,flops,filename//C_NULL_CHAR)
    return
  end subroutine grvy_timer_save_hist_exp

  subroutine grvy_timer_set_summarize_width(value, return_flag)
    use iso_c_binding
    implicit none
    integer  (C_int), value, intent(in)  :: value
    integer  (C_int), intent(inout)      :: return_flag
    
    return_flag = grvy_timer_set_summarize_width_passthrough(value)
  end subroutine grvy_timer_set_summarize_width

  ! ----------------
  ! I/O wrappers
  ! ----------------

  !>
  ! \brief Recursively copies contents of existing directory
  ! (\a from_dir) to a new directory (\a to_dir).  If \a to_dir does not
  ! exist, it will be created; otherwise it must be empty.
  ! 
  ! \param[in] from_dir
  ! \param[in] to_dir
  !

  subroutine grvy_copy_dir(from_dir,to_dir,return_flag)
    use iso_c_binding
    implicit none
    character(len=*),intent(in)         :: from_dir 
    character(len=*),intent(in)         :: to_dir 
    integer  (C_int),intent(inout)      :: return_flag
    
    return_flag = grvy_copy_dir_passthrough(from_dir//C_NULL_CHAR,to_dir//C_NULL_CHAR)
    return
  end subroutine grvy_copy_dir

  ! ----------------
  ! Misc. wrappers
  ! ----------------

  subroutine grvy_check_file_path(pathname,return_flag) 
    use iso_c_binding
    implicit none
    character(len=*),intent(in)         :: pathname
    integer  (C_int),intent(inout)      :: return_flag
    
    return_flag = grvy_check_file_path_passthrough(pathname//C_NULL_CHAR)
  end subroutine grvy_check_file_path

  subroutine grvy_create_unique_dir(name_template,return_flag) 
    use iso_c_binding
    implicit none
    character(len=*),intent(inout)      :: name_template
    integer  (C_int),intent(inout)      :: return_flag

    ! Local vars
    
    integer   :: string_length
    character :: tmp_string*1024

    !!!return_flag = grvy_create_unique_dir_passthrough(name_template//C_NULL_CHAR)
    
    string_length = len(trim(name_template))
    
    if(string_length .ge. 1024)then
       print*,'Error: unable to create scratch_dir for templates >= 1024 characters'
       return_flag = 1
       return
    endif
    
    ! Save a copy of incoming string and append C null character

    write(tmp_string,'(a)') name_template//C_NULL_CHAR

    ! Call C version

    return_flag = grvy_create_unique_dir_passthrough(tmp_string)

    ! Update name_template (without C null)

    write(name_template,'(a)') tmp_string(1:string_length)

  end subroutine grvy_create_unique_dir

  subroutine grvy_create_scratch_dir(name_template,return_flag) 
    use iso_c_binding
    implicit none
    character(len=*),intent(inout)      :: name_template
    integer  (C_int),intent(inout)      :: return_flag

!!!    return_flag = grvy_create_unique_dir_passthrough(name_template//C_NULL_CHAR)

    ! Local vars
    
    integer   :: string_length
    character :: tmp_string*1024
    
    string_length = len(trim(name_template))
    
    if(string_length .ge. 1024)then
       print*,'Error: unable to create scratch_dir for templates >= 1024 characters'
       return_flag = 1
       return
    endif
    
    ! Save a copy of incoming string and append C null character

    write(tmp_string,'(a)') name_template//C_NULL_CHAR

    ! Call C version

    return_flag = grvy_create_scratch_dir_passthrough(tmp_string)

    ! Update name_template (without C null)

    write(name_template,'(a)') tmp_string(1:string_length)

  end subroutine grvy_create_scratch_dir

  ! ----------------
  ! Ocore wrappers
  ! ----------------
  
  subroutine grvy_ocore_init(filename,num_ocore_tasks,COMM_GLOBAL,return_flag)
    use iso_c_binding
    implicit none

    character(len=*),intent(in)         :: filename
    integer  (C_int),intent(in)         :: num_ocore_tasks
    integer  (C_int),intent(in)         :: COMM_GLOBAL
    integer  (C_int),intent(inout)      :: return_flag

    return_flag = grvy_ocore_init_passthrough(filename//C_NULL_CHAR,num_ocore_tasks,COMM_GLOBAL)

    return
  end subroutine grvy_ocore_init
  
end module grvy
