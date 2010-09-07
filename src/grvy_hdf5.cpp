// -*-c++-*
//-----------------------------------------------------------------------bl-
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
//-----------------------------------------------------------------------el-

#include<grvy_classes.h>
#include<grvy_int.h>
#include<grvy.h>

using namespace std;
using namespace GRVY;

#ifdef HAVE_HDF5

GRVY_HDF5_Class::GRVY_HDF5_Class()
{
  m_fileId = NULL;
}

int GRVY_HDF5_Class::file_create(const char *filename,bool overwrite_existing)
{
  unsigned int flags;

  if(overwrite_existing)
    {
      flags = H5F_ACC_TRUNC;
      grvy_printf(GRVY_DEBUG,"%s: Creating new hdf file %s (overwrite=true)\n",__func__,filename);
    }
  else
    {
      flags = H5F_ACC_EXCL;
      grvy_printf(GRVY_DEBUG,"%s: Creating new hdf file %s (overwrite=false)\n",__func__,filename);
    }

  if(m_fileId != NULL)
    {
      grvy_printf(GRVY_FATAL,"hdf5_open: current state has actively opened file - please close first\n",filename);
      exit(1);
    }

  silence_hdf_error_handler();

  if ( (m_fileId = H5Fcreate(filename, flags, H5P_DEFAULT, H5P_DEFAULT)) < 0)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to create *new* HDF file\n",__func__,filename);
      if(!overwrite_existing)
	{
	  grvy_printf(GRVY_FATAL,"%s: Note that the file must not exist when overwrite_existing=false\n",__func__);
	}
      exit(1);
    }

  restore_hdf_error_handler();  
  grvy_printf(GRVY_DEBUG,"%s: Successfully created new HDF file\n",filename);
  return(0); 
}

int GRVY_HDF5_Class::close()
{
  grvy_printf(GRVY_DEBUG,"%s: Closing HDF file and resetting state\n",__func__);

  if(m_fileId == NULL)
    {
      grvy_printf(GRVY_WARN,"%s: current state has no actively opened file - ignoring close request\n",__func__);
      return(0);
    }

  H5Fclose(m_fileId);

  m_fileId = NULL;
  
  grvy_printf(GRVY_DEBUG,"%s: Successfully closed HDF file\n",__func__);
  return(0); 
}

void GRVY_HDF5_Class::silence_hdf_error_handler()
{
  
  // save state of internal hdf error handler
 
  H5Eget_auto2(H5E_DEFAULT,&error_orig_func, &error_orig_data);

  // Turn off error output

  H5Eset_auto2(H5E_DEFAULT,NULL, NULL);
}

void GRVY_HDF5_Class::restore_hdf_error_handler()
{
  // Restore previous error handler

  H5Eset_auto2(H5E_DEFAULT,error_orig_func, error_orig_data);
  
}

#else

GRVY_HDF5_Class::GRVY_HDF5_Class()
{
  cout << endl;
  grvy_printf(GRVY_FATAL,"libGRVY not built with HDF support\n\n");
  grvy_printf(GRVY_FATAL,"Please enable support using the \"--with-hdf5\" option to configure \n");
  grvy_printf(GRVY_FATAL,"and reinstall if you desire to use HDF5 related functionality.\n\n");
  exit(1);
}

#endif



