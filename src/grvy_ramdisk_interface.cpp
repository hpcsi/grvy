// -*-c++-*-
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
// grvy_ramdisk_interface: C interface for MPI out-of-core ramdisk
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<stdio.h>
#include<string.h>
#include<grvy_classes.h>
#include<grvy_int.h>
#include<grvy.h>
#include<string>

// OPTIONAL Module - Only available with MPI/Ocore

#ifdef ENABLE_OCORE

using namespace std;
using namespace GRVY;

int grvy_ocore_init(const char *input_file, int num_ocore_tasks, MPI_Comm GLOB_COMM)
{
  // create new Ocore object on 1st call

  if(_GRVY_Ocore == NULL)
    _GRVY_Ocore = new GRVY_MPI_Ocore_Class();

  // initialize 

  return(_GRVY_Ocore->Initialize(input_file,num_ocore_tasks,GLOB_COMM));
}

int grvy_ocore_init_fortran(const char *input_file, int num_ocore_tasks, int GLOB_COMM_Fortran)
{
  MPI_Comm GLOB_COMM_C;

  // create new Ocore object on first call

  if(_GRVY_Ocore == NULL)
    _GRVY_Ocore = new GRVY_MPI_Ocore_Class();

  // Convert fortran communicator to C

  GLOB_COMM_C = MPI_Comm_f2c(GLOB_COMM_Fortran);

  // initialize 

  return(_GRVY_Ocore->Initialize(input_file,num_ocore_tasks,GLOB_COMM_C));
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
int grvy_ocore_write_int64   (size_t offset, long long int *data) {return(_GRVY_Ocore->Write(offset,data));};

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

// grvy_ocore_blocksize(): return ocore blocksize 

int grvy_ocore_blocksize()
{
  return(_GRVY_Ocore->Blocksize());
}

// grvy_ocore_pop_record(): return data record and pop/remove from ocore pool

size_t grvy_ocore_pop_record_float        (       float  *data) { return(_GRVY_Ocore->PopRecord(data)); }
size_t grvy_ocore_pop_record_double       (       double *data) { return(_GRVY_Ocore->PopRecord(data)); }
size_t grvy_ocore_pop_record_int          (          int *data) { return(_GRVY_Ocore->PopRecord(data)); }
size_t grvy_ocore_pop_record_int64        (long long int *data) { return(_GRVY_Ocore->PopRecord(data)); }

#else

extern "C" int grvy_ocore_init_fortran(const char *input_file, int num_ocore_tasks, int GLOB_COMM_Fortran)
{
  grvy_printf(GRVY_FATAL,"\n\nlibGRVY not built with MPI OCORE support\n\n");
  grvy_printf(GRVY_FATAL,"Please enable support using the \"--enable-ocore\" option to configure \n");
  grvy_printf(GRVY_FATAL,"and reinstall if you desire to use MPI out-of-core related functionality.\n\n");
  exit(1);
}

#endif
