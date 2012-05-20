// -*-c++-*-
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011,2012 The PECOS Development Team
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
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#define _GRVY_GLOBAL_DEF

#include<stdlib.h>
#include<string.h>
#include "grvy_classes.h"
#include "grvy.h"
#include "grvy_int.h"
#include "fortran_string_order.h"

#include <boost/algorithm/string.hpp>

// We use our own namespace for GetPot to avoid collisions if we're
// linked against a different version

#define GETPOT_NAMESPACE GRVYGetPot

// And we don't support threaded GetPot usage yet

#define GETPOT_DISABLE_MUTEX
#include "getpot.h"

using namespace std;
using namespace GRVY;

// GetPot constants

const float     Float_Def = -9999999.0f;
const double   Double_Def = -9999999.0e1;
const int         Int_Def = -9999999;
const long       Long_Def = -9999999;
const char*      Char_Def = "unknown";

static GRVY_Input_Class _GRVY_Input;     // input class


//-------------------------------------------------------
// private implementation class definition for GRVY_Input
//-------------------------------------------------------

namespace GRVY {

  class GRVY_Input_Class::GRVY_Input_ClassImp
  {
    
  public:
    GRVY_Input_ClassImp    () {}
   ~GRVY_Input_ClassImp    () {}

    //void Initialize   ();
    int  VerifyInit   ();
    void PrintRegVars  (const char *prefix);

    GRVYGetPot::GetPot* ifile;    // input file
    bool initialized;     	  // input file initialized?

    // Registry Maps

    std::map<std::string, int         > default_ints;
    std::map<std::string, float       > default_floats;
    std::map<std::string, double      > default_doubles;
    std::map<std::string, bool        > default_bools;
    std::map<std::string, std::string > default_strings;
    
    // GetPot Defaults
    
    float  Float_Def;
    double Double_Def;
    int    Int_Def;
    long   Long_Def;
    std::string String_Def;
    const char  *Char_Def;
    const char  *comment_start;
    const char  *comment_end;
    
    template <typename T> T Get_Default   (T);
    
    GRVY_Timer_Class *self;	       // back pointer to public class
    
  private:

  };

} // matches namespace GRVY

  //-------------------------------------
  // GRVY_Input_Class:: Member Functions
  //-------------------------------------

namespace GRVY {
  
  GRVY_Input_Class::GRVY_Input_Class() :m_pimpl(new GRVY_Input_ClassImp() )
  {
    m_pimpl->Float_Def     = -9999999.0f;
    m_pimpl->Double_Def    = -9999999.0e1;
    m_pimpl->Int_Def       = -9999999;
    m_pimpl->Long_Def      = -9999999;
    m_pimpl->Char_Def      = "unknown";
    m_pimpl->String_Def    = "unknown";
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

  //  int GRVY_Input_Class::Open(const char *filename)
  //  {
  //    return(m_pimpl->Open(filename));
  //  }

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

  void GRVY_Input_Class::MsgToggle(int flag)
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
    return(Int_Def);
  }

  template <> float GRVY_Input_Class::GRVY_Input_ClassImp::Get_Default<float>(float)
  {
    return(Float_Def);
  }

  template <> double GRVY_Input_Class::GRVY_Input_ClassImp::Get_Default<double>(double)
  {
    return(Double_Def);
  }

  template <> std::string GRVY_Input_Class::GRVY_Input_ClassImp::Get_Default<std::string>(std::string)
  {
    return(String_Def);
  }


  template <typename T> int GRVY_Input_Class:: Read_Var(const char *var, T *value, T Var_Def)
  {
    if(! m_pimpl->VerifyInit()) return 0;

    if(Var_Def != m_pimpl->Get_Default(Var_Def) )
      {
	grvy_printf(GRVY_DEBUG,"Registering user-supplied default value for %s\n",var);
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
    return(Read_Var(var,value,m_pimpl->String_Def) );
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
	else 
	  {
	    _GRVY_message(GRVY_INFO,"fread_string","Using pre-registered value for variable",var);
	  }
      }
    return 1;
  }

  int GRVY_Input_Class:: Read_Var_iVec(const char *var, std::string *value, int elem)
  {
    int i;

    if(! m_pimpl->VerifyInit()) return 0;

    *value = (*m_pimpl->ifile)(var,m_pimpl->String_Def.c_str(),elem);

    if(*value == m_pimpl->String_Def)
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

    grvy_printf(GRVY_DEBUG,"Registering user-supplied default bool value for %s\n",var);
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
  
    tstring = (*m_pimpl->ifile)(var,Char_Def);
    *value = (char *) malloc(tstring.length()*sizeof(char)+1);
    strcpy(value[0],tstring.c_str());

    if(strcmp(*value,m_pimpl->Char_Def) == 0)
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

    if( ! m_pimpl->VerifyInit() ) return 0;
  
    tstring = (*m_pimpl->ifile)(var,Char_Def,elem);
    *value = (char *) malloc(tstring.length()*sizeof(char)+1);
    strcpy(value[0],tstring.c_str());

    if(strcmp(*value,m_pimpl->Char_Def) == 0)
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

//-----------------------------------------------------------------
//                       ye ol' C Interfaces
//-----------------------------------------------------------------

extern "C" int grvy_input_fopen(const char *filename)
{

  // with the switch over to adopt iso_c_bindings for our Fortran
  // interface, it's possible that Fortran is calling this routine
  // directly; consequently, we perform a little extra whitspace
  // trimming here to show some love...

  std::string file_new = filename;

  boost::trim(file_new);

  return( _GRVY_Input.Open(file_new.c_str()) );
}

extern "C" int grvy_input_fdump()
{
  return( _GRVY_Input.Fdump() );
}

extern "C" int grvy_input_fdump_delim(const char *prefix)
{
  return( _GRVY_Input.Fdump(prefix) );
}

extern "C" int grvy_input_fdump_file(const char *prefix, const char *filename)
{
  return( _GRVY_Input.Fdump(prefix,filename) );
}

extern "C" int grvy_input_fclose()
{
  return( _GRVY_Input.Close() );
}

extern "C" int grvy_input_fread_float(const char *var,float *value)
{
  return( _GRVY_Input.Read_Var(var,value,Float_Def) );
}

extern "C" int grvy_input_fread_float_vec(const char *var,float *value,int nelems)
{
  return( _GRVY_Input.Read_Var_Vec(var,value,nelems,Float_Def) );
}

extern "C" int grvy_input_fread_float_ivec(const char *var,float *value,int elem)
{
  return( _GRVY_Input.Read_Var_iVec(var,value,elem,Float_Def) );
}

extern "C" int grvy_input_fread_double(const char *var,double *value)
{
  return( _GRVY_Input.Read_Var(var,value,Double_Def) );
}

extern "C" int grvy_input_fread_logical_from_int(const char *var,int *value, int default_value)
{
  bool flag;
  bool default_bool  = (default_value  == 1) ? true : false ;
  int return_value   = _GRVY_Input.Read_Var(var,&flag,default_bool);

  *value = (flag) ? 1 : 0;
  return(return_value);
}

extern "C" int grvy_input_fread_double_vec(const char *var,double *value,int nelems)
{
  return( _GRVY_Input.Read_Var_Vec(var,value,nelems,Double_Def) );
}

extern "C" int grvy_input_fread_double_ivec(const char *var,double *value,int elem)
{
  return( _GRVY_Input.Read_Var_iVec(var,value,elem,Double_Def) );
}

extern "C" int grvy_input_fread_int(const char *var,int *value)
{
  return( _GRVY_Input.Read_Var(var,value,Int_Def) );
}

extern "C" int grvy_input_fread_int_vec(const char *var,int *value,int nelems)
{
  return( _GRVY_Input.Read_Var_Vec(var,value,nelems,Int_Def) );
}

extern "C" int grvy_input_fread_int_ivec(const char *var,int *value,int elem)
{
  return( _GRVY_Input.Read_Var_iVec(var,value,elem,Int_Def) );
}

extern "C" int grvy_input_fread_char(const char *var, char **value)
{
  return( _GRVY_Input.Read_Var(var,value) );
}

extern "C" int grvy_input_fread_char_ivec(const char *var,char **value,int elem)
{
  return( _GRVY_Input.Read_Var_iVec(var,value,elem) );
}

// Variable Registration Routines

extern "C" int grvy_input_register_int(const char *var,int value)
{
  _GRVY_Input.Register_Var(var,value);
  return 1;
}

extern "C" int grvy_input_register_float(const char *var,float value)
{
  _GRVY_Input.Register_Var(var,value);
  return 1;
}

extern "C" int grvy_input_register_double(const char *var,double value)
{
  _GRVY_Input.Register_Var(var,value);
  return 1;
}

extern "C" int grvy_input_register_char       (const char *var,char *value)
{
  _GRVY_Input.Register_Var(var,value);
  return 1;
}

extern "C" int grvy_input_register_get_int    (const char *var,int *value)
{
  return( _GRVY_Input.Get_Var(var,value) );
}

extern "C" int grvy_input_register_get_float  (const char *var,float *value)
{
  return( _GRVY_Input.Get_Var(var,value) );
}

extern "C" int grvy_input_register_get_double (const char *var,double *value)
{
  return( _GRVY_Input.Get_Var(var,value) );
}

extern "C" int grvy_input_register_get_char   (const char *var,char **value)
{
  return( _GRVY_Input.Get_Var(var,value) );
}

//-----------------------------------------------------------------
//                     Fortran Interfaces
// 
// update: 12/2010 - deprecating many of these versions in favor of 
// using iso_c_binding....
//-----------------------------------------------------------------

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fread_char_(char *var,char *value,int *flag,int _namelen, int _storage)
#else
extern "C" void grvy_input_fread_char_(char *var,int _namelen,char *value,int _storage,int *flag)
#endif
{

  char *tmpvar;

  // Initialize storage provided by user to be empty; this will allow
  // the Fortran len_trim() intrinsic to behave as expected by Fortran
  // users.  Thanks to Marco Panesi for pointing out the undesired
  // Fortran behavior.

  for(int i=0;i<_storage;i++)
    value[i]=' ';

  // Note: tiny bit sneaky; for C/C++ we will receive a char * back
  // with the correct amount of storage space for the desired input string.
  // For Fortran, we will require that the space be allocated in advance and
  // hence, we need an extra copy here after completing the call.

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_char(name,&tmpvar);

  // make sure user provided enough space

  if(strlen(tmpvar) > _storage)
    {
      _GRVY_message(GRVY_ERROR,__func__,"Provided character storage is too small (string not read)");
      *flag=0;
    }
  else
    strncpy(value,tmpvar,strlen(tmpvar));

  delete[] name;
  return;
}


#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fread_char_ivec_(char *var,char *value,int *elem,int *flag,
					    int _namelen, int _storage)
#else
extern "C" void grvy_input_fread_char_ivec_(char *var,int _namelen,char *value,
					    int _storage,int *elem,int *flag)
#endif
{

  char *tmpvar;
  char *name = grvy_f2c_char(var,_namelen);
  int cindex;

  // Convert from Fortran to C style indexing

  cindex  = *elem - 1;

  // Read the desired string

  *flag = grvy_input_fread_char_ivec(name,&tmpvar,cindex);

  // Initialize storage provided by user to be empty; this will allow
  // the Fortran len_trim() intrinsic to behave as expected by Fortran
  // users.  Thanks to Marco Panesi for pointing out the undesired
  // Fortran behavior.

  for(int i=0;i<_storage;i++)
    value[i]=' ';

  if(strlen(tmpvar) > _storage)
    {
      _GRVY_message(GRVY_ERROR,__func__,"Provided character storage is too small (string not read)");
      *flag=0;
    }
  else
    strncpy(value,tmpvar,strlen(tmpvar));

  delete[] name;
  free(tmpvar);
  return;
}

// DEPRECATED function; can be controlled via grvy_log_setlevel

#if 0
extern "C" void grvy_input_toggle_messages_(int *flag)
{
  grvy_input_toggle_messages(*flag);
  return;
}
#endif

// Variable Registration Routines

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_register_int_(char *var,int *value,int *flag,int _namelen)
#else
extern "C" void grvy_input_register_int_(char *var,int _namelen,int *value,int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_register_int(name,*value);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_register_float_(char *var,float *value,int *flag,int _namelen)
#else
extern "C" void grvy_input_register_float_(char *var,int _namelen,float *value,int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_register_float(name,*value);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_register_double_(char *var,double *value,int *flag,int _namelen)
#else
extern "C" void grvy_input_register_double_(char *var,int _namelen,double *value,int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_register_double(name,*value);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_register_char_(char *var,char *value,int *flag,int _namelen, int _storage)
#else
extern "C" void grvy_input_register_char_(char *var,int _namelen,char *value,int _storage, int *flag)
#endif
{

  char *name   = grvy_f2c_char(var,  _namelen);
  char *string = grvy_f2c_char(value,_storage);

  *flag = grvy_input_register_char(name,string);

  delete[] name;
  delete[] string;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_register_get_int_    (char *var,int *value,int *flag,int _namelen)
#else
extern "C" void grvy_input_register_get_int_    (char *var,int _namelen,int *value,int *flag)
#endif
{
  char *name = grvy_f2c_char(var,_namelen);
  *flag = _GRVY_Input.Get_Var(name,value);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_register_get_float_  (char *var,float *value,int *flag,int _namelen)
#else
extern "C" void grvy_input_register_get_float_  (char *var,int _namelen,float *value,int *flag)
#endif
{
  char *name = grvy_f2c_char(var,_namelen);
  *flag =  _GRVY_Input.Get_Var(name,value);

  delete[] name;
  return;
}


#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_register_get_double_ (char *var,double *value,int *flag,int _namelen)
#else
extern "C" void grvy_input_register_get_double_ (char *var,int _namelen,double *value,int *flag)
#endif
{
  char *name = grvy_f2c_char(var,_namelen);
  *flag = _GRVY_Input.Get_Var(name,value);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_register_get_char_   (char *var,char *value,int *flag,int _namelen,int _storage)
#else
extern "C" void grvy_input_register_get_char_   (char *var,int _namelen,char *value,int _storage,int *flag)
#endif
{
  char *tmpvar;

  // Note: tiny bit sneaky; for C/C++ we will receive a char * back
  // with the correct amount of storage space for the desired input string.
  // For Fortran, we will require that the space be allocated in advance and
  // hence, we need an extra copy here after completing the call.

  char *name = grvy_f2c_char(var,_namelen);
  *flag = _GRVY_Input.Get_Var(name,&tmpvar);

  strncpy(value,tmpvar,strlen(tmpvar));

  delete[] name;
  return;
}

