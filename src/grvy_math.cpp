// -*-c++-*-
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor, 
// Boston, MA  02110-1301  USA
//
//--------------------------------------------------------------------------
//
// grvy_math: Convenience math utility functions.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<grvy_classes.h>
#include<grvy.h>

using namespace GRVY;

GRVY_Math_Class _GRVY_Math; //math class

//-----------------------------------------------------------------
//                       ye ol' C Interfaces
//-----------------------------------------------------------------

int grvy_double_isnan(double *val)
{
  return( _GRVY_Math.isnan<double>( *val ) );
}

int grvy_double_isinf(double *val)
{
  return( _GRVY_Math.isinf<double>( *val ) );
}

int grvy_float_isnan(float *val)
{
  return( _GRVY_Math.isnan<float>( *val ) );
}

int grvy_float_isinf(float *val)
{
  return( _GRVY_Math.isinf<float>( *val ) );
}

//-----------------------------------------------------------------
//                     Fortran Interfaces
//-----------------------------------------------------------------
extern "C" int grvy_double_isnan_( double *val )
{
  return( _GRVY_Math.isnan<double>( *val ) );
}

extern "C" int grvy_double_isinf_( double *val )
{
  return( _GRVY_Math.isinf<double>( *val ) );
}
