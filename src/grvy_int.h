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

#include <boost/program_options.hpp>

namespace GRVY {

  //#define ginfo (...) grvy_printf(GRVY_INFO, __VA_ARGS__)
  //#define gerror(...) grvy_printf(GRVY_ERROR,__VA_ARGS__)

  //#define DBGPRINT(fmt, ...) printf(fmt, __VA_ARGS__)

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
  extern int  _GRVY_RemoveAll             (const char *path);
  extern int  _GRVY_RemoveAll_nftw_helper (const char *path, const struct stat * st, int flag, struct FTW *f);

  extern char *grvy_f2c_char              (char*input,int len);
#if 0
  extern char *grvy_f2c_char_no_spaces    (char*input,int len); // not used anymore
#endif
  extern char *grvy_trim_whitespace       (char *);
  extern int  verify_string_ends_with_6_Xs(char *name_template);

  // Evil Globals Vars

#ifdef _GRVY_GLOBAL_DEF

  GRVY_Log_Class    _GRVY_Log;                           // Log mechanism
  GLOBAL const char *_GRVY_gtimer  = "GRVY_Unassigned";  // default global timer key

  GRVY_Timer_Class *_GRVY_Timers   =  NULL;              // performance timer 

  GLOBAL double _GRVY_TIMER_THRESH       = 9.0e-8;      // low water mark for expected timer usage deltas        
  GLOBAL double _GRVY_PERC_TOL           = 1e-3;        // tolerance for defining acceptable global percentages

#else

  GLOBAL GRVY_Log_Class _GRVY_Log;
  GLOBAL const char *_GRVY_gtimer;

  GLOBAL GRVY_Timer_Class *_GRVY_Timers;
  GLOBAL double _GRVY_TIMER_THRESH;
  GLOBAL double _GRVY_PERC_TOL;

#endif

  // Macro Defines

  //#define USE_HDF5_PTABLE

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
    std::map<std::string,hid_t> datasetIds;   // hdf5 dataset handles
    std::map<std::string,hid_t> attributeIds; // hdf5 attribute handles
    
    H5E_auto2_t error_orig_func;              // error-handle func
    void       *error_orig_data;              // error-handle stack data
    
    bool  is_group_open            (std::string groupname);
    void  silence_hdf_error_handler();
    void  restore_hdf_error_handler();
    void  close_open_objects       ();
    bool  PTableExists             (std::string groupname, std::string tablename);
    hid_t PTableOpen               (std::string groupname, std::string tablename);
    int   PTableClose              (hid_t tableId);
    hsize_t PTableNumPackets       (hid_t tableId);

    template <typename T> int AttributeWrite(std::string, std::string, T);
    template <typename T> int AttributeRead (std::string, std::string, T&);
    
    //int AttributeRead (std::string, std::string, std::string&);

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

    GRVY_HDF5_Class *self;	// back pointer to public class

#endif
  };

  // -----------------------------------------------------
  // Additional convenience functions, typdefs, and macros
  // -----------------------------------------------------

  // common logging functions
  // 
  // Note: Intel 11.1 doesn't support variadic templates, commenting out for the time being

#if 0
  template<typename... Args> inline int Gdebug(const Args&... args) { return(grvy_printf(GRVY_DEBUG,args...));}
  template<typename... Args> inline int Ginfo (const Args&... args) { return(grvy_printf(GRVY_INFO, args...));}
  template<typename... Args> inline int Gwarn (const Args&... args) { return(grvy_printf(GRVY_WARN, args...));}
  template<typename... Args> inline int Gerror(const Args&... args) { return(grvy_printf(GRVY_ERROR,args...));}
  template<typename... Args> inline int GFATAL(const Args&... args) { return(grvy_printf(GRVY_FATAL,args...));}
#endif

  // commonly used STL typedefs

  typedef std::map<std::string,double> MAP_string_to_double;

  // set_map_constant_value(): Sets all elements of an STL map to have the same value

  template <typename Tkey, typename Tvalue> 
  void set_map_constant_value(std::map<Tkey,Tvalue> &amap,const Tvalue newval)
  {
    for(typename std::map<Tkey,Tvalue>::iterator it=amap.begin();it != amap.end(); ++it)
      {
	it->second = newval;
      }

    return;
  }

  // read_boost_option(): Reads defined option from boost::program_options variable map

  template <typename T> T read_boost_option(boost::program_options::variables_map &vmap,
					    std::string varname, T default_value)
  {
    if(vmap.count(varname))
      {
	T value = vmap[varname].as<T>();
    
	if(_GRVY_Log.isLog(GRVY_DEBUG))
	  std::cout << __func__ << ": User provided " << varname << " = " << value << std::endl;

	return(value);
      }
    else
      {
      	if(_GRVY_Log.isLog(GRVY_DEBUG))
	  std::cout << __func__ << ": Using default " << varname << " = " << default_value << std::endl;
	return(default_value);
      }
  }

  // numDigits_positive(): Counts number of digits for positive number

  template <class T> int numDigits_positive(T number)
  {
    int digits = 0;
    
    if(number < 0) return 0;
    
    while (number) {
      number /= 10;
      digits++;
    }
    return digits;
  }

} // end namespace GRVY

#endif
