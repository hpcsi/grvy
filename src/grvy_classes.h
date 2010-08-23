// -*-c++-*-

//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
//
// grvy_classes.h: Basic class definitions
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#ifndef GRVY_CLASSES_H_
#define GRVY_CLASSES_H_

#include<limits>
#include<map>
#include<vector>
#include<string>
#include<stack>

// We use our own namespace for GetPot to avoid collisions if we're
// linked against a different version

#define GETPOT_NAMESPACE GRVYGetPot

// And we don't support threaded GetPot usage yet

#define GETPOT_DISABLE_MUTEX
#include "getpot.h"

// But we don't want to confuse any other GetPot version that might be
// included through a later header

#undef GETPOT_DISABLE_MUTEX
#undef GETPOT_NAMESPACE

#include<boost/accumulators/accumulators.hpp>
#include<boost/accumulators/statistics/mean.hpp>
#include<boost/accumulators/statistics/stats.hpp>
#include<boost/accumulators/statistics/count.hpp>
#include<boost/accumulators/statistics/variance.hpp>
#include<boost/math/special_functions.hpp>

using namespace boost::accumulators;

namespace GRVY {

//---------------------
// Versioning Routines
//---------------------

  void GRVY_version_stdout();
  int  GRVY_get_numeric_version();

//---------------------
// Input Parsing Class
//---------------------

class GRVY_Input_Class {
 private:
  GRVYGetPot::GetPot ifile;          // input file
  short int          initialized;    // input file initialized?

  // Registry Maps

  std::map<std::string, int         > default_ints;
  std::map<std::string, float       > default_floats;
  std::map<std::string, double      > default_doubles;
  std::map<std::string, bool        > default_bools;
  std::map<std::string, std::string > default_strings;

  // GetPot Defaults

  float  Float_Def;
  double Double_Def;
  int    Int_Def;
  long   Long_Def;
  std::string String_Def;
  const char  *Char_Def;
  const char  *comment_start;
  const char  *comment_end;

  template <typename T> T Get_Default   (T);

 public:
  GRVY_Input_Class  ();
  void Initialize   ();
  int  VerifyInit   ();
  int  Open         (const char *filename);
  int  Close        ();
  void MsgToggle    (int flag);

  int  Fdump        ();
  int  Fdump        (const char *prefix);
  int  Fdump        (const char *prefix, const char *filename);

  // Numerical Read Functions

  template <typename T> int Read_Var      (const char *var, T *value );
  template <typename T> int Read_Var_Vec  (const char *var, T *value, int nelems);
  template <typename T> int Read_Var_iVec (const char *var, T *value, int elem);

  template <typename T> int Read_Var      (const char *var, T *value, T vardef);
  template <typename T> int Read_Var_Vec  (const char *var, T *value, int nelems, T vardef);
  template <typename T> int Read_Var_iVec (const char *var, T *value, int elem,   T vardef);

  // Character String Read Functions

  int Read_Var                            (const char *var, char **value);
  int Read_Var_iVec                       (const char *var, char **value, int elem);

  // C++ String Read Functions

  int Read_Var                            (const char *var, std::string *value);
  int Read_Var                            (const char *var, std::string *value, std::string vardef);
  //  int Read_Var_iVec                       (const char *var, char **value, int elem);

  // Bool Read Functions

  int Read_Var                            (const char *var, bool *value, bool vardef);

  // Default Variable Registration Functions

  void Register_Var  (const char *varname, int     var);
  void Register_Var  (const char *varname, float   var);
  void Register_Var  (const char *varname, double  var);
  void Register_Var  (const char *varname, bool    var);
  void Register_Var  (const char *varname, char   *var);
  void Register_Var  (const char *varname, std::string var);

  int  Get_Var       (const char *varname, int    *var);
  int  Get_Var       (const char *varname, float  *var);
  int  Get_Var       (const char *varname, double *var);
  int  Get_Var       (const char *varname, char  **var);
  int  Get_Var       (const char *varname, std::string *var);

  void PrintRegVars  (const char *prefix);
};

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
  private:
    int log_level;			               // Current log level priority
    std::map<int,std::string> LogMask;                      // String masks for log messages
  public:
    GRVY_Log_Class();		

    bool isLog(int priority) {                                      // inlined log priority test
      return(priority <= log_level); }                              
    void msg       (int priority, std::string msg);                      // post new log message with a priority
    int  msg_printf(int priority, const char *format,va_list argp); // post printf style log message
    void change_priority(int priority);                             // change current log level priority

  };

}

#endif
