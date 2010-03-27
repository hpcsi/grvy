// -*-c++-*-
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
//
// grvy_int.h: Internals for GRVY
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<map>
#include<vector>
#include<string>

namespace GRVY {

#ifdef _GRVY_GLOBAL_DEF
#define GLOBAL
#else
#define GLOBAL extern
#endif

  // Internal Routines

  //  extern void _GRVY_message (const char *mask, const char *func, const char *message);

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
    bool operator()(const vector <double> v1, const vector <double> v2 )  {
      return ( v1[0] - v2[0] ) > 0; }
  };

  class TimerCmpLowHigh {
  public:
    bool operator()(const vector <double> v1, const vector <double> v2 )  {
      return ( v1[0] - v2[0] ) < 0; }
  };

  typedef map<std::string,vector <double> >                      _GRVY_Type_TimerMap;
  typedef map<std::vector <double>,std::string,TimerCmpHighLow > _GRVY_Type_TimerMapSortHL;  
  typedef map<std::vector <double>,std::string,TimerCmpLowHigh > _GRVY_Type_TimerMapSortLH;  

}
