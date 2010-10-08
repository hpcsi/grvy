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
// grvy_classes.h: Basic class definitions
//
// $Id$
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
#endif

#include<boost/accumulators/accumulators.hpp>
#include<boost/accumulators/statistics/mean.hpp>
#include<boost/accumulators/statistics/stats.hpp>
#include<boost/accumulators/statistics/count.hpp>
#include<boost/accumulators/statistics/variance.hpp>
#include<boost/math/special_functions.hpp>

using namespace boost::accumulators;

namespace GRVY {

//--------------------------
// Performance Timing Class
//--------------------------

typedef struct GRVY_Timer_Data {
  double timings[2];
  accumulator_set <double,features<tag::mean,tag::count,tag::variance> > stats;
} tTimer_Data;

typedef std::map <std::string, GRVY_Timer_Data > _GRVY_Type_TimerMap2;

class GRVY_Timer_Class {
 private:
  short int initialized;              // initialized?
  double    timer_last;               // raw timer value of last call
  double    timer_finalize;           // raw timer value at time of finalize()
  std::string    timer_name;          // user name supplied for the timer
  int       num_begins;	              // number of active begin timers (used for callgraph determination)
  std::stack <std::string> callgraph; // callgraph to support embedded timers
  bool      beginTrigger;             // a trigger used for embedded timers
  _GRVY_Type_TimerMap2     TimerMap;  // map used to store performance timers for each defined key

  accumulator_set <double,features<tag::mean,tag::count,tag::variance> > stats_empty; // empty accumulator

 public:
  GRVY_Timer_Class      ();
  void   Reset          ();
  void   Summarize      ();
  void   VerifyInit     ();

  void   SaveTimerName  (const char *id);
  void   BeginTimer     (const char *name, bool embeddedFlag);
  void   EndTimer       (const char *name, bool embeddedFlag);

  double ElapsedSeconds (const char *id);
  int    StatsCount     (const char *id);
  double StatsMean      (const char *id);
  double StatsVariance  (const char *id);
  double ElapsedGlobal  ();
  double RawTimer       ();

  // Historical Timing Functions

  int InitHistDB           (const char *filename);
  int SaveHistTiming       ();
  int SaveHistTiming       (const char *id, double timing);
  void SummarizeHistTiming (const char *filename);
  

};

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

    // C++ ostream style logging (<<)

    template <typename T> 
    inline GRVY_Log_Class operator <<(const T &val) {{std::cout << val;}};
    inline GRVY_Log_Class operator <<(std::ostream &(*pf)(std::ostream&)) {{std::cout << *pf;}};

    // Basic std::string message

    void msg       (int priority, std::string msg);                 // post new log message with a priority

    // C style printf logging

    int  msg_printf(int priority, const char *format,va_list argp); // post printf style log message
  

  private:
    int log_level;			                            // Current log level priority
    std::map<int,std::string> LogMask;                              // String masks for log messages

  };

//--------------------------
// HDF5 Utility Class
//--------------------------

  class GRVY_HDF5_Class {
  private: 

#ifdef HAVE_HDF5
    hid_t m_fileId;		        // hdf5 file handle
    H5E_auto2_t error_orig_func;	// error-handle func
    void       *error_orig_data;        // error-handle stack data
#endif

  public:
    GRVY_HDF5_Class      ();
#ifdef HAVE_HDF5
    int  file_create(const char *filename, bool overwrite_existing);
    int  close();
    void silence_hdf_error_handler();
    void restore_hdf_error_handler();
#endif
  };

}

#endif
