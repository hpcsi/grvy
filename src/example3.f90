

subroutine init_data(offset,size,data)
  implicit none
  integer, intent(in)  :: offset
  integer, intent(in)  :: size
  real*8,  intent(out) :: data(*)

  integer :: i

  do i = 1,size
     data(i) = offset*i
  enddo
end subroutine init_data

integer function verify_data(offset,size,data)
  implicit none

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
end subroutine zero_data

program main
  implicit none

  integer           :: num_local;
  integer,parameter :: blocksize = 8192;
  real*8            :: data(blocksize)
  integer           :: i

!  MPI_Init(&argc,&argv);
!  MPI_Comm_rank(MPI_COMM_WORLD,&num_local);

!  assert( grvy_ocore_init("myfile",blocksize) == 0);

  print*,'hello koomie'


!   if(num_local == 0)
!     {
!       double data[blocksize];

!       const int num_records = 800;

!       // Store some known data

!       for(i=0;i<num_records;i++)
! 	{
! 	  init_data(i,blocksize,data);
! 	  grvy_ocore_write(i,data);
! 	}

!       // Change data locally

!       for(i=0;i<num_records;i++)
! 	zero_data(i,blocksize,data);

!       // Read back known data in opposite order and verify

!       for(i=num_records-1;i>=0;i--)
! 	{
! 	  grvy_ocore_read(i,data);
! 	  assert(verify_data(i,blocksize,data) );
! 	}

!     }

!   grvy_ocore_finalize();
!   MPI_Finalize();

! }
  stop
end program main
  

