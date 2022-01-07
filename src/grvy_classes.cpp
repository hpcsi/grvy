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
// grvy_classes.cpp: Basic class member functions.
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<sys/time.h>
#include<stdarg.h>
#include<time.h>
#include "grvy_classes.h"
#include "grvy.h"
#include "grvy_int.h"

namespace GRVY {

  //-------------------------------------
  // Versioning Routines
  //-------------------------------------

  void GRVY_version_stdout()
  {

    if(_GRVY_Log.isLog(GRVY_INFO))
      {
	std::cout << "--------------------------------------------------------" << std::endl;
	std::cout << "libGRVY Library: Version = " << GRVY_LIB_VERSION; 
	std::cout << " (" << GRVY_get_numeric_version() << ")" << std::endl << std::endl; 
	
	std::cout << GRVY_LIB_RELEASE << std::endl << std::endl;
	
	std::cout << "Build Date   = " << GRVY_BUILD_DATE     << std::endl;
	std::cout << "Build Host   = " << GRVY_BUILD_HOST     << std::endl;
	std::cout << "Build User   = " << GRVY_BUILD_USER     << std::endl;
	std::cout << "Build Arch   = " << GRVY_BUILD_ARCH     << std::endl;
	std::cout << "Build Rev    = " << GRVY_BUILD_VERSION  << std::endl << std::endl;
	
	std::cout << "C++ Config   = " << GRVY_CXX << " " << GRVY_CXXFLAGS << std::endl;
	std::cout << "F90 Config   = " << GRVY_FC  << " " << GRVY_FCFLAGS  << std::endl;
	std::cout << "--------------------------------------------------------" << std::endl;

	std::cout << "Optional Library Support:" << std::endl;

#ifdef  HAVE_HDF5
	std::cout << "--> HDF5 enabled = yes" << std::endl;
#else
	std::cout << "--> HDF5 enabled = no" << std::endl;
#endif

#ifdef  HAVE_MPI
	std::cout << "--> MPI enabled  = yes" << std::endl;
#else
	std::cout << "--> MPI enabled  = no" << std::endl;
#endif

	std::cout << "--------------------------------------------------------" << std::endl;
      }
  }

  int GRVY_get_numeric_version()
  {
    // Note: return format follows the versioning convention xx.yy.zz where
    // 
    // xx = major version number
    // yy = minor version number
    // zz = micro version number
    // 
    // For example:
    // v.   0.23  -> 002300 = 2300
    // v   0.23.1 -> 002301 = 2301
    // v. 10.23.2 -> 102302

    int major_version = 0;
    int minor_version = 0;
    int micro_version = 0;

#ifdef GRVY_MAJOR_VERSION
    major_version = GRVY_MAJOR_VERSION;
#endif

#ifdef GRVY_MINOR_VERSION
    minor_version = GRVY_MINOR_VERSION;
#endif

#ifdef GRVY_MICRO_VERSION
    micro_version = GRVY_MICRO_VERSION;
#endif
      
    return(major_version*10000 + minor_version*100 + micro_version);

  }

  //-------------------------------------
  // GRVY_Math_Class:: Member Functions
  //-------------------------------------

  GRVY_Math_Class::GRVY_Math_Class( ) // default constructor
  {
    return;
  }

  //-------------------------------------
  // GRVY_Log_Class:: Member Functions
  //-------------------------------------

  GRVY_Log_Class::GRVY_Log_Class()
  {

    // set default log level
    
    log_level = GRVY_INFO;

    // set default log masks for each priority level

#if 0
    LogMask[GRVY_FATAL] = "[*] Fatal: ";
    LogMask[GRVY_ERROR] = "[*] Error: ";
    LogMask[GRVY_WARN ] = "[*]  Warn: ";
    LogMask[GRVY_INFO ] = "[*]  Info: ";
    LogMask[GRVY_DEBUG] = "[*] Debug: ";
#else
    LogMask[GRVY_FATAL] = "";
    LogMask[GRVY_ERROR] = "";
    LogMask[GRVY_WARN ] = "";
    LogMask[GRVY_INFO ] = "";
    LogMask[GRVY_DEBUG] = "";
#endif

    return;
  }

  void GRVY_Log_Class::change_priority(int priority)
  {
    log_level = priority;
    return;
  }

 int GRVY_Log_Class::get_priority()
  {
    return(log_level);
  }

  void GRVY_Log_Class::change_mask(int priority, const std::string delimiter)
  {
    LogMask[priority] = delimiter;
    return;
  }

  void GRVY_Log_Class::msg(int priority, const std::string msg)
  {
    if(isLog(priority))
      {
	std::cout << LogMask[priority];
	std::cout << msg << std::endl;
      }

    return;
  }

  // C style printf logging

  int GRVY_Log_Class::msg_printf(int priority, const char *format,va_list arg)
  {
    int done;

    printf("%s",LogMask[priority].c_str());
    done = vprintf (format, arg);
    return done;
  }

} // matches namespace grvy
