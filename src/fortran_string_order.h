//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008-2013,2018-2022 The PECOS Development Team
// Additional Copyright (C) 2018 individual authors
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
//-----------------------------------------------------------------------el-
//
// fortran_string_order.h: Ordering of Fortran string arguments
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

// Define a Fortran string length argument list order.  Online docs indicate
// that SYSV should be different then Linux; but this does not necessarily seem
// to be true.  If the Fortran interface blows up on a new platform, it is most
// likely due to this ordering.

#if defined __APPLE_CC__ || defined __linux__
#define _GRVY_FORTRAN_STRING_ORDER1
#endif
