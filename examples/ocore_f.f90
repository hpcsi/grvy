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
!! ocore_f.f90: Fortran example illustrating MPI out-of-core usage.
!!
!! $Id$
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

program main
  use grvy
  implicit none
  include 'mpif.h'

  integer,parameter   :: blocksize = 8192; ! number of elements per ocore record
  real*8              :: data(blocksize)   ! individual data record
  integer             :: num_records = 800 ! # of records defined in this example

  integer             :: ierr              ! Ocore return code
  integer*8           :: elem, index
  integer*8           :: pool_size
  integer             :: verify_data 

  call MPI_Init(ierr)

  ! Initialize MPI-based Ocore ramdisk (myfile is a GRVY-style
  ! input file with optional Ocore controls). blocksize is the number
  ! of elements in each record and is constant. 

  !!!call grvy_ocore_init("mpi_ocore.input",blocksize,ierr)
  call grvy_ocore_init("mpi_ocore.input",1,MPI_COMM_WORLD,ierr)

  if(ierr .ne. 0)then
     print*,'Unable to initialize GRVY Ocore'
     stop
  endif

  ! Do some work on the master task - children tasks poll until
  ! read/write requests are made from the master task
  
   if(grvy_ocore_master() .eq. 1)then

      ! Write some known data records to Ocore ramdisk - the
      ! grvy_ocore_write() function distributes data over the
      ! network to children Ocore tasks which cache the data in
      ! memory. Once the memory cache is full, it will (optionally)
      ! dump less-freqently accessed records to a disk-based temporary
      ! file.

      do elem = 1,num_records
         call init_data             (elem,blocksize,data)
         ierr = grvy_ocore_write_real8(elem,data)
      enddo

      ! Change the local data record

      data(:) = 0.0d0
      
      ! Read back known data records in opposite order and verify

      do elem = num_records,1,-1
         ierr = grvy_ocore_read_real8(elem,data);
         if( verify_data(elem,blocksize,data) .ne. 1)then
            stop
         endif
      enddo

      ! Pull all active data from pool

      pool_size = grvy_ocore_num_active()

      do elem=1,pool_size
         index = grvy_ocore_pop_record_real8(data)
         if(verify_data(index,blocksize,data) .ne. 1)then
            stop
         endif
      enddo

   endif

  call grvy_ocore_finalize()
  call MPI_Finalize(ierr)

  stop
end program main

! -------------------------------------------------------
! init_data(): initialize all records to a known state 
!              based on given seed value
! -------------------------------------------------------

subroutine init_data(seed,size,data)
  implicit none
  integer*8, intent(in)  :: seed
  integer,   intent(in)  :: size
  real*8,    intent(out) :: data(size)

  integer :: i

  do i = 1,size
     data(i) = seed*i
  enddo

  return
end subroutine init_data

! -------------------------------------------------------
! verify_data(): verify all records against known state 
!                based on given seed value
! -------------------------------------------------------

function verify_data(seed,size,data)
  implicit none

  integer verify_data

  integer*8, intent(in)    :: seed
  integer,   intent(in)    :: size
  real*8,    intent(inout) :: data(size)

  integer :: i

  do i = 1,size
     if( abs(data(i) - 1.0d0*seed*i) .gt.  1.0d-15)then
        print*,'Verification error: expecting ',1.0d0*seed*i,' but found ',data(i),' for index ',i
        verify_data = 0
        return 
     endif
  enddo

  verify_data = 1

  return
end function verify_data

! -------------------------------------------------------
! zero_data(): initialize all records to zero
! -------------------------------------------------------

subroutine zero_data(size, data)
  implicit none

  integer, intent(in)  :: size
  real*8,  intent(out) :: data(size)

  data(:) = 0.0d0

  return
end subroutine zero_data
