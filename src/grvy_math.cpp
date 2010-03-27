// -*-c++-*-
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//
// Copyright (C) 2008,2009,2010 The PECOS Development Team
//
// Please see http://pecos.ices.utexas.edu for more information.
//
// This file is part of the PECOS GRVY Toolkit
//
// GRVY is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GRVY is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GRVY.  If not, see <http://www.gnu.org/licenses/>.
//
//--------------------------------------------------------------------------
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
