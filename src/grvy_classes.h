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
// grvy_classes.h: Basic class definitions
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#ifndef GRVY_CLASSES_H_
#define GRVY_CLASSES_H_

#include<grvy.h>

#include<cstdarg>
#include<limits>
#include<map>
#include<vector>
#include<string>
#include<stack>
#include<ostream>
#include<iostream>
#include<config_grvy.h>

#ifdef HAVE_HDF5
#include <hdf5.h>
#include <H5PTpublic.h>
#endif

#include<boost/math/special_functions.hpp>

#if 0
namespace GRVY_Internal {

  // GetPot-related constants (used to decide if we do not have a
  // default value)

  const static float     Float_Def = -9999999.0f;
  const static double   Double_Def = -9999999.0e1;
  const static int         Int_Def = -9999999;
  const static long       Long_Def = -9999999;
  const static char*      Char_Def = "unknown";

  const static std::string  String_Def("unknown");
}
#endif

namespace GRVY {

//--------------------------
// Math Class
//--------------------------

class GRVY_Math_Class {
 private:

 public:
  GRVY_Math_Class      ();

// See http://software.intel.com/en-us/forums/intel-c-compiler/topic/64188/
#if    (defined __INTEL_COMPILER)                    \
    && (__INTEL_COMPILER <= 1010)                    \
    && ((__INTEL_COMPILER_BUILD_DATE/10000) <= 2009) \
    && (defined __GLIBCXX__)                         \
    && (__GLIBCXX__ == 20090316)

  template< typename T >
  inline int isnan(T val)
  {
    return val != val;
  }

  template< typename T >
  inline int isinf(T val)
  {
      return val <= std::numeric_limits<T>::min()
          || val >= std::numeric_limits<T>::max();
  }

#else

  // Inlining the function so have both declaration and definition here
  template< typename T >
  inline int isnan(T val)
  {
    // Parentheses are important to ensure we don't collide
    // with isnan() from other stuff. See Boost documentation for more detail.
    bool is_val_a_nan = (boost::math::isnan)( val );

    // C++ implicit conversion from bool to int here
    return is_val_a_nan;

  }

  // Inlining the function so have both declaration and definition here
  template< typename T >
  inline int isinf(T val)
  {
    // Parentheses are important to ensure we don't collide
    // with isnan() from other stuff. See Boost documentation for more detail.
    bool is_val_inf = (boost::math::isinf)( val );

    // C++ implicit conversion from bool to int here
    return is_val_inf;

  }

#endif // End http://software.intel.com/en-us/forums/intel-c-compiler/topic/64188/

};

  //--------------------------
  // Logging Class
  //--------------------------

  class GRVY_Log_Class {
  public:
    GRVY_Log_Class();		
    bool isLog(int priority) {return(priority <= log_level);}       // log priority test
    void change_priority(int priority);                             // change current log level priority
    int get_priority();		                                    // return current log level priority
    void change_mask(int loglevel,std::string delimiter);           // change default log delimiter

    // C++ ostream style logging (<<)

    template <typename T> 
    inline GRVY_Log_Class operator <<(const T &val) {{std::cout << val;}};
    inline GRVY_Log_Class operator <<(std::ostream &(*pf)(std::ostream&)) {{std::cout << *pf; return *this;}};

    // Basic std::string message

    void msg       (int priority, std::string msg);                 // post new log message with a priority

    // C style printf logging

    int  msg_printf(int priority, const char *format,va_list argp); // post printf style log message
  

  private:
    int log_level;			                            // Current log level priority
    std::map<int,std::string> LogMask;                              // String masks for log messages

  };



}

#endif
