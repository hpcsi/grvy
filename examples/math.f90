program test_isnan_isinf

  use hpct

  implicit none

  real*8 :: should_be_nan 
  real*8 :: should_be_inf

  real*8 :: m_one, one, zero

  integer :: is_nan, is_inf

  !------------------------------------------------
  ! Simple C example to illustre basic input 
  ! file parsing via HPCT.
  !
  ! $Id:  $
  !------------------------------------------------
  
  m_one = -1.d0
  one   =  1.d0
  zero  =  0.d0

  should_be_nan = sqrt(m_one)
  should_be_inf = one/zero

  call hpct_double_isnan( should_be_nan, is_nan )
  if( is_nan .eq. 1 )then
     write(*,*) "NaN detected"
  endif

  call hpct_double_isinf( should_be_inf, is_inf )
  if( is_inf .eq. 1 )then
     write(*,*) "Inf detected"
  endif

  call hpct_double_isnan( 5.d0, is_nan )
  if( is_nan .eq. 1 )then
     write(*,*) "NaN detected"
  endif

  call hpct_double_isinf( 1.d0, is_inf )
  if( is_inf .eq. 1 )then
     write(*,*) "Inf detected"
  endif

  return

end program test_isnan_isinf
