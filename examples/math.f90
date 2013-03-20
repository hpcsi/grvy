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
!! math.f90: Fortran example illustrating inf/nan math tests.
!!
!! $Id$
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

program test_isnan_isinf

  use grvy

  implicit none

  real*8 :: should_be_nan 
  real*8 :: should_be_inf

  real*8 :: m_one, one, zero

  integer :: is_nan, is_inf

  m_one = -1.d0
  one   =  1.d0
  zero  =  0.d0

  should_be_nan = sqrt(m_one)
  should_be_inf = one/zero

  call grvy_double_isnan( should_be_nan, is_nan )
  if( is_nan .eq. 1 )then
     write(*,*) "NaN detected"
  endif

  call grvy_double_isinf( should_be_inf, is_inf )
  if( is_inf .eq. 1 )then
     write(*,*) "Inf detected"
  endif

  call grvy_double_isnan( 5.d0, is_nan )
  if( is_nan .eq. 1 )then
     write(*,*) "NaN detected"
  endif

  call grvy_double_isinf( 1.d0, is_inf )
  if( is_inf .eq. 1 )then
     write(*,*) "Inf detected"
  endif

  return

end program test_isnan_isinf
