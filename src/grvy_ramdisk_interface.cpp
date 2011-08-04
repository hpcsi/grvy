// -*-c++-*-
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011 The PECOS Development Team
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
// grvy_ramdisk_interface: C interface for MPI out-of-core ramdisk
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<stdio.h>
#include<string.h>
#include<grvy_classes.h>
#include<grvy_int.h>
#include<grvy.h>
#include<string>

using namespace std;
using namespace GRVY;

int grvy_ocore_init(const char *input_file)
{
  // create new timer on 1st call

  if(_GRVY_Ocore == NULL)
    _GRVY_Ocore = new GRVY_MPI_Ocore_Class();

  // initialize 

  return(_GRVY_Ocore->Initialize(input_file));
}

// grvy_ocore_finalize(): Finalize MPI ocore and dump statistics

void grvy_ocore_finalize()
{
  _GRVY_Ocore->Finalize();

  delete _GRVY_Ocore;
  return;
}

// ----------------------------
// grvy_ocore_write functions
// ----------------------------

int grvy_ocore_write_float   (size_t offset,         float *data) {return(_GRVY_Ocore->Write(offset,data));};
int grvy_ocore_write_double  (size_t offset,        double *data) {return(_GRVY_Ocore->Write(offset,data));};
int grvy_ocore_write_int     (size_t offset,           int *data) {return(_GRVY_Ocore->Write(offset,data));};
int grvy_ocore_write_longlong(size_t offset, long long int *data) {return(_GRVY_Ocore->Write(offset,data));};

// ----------------------------
// grvy_ocore_read functions
// ----------------------------

int grvy_ocore_read_float (size_t offset,          float *data) {return(_GRVY_Ocore->Read(offset,data));};
int grvy_ocore_read_double(size_t offset,         double *data) {return(_GRVY_Ocore->Read(offset,data));};
int grvy_ocore_read_int   (size_t offset,            int *data) {return(_GRVY_Ocore->Read(offset,data));};
int grvy_ocore_read_int64 (size_t offset,  long long int *data) {return(_GRVY_Ocore->Read(offset,data));};

// grvy_ocore_master(): determine if calling process is the master MPI task

int grvy_ocore_master()
{
  return( ( _GRVY_Ocore->isMaster() ) ? 1 : 0 );
}

// grvy_ocore_master(): determine if MPI Ocore is enabled

int grvy_ocore_enabled()
{
  return( ( _GRVY_Ocore->isEnabled() ) ? 1 : 0 );
}

// grvy_ocore_num_active(): return number of active records;

size_t grvy_ocore_num_active()
{
  return(_GRVY_Ocore->NumActive());
}

// grvy_ocore_pop_record(): return data record and pop/remove from ocore pool

size_t grvy_ocore_pop_record_float        (       float  *data) { return(_GRVY_Ocore->PopRecord(data)); }
size_t grvy_ocore_pop_record_double       (       double *data) { return(_GRVY_Ocore->PopRecord(data)); }
size_t grvy_ocore_pop_record_int          (          int *data) { return(_GRVY_Ocore->PopRecord(data)); }
size_t grvy_ocore_pop_record_long_long_int(long long int *data) { return(_GRVY_Ocore->PopRecord(data)); }
