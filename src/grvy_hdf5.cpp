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
//
// $Id$
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<grvy_classes.h>
#include<grvy_int.h>
#include<grvy.h>

using namespace std;
using namespace GRVY;

namespace GRVY{

class GRVY_HDF5_ClassImp
{
 public:

  GRVY_HDF5_ClassImp() {}
 ~GRVY_HDF5_ClassImp() {}
  
  hid_t m_fileId;                        // hdf5 file handle
  std::map<std::string,hid_t> m_groupIds;// hdf5 group handles
  
  H5E_auto2_t error_orig_func;           // error-handle func
  void       *error_orig_data;           // error-handle stack data
  
  void silence_hdf_error_handler();
  void restore_hdf_error_handler();
};

}  // matches namespace GRVY

#ifdef HAVE_HDF5

GRVY_HDF5_Class::GRVY_HDF5_Class() :m_pimpl(new GRVY_HDF5_ClassImp() )
{
  m_pimpl->m_fileId = NULL;
}

GRVY_HDF5_Class::~GRVY_HDF5_Class() 
{
  //  delete m_pimpl;
}

int GRVY_HDF5_Class::Create(const char *filename,bool overwrite_existing)
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

  if(m_pimpl->m_fileId)
    {
      grvy_printf(GRVY_FATAL,"hdf5_open: current state has actively opened file - please close first\n",filename);
      exit(1);
    }

  m_pimpl->silence_hdf_error_handler();

  if ( (m_pimpl->m_fileId = H5Fcreate(filename, flags, H5P_DEFAULT, H5P_DEFAULT)) < 0)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to create *new* HDF file (%s)\n",__func__,filename);
      if(!overwrite_existing)
	{
	  grvy_printf(GRVY_FATAL,"%s: Note that the file must not exist when overwrite_existing=false\n",__func__);
	}
      exit(1);
    }

  m_pimpl->restore_hdf_error_handler();  
  grvy_printf(GRVY_DEBUG,"%s: Successfully created new HDF file\n",filename);
  return(0); 
}

int GRVY_HDF5_Class::Open(const char *filename,bool readonly)
{
  unsigned int flags;

  if(readonly)
    {
      flags = H5F_ACC_RDONLY;
      grvy_printf(GRVY_DEBUG,"%s: Opening existing hdf file %s (readonly=true)\n",__func__,filename);
    }
  else
    {
      flags = H5F_ACC_RDWR;
      grvy_printf(GRVY_DEBUG,"%s: Opening existing hdf file %s (readonly=false)\n",__func__,filename);
    }



  if(!m_pimpl->m_fileId)
    {
      grvy_printf(GRVY_FATAL,"hdf5_open: current state has actively opened file - please close first\n",filename);
      exit(1);
    }

  m_pimpl->silence_hdf_error_handler();

  if ( (m_pimpl->m_fileId = H5Fopen(filename, flags, H5P_DEFAULT)) < 0)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to open *existing* HDF file (%s)\n",__func__,filename);
      exit(1);
    }
    
  m_pimpl->restore_hdf_error_handler();  
  grvy_printf(GRVY_DEBUG,"%s: Successfully opened existing HDF file\n",filename);
  return(0); 
}

int GRVY_HDF5_Class::CreateGroup(const char *descname)
{
  hid_t groupId;
  time_t now;
  char comment[120];

  // Do we have a record of this group?

  if(GroupExists(descname))
    {
      grvy_printf(GRVY_WARN,"%s: Group %s already exists\n",__func__,descname);
      return 1;
    }

  if ( (groupId = H5Gcreate2(m_pimpl->m_fileId,descname,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT)) < 0)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to create HDF group (%s)\n",__func__,descname);
      exit(1);
    }

  time (&now);
  sprintf(comment,"Created on %.24s",ctime(&now));
  
  if( H5Gset_comment(m_pimpl->m_fileId,descname,comment) < 0)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to add comment for group (%s)\n",__func__,descname);
      exit(1);
    }

  grvy_log_char(GRVY_DEBUG,__func__,"Successfully created new HDF group",descname);
  return 0;
}

int GRVY_HDF5_Class::GroupExists(const char *descname)
{
  map<std::string,hid_t>::iterator it = m_pimpl->m_groupIds.find(descname);

  return(it != m_pimpl->m_groupIds.end());
}
    
int GRVY_HDF5_Class::Close()
{
  grvy_printf(GRVY_DEBUG,"%s: Closing HDF file and resetting state\n",__func__);

  if(!m_pimpl->m_fileId)
    {
      grvy_printf(GRVY_WARN,"%s: current state has no actively opened file - ignoring close request\n",__func__);
      return(0);
    }

  // TODO: verify correct error code for close

  H5Fclose(m_pimpl->m_fileId);

  m_pimpl->m_fileId = NULL;
  
  grvy_printf(GRVY_DEBUG,"%s: Successfully closed HDF file\n",__func__);
  return(0); 
}

void GRVY_HDF5_ClassImp::silence_hdf_error_handler()
{
  
  // save state of internal hdf error handler
 
  H5Eget_auto2(H5E_DEFAULT,&error_orig_func, &error_orig_data);

  // Turn off error output

  H5Eset_auto2(H5E_DEFAULT,NULL, NULL);
}

void GRVY_HDF5_ClassImp::restore_hdf_error_handler()
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
  cout << endl;
  exit(1);
}

GRVY_HDF5_Class::Create(const char *, bool){}
GRVY_HDF5_Class::Open  (const char *, bool){}
GRVY_HDF5_Class::Close (){}

GRVY_HDF5_Class::CreateGroup(const char *){}
GRVY_HDF5_Class::GroupExists(const char *){}


#endif



