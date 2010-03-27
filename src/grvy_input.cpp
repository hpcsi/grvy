// -*-c++-*-
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//
// Copyright (C) 2008,2009,2010 The PECOS Development Team
//
// Please see http://pecos.ices.utexas.edu for more information.
//
// This file is part of the PECOS GRVY Toolkit
//
// GRVY is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GRVY is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GRVY.  If not, see <http://www.gnu.org/licenses/>.
//
//--------------------------------------------------------------------------
//
// grvy_input: Simple ASCII input file parsing using GetPot()
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#define _GRVY_GLOBAL_DEF

#include<stdlib.h>
#include<grvy_classes.h>
#include<grvy_int.h>
#include<grvy.h>
#include"fortran_string_order.h"

using namespace std;
using namespace GRVY;

// GetPot constants

const float     Float_Def = -9999999.0f;
const double   Double_Def = -9999999.0e1;
const int         Int_Def = -9999999;
const long       Long_Def = -9999999;
const char*      Char_Def = "unknown";

static GRVY_Input_Class _GRVY_Input;     // input class

//-----------------------------------------------------------------
//                       ye ol' C Interfaces
//-----------------------------------------------------------------

extern "C" int grvy_input_fopen(const char *filename)
{
  return( _GRVY_Input.Open(filename) );
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

// grvy_input_toggle_messages(): enable/disable error messages

extern "C" void grvy_input_toggle_messages(int flag)
{
  _GRVY_Input.MsgToggle(flag);
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
//-----------------------------------------------------------------

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fopen_(char *filename,int *flag,int _namelen)
#else
extern "C" void grvy_input_fopen_(char *filename,int _namelen,int *flag)
#endif
{
  char *name = grvy_f2c_char(filename,_namelen);
  *flag = grvy_input_fopen(name);

  delete[] name;
  return;
}

extern "C" void grvy_input_fclose_()
{
  grvy_input_fclose();
  return;
}

extern "C" void grvy_input_fdump_(int *flag)
{

  *flag = grvy_input_fdump();
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fdump_delim_(char *prefix, int *flag, int _namelen )
#else
extern "C" void grvy_input_fdump_delim_(char *prefix, int _namelen, int *flag)
#endif
{
  char *name = grvy_f2c_char(prefix,_namelen);
  *flag = grvy_input_fdump_delim(name);
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fdump_file_(char *prefix, char *filename, int *flag, int _namelen, int _filelen )
#else
extern "C" void grvy_input_fdump_file_(char *prefix, int _namelen, char *filename, int _filelen, int *flag)
#endif
{
  char *name = grvy_f2c_char(prefix,  _namelen);
  char *file = grvy_f2c_char(filename,_filelen);

    *flag = grvy_input_fdump_file(name,file);
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fread_real_(char *var,float *value,int *flag,int _namelen)
#else
extern "C" void grvy_input_fread_real_(char *var,int _namelen,float *value,int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_float(name,value);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fread_double_(char *var,double *value,int *flag,int _namelen)
#else
extern "C" void grvy_input_fread_double_(char *var,int _namelen,double *value,int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_double(name,value);

  delete[] name;
  return;
}


#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fread_int_(char *var,int *value,int *flag,int _namelen)
#else
extern "C" void grvy_input_fread_int_(char *var,int _namelen,int *value,int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_int(name,value);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fread_int_vec_(char *var,int *value,int *nelems,int *flag,int _namelen)
#else
extern "C" void grvy_input_fread_int_vec_(char *var,int _namelen,int *value,int *nelems, int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_int_vec(name,value,*nelems);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fread_int_ivec_(char *var,int *value,int *elem,
					   int *flag,int _namelen)
#else
extern "C" void grvy_input_fread_int_ivec_(char *var,int _namelen,int *value,
					   int *elem, int *flag)
#endif
{
  int cindex;

  char *name = grvy_f2c_char(var,_namelen);

  // Convert from Fortran to C style indexing

  cindex  = *elem - 1;

  *flag = grvy_input_fread_int_ivec(name,value,cindex);

  delete[] name;
  return;
}


#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fread_real_vec_(char *var,float *value,int *nelems,int *flag,int _namelen)
#else
extern "C" void grvy_input_fread_real_vec_(char *var,int _namelen,float *value,int *nelems, int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_float_vec(name,value,*nelems);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fread_double_vec_(char *var,double *value,int *nelems,
					     int *flag,int _namelen)
#else
extern "C" void grvy_input_fread_double_vec_(char *var,int _namelen,double *value,
					     int *nelems, int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_double_vec(name,value,*nelems);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fread_double_ivec_(char *var,double *value,int *elem,
					     int *flag,int _namelen)
#else
extern "C" void grvy_input_fread_double_ivec_(char *var,int _namelen,double *value,
					     int *elem, int *flag)
#endif
{
  int cindex;

  char *name = grvy_f2c_char(var,_namelen);

  // Convert from Fortran to C style indexing

  cindex  = *elem - 1;

  *flag = grvy_input_fread_double_ivec(name,value,cindex);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void grvy_input_fread_char_(char *var,char *value,int *flag,int _namelen, int _storage)
#else
extern "C" void grvy_input_fread_char_(char *var,int _namelen,char *value,int _storage,int *flag)
#endif
{

  char *tmpvar;

  // Note: tiny bit sneaky; for C/C++ we will receive a char * back
  // with the correct amount of storage space for the desired input string.
  // For Fortran, we will require that the space be allocated in advance and
  // hence, we need an extra copy here after completing the call.

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_char(name,&tmpvar);

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

  strncpy(value,tmpvar,strlen(tmpvar));

  delete[] name;
  free(tmpvar);
  return;
}

extern "C" void grvy_input_toggle_messages_(int *flag)
{
  grvy_input_toggle_messages(*flag);
  return;
}

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

