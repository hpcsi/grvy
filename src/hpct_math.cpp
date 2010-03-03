// -*-c++-*-
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//
// Copyright (C) 2008,2009 The PECOS Development Team
//
// Please see http://pecos.ices.utexas.edu for more information.
//
// This file is part of the PECOS HPC Toolkit (HPCT)
//
// HPCT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// HPCT is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HPCT.  If not, see <http://www.gnu.org/licenses/>.
//
//--------------------------------------------------------------------------
// hpct_math: Convenience math utility functions.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<hpct_classes.h>
#include<hpct.h>

using namespace HPCT;

HPCT_Math_Class _HPCT_Math; //math class

//-----------------------------------------------------------------
//                       ye ol' C Interfaces
//-----------------------------------------------------------------

int hpct_double_isnan(double *val)
{
  return( _HPCT_Math.isnan<double>( *val ) );
}

int hpct_double_isinf(double *val)
{
  return( _HPCT_Math.isinf<double>( *val ) );
}

int hpct_float_isnan(float *val)
{
  return( _HPCT_Math.isnan<float>( *val ) );
}

int hpct_float_isinf(float *val)
{
  return( _HPCT_Math.isinf<float>( *val ) );
}

//-----------------------------------------------------------------
//                     Fortran Interfaces
//-----------------------------------------------------------------
extern "C" int hpct_double_isnan_( double *val )
{
  return( _HPCT_Math.isnan<double>( *val ) );
}

extern "C" int hpct_double_isinf_( double *val )
{
  return( _HPCT_Math.isinf<double>( *val ) );
}
