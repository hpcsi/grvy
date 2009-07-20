// -*-c++-*-
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//
// Copyright (C) 2008,2009 The PECOS Development Team
//
// Please see http://pecos.ices.utexas.edu for more information.
//
// This file is part of the PECOS HPC Toolkit (HPCT)
//
// HPCT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// HPCT is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HPCT.  If not, see <http://www.gnu.org/licenses/>.
//
//--------------------------------------------------------------------------
//
// hpct_int.h: Internals for HPCT
//
// $Id: hpct.h 32 2009-01-07 05:55:46Z karl $
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<map>
#include<vector>
#include<string>

namespace HPCT {

#ifdef _HPCT_GLOBAL_DEF
#define GLOBAL
#else
#define GLOBAL extern
#endif

// Internal Routines

extern void _HPCT_message (const char *mask, const char *func, const char *message);
extern void _HPCT_message (const char *mask, const char *func, const char *message, const char *char_item);
extern void _HPCT_message (const char *mask, const char *func, const char *message, int int_item);
extern void _HPCT_message (const char *mask, const char *func, const char *message, float float_item);
extern void _HPCT_message (const char *mask, const char *func, const char *message, double double_item);

extern char *hpct_f2c_char(char*input,int len);

// Evil Globals Vars

#ifdef _HPCT_GLOBAL_DEF

GLOBAL const char *_HPCT_emask   = "[*] Error";        // default error mask
GLOBAL const char *_HPCT_wmask   = "[*] Warning";      // default warning mask
GLOBAL const char *_HPCT_imask   = "[*] Info";         // default info mask
GLOBAL const char *_HPCT_gtimer  = "HPCT_Unassigned";  // default global timer key

HPCT_Timer_Class *_HPCT_Timers               =  NULL;   // performance timer 
GLOBAL double _HPCT_TIMER_THRESH       = 9.0e-8;  // low water mark for expected timer usage deltas        
GLOBAL double _HPCT_PERC_TOL           = 1e-3;    // tolerance for defining acceptable global percentages

#else

GLOBAL const char *_HPCT_emask;
GLOBAL const char *_HPCT_wmask;
GLOBAL const char *_HPCT_imask;
GLOBAL const char *_HPCT_gtimer;

GLOBAL HPCT_Timer_Class *_HPCT_Timers;
GLOBAL double _HPCT_TIMER_THRESH;
GLOBAL double _HPCT_PERC_TOL;

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

typedef map<std::string,vector <double> >                      _HPCT_Type_TimerMap;
typedef map<std::vector <double>,std::string,TimerCmpHighLow > _HPCT_Type_TimerMapSortHL;  
typedef map<std::vector <double>,std::string,TimerCmpLowHigh > _HPCT_Type_TimerMapSortLH;  

}
