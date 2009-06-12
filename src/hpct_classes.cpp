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
// hpct.h: Basic class member functions.
//
// $Id: hpct.h 2422 2009-04-19 19:33:42Z karl $
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include "hpct_classes.h"
#include "hpct_int.h"

//-------------------------------------
// HPCT_Input_Class:: Member Functions
//-------------------------------------

HPCT_Input_Class::HPCT_Input_Class()   // default constructor
{
  Float_Def     = -9999999.0f;
  Double_Def    = -9999999.0e1;
  Int_Def       = -9999999;
  Long_Def      = -9999999;
  Char_Def      = "unknown";
  comment_start = "#";
  comment_end   = "\n";

  initialized   = 0;

  // Convention is to assume that user wants error messages.
  // Thou shalt turn them off otherwise.

  silent        = 0; 

}

int HPCT_Input_Class:: VerifyInit()
{
  if(!initialized)
    {
      printf("\n%s (%s): uninitialized input file - verify file has been opened\n",
             _HPCT_emask,__func__);
      return 0;
    }
  else
    return 1;

}

int HPCT_Input_Class:: Open(const char *filename)
{
  ifile = GetPot(filename,comment_start,comment_end);

  if(ifile.size() <= 1)
    {
      if(!silent)
	printf("\n%s (%s): non-existent or empty file -> %s\n",_HPCT_emask,__func__,filename);
      return 0;
    }
  else
    {
      initialized=1;
      return 1;
    }
}

int HPCT_Input_Class:: Close()
{
  initialized=0;
  return 1;
}

int HPCT_Input_Class:: Fdump()
{
  if(! VerifyInit()) return 0;

  ifile.print();		// dump the raw file contents
  PrintRegVars("");		// include any registered defaults

  return 1;
}

int HPCT_Input_Class:: Fdump(const char *prefix)
{

  if(! VerifyInit()) return 0;

  ifile.print(prefix);		// dump the raw file contents
  PrintRegVars(prefix);		// include any registered defaults

  return 1;
}

int HPCT_Input_Class:: Fdump(const char *prefix, const char *filename)
{

  if(! VerifyInit()) return 0;

  std::streambuf *cout_sbuf = std::cout.rdbuf();   // save original stdout buff
  std::ofstream fout(filename,ios::app|ios::out);  // file for append
  std::cout.rdbuf(fout.rdbuf());                   // redirect cout to file        

  ifile.print(prefix);				   // dumpe the raw file contents
  PrintRegVars(prefix);		                   // include any registered defaults
  std::cout.rdbuf(cout_sbuf);                      // restore cout stream

  return 1;
}

void HPCT_Input_Class::PrintRegVars(const char *prefix)
{

  map<std::string, int    > :: const_iterator int_index;
  map<std::string, float  > :: const_iterator flt_index;
  map<std::string, double > :: const_iterator dbl_index;
  map<std::string, string > :: const_iterator str_index;

  cout << prefix << "[HPCT Registered Variable Default Values]" << endl;

  for(int_index=default_ints.begin(); int_index != default_ints.end(); ++int_index)
    cout << prefix << (int_index->first).c_str() << "=" << int_index->second << endl;

  for(flt_index=default_floats.begin(); flt_index != default_floats.end(); ++flt_index)
    cout << prefix << (flt_index->first).c_str() << "=" << flt_index->second << endl;

  for(dbl_index=default_doubles.begin(); dbl_index != default_doubles.end(); ++dbl_index)
    cout << prefix << (dbl_index->first).c_str() << "=" << dbl_index->second << endl;

  for(str_index=default_strings.begin(); str_index != default_strings.end(); ++str_index)
    cout << prefix << (str_index->first).c_str() << "=" << str_index->second << endl;

  cout << endl;
}

void HPCT_Input_Class:: ErrorMsg(const char *func_name,const char *var_name)
{
  if(!silent)
    printf("\n%s (%s): unable to query variable -> %s\n",_HPCT_emask,func_name,var_name);
}

void HPCT_Input_Class:: MsgToggle(int flag)
{
  if(flag == 0)
    silent = 1;
  else
    silent = 0;
}

//--------------
// Scalar Reads
//--------------

template <typename T> int HPCT_Input_Class:: Read_Var(const char *var, T *value, T Var_Def)
{
  if(! VerifyInit()) return 0;

  *value = ifile(var,Var_Def);

  if(*value == Var_Def)
    {
      if( !Get_Var(var,value) )
	{
	  ErrorMsg("fread",var);
	  return 0;
	}
      else
	{
	  if(!silent)
	    _HPCT_message(_HPCT_imask,"fread","Using pre-registered value for variable",var);
	}
    }
  
  return 1;
}

//--------------
// Vector Reads
//--------------

template <typename T> int HPCT_Input_Class:: Read_Var_Vec(const char *var, T *value, int nelems,T Var_Def)
{
  int i;

  if(! VerifyInit()) return 0;

  for(i=0;i<nelems;i++)
    {
      value[i] = ifile(var,Var_Def,i);

      if(value[i] == Var_Def)
        {
	  ErrorMsg("fread_ivec",var);
          return 0;
        }
    }
 
 return 1;
}

//------------------
// ith Vector Reads
//------------------

template <typename T> int HPCT_Input_Class:: Read_Var_iVec(const char *var, T *value, int elem,T Var_Def)
{
  int i;

  if(! VerifyInit()) return 0;

  *value = ifile(var,Var_Def,elem);

  if(*value == Var_Def)
    {
      ErrorMsg("fread_ivec",var);
      return 0;
    }
 
 return 1;
} 

//------------------------
// Character String Reads
//------------------------

int HPCT_Input_Class:: Read_Var(const char *var, char **value)
{
  string tstring;

  if(! VerifyInit()) return 0;
  
  tstring = ifile(var,Char_Def);
  *value = (char *) malloc(tstring.length()*sizeof(char)+1);
  strcpy(value[0],tstring.c_str());

  if(strcmp(*value,Char_Def) == 0)
    {

      if( !Get_Var(var,value) )
	{
	  ErrorMsg("fread_char",var);
	  return 0;
	}
      else 
	{
	  if(!silent)
	    _HPCT_message(_HPCT_imask,"fread_char","Using pre-registered value for variable",var);
	}
    }
  //  else
  return 1;
}

int HPCT_Input_Class:: Read_Var_iVec(const char *var, char **value, int elem)
{
  string tstring;

  if(! VerifyInit()) return 0;
  
  tstring = ifile(var,Char_Def,elem);
  *value = (char *) malloc(tstring.length()*sizeof(char)+1);
  strcpy(value[0],tstring.c_str());

  if(strcmp(*value,Char_Def) == 0)
    {
      ErrorMsg("fread_char_ivec",var);
      return 0;
    }
  else
    return 1;
}

//---------------------------------------
// Default Variable Value Registrations
//---------------------------------------

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
      if(!silent)
	_HPCT_message(_HPCT_imask,"register_get","No registered variable named",varname);
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
      if(!silent)
	_HPCT_message(_HPCT_imask,"register_get","No registered variable named",varname);
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
      if(!silent)
	_HPCT_message(_HPCT_imask,"register_get","No registered variable named",varname);
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
      if(!silent)
	_HPCT_message(_HPCT_imask,"register_get","No registered variable named",varname);
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

//------------------------------
// Supported Function Templates
//------------------------------

template int HPCT_Input_Class::Read_Var <int>          (const char *var, int    *value, int    vardef);
template int HPCT_Input_Class::Read_Var <float>        (const char *var, float  *value, float  vardef);
template int HPCT_Input_Class::Read_Var <double>       (const char *var, double *value, double vardef);

template int HPCT_Input_Class:: Read_Var_Vec <int>     (const char *var, int    *value, int nelem, int    Var_Def);
template int HPCT_Input_Class:: Read_Var_Vec <float>   (const char *var, float  *value, int nelem, float  Var_Def);
template int HPCT_Input_Class:: Read_Var_Vec <double>  (const char *var, double *value, int nelem, double Var_Def);

template int HPCT_Input_Class:: Read_Var_iVec <int>    (const char *var, int    *value, int elem,  int    Var_Def);
template int HPCT_Input_Class:: Read_Var_iVec <float>  (const char *var, float  *value, int elem,  float  Var_Def);
template int HPCT_Input_Class:: Read_Var_iVec <double> (const char *var, double *value, int elem,  double Var_Def);
