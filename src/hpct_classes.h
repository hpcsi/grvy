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

using namespace std;

class HPCT_Input_Class {
 private:
  //  GetPot ifile;		      // input file
  int Initialized;		      // input file initialized?
  map<std::string, int         > default_ints;      
  map<std::string, float       > default_floats;    
  map<std::string, double      > default_doubles;
  map<std::string, std::string > default_strings;

 public:
  void Initialize();

  void Register_Var (const char *varname,    int var);
  void Register_Var (const char *varname,  float var);
  void Register_Var (const char *varname, double var);
  void Register_Var (const char *varname,  char *var);

  int Get_Var      (const char *varname,    int *var);
  int Get_Var      (const char *varname,  float *var);
  int Get_Var      (const char *varname, double *var);
  int Get_Var      (const char *varname,  char **var);

};


