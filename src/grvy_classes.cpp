// -*-c++-*-
//-----------------------------------------------------------------------bl-
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
//-----------------------------------------------------------------------el-
//
// grvy_classes.cpp: Basic class member functions.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<sys/time.h>
#include<stdarg.h>
#include<time.h>
#include "grvy_classes.h"
#include "grvy.h"
#include "grvy_int.h"

// We use our own namespace for GetPot to avoid collisions if we're
// linked against a different version
#define GETPOT_NAMESPACE GRVYGetPot

// And we don't support threaded GetPot usage yet
#define GETPOT_DISABLE_MUTEX
#include "getpot.h"

namespace GRVY {

  //-------------------------------------
  // Versioning Routines
  //-------------------------------------

  void GRVY_version_stdout()
  {

    if(_GRVY_Log.isLog(GRVY_INFO))
      {
	std::cout << "--------------------------------------------------------" << std::endl;
	std::cout << "libGRVY Library: Version = " << GRVY_LIB_VERSION; 
	std::cout << " (" << GRVY_get_numeric_version() << ")" << std::endl << std::endl; 
	
	std::cout << GRVY_LIB_RELEASE << std::endl << std::endl;
	
	std::cout << "Build Date   = " << GRVY_BUILD_DATE     << std::endl;
	std::cout << "Build Host   = " << GRVY_BUILD_HOST     << std::endl;
	std::cout << "Build User   = " << GRVY_BUILD_USER     << std::endl;
	std::cout << "Build Arch   = " << GRVY_BUILD_ARCH     << std::endl;
	std::cout << "Build Rev    = " << GRVY_BUILD_VERSION  << std::endl << std::endl;
	
	std::cout << "C++ Config   = " << GRVY_CXX << " " << GRVY_CXXFLAGS << std::endl;
	std::cout << "F90 Config   = " << GRVY_FC  << " " << GRVY_FCFLAGS  << std::endl;
	std::cout << "--------------------------------------------------------" << std::endl;
      }
  }

  int GRVY_get_numeric_version()
  {
    // Note: return format follows the versioning convention xx.yy.zz where
    // 
    // xx = major version number
    // yy = minor version number
    // zz = micro version number
    // 
    // For example:
    // v.   0.23  -> 002300 = 2300
    // v   0.23.1 -> 002301 = 2301
    // v. 10.23.2 -> 102302

    int major_version = 0;
    int minor_version = 0;
    int micro_version = 0;

#ifdef GRVY_MAJOR_VERSION
    major_version = GRVY_MAJOR_VERSION;
#endif

#ifdef GRVY_MINOR_VERSION
    minor_version = GRVY_MINOR_VERSION;
#endif

#ifdef GRVY_MICRO_VERSION
    micro_version = GRVY_MICRO_VERSION;
#endif
      
    return(major_version*10000 + minor_version*100 + micro_version);

  }

  //-------------------------------------
  // GRVY_Input_Class:: Member Functions
  //-------------------------------------

  GRVY_Input_Class::GRVY_Input_Class()   // default constructor
  {
    Float_Def     = -9999999.0f;
    Double_Def    = -9999999.0e1;
    Int_Def       = -9999999;
    Long_Def      = -9999999;
    Char_Def      = "unknown";
    String_Def    = "unknown";
    comment_start = "#";
    comment_end   = "\n";

    initialized   = 0;
  }

  // Moved clean up of ifile to Close

#if 0
  GRVY_Input_Class::~GRVY_Input_Class()
  {
    delete ifile;
  }
#endif

  int GRVY_Input_Class:: VerifyInit()
  {
    if(!initialized)
      {
	_GRVY_message(GRVY_ERROR,__func__,"uninitialized input file - verify file has been opened");
	return 0;
      }
    else
      return 1;

  }

  int GRVY_Input_Class:: Open(const char *filename)
  {
    if(initialized)
      {
	_GRVY_message(GRVY_ERROR,__func__,"close previous input file first prior to opening new input file -> ",
		      filename);
	return 0;
      }

    ifile = new GETPOT_NAMESPACE::GetPot(filename,comment_start,comment_end);

    if(ifile->size() <= 1)
      {
	_GRVY_message(GRVY_ERROR,__func__,"non-existent or empty file -> ",filename);
	return 0;
      }
    else
      {
	initialized=1;
	return 1;
      }
  }

  int GRVY_Input_Class:: Close()
  {
    if(initialized)
      {
	initialized=0;
	delete ifile;
      }
    return 1;
  }

  int GRVY_Input_Class:: Fdump()
  {
    if(! VerifyInit()) return 0;

    ifile->print();		// dump the raw file contents
    PrintRegVars("");		// include any registered defaults

    return 1;
  }

  int GRVY_Input_Class:: Fdump(const char *prefix)
  {

    if(! VerifyInit()) return 0;

    ifile->print(prefix);		// dump the raw file contents
    PrintRegVars(prefix);		// include any registered defaults

    return 1;
  }

  int GRVY_Input_Class:: Fdump(const char *prefix, const char *filename)
  {

    if(! VerifyInit()) return 0;

    std::streambuf *cout_sbuf = std::cout.rdbuf();   // save original stdout buff
    std::ofstream fout(filename,std::ios::app|std::ios::out);  // file for append
    std::cout.rdbuf(fout.rdbuf());                   // redirect cout to file        

    ifile->print(prefix);				   // dumpe the raw file contents
    PrintRegVars(prefix);		                   // include any registered defaults
    std::cout.rdbuf(cout_sbuf);                      // restore cout stream

    return 1;
  }

  void GRVY_Input_Class::PrintRegVars(const char *prefix)
  {

    std::map<std::string, int    > :: const_iterator int_index;
    std::map<std::string, float  > :: const_iterator flt_index;
    std::map<std::string, double > :: const_iterator dbl_index;
    std::map<std::string, bool   > :: const_iterator bool_index;
    std::map<std::string, std::string > :: const_iterator str_index;

    std::cout << prefix << "[GRVY Registered Variable Default Values]" << std::endl;

    for(int_index=default_ints.begin(); int_index != default_ints.end(); ++int_index)
      std::cout << prefix << (int_index->first).c_str() << "=" << int_index->second << std::endl;

    for(flt_index=default_floats.begin(); flt_index != default_floats.end(); ++flt_index)
      std::cout << prefix << (flt_index->first).c_str() << "=" << flt_index->second << std::endl;

    for(dbl_index=default_doubles.begin(); dbl_index != default_doubles.end(); ++dbl_index)
      std::cout << prefix << (dbl_index->first).c_str() << "=" << dbl_index->second << std::endl;

    for(str_index=default_strings.begin(); str_index != default_strings.end(); ++str_index)
      std::cout << prefix << (str_index->first).c_str() << "=" << str_index->second << std::endl;

    for(bool_index=default_bools.begin(); bool_index != default_bools.end(); ++bool_index)
      std::cout << prefix << (bool_index->first).c_str() << "=" << bool_index->second << std::endl;

    std::cout << std::endl;
  }

  void GRVY_Input_Class:: MsgToggle(int flag)
  {
    if(flag == 0)
      {
	_GRVY_Log.change_priority(GRVY_NOLOG);
      }
    else
      {
	_GRVY_Log.change_priority(GRVY_INFO);
      }

    return;
  }

  //--------------
  // Scalar Reads
  //--------------

  template <typename T> int GRVY_Input_Class:: Read_Var(const char *var, T *value)
  {
    return (Read_Var( var,value,Get_Default(*value)));
  }

  template <typename T> int GRVY_Input_Class:: Read_Var_Vec(const char *var, T *value, int nelem)
  {
    return (Read_Var_Vec( var,value,nelem,Get_Default(*value)));
  }

  template <typename T> int GRVY_Input_Class:: Read_Var_iVec(const char *var, T *value, int elem)
  {
    return(Read_Var_iVec( var,value,elem,Get_Default(*value)));
  }

  template<> int GRVY_Input_Class::Get_Default<int>(int var)
  {
    return(Int_Def);
  }

  template <> float GRVY_Input_Class::Get_Default<float>(float)
  {
    return(Float_Def);
  }

  template <> double GRVY_Input_Class::Get_Default<double>(double)
  {
    return(Double_Def);
  }

  template <> std::string GRVY_Input_Class::Get_Default<std::string>(std::string)
  {
    return(String_Def);
  }


  template <typename T> int GRVY_Input_Class:: Read_Var(const char *var, T *value, T Var_Def)
  {
    if(! VerifyInit()) return 0;

    if(Var_Def != Get_Default(Var_Def) )
      {
	grvy_printf(GRVY_DEBUG,"Registering user-supplied default value for %s\n",var);
	Register_Var(var,Var_Def);
      }

    *value = (*ifile)(var,Var_Def);

    if(*value == Var_Def)
      {
	if( !Get_Var(var,value) )
	  {
	    _GRVY_message(GRVY_ERROR,"fread","Unable to query variable -> ",var);
	    return 0;
	  }
	else
	  {
	    _GRVY_message(GRVY_INFO,"fread: Using pre-registered value for variable",var,*value);
	  }
      }
  
    return 1;
  }

  //--------------
  // Array Reads
  //--------------

  template <typename T> int GRVY_Input_Class:: Read_Var_Vec(const char *var, T *value, int nelems,T Var_Def)
  {
    int i;

    if(! VerifyInit()) return 0;

    for(i=0;i<nelems;i++)
      {
	value[i] = (*ifile)(var,Var_Def,i);

	if(value[i] == Var_Def)
	  {
	    _GRVY_message(GRVY_ERROR,"fread_ivec","Unable to query variable -> ",var);
	    return 0;
	  }
      }
 
    return 1;
  }

  //-------------------------
  // ith Element Array Reads
  //-------------------------

  template <typename T> int GRVY_Input_Class:: Read_Var_iVec(const char *var, T *value, int elem,T Var_Def)
  {
    int i;

    if(! VerifyInit()) return 0;

    *value = (*ifile)(var,Var_Def,elem);

    if(*value == Var_Def)
      {
	_GRVY_message(GRVY_ERROR,"fread_ivec","Unable to query variable -> ",var);
	return 0;
      }
 
    return 1;
  } 

  //------------------------
  // C++ String Reads
  //------------------------

  int GRVY_Input_Class:: Read_Var(const char *var, std::string *value)
  {
    return(Read_Var(var,value,String_Def) );
  }

  int GRVY_Input_Class:: Read_Var(const char *var, std::string *value, std::string Var_Def)
  {

    if(! VerifyInit()) return 0;

    if(Var_Def != Get_Default(Var_Def) )
      {
	grvy_printf(GRVY_DEBUG,"Registering user-supplied default value for %s\n",var);
	Register_Var(var,Var_Def);
      }
    
  
    *value = (*ifile)(var,Var_Def.c_str());

    if( *value == Var_Def )
      {
	if( !Get_Var(var,value) )
	  {
	    _GRVY_message(GRVY_ERROR,"fread_string","Unable to query variable -> ",var);
	    return 0;
	  }
	else 
	  {
	    _GRVY_message(GRVY_INFO,"fread_string","Using pre-registered value for variable",var);
	  }
      }
    return 1;
  }

  int GRVY_Input_Class:: Read_Var(const char *var, bool *value, bool Var_Def)
  {

    if(! VerifyInit()) return 0;

    // All boolean queries must supply a default so let's register them.

    grvy_printf(GRVY_DEBUG,"Registering user-supplied default bool value for %s\n",var);
    Register_Var(var,Var_Def);
  
    *value = (*ifile)(var,Var_Def);

    return 1;
  }

  //------------------------
  // Character String Reads
  //------------------------

  int GRVY_Input_Class:: Read_Var(const char *var, char **value)
  {
    std::string tstring;

    if(! VerifyInit()) return 0;
  
    tstring = (*ifile)(var,Char_Def);
    *value = (char *) malloc(tstring.length()*sizeof(char)+1);
    strcpy(value[0],tstring.c_str());

    if(strcmp(*value,Char_Def) == 0)
      {

	if( !Get_Var(var,value) )
	  {
	    _GRVY_message(GRVY_ERROR,"fread_char","Unable to query variable -> ",var);
	    return 0;
	  }
	else 
	  {
	    _GRVY_message(GRVY_INFO,"fread_char","Using pre-registered value for variable",var);
	  }
      }
    return 1;
  }

  int GRVY_Input_Class:: Read_Var_iVec(const char *var, char **value, int elem)
  {
    std::string tstring;

    if(! VerifyInit()) return 0;
  
    tstring = (*ifile)(var,Char_Def,elem);
    *value = (char *) malloc(tstring.length()*sizeof(char)+1);
    strcpy(value[0],tstring.c_str());

    if(strcmp(*value,Char_Def) == 0)
      {
	_GRVY_message(GRVY_ERROR,"fread_char_ivec","Unable to query variable -> ",var);
	return 0;
      }
    else
      return 1;
  }

  //---------------------------------------
  // Default Variable Value Registrations
  //---------------------------------------

  void GRVY_Input_Class:: Register_Var (const char *varname, int var)
  {
    default_ints[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, float var)
  {
    default_floats[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, double var)
  {
    default_doubles[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, const char *var)
  {
    default_strings[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, std::string var)
  {
    default_strings[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, bool var)
  {
    default_bools[varname] = var;
    return;
  }

  int GRVY_Input_Class:: Get_Var (const char *varname, int *var)
  {
    std::map<std::string, int > :: const_iterator index;
  
    index = default_ints.find(varname);

    if( index == default_ints.end() )
      {
	_GRVY_message(GRVY_INFO,"register_get","No registered variable named",varname);
	return(0);
      }
    else
      {
	*var = index->second;
	return(1);
      }

  }

  int GRVY_Input_Class:: Get_Var (const char *varname, float *var)
  {
    std::map<std::string, float > :: const_iterator index;
  
    index = default_floats.find(varname);

    if( index == default_floats.end() )
      {
	_GRVY_message(GRVY_INFO,"register_get","No registered variable named",varname);
	return(0);
      }
    else
      {
	*var = index->second;
	return(1);
      }

  }

  int GRVY_Input_Class:: Get_Var (const char *varname, double *var)
  {
    std::map<std::string, double > :: const_iterator index;
  
    index = default_doubles.find(varname);

    if( index == default_doubles.end() )
      {
	_GRVY_message(GRVY_INFO,"register_get","No registered variable named",varname);
	return(0);
      }
    else
      {
	*var = index->second;
	return(1);
      }

  }

  int GRVY_Input_Class:: Get_Var (const char *varname, char **var)
  {
    std::map<std::string, std::string > :: const_iterator index;
    std::string tstring;
  
    index = default_strings.find(varname);

    if( index == default_strings.end() )
      {
	_GRVY_message(GRVY_INFO,"register_get","No registered variable named",varname);
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

  int GRVY_Input_Class:: Get_Var (const char *varname, std::string *var)
  {
    std::map<std::string, std::string > :: const_iterator index;
  
    index = default_strings.find(varname);

    if( index == default_strings.end() )
      {
	_GRVY_message(GRVY_INFO,"register_get","No registered variable named",varname);
	return(0);
      }
    else
      {
	*var = index->second;
	return(1);
      }
  }

  //------------------------------
  // Supported Function Templates
  //------------------------------

  // (a) cases with no default value....

  template int GRVY_Input_Class::Read_Var <int>          (const char *var, int    *value);
  template int GRVY_Input_Class::Read_Var <float>        (const char *var, float  *value);
  template int GRVY_Input_Class::Read_Var <double>       (const char *var, double *value);

  template int GRVY_Input_Class::Read_Var_Vec <int>      (const char *var, int    *value, int nelem);
  template int GRVY_Input_Class::Read_Var_Vec <float>    (const char *var, float  *value, int nelem);
  template int GRVY_Input_Class::Read_Var_Vec <double>   (const char *var, double *value, int nelem);

  template int GRVY_Input_Class::Read_Var_iVec <int>     (const char *var, int    *value, int elem);
  template int GRVY_Input_Class::Read_Var_iVec <float>   (const char *var, float  *value, int elem);
  template int GRVY_Input_Class::Read_Var_iVec <double>  (const char *var, double *value, int elem);

  // (a) cases with default value provided....

  template int GRVY_Input_Class::Read_Var <int>          (const char *var, int    *value, int    vardef);
  template int GRVY_Input_Class::Read_Var <float>        (const char *var, float  *value, float  vardef);
  template int GRVY_Input_Class::Read_Var <double>       (const char *var, double *value, double vardef);

  template int GRVY_Input_Class::Read_Var_Vec <int>      (const char *var, int    *value, int nelem, int    Var_Def);
  template int GRVY_Input_Class::Read_Var_Vec <float>    (const char *var, float  *value, int nelem, float  Var_Def);
  template int GRVY_Input_Class::Read_Var_Vec <double>   (const char *var, double *value, int nelem, double Var_Def);

  template int GRVY_Input_Class::Read_Var_iVec <int>     (const char *var, int    *value, int elem,  int    Var_Def);
  template int GRVY_Input_Class::Read_Var_iVec <float>   (const char *var, float  *value, int elem,  float  Var_Def);
  template int GRVY_Input_Class::Read_Var_iVec <double>  (const char *var, double *value, int elem,  double Var_Def);




  //-------------------------------------
  // GRVY_Math_Class:: Member Functions
  //-------------------------------------

  GRVY_Math_Class::GRVY_Math_Class( ) // default constructor
  {
    return;
  }

  //-------------------------------------
  // GRVY_Log_Class:: Member Functions
  //-------------------------------------

  GRVY_Log_Class::GRVY_Log_Class()
  {

    // set default log level
    
    log_level = GRVY_INFO;

    // set default log masks for each priority level

#if 0
    LogMask[GRVY_FATAL] = "[*] Fatal: ";
    LogMask[GRVY_ERROR] = "[*] Error: ";
    LogMask[GRVY_WARN ] = "[*]  Warn: ";
    LogMask[GRVY_INFO ] = "[*]  Info: ";
    LogMask[GRVY_DEBUG] = "[*] Debug: ";
#else
    LogMask[GRVY_FATAL] = "";
    LogMask[GRVY_ERROR] = "";
    LogMask[GRVY_WARN ] = "";
    LogMask[GRVY_INFO ] = "";
    LogMask[GRVY_DEBUG] = "";
#endif

    return;
  }

  void GRVY_Log_Class::change_priority(int priority)
  {
    log_level = priority;
    return;
  }

  void GRVY_Log_Class::change_mask(int priority, const std::string delimiter)
  {
    LogMask[priority] = delimiter;
    return;
  }

  void GRVY_Log_Class::msg(int priority, const std::string msg)
  {
    if(isLog(priority))
      {
	std::cout << LogMask[priority];
	std::cout << msg << std::endl;
      }

    return;
  }

  // C style printf logging

  int GRVY_Log_Class::msg_printf(int priority, const char *format,va_list arg)
  {
    int done;

    printf("%s",LogMask[priority].c_str());
    done = vprintf (format, arg);
    return done;
  }

} // matches namespace grvy
