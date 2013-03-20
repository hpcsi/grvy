// -*-c++-*-
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
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
// grvy_input_interface: C interface for input parsingroutines
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<stdio.h>
#include<string.h>
#include<grvy_classes.h>
#include<grvy_int.h>
#include<grvy.h>
#include "fortran_string_order.h"
#include<string>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace GRVY;

namespace GRVY_Internal {

  // GetPot-related constants (used to decide if we do not have a
  // default value)

  const float     Float_Def = -9999999.0f;
  const double   Double_Def = -9999999.0e1;
  const int         Int_Def = -9999999;
  const long       Long_Def = -9999999;

}

// Utility function to create new input parsing class on 1st call for
// C interface

std::auto_ptr<GRVY_Input_Class> _GRVY_Input;

namespace GRVY_Internal {
  void verify_C_parser_allocated();
}

void GRVY_Internal::verify_C_parser_allocated()
{

  if(_GRVY_Input.get() == NULL)
    _GRVY_Input.reset (new GRVY_Input_Class());

  return;
}

//-------------
// C Interface
//-------------

extern "C" int grvy_input_fopen(const char *filename)
{
  GRVY_Internal::verify_C_parser_allocated();

  // with the switch over to adopt iso_c_bindings for our Fortran
  // interface, it's possible that Fortran is calling this routine
  // directly; consequently, we perform a little extra whitspace
  // trimming here to show some love...

  std::string file_new = filename;

  boost::trim(file_new);

  return( _GRVY_Input->Open(file_new.c_str()) );
}

extern "C" int grvy_input_fdump()
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Fdump() );
}

extern "C" int grvy_input_fdump_delim(const char *prefix)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Fdump(prefix) );
}

extern "C" int grvy_input_fdump_file(const char *prefix, const char *filename)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Fdump(prefix,filename) );
}

extern "C" int grvy_input_fclose()
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Close() );
}

extern "C" int grvy_input_fread_float(const char *var,float *value)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Read_Var(var,value,GRVY_Internal::Float_Def) );
}

extern "C" int grvy_input_fread_float_vec(const char *var,float *value,int nelems)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Read_Var_Vec(var,value,nelems,GRVY_Internal::Float_Def) );
}

extern "C" int grvy_input_fread_float_ivec(const char *var,float *value,int elem)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Read_Var_iVec(var,value,elem,GRVY_Internal::Float_Def) );
}

extern "C" int grvy_input_fread_double(const char *var,double *value)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Read_Var(var,value,GRVY_Internal::Double_Def) );
}

extern "C" int grvy_input_fread_logical_from_int(const char *var,int *value, int default_value)
{
  GRVY_Internal::verify_C_parser_allocated();

  bool flag;
  bool default_bool  = (default_value  == 1) ? true : false ;
  int return_value   = _GRVY_Input->Read_Var(var,&flag,default_bool);

  *value = (flag) ? 1 : 0;
  return(return_value);
}

extern "C" int grvy_input_fread_double_vec(const char *var,double *value,int nelems)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Read_Var_Vec(var,value,nelems,GRVY_Internal::Double_Def) );
}

extern "C" int grvy_input_fread_double_ivec(const char *var,double *value,int elem)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Read_Var_iVec(var,value,elem,GRVY_Internal::Double_Def) );
}

extern "C" int grvy_input_fread_int(const char *var,int *value)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Read_Var(var,value,GRVY_Internal::Int_Def) );
}

extern "C" int grvy_input_fread_int_vec(const char *var,int *value,int nelems)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Read_Var_Vec(var,value,nelems,GRVY_Internal::Int_Def) );
}

extern "C" int grvy_input_fread_int_ivec(const char *var,int *value,int elem)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Read_Var_iVec(var,value,elem,GRVY_Internal::Int_Def) );
}

extern "C" int grvy_input_fread_char(const char *var, char **value)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Read_Var(var,value) );
}

extern "C" int grvy_input_fread_char_ivec(const char *var,char **value,int elem)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Read_Var_iVec(var,value,elem) );
}

// Variable Registration Routines

extern "C" int grvy_input_register_int(const char *var,int value)
{
  GRVY_Internal::verify_C_parser_allocated();
  _GRVY_Input->Register_Var(var,value);
  return 1;
}

extern "C" int grvy_input_register_float(const char *var,float value)
{
  GRVY_Internal::verify_C_parser_allocated();
  _GRVY_Input->Register_Var(var,value);
  return 1;
}

extern "C" int grvy_input_register_double(const char *var,double value)
{
  GRVY_Internal::verify_C_parser_allocated();
  _GRVY_Input->Register_Var(var,value);
  return 1;
}

extern "C" int grvy_input_register_char       (const char *var,char *value)
{
  GRVY_Internal::verify_C_parser_allocated();
  _GRVY_Input->Register_Var(var,value);
  return 1;
}

extern "C" int grvy_input_register_get_int    (const char *var,int *value)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Get_Var(var,value) );
}

extern "C" int grvy_input_register_get_float  (const char *var,float *value)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Get_Var(var,value) );
}

extern "C" int grvy_input_register_get_double (const char *var,double *value)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Get_Var(var,value) );
}

extern "C" int grvy_input_register_get_char   (const char *var,char **value)
{
  GRVY_Internal::verify_C_parser_allocated();
  return( _GRVY_Input->Get_Var(var,value) );
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
  *flag = _GRVY_Input->Get_Var(name,value);

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
  *flag =  _GRVY_Input->Get_Var(name,value);

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
  *flag = _GRVY_Input->Get_Var(name,value);

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
  *flag = _GRVY_Input->Get_Var(name,&tmpvar);

  strncpy(value,tmpvar,strlen(tmpvar));

  delete[] name;
  return;
}




