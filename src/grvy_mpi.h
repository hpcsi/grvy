// -*-c++-*-
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008-2013,2018-2021 The PECOS Development Team
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
// grvy_mpi.h: MPI related internals for GRVY
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#ifndef GRVY_INT_MPI_H
#define GRVY_INT_MPI_H

#ifdef HAVE_MPI

// Request OpenMPI to ignore C++ bindings (necessary so we can also
// link C code against libgrvy)

#define OMPI_SKIP_MPICXX

#include<mpi.h>

#undef OMPI_SKIP_MPICXX

namespace GRVY_Internal {

  // Map PODs to MPI datatypes

  MPI_Datatype Get_MPI_Type(          int value) { return(MPI_INT       ); };
  MPI_Datatype Get_MPI_Type(long long int value) { return(MPI_LONG_LONG ); };
  MPI_Datatype Get_MPI_Type(        float value) { return(MPI_FLOAT     ); };
  MPI_Datatype Get_MPI_Type(       double value) { return(MPI_DOUBLE    ); };
  
} // end namespace GRVY_Internal 

#endif

#endif
