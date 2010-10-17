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

#ifdef HAVE_HDF5
#include <H5PTpublic.h>
#endif

const int MAX_TIMER_WIDTH=120;	// 

using namespace std;
using namespace GRVY;

namespace GRVY {

class GRVY_HDF5_Class::GRVY_HDF5_ClassImp
{
public:
  
  GRVY_HDF5_ClassImp() {}
 ~GRVY_HDF5_ClassImp() {}
  
#ifdef HAVE_HDF5  
  hid_t fileId;                             // hdf5 file handle
  std::map<std::string,hid_t> groupIds;     // hdf5 group handles
  std::map<std::string,hid_t> dataspaceIds; // hdf5 dataspace hancles
  
  H5E_auto2_t error_orig_func;              // error-handle func
  void       *error_orig_data;              // error-handle stack data
  
  void silence_hdf_error_handler();
  void restore_hdf_error_handler();
  void close_open_objects();
#endif
};

}  // matches namespace GRVY

#ifdef HAVE_HDF5

GRVY_HDF5_Class::GRVY_HDF5_Class() :m_pimpl(new GRVY_HDF5_ClassImp() )
{
  m_pimpl->fileId = NULL;
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

  if(m_pimpl->fileId)
    {
      grvy_printf(GRVY_FATAL,"hdf5_open: current state has actively opened file - please close first\n",filename);
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

  if(!m_pimpl->fileId)
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

  if ( (groupId = H5Gcreate2(m_pimpl->fileId,descname,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT)) < 0)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to create HDF group (%s)\n",__func__,descname);
      exit(1);
    }

  time (&now);
  sprintf(comment,"Created on %.24s",ctime(&now));
  
  if( H5Gset_comment(m_pimpl->fileId,descname,comment) < 0)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to add comment for group (%s)\n",__func__,descname);
      exit(1);
    }

  // this groupId is now active -> we save the hdf identifier for future use

  m_pimpl->groupIds[descname] = groupId;

  grvy_printf(GRVY_DEBUG,"%s: Successfully created new  HDF group (%s)\n",__func__,descname);
  return 0;
}

int GRVY_HDF5_Class::CreatePTable(const char *groupname, const char *tablename)
{
  hid_t tableId;
  hid_t groupId, dataspaceId;
  time_t now;
  char comment[120];

  // make sure the group exist

  if(GroupExists(groupname))
    {
      groupId = m_pimpl->groupIds[groupname];
    }
  else 
    {
      grvy_printf(GRVY_FATAL,"%s: Group %s does not exist, please create first\n",__func__,groupname);
      exit(1);
    }



  typedef struct ptable_v0_10 {
    char timer_name[MAX_TIMER_WIDTH];
    double measurement;
    double mean;
    double variance;
    int count;
  };

  hid_t strtype;
  ptable_v0_10 data[2];
  hid_t ptable_type;

  strtype = H5Tcopy(H5T_C_S1);

  H5Tset_size(strtype,(size_t)MAX_TIMER_WIDTH);
  H5Tset_strpad(strtype,H5T_STR_NULLTERM);

  grvy_printf(GRVY_INFO,"creating compound datatype\n");

  if( (ptable_type = H5Tcreate (H5T_COMPOUND, sizeof(ptable_v0_10))) < 0 )
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to create compound HDF datatype\n",__func__);
      exit(1);
    }
    
  H5Tinsert(ptable_type, "timer_name", HOFFSET(ptable_v0_10, timer_name), strtype);
  H5Tinsert(ptable_type, "measurement",HOFFSET(ptable_v0_10, measurement),H5T_NATIVE_DOUBLE);
  H5Tinsert(ptable_type, "mean",       HOFFSET(ptable_v0_10, mean),       H5T_NATIVE_DOUBLE);
  H5Tinsert(ptable_type, "variance",   HOFFSET(ptable_v0_10, variance),   H5T_NATIVE_DOUBLE);
  H5Tinsert(ptable_type, "count",      HOFFSET(ptable_v0_10, count),      H5T_NATIVE_INT);

  if( (tableId = H5PTcreate_fl(groupId,tablename,ptable_type,(hsize_t)256,-1)) == H5I_BADID)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to create HDF packet table (%s)\n",__func__,tablename);
      exit(1);
    }

  // just prototyping; create a couple of example entries by hand

  sprintf(data[0].timer_name,"foo foo");
  data[0].measurement = 123.456;
  data[0].mean = 122.5;
  data[0].variance = 6e-5;
  data[0].count = 25;

  assert(H5PTappend( tableId, 1, &(data[0]) ) >= 0);

  sprintf(data[0].timer_name,"boo boo");
  data[0].measurement = 789.123;
  data[0].mean = 80.0;
  data[0].variance = 5e-1;
  data[0].count = 100001;

  assert(H5PTappend( tableId, 1, &(data[0]) ) >= 0);

  // this groupId is now active -> we save the hdf identifier for future use

  //  m_pimpl->groupIds[descname] = groupId;

  grvy_printf(GRVY_DEBUG,"%s: Successfully created new packet table (%s)\n",__func__,tableId);
  return 0;
}



inline int GRVY_HDF5_Class::GroupExists(const char *descname)
{
  return(m_pimpl->groupIds.count(descname));
}
    
int GRVY_HDF5_Class::Close()
{
  grvy_printf(GRVY_DEBUG,"%s: Closing HDF file and resetting state\n",__func__);

  if(!m_pimpl->fileId)
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

  m_pimpl->fileId = NULL;
  
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

int GRVY_HDF5_Class::Create(const char *, bool){return 0;}
int GRVY_HDF5_Class::Open  (const char *, bool){return 0;}
int GRVY_HDF5_Class::Close (){return 0;}

int GRVY_HDF5_Class::CreateGroup (const char *){return 0;}
int GRVY_HDF5_Class::GroupExists (const char *){return 0;}
int GRVY_HDF5_Class::CreatePTable(const char *, const char *){return 0;};


#endif



