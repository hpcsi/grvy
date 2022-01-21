// -*-c++-*-
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008-2013,2018-2022 The PECOS Development Team
// Additional Copyright (C) 2018 individual authors
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
// grvy_input: Simple ASCII input file parsing using GetPot()
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#define _GRVY_GLOBAL_DEF

#include<stdlib.h>
#include<string.h>
#include "grvy_classes.h"
#include "grvy.h"
#include "grvy_int.h"

// We use our own namespace for GetPot to avoid collisions if we're
// linked against a different version

#define GETPOT_NAMESPACE GRVYGetPot

// And we don't support threaded GetPot usage yet

#define GETPOT_DISABLE_MUTEX
#include <GetPot>

using namespace std;
using namespace GRVY;

namespace GRVY_Internal {

  // GetPot-related constants (used to decide if we do not have a
  // default value)

  const float     Float_Def = -9999999.0f;
  const double   Double_Def = -9999999.0e1;
  const int         Int_Def = -9999999;
  const long       Long_Def = -9999999;
  const char*      Char_Def = "unknown";

  const std::string  String_Def("unknown");
}


//static GRVY_Input_Class _GRVY_Input;     // input class


//-------------------------------------------------------
// private implementation class definition for GRVY_Input
//-------------------------------------------------------

namespace GRVY {

  class GRVY_Input_Class::GRVY_Input_ClassImp
  {
    
  public:
    GRVY_Input_ClassImp    () {}
   ~GRVY_Input_ClassImp    () {}

    int  VerifyInit        ();
    void PrintRegVars      (const char *prefix);

    GRVYGetPot::GetPot* ifile;    // input file
    bool initialized;     	  // input file initialized?

    // Registry Maps

    std::map<std::string, int         > default_ints;
    std::map<std::string, float       > default_floats;
    std::map<std::string, double      > default_doubles;
    std::map<std::string, bool        > default_bools;
    std::map<std::string, std::string > default_strings;
    
    // GetPot Defaults
    
    const char  *comment_start;
    const char  *comment_end;
    
    template <typename T> T Get_Default   (T);
    
    GRVY_Timer_Class *self;	  // back pointer to public class
    
  private:

  };

} // matches namespace GRVY

  //-------------------------------------
  // GRVY_Input_Class:: Member Functions
  //-------------------------------------

namespace GRVY {
  
  GRVY_Input_Class::GRVY_Input_Class() :m_pimpl(new GRVY_Input_ClassImp() )
  {
    m_pimpl->comment_start = "#";
    m_pimpl->comment_end   = "\n";
    m_pimpl->initialized   = false;
  }

  GRVY_Input_Class::~GRVY_Input_Class()
  {
    // using auto_ptr for proper cleanup
  }

  int GRVY_Input_Class::GRVY_Input_ClassImp::VerifyInit()
  {
    if(!initialized)
      {
	_GRVY_message(GRVY_ERROR,__func__,"uninitialized input file - verify file has been opened");
	return 0;
      }
    else
      return 1;
  }

  int GRVY_Input_Class::Open(const char *filename)
  {
    if(m_pimpl->initialized)
      {
	_GRVY_message(GRVY_ERROR,__func__,"close previous input file first prior to opening new input file -> ",
		      filename);
	return 0;
      }

    // --------------------------------------------------------------
    // DOS newline check - GetPot can't handle; warn the user to use
    // dos2unix
    // --------------------------------------------------------------

    FILE *fp = fopen(filename,"r");

    if(fp == NULL )
      {
      	_GRVY_message(GRVY_ERROR,__func__,"unable to open input file -> ",filename);
	return 0;
      }
    else
      {
	int tmpchar;
	
	while((tmpchar = getc(fp)) != EOF) {
	  if(tmpchar == '\x0d') {

	    grvy_printf(GRVY_ERROR,"\nDOS newline detected - unable to parse file (%s)\n",filename);
	    grvy_printf(GRVY_ERROR,"Consider running dos2unix on the file to remove existing DOS newlines.\n\n");

	    fclose(fp);
	    return 0;
	  } 
	}

	fclose(fp);
      }

    m_pimpl->ifile = new GETPOT_NAMESPACE::GetPot(filename,m_pimpl->comment_start,m_pimpl->comment_end);

    if(m_pimpl->ifile->size() <= 1)
      {
	_GRVY_message(GRVY_ERROR,__func__,"non-existent or empty file -> ",filename);
	return 0;
      }
    else
      {
	m_pimpl->initialized=true;
	return 1;
      }
  }

  int GRVY_Input_Class::Load(std::stringstream &buffer)
  {
    if(m_pimpl->initialized)
      {
	_GRVY_message(GRVY_ERROR,__func__,"close previous input file first prior to loading new input");
	return 0;
      }

    m_pimpl->ifile = new GETPOT_NAMESPACE::GetPot(buffer,m_pimpl->comment_start,m_pimpl->comment_end);

    if(m_pimpl->ifile->size() <= 1)
      {
	_GRVY_message(GRVY_ERROR,__func__,"non-existent or empty buffer contents");
	return 0;
      }
    else
      {
	m_pimpl->initialized=true;
	return 1;
      }

  }

  int GRVY_Input_Class::Close()
  {
    if(m_pimpl->initialized)
      {
	m_pimpl->initialized=false;
	delete m_pimpl->ifile;
      }
    return 1;
  }

  int GRVY_Input_Class::Fdump()
  {
    if(! m_pimpl->VerifyInit()) return 0;

    m_pimpl->ifile->print();		// dump the raw file contents
    m_pimpl->PrintRegVars("");		// include any registered defaults

    return 1;
  }

  int GRVY_Input_Class::Fdump(const char *prefix)
  {

    if(! m_pimpl->VerifyInit()) return 0;

    m_pimpl->ifile->print(prefix);		// dump the raw file contents
    m_pimpl->PrintRegVars(prefix);		// include any registered defaults

    return 1;
  }

  int GRVY_Input_Class::Fdump(const char *prefix, const char *filename)
  {

    if(! m_pimpl->VerifyInit()) return 0;

    std::streambuf *cout_sbuf = std::cout.rdbuf();             // save original stdout buff
    std::ofstream fout(filename,std::ios::app|std::ios::out);  // file for append
    std::cout.rdbuf(fout.rdbuf());                             // redirect cout to file        

    m_pimpl->ifile->print(prefix);			       // dumpe the raw file contents
    m_pimpl->PrintRegVars(prefix);		               // include any registered defaults

    std::cout.rdbuf(cout_sbuf);                                // restore cout stream

    return 1;
  }

  void GRVY_Input_Class::GRVY_Input_ClassImp::PrintRegVars(const char *prefix)
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

  //--------------
  // Scalar Reads
  //--------------

  template <typename T> int GRVY_Input_Class:: Read_Var(std::string var, T *value)
  {
    return (Read_Var( var.c_str(),value,m_pimpl->Get_Default(*value)));
  }

  template <typename T> int GRVY_Input_Class:: Read_Var(const char *var, T *value)
  {
    return (Read_Var( var,value,m_pimpl->Get_Default(*value)));
  }

  template <typename T> int GRVY_Input_Class:: Read_Var_Vec(std::string var, T *value, int nelem)
  {
    return (Read_Var_Vec( var.c_str(),value,nelem,m_pimpl->Get_Default(*value)));
  }

  template <typename T> int GRVY_Input_Class:: Read_Var_Vec(const char *var, T *value, int nelem)
  {
    return (Read_Var_Vec( var,value,nelem,m_pimpl->Get_Default(*value)));
  }

  template <typename T> int GRVY_Input_Class:: Read_Var_iVec(std::string var, T *value, int elem)
  {
    return(Read_Var_iVec( var.c_str(),value,elem,m_pimpl->Get_Default(*value)));
  }

  template <typename T> int GRVY_Input_Class:: Read_Var_iVec(const char *var, T *value, int elem)
  {
    return(Read_Var_iVec( var,value,elem,m_pimpl->Get_Default(*value)));
  }

  template<> int GRVY_Input_Class::GRVY_Input_ClassImp::Get_Default<int>(int var)
  {
    return(GRVY_Internal::Int_Def);
  }

  template <> float GRVY_Input_Class::GRVY_Input_ClassImp::Get_Default<float>(float)
  {
    return(GRVY_Internal::Float_Def);
  }

  template <> double GRVY_Input_Class::GRVY_Input_ClassImp::Get_Default<double>(double)
  {
    return(GRVY_Internal::Double_Def);
  }

  template <> std::string GRVY_Input_Class::GRVY_Input_ClassImp::Get_Default<std::string>(std::string)
  {
    return(GRVY_Internal::String_Def);
  }


  template <typename T> int GRVY_Input_Class:: Read_Var(const char *var, T *value, T Var_Def)
  {
    if(! m_pimpl->VerifyInit()) return 0;

    if(Var_Def != m_pimpl->Get_Default(Var_Def) )
      {
        // grvy_printf(GRVY_DEBUG,"Registering user-supplied default value for %s\n",var);
	Register_Var(var,Var_Def);
      }

    *value = (*m_pimpl->ifile)(var,Var_Def);

    if(*value == Var_Def)
      {
	if( !Get_Var(var,value) )
	  {
	    _GRVY_message(GRVY_ERROR,"fread","Unable to query variable -> ",var);
	    return 0;
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

    if(! m_pimpl->VerifyInit()) return 0;

    for(i=0;i<nelems;i++)
      {
	value[i] = (*m_pimpl->ifile)(var,Var_Def,i);

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

    if(! m_pimpl->VerifyInit()) return 0;

    *value = (*m_pimpl->ifile)(var,Var_Def,elem);

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
    return(Read_Var(var,value,GRVY_Internal::String_Def) );
  }

  int GRVY_Input_Class:: Read_Var(const char *var, std::string *value, std::string Var_Def)
  {

    if(! m_pimpl->VerifyInit()) return 0;

    if(Var_Def != m_pimpl->Get_Default(Var_Def) )
      {
	grvy_printf(GRVY_DEBUG,"Registering user-supplied default value for %s\n",var);
	Register_Var(var,Var_Def);
      }
  
    *value = (*m_pimpl->ifile)(var,Var_Def.c_str());

    if( *value == Var_Def )
      {
	if( !Get_Var(var,value) )
	  {
	    _GRVY_message(GRVY_ERROR,"fread_string","Unable to query variable -> ",var);
	    return 0;
	  }
#if 0
	else 
	  {
	    _GRVY_message(GRVY_INFO,"fread_string","Using pre-registered value for variable",var);
	  }
#endif
      }
    return 1;
  }

  int GRVY_Input_Class:: Read_Var_iVec(const char *var, std::string *value, int elem)
  {
    int i;

    if(! m_pimpl->VerifyInit()) return 0;

    *value = (*m_pimpl->ifile)(var,GRVY_Internal::String_Def.c_str(),elem);

    if(*value == GRVY_Internal::String_Def)
      {
	_GRVY_message(GRVY_ERROR,"fread_ivec_string","Unable to query variable -> ",var);
	return 0;
      }
 
    return 1;
  } 

  int GRVY_Input_Class:: Read_Var(std::string var, bool *value, bool Var_Def)
  {
    return (Read_Var( var.c_str(),value,Var_Def));
  }

  int GRVY_Input_Class:: Read_Var(const char *var, bool *value, bool Var_Def)
  {

    if( !m_pimpl->VerifyInit() ) return 0;

    // All boolean queries must supply a default so let's register them.

    grvy_printf(GRVY_DEBUG,"Registering user-supplied default bool value for %s (%i)\n",var,Var_Def);
    Register_Var(var,Var_Def);
  
    *value = (*m_pimpl->ifile)(var,Var_Def);

    return 1;
  }

  //------------------------
  // Character String Reads
  //------------------------

  int GRVY_Input_Class:: Read_Var(const char *var, char **value)
  {
    std::string tstring;

    if( !m_pimpl->VerifyInit() ) return 0;
  
    tstring = (*m_pimpl->ifile)(var,GRVY_Internal::Char_Def);
    *value = (char *) malloc(tstring.length()*sizeof(char)+1);
    strcpy(value[0],tstring.c_str());

    if(strcmp(*value,GRVY_Internal::Char_Def) == 0)
      {

	if( !Get_Var(var,value) )
	  {
	    _GRVY_message(GRVY_ERROR,"fread_char","Unable to query variable -> ",var);
	    return 0;
	  }
#if 0
	else 
	  {
	    _GRVY_message(GRVY_INFO,"fread_char","Using pre-registered value for variable",var);
	  }
#endif
      }
    return 1;
  }

  int GRVY_Input_Class:: Read_Var_iVec(const char *var, char **value, int elem)
  {
    std::string tstring;

    if( ! m_pimpl->VerifyInit() ) return 0;
  
    tstring = (*m_pimpl->ifile)(var,GRVY_Internal::Char_Def,elem);
    *value = (char *) malloc(tstring.length()*sizeof(char)+1);
    strcpy(value[0],tstring.c_str());

    if(strcmp(*value,GRVY_Internal::Char_Def) == 0)
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
    m_pimpl->default_ints[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, float var)
  {
    m_pimpl->default_floats[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, double var)
  {
    m_pimpl->default_doubles[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, const char *var)
  {
    m_pimpl->default_strings[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, std::string var)
  {
    m_pimpl->default_strings[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, bool var)
  {
    m_pimpl->default_bools[varname] = var;
    return;
  }

  int GRVY_Input_Class:: Get_Var (const char *varname, int *var)
  {
    std::map<std::string, int > :: const_iterator index;
  
    index = m_pimpl->default_ints.find(varname);

    if( index == m_pimpl->default_ints.end() )
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
  
    index = m_pimpl->default_floats.find(varname);

    if( index == m_pimpl->default_floats.end() )
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
  
    index = m_pimpl->default_doubles.find(varname);

    if( index == m_pimpl->default_doubles.end() )
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
  
    index = m_pimpl->default_strings.find(varname);

    if( index == m_pimpl->default_strings.end() )
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
  
    index = m_pimpl->default_strings.find(varname);

    if( index == m_pimpl->default_strings.end() )
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

  template int GRVY_Input_Class::Read_Var <int>          (std::string var, int    *value);
  template int GRVY_Input_Class::Read_Var <float>        (std::string var, float  *value);
  template int GRVY_Input_Class::Read_Var <double>       (std::string var, double *value);

  template int GRVY_Input_Class::Read_Var_Vec <int>      (std::string var, int    *value, int nelem);
  template int GRVY_Input_Class::Read_Var_Vec <float>    (std::string var, float  *value, int nelem);
  template int GRVY_Input_Class::Read_Var_Vec <double>   (std::string var, double *value, int nelem);

  template int GRVY_Input_Class::Read_Var_iVec <int>     (std::string var, int    *value, int elem);
  template int GRVY_Input_Class::Read_Var_iVec <float>   (std::string var, float  *value, int elem);
  template int GRVY_Input_Class::Read_Var_iVec <double>  (std::string var, double *value, int elem);

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


} // end GRVY namespace

