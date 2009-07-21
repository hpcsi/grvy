// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
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
// -------------------------------------------------------------------------
//
// hpct_classes.h: Basic class definitions
//
// $Id: hpct.h 2422 2009-04-19 19:33:42Z karl $
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include<map>
#include<vector>
#include<string>
#include<GetPot>
#include<boost/accumulators/accumulators.hpp>
#include<boost/accumulators/statistics/mean.hpp>
#include<boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include<boost/accumulators/statistics/variance.hpp>

using namespace std;
using namespace boost::accumulators;

namespace HPCT {

//---------------------
// Input Parsing Class
//---------------------

class HPCT_Input_Class {
 private:
  GetPot    ifile;		                     // input file
  short int initialized;	                     // input file initialized?
  short int silent;				     // Silence error messages?

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
  HPCT_Input_Class  ();
  void Initialize   ();
  int  VerifyInit   ();
  int  Open         (const char *filename);
  int  Close        ();
  void MsgToggle    (int flag);
  void ErrorMsg     (const char *func_name, const char *var);

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

typedef struct HPCT_Timer_Data {
  double timings[2];
  accumulator_set <double,stats<tag::mean,tag::count,tag::variance> > stats;
} tTimer_Data;

typedef map <std::string, HPCT_Timer_Data > _HPCT_Type_TimerMap2;

class HPCT_Timer_Class {
 private:
  short int initialized;	  // class is initialized
  double    timer_last;           // timer value at last call
  string    timer_name;		  // user name supplied for the timer
  _HPCT_Type_TimerMap2 TimerMap;  // map used to store performance timers for each defined key

  accumulator_set <double,stats<tag::mean,tag::count,tag::variance> > stats_empty; // empty accumulator

 public:
  HPCT_Timer_Class      ();
  void Reset            ();
  void Summarize        ();
  void VerifyInit       ();

  void SaveTimerName    (const char *id);
  void BeginTimer       (const char *name);
  void EndTimer         (const char *name);

  double ElapsedSeconds (const char *id);
  double RawTimer       ();
};

}
