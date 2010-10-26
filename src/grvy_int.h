// -*-c++-*-
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
// grvy_int.h: Internals for GRVY
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#ifndef GRVY_INT_H
#define GRVY_INT_H

#include<map>
#include<vector>
#include<string>
#include<grvy.h>

namespace GRVY {

#ifdef _GRVY_GLOBAL_DEF
#define GLOBAL
#else
#define GLOBAL extern
#endif

  // Internal Routines

  extern void _GRVY_message (char *message);
  extern void _GRVY_message (int LogLevel, const char *func, const char *message);
  extern void _GRVY_message (int LogLevel, const char *func, const char *message, const char *char_item);

  template <typename T> void _GRVY_message (int LogLevel, const char *func, const char *message, T item);

  extern int  _GRVY_CheckDir      (const char *dirname);

  extern void _GRVY_create_scratch_dir_atexit_handler();
  extern int  _GRVY_RemoveAll            (const char *path);
  extern int  _GRVY_RemoveAll_nftw_helper(const char *path, const struct stat * st, int flag, struct FTW *f);

  extern char *grvy_f2c_char(char*input,int len);
  extern int  verify_string_ends_with_6_Xs(char *name_template);

  // Evil Globals Vars

#ifdef _GRVY_GLOBAL_DEF

  GRVY_Log_Class    _GRVY_Log;                           // Log mechanism
  GLOBAL const char *_GRVY_emask   = "[*] Error";        // default error mask
  GLOBAL const char *_GRVY_wmask   = "[*] Warning";      // default warning mask
  GLOBAL const char *_GRVY_imask   = "[*] Info";         // default info mask
  GLOBAL const char *_GRVY_gtimer  = "GRVY_Unassigned";  // default global timer key

  GRVY_Timer_Class *_GRVY_Timers               =  NULL;   // performance timer 

  GLOBAL double _GRVY_TIMER_THRESH       = 9.0e-8;  // low water mark for expected timer usage deltas        
  GLOBAL double _GRVY_PERC_TOL           = 1e-3;    // tolerance for defining acceptable global percentages

#else

  GLOBAL GRVY_Log_Class _GRVY_Log;
  GLOBAL const char *_GRVY_emask;
  GLOBAL const char *_GRVY_wmask;
  GLOBAL const char *_GRVY_imask;
  GLOBAL const char *_GRVY_gtimer;

  GLOBAL GRVY_Timer_Class *_GRVY_Timers;
  GLOBAL double _GRVY_TIMER_THRESH;
  GLOBAL double _GRVY_PERC_TOL;

#endif

  // Classes/Typedefs

  class TimerCmpHighLow {
  public:
    bool operator()(const std::vector <double> v1, const std::vector <double> v2 )  {
      return ( v1[0] - v2[0] ) > 0; }
  };

  class TimerCmpLowHigh {
  public:
    bool operator()(const std::vector <double> v1, const std::vector <double> v2 )  {
      return ( v1[0] - v2[0] ) < 0; }
  };

  typedef std::map<std::string,std::vector <double> >                      _GRVY_Type_TimerMap;
  typedef std::map<std::vector <double>,std::string,TimerCmpHighLow > _GRVY_Type_TimerMapSortHL;  
  typedef std::map<std::vector <double>,std::string,TimerCmpLowHigh > _GRVY_Type_TimerMapSortLH;  

  // ---------------------------------------------------------------
  // Private class implementations which require declaration across
  // multiple classes
  // ---------------------------------------------------------------

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
    
    bool  is_group_open            (std::string groupname);
    void  silence_hdf_error_handler();
    void  restore_hdf_error_handler();
    void  close_open_objects       ();
    bool  PTableExists             (std::string groupname, std::string tablename);
    hid_t PTableOpen               (std::string groupname, std::string tablename);

    template <typename T> int AttributeWrite(std::string, std::string, T);
    template <typename T> hid_t get_little_endian_type   (T value);

    // HDF intrinsic data type support functions (little endian)

    hid_t get_little_endian_type(short int         ) { return(H5T_STD_I16LE);  };
    hid_t get_little_endian_type(int               ) { return(H5T_STD_I32LE);  };
    hid_t get_little_endian_type(long              ) { return(H5T_STD_I64LE);  };
									       
    hid_t get_little_endian_type(unsigned short int) { return(H5T_STD_U16LE);  };
    hid_t get_little_endian_type(unsigned int      ) { return(H5T_STD_U32LE);  };
    hid_t get_little_endian_type(unsigned long     ) { return(H5T_STD_U64LE);  };

    hid_t get_little_endian_type(float             ) { return(H5T_IEEE_F32LE); };
    hid_t get_little_endian_type(double            ) { return(H5T_IEEE_F64LE); };

#endif
  };

}

#endif
