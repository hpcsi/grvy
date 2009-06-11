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
// hpct.h: Basic class definitions
//
// $Id: hpct.h 2422 2009-04-19 19:33:42Z karl $
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include<map>
#include<vector>
#include<string>
#include<GetPot>

using namespace std;

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
  char  *Char_Def;
  char  *comment_start;
  char  *comment_end;

 public:
  HPCT_Input_Class  ();
  void Initialize   ();
  int  VerifyInit   ();
  int  Open         (const char *filename);
  int  Close        ();

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
};


