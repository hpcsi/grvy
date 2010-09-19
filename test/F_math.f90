!!-----------------------------------------------------------------------bl-
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
!!-----------------------------------------------------------------------el-
!!
!! Regression/Test Suite for libGRVY.
!!
!! $Id$
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

program test_isnan_isinf
  use grvy

  implicit none

  real*8  :: should_be_nan 
  real*8  :: should_be_inf
  real*8  :: m_one, one, zero
  integer :: is_nan, is_inf, return_code

  !------------------------------------------------
  ! Simple FORTRAN 90 example to test isnan and 
  ! isinf interfaces.
  !------------------------------------------------
  
  m_one = -1.d0
  one   =  1.d0
  zero  =  0.d0

  should_be_nan = sqrt(m_one)
  should_be_inf = one/zero

  return_code = 0

  is_nan = grvy_double_isnan( should_be_nan )
  if( is_nan .ne. 1 )then
     return_code = 1
  endif

  is_inf = grvy_double_isinf( should_be_inf )
  if( is_inf .ne. 1 )then
     return_code = 1
  endif

  is_nan =  grvy_double_isnan( 5.d0 )
  if( is_nan .ne. 0 )then
     return_code = 1
  endif

  is_inf =  grvy_double_isinf( 1.d0 )
  if( is_inf .ne. 0 )then
     return_code = 1
  endif

  call exit(return_code)

end program test_isnan_isinf
