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
#include<grvy_env.h>
#include<grvy.h>

#ifdef HAVE_HDF5
#include <H5PTpublic.h>
#endif

using namespace std;
using namespace GRVY;

// Note: see grvy_int.h for GRVY_HDF5_ClassImp declaration

#ifdef HAVE_HDF5

GRVY_HDF5_Class::GRVY_HDF5_Class() :m_pimpl(new GRVY_HDF5_ClassImp() )
{
  m_pimpl->fileId = -1;
}

GRVY_HDF5_Class::~GRVY_HDF5_Class() 
{
  // switched to auto_ptr for dtor;
  // delete m_pimpl;
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

  if(m_pimpl->fileId >= 0)
    {
      grvy_printf(GRVY_FATAL,"%s: current state has actively opened file - please close first\n",__func__,filename);
      exit(1);
    }

  m_pimpl->silence_hdf_error_handler();

  if ( (m_pimpl->fileId = H5Fcreate(filename, flags, H5P_DEFAULT, H5P_DEFAULT)) < 0)
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

bool GRVY_HDF5_Class::Exists(const char *filename)
{
  hid_t fileId;

  m_pimpl->silence_hdf_error_handler();

  if ( (fileId = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT)) < 0)
    {
      m_pimpl->restore_hdf_error_handler();  
      return(false);
    }
  else
    {
      H5Fclose(fileId);
      m_pimpl->restore_hdf_error_handler();  
      return(true);
    }
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

  if(m_pimpl->fileId >= 0)
    {
      grvy_printf(GRVY_FATAL,"hdf5_open: current state has actively opened file - please close first\n",filename);
      exit(1);
    }

  m_pimpl->silence_hdf_error_handler();

  if ( (m_pimpl->fileId = H5Fopen(filename, flags, H5P_DEFAULT)) < 0)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to open *existing* HDF file (%s)\n",__func__,filename);
      exit(1);
    }
    
  m_pimpl->restore_hdf_error_handler();  
  grvy_printf(GRVY_DEBUG,"%s: Successfully opened existing HDF file (%s)\n",__func__,filename);
  return(0); 
}

int GRVY_HDF5_Class::GroupCreate(string descname)
{
  hid_t groupId;
  time_t now;
  char comment[120];

  // Do we have a record of this group already open?

  if(m_pimpl->is_group_open(descname))
    {
      grvy_printf(GRVY_WARN,"%s: Group %s already exists\n",__func__,descname.c_str());
      return 1;
    }

  // Enable intermediate group creation to allow for user providing a
  // nested path

  herr_t status;
  hid_t grp_crt_plist;

  grp_crt_plist = H5Pcreate(H5P_LINK_CREATE);
  status = H5Pset_create_intermediate_group(grp_crt_plist, 1);

  if ( (groupId = H5Gcreate2(m_pimpl->fileId,descname.c_str(),grp_crt_plist,H5P_DEFAULT,H5P_DEFAULT)) < 0)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to create HDF group (%s)\n",__func__,descname.c_str());
      exit(1);
    }

  time (&now);
  sprintf(comment,"Created on %.24s",ctime(&now));
  
  if( H5Gset_comment(m_pimpl->fileId,descname.c_str(),comment) < 0)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to add comment for group (%s)\n",__func__,descname.c_str());
      exit(1);
    }

  // this groupId is now active -> save the hdf identifier for future use

  m_pimpl->groupIds[descname] = groupId;

  grvy_printf(GRVY_DEBUG,"%s: Successfully created new  HDF group (%s)\n",__func__,descname.c_str());
  return 0;
}

bool GRVY_HDF5_Class::GroupExists(string groupname)
{
  hid_t groupId;

  m_pimpl->silence_hdf_error_handler();

  if ( (groupId = H5Gopen(m_pimpl->fileId, groupname.c_str(),H5P_DEFAULT)) < 0)
    {
      m_pimpl->restore_hdf_error_handler();  
      return(false);
    }
  else
    {
      H5Gclose(groupId);
      m_pimpl->restore_hdf_error_handler();  
      return(true);
    }
}

int GRVY_HDF5_Class::GroupOpen(string groupname)
{
  if(m_pimpl->fileId < 0)
    {
      grvy_printf(GRVY_FATAL,"%s: no hdf5 file is actively open - unable to open group %s\n",
		  __func__,groupname.c_str());
      exit(1);
    }

  m_pimpl->silence_hdf_error_handler();

  if ( (m_pimpl->groupIds[groupname] = H5Gopen(m_pimpl->fileId, groupname.c_str(),H5P_DEFAULT)) < 0)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to open *existing* HDF group (%s)\n",__func__,groupname.c_str());
      exit(1);
    }  

  m_pimpl->restore_hdf_error_handler();  

  grvy_printf(GRVY_DEBUG,"%s: Successfully opened existing HDF group (%s)\n",__func__,groupname.c_str());
  return(0); 
}

bool GRVY_HDF5_Class::GRVY_HDF5_ClassImp::is_group_open(string descname)
{
  if(groupIds.count(descname) > 0)
    return true;
  else
    return false;
}

// Attribute-related functions

int GRVY_HDF5_Class::AttributeWrite(string groupname, string attribute, string value)
{
  hid_t strtype;

  strtype = H5Tcopy(H5T_C_S1);

  H5Tset_strpad(strtype,H5T_STR_NULLTERM);
  H5Tset_size  (strtype,value.size()); 

  hid_t dataspaceId  = H5Screate(H5S_SCALAR);
  hid_t attrId       = H5Acreate(m_pimpl->groupIds[groupname],attribute.c_str(),
				 strtype,dataspaceId,H5P_DEFAULT,H5P_DEFAULT);

  H5Awrite(attrId,strtype,value.c_str());
  
  H5Aclose(attrId);
  H5Sclose(dataspaceId);
  H5Tclose(strtype);
  
}

// Packet Table related functions

bool GRVY_HDF5_Class::GRVY_HDF5_ClassImp::PTableExists(string groupname,string tablename)
{
  hid_t tableId;

  silence_hdf_error_handler();

  if ( (tableId = H5PTopen(groupIds[groupname],tablename.c_str())) == H5I_BADID )
    {
      restore_hdf_error_handler();  
      return(false);
    }
  else
    {
      H5PTclose(tableId);
      restore_hdf_error_handler();  
      return(true);
    }
}

hid_t GRVY_HDF5_Class::GRVY_HDF5_ClassImp::PTableOpen(string groupname,string tablename)
{
  hid_t tableId;

  silence_hdf_error_handler();

  if ( (tableId = H5PTopen(groupIds[groupname],tablename.c_str())) == H5I_BADID )
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to open *existing* HDF PTable (%s)\n",__func__,tablename.c_str());
      restore_hdf_error_handler();  
      exit(1);
    }
  else
    {
      return(tableId);
    }
}
    
int GRVY_HDF5_Class::Close()
{
  grvy_printf(GRVY_DEBUG,"%s: Closing HDF file and resetting state\n",__func__);

  if(m_pimpl->fileId < 0)
    {
      grvy_printf(GRVY_WARN,"%s: current state has no actively opened file - ignoring close request\n",__func__);
      return(0);
    }

  // Note: according to the hdf spec a file close() is a bit sneaky in
  // that if other datasets or groups are still open prior to the
  // close(), then the file is not really closed.  Consequently, we do
  // a little extra dirty work here and make sure to close all objects
  // opened previously via the public interface.

  m_pimpl->close_open_objects();

  H5Fclose(m_pimpl->fileId);

  m_pimpl->fileId = -1;
  
  grvy_printf(GRVY_DEBUG,"%s: Successfully closed HDF file\n",__func__);
  return(0); 
}

void GRVY_HDF5_Class::GRVY_HDF5_ClassImp::silence_hdf_error_handler()
{
  
  // save state of internal hdf error handler
 
  H5Eget_auto2(H5E_DEFAULT,&error_orig_func, &error_orig_data);

  // Turn off error output

  H5Eset_auto2(H5E_DEFAULT,NULL, NULL);
}

void GRVY_HDF5_Class::GRVY_HDF5_ClassImp::restore_hdf_error_handler()
{
  // Restore previous error handler

  H5Eset_auto2(H5E_DEFAULT,error_orig_func, error_orig_data);
  
}

void GRVY_HDF5_Class::GRVY_HDF5_ClassImp::close_open_objects()
{
  // Close any open groups

  map<std::string,hid_t>::iterator it;

  for ( it=groupIds.begin(); it != groupIds.end(); it++ )
    {
      grvy_printf(GRVY_DEBUG,"%s: Closing open group (%s)\n",__func__,(*it).first.c_str());

      if(H5Gclose((*it).second) < 0)
	grvy_printf(GRVY_WARN,"%s: Unable to close group (%s)\n",__func__,(*it).second);

      groupIds.erase(it);
    }

  // TODO: continue to add here for any other hdf objects which get opened
}

#else

//----------------------------------------------------------
// NOOP public interface for case when hdf5 is not linked in
//----------------------------------------------------------

GRVY_HDF5_Class::GRVY_HDF5_Class()
{
  cout << endl;
  grvy_printf(GRVY_FATAL,"libGRVY not built with HDF support\n\n");
  grvy_printf(GRVY_FATAL,"Please enable support using the \"--with-hdf5\" option to configure \n");
  grvy_printf(GRVY_FATAL,"and reinstall if you desire to use HDF5 related functionality.\n\n");
  cout << endl;
  exit(1);
}

GRVY_HDF5_Class::~GRVY_HDF5_Class()
{
  cout << endl;
  grvy_printf(GRVY_FATAL,"libGRVY not built with HDF support\n\n");
  grvy_printf(GRVY_FATAL,"Please enable support using the \"--with-hdf5\" option to configure \n");
  grvy_printf(GRVY_FATAL,"and reinstall if you desire to use HDF5 related functionality.\n\n");
  cout << endl;
  exit(1);
}

int  GRVY_HDF5_Class::Create(const char *, bool){return 0;}
int  GRVY_HDF5_Class::Open  (const char *, bool){return 0;}
int  GRVY_HDF5_Class::Close (){return 0;}
bool GRVY_HDF5_Class::Exists(const char *){return 0;}

int  GRVY_HDF5_Class::GroupOpen   (std::string){return 0;}
int  GRVY_HDF5_Class::GroupCreate (std::string){return 0;}
bool GRVY_HDF5_Class::GroupExists (std::string){return 0;}
#if 0
int  GRVY_HDF5_Class::CreatePTable(const char *, const char *){return 0;};
#endif


#endif



