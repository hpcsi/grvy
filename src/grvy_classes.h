// -*-c++-*-
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
// Copyright (C) 2008,2009,2010 The PECOS Development Team
//
// Please see http://pecos.ices.utexas.edu for more information.
//
// This file is part of the PECOS GRVY Toolkit
//
// GRVY is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GRVY is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GRVY.  If not, see <http://www.gnu.org/licenses/>.
//
// -------------------------------------------------------------------------
//
// grvy_classes.h: Basic class definitions
//
// $Id$
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#ifndef GRVY_CLASSES_H_
#define GRVY_CLASSES_H_

#include<limits>
#include<map>
#include<vector>
#include<string>

// We use our own namespace for GetPot to avoid collisions if we're
// linked against a different version
#define GETPOT_NAMESPACE GRVYGetPot
// And we don't support threaded GetPot usage yet
#define GETPOT_DISABLE_MUTEX
#include<common/getpot/getpot.h>
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

using namespace std;
using namespace boost::accumulators;

namespace GRVY {

//---------------------
// Input Parsing Class
//---------------------

class GRVY_Input_Class {
 private:
  GRVYGetPot::GetPot ifile;          // input file
  short int          initialized;    // input file initialized?

  // Registry Maps

  map<std::string, int         > default_ints;
  map<std::string, float       > default_floats;
  map<std::string, double      > default_doubles;
  map<std::string, std::string > default_strings;

  // GetPot Defaults

  float  Float_Def;
  double Double_Def;
  int    Int_Def;
  long   Long_Def;
  const char  *Char_Def;
  const char  *comment_start;
  const char  *comment_end;

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

  template <typename T> int Read_Var      (const char *var, T *value, T vardef);
  template <typename T> int Read_Var_Vec  (const char *var, T *value, int nelems, T vardef);
  template <typename T> int Read_Var_iVec (const char *var, T *value, int elem,   T vardef);

  // String Read Functions

  int Read_Var                            (const char *var, char **value);
  int Read_Var_iVec                       (const char *var, char **value, int elem);

  // Default Variable Registration Functions

  void Register_Var  (const char *varname, int     var);
  void Register_Var  (const char *varname, float   var);
  void Register_Var  (const char *varname, double  var);
  void Register_Var  (const char *varname, char   *var);

  int  Get_Var       (const char *varname, int    *var);
  int  Get_Var       (const char *varname, float  *var);
  int  Get_Var       (const char *varname, double *var);
  int  Get_Var       (const char *varname, char  **var);

  void PrintRegVars  (const char *prefix);
};

//--------------------------
// Performance Timing Class
//--------------------------

typedef struct GRVY_Timer_Data {
  double timings[2];
  accumulator_set <double,features<tag::mean,tag::count,tag::variance> > stats;
} tTimer_Data;

typedef map <std::string, GRVY_Timer_Data > _GRVY_Type_TimerMap2;

class GRVY_Timer_Class {
 private:
  short int initialized;          // initialized?
  double    timer_last;           // raw timer value of last call
  double    timer_finalize;       // raw timer value at time of finalize()
  string    timer_name;           // user name supplied for the timer
  _GRVY_Type_TimerMap2 TimerMap;  // map used to store performance timers for each defined key

  accumulator_set <double,features<tag::mean,tag::count,tag::variance> > stats_empty; // empty accumulator

 public:
  GRVY_Timer_Class      ();
  void Reset            ();
  void Summarize        ();
  void VerifyInit       ();

  void SaveTimerName    (const char *id);
  void BeginTimer       (const char *name);
  void EndTimer         (const char *name);

  double ElapsedSeconds (const char *id);
     int StatsCount     (const char *id);
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
    map<int,std::string> LogMask;                      // String masks for log messages
  public:
    GRVY_Log_Class();		

    bool isLog(int priority) {                                      // inlined log priority test
      return(priority <= log_level); }                              
    void msg       (int priority, string msg);                      // post new log message with a priority
    int  msg_printf(int priority, const char *format,va_list argp); // post printf style log message
    void change_priority(int priority);                             // change current log level priority

  };

}

#endif
