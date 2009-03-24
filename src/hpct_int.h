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

const char *_HPCT_emask   = "[*] Error";       // default error mask
const char *_HPCT_wmask   = "[*] Warning";     // default warning mask
const char *_HPCT_gtimer  = "HPCT_Unassigned"; // default global timer key

// Timer Foo

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

// Messaging Foo

extern void _HPCT_message(const char *mask, const char *func, const char *message);
extern void _HPCT_message(const char *mask, const char *func, const char *message, const char *char_item);
extern void _HPCT_message(const char *mask, const char *func, const char *message, int int_item);
extern void _HPCT_message(const char *mask, const char *func, const char *message, float float_item);
extern void _HPCT_message(const char *mask, const char *func, const char *message, double double_item);

// Utilities

extern char *hpct_f2c_char(char*input,int len);
