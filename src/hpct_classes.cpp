//--------------------------------------------------------------------------
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
// hpct.h: Basic class member functions.
//
// $Id: hpct.h 2422 2009-04-19 19:33:42Z karl $
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include "hpct_classes.h"
#include "hpct_int.h"

// HPCT_Input_Class:: Member Functions

void HPCT_Input_Class:: Register_Var (const char *varname, int var)
{
  default_ints[varname] = var;
  return;
}

void HPCT_Input_Class:: Register_Var (const char *varname, float var)
{
  default_floats[varname] = var;
  return;
}

void HPCT_Input_Class:: Register_Var (const char *varname, double var)
{
  default_doubles[varname] = var;
  return;
}

void HPCT_Input_Class:: Register_Var (const char *varname, char *var)
{
  default_strings[varname] = var;
  return;
}

int HPCT_Input_Class:: Get_Var (const char *varname, int *var)
{
  map<std::string, int > :: const_iterator index;
  
  index = default_ints.find(varname);

  if( index == default_ints.end() )
    {
      if(_HPCT_Input_Output_Errors)
	_HPCT_message(_HPCT_emask,__func__,"No registered variable named",varname);
      return(0);
    }
  else
    {
      *var = index->second;
      return(1);
    }

}

int HPCT_Input_Class:: Get_Var (const char *varname, float *var)
{
  map<std::string, float > :: const_iterator index;
  
  index = default_floats.find(varname);

  if( index == default_floats.end() )
    {
      if(_HPCT_Input_Output_Errors)
	_HPCT_message(_HPCT_emask,__func__,"No registered variable named",varname);
      return(0);
    }
  else
    {
      *var = index->second;
      return(1);
    }

}

int HPCT_Input_Class:: Get_Var (const char *varname, double *var)
{
  map<std::string, double > :: const_iterator index;
  
  index = default_doubles.find(varname);

  if( index == default_doubles.end() )
    {
      if(_HPCT_Input_Output_Errors)
	_HPCT_message(_HPCT_emask,__func__,"No registered variable named",varname);
      return(0);
    }
  else
    {
      *var = index->second;
      return(1);
    }

}

int HPCT_Input_Class:: Get_Var (const char *varname, char **var)
{
  map<std::string, string > :: const_iterator index;
  string tstring;
  
  index = default_strings.find(varname);

  if( index == default_strings.end() )
    {
      if(_HPCT_Input_Output_Errors)
	_HPCT_message(_HPCT_emask,__func__,"No registered variable named",varname);
      return(0);
    }
  else
    {
      tstring = index->second;
      *var = (char *) malloc(tstring.length()*sizeof(char)+1);
      strcpy(var[0],tstring.c_str());
      return(1);
    }

}


