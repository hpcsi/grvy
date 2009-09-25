//--------------------------------------------------------------------------
// -------------------------------------------------------------------------
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
// -------------------------------------------------------------------------
//
// fortran_string_order.h: Ordering of Fortran string arguments
//
// $Id
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

// Define a Fortran string length argument list order.  Online docs indicate
// that SYSV should be different then Linux; but this does not necessarily seem
// to be true.  If the Fortran interface blows up on a new platform, it is most
// likely due to this ordering.
#if defined __APPLE_CC__ || defined __linux__
#define _HPCT_FORTRAN_STRING_ORDER1
#endif
