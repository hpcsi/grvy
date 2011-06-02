
program main
  use grvy
  implicit none

  integer,parameter   :: blocksize = 8192;
  real*8              :: data(blocksize)
  integer             :: num_records = 800

  integer             :: i
  integer             :: ierr
  integer*8           :: elem
  integer             :: verify_data

  call grvy_ocore_init("myfile",blocksize,ierr)

  if(ierr .ne. 0)then
     print*,'Unable to initialize GRVY Ocore'
     stop
  endif
  
   if(grvy_ocore_master() .eq. 1)then

      ! Store some known data

      do elem = 1,num_records
         i = elem
         call init_data(i,blocksize,data)
         call grvy_ocore_write(elem,data,ierr)
      enddo

      ! Change data locally

      do i = 1,num_records
         call zero_data(i,blocksize,data)
      enddo

      ! Read back known data in opposite order and verify

      do elem = num_records,1,-1
         call grvy_ocore_read(elem,data,ierr);
         if( verify_data(elem,blocksize,data) .ne. 1)then
            stop
         endif
      enddo

   endif

  call grvy_ocore_finalize();

  stop
end program main
  

subroutine init_data(offset,size,data)
  implicit none
  integer, intent(in)  :: offset
  integer, intent(in)  :: size
  real*8,  intent(out) :: data(*)

  integer :: i

  do i = 1,size
     data(i) = offset*i
  enddo

  return
end subroutine init_data

function verify_data(offset,size,data)
  implicit none

  integer verify_data

  integer, intent(in)  :: offset
  integer, intent(in)  :: size
  real*8,  intent(out) :: data(size)

  integer :: i

  do i = 1,size
     if( abs(data(i) - 1.0d0*offset*i) .gt.  1.0d-15)then
        print*,'Verification error: expecting ',1.0d0*offset*i,' but found ',data(i),' for index ',i
        verify_data = 0
        return 
     endif
  enddo

  verify_data = 1
  return
end function verify_data

subroutine zero_data(offset, size, data)
  implicit none

  integer, intent(in)  :: offset
  integer, intent(in)  :: size
  real*8,  intent(out) :: data(size)

  integer :: i

  do i =1,size
     data(i) = 0.0d0
  enddo

  return
end subroutine zero_data
