//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
//
// hpct_input: Simple ASCII input file parsing using GetPot()
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#define _HPCT_GLOBAL_DEF

#include<stdlib.h>
#include<hpct_classes.h>
#include<hpct_int.h>
#include<hpct.h>
#include<GetPot>

#include"fortran_string_order.h"

using namespace std;
using namespace HPCT;


aabcd = 12345asdf;lkj

// GetPot constants

const float     Float_Def = -9999999.0f;
const double   Double_Def = -9999999.0e1;
const int         Int_Def = -9999999;
const long       Long_Def = -9999999;
const char*      Char_Def = "unknown";

static HPCT_Input_Class _HPCT_Input;     // input class
const char *_Error_Mask   = "[*] Error"; // default error notification

//-----------------------------------------------------------------
//                       ye ol' C Interfaces
//-----------------------------------------------------------------

extern "C" int hpct_input_fopen(const char *filename)
{
  return( _HPCT_Input.Open(filename) );
}

extern "C" int hpct_input_fdump()
{
  return( _HPCT_Input.Fdump() );
}

extern "C" int hpct_input_fdump_delim(const char *prefix)
{
  return( _HPCT_Input.Fdump(prefix) );
}

extern "C" int hpct_input_fdump_file(const char *prefix, const char *filename)
{
  return( _HPCT_Input.Fdump(prefix,filename) );
}

extern "C" int hpct_input_fclose()
{
  return( _HPCT_Input.Close() );
}

extern "C" int hpct_input_fread_float(const char *var,float *value)
{
  return( _HPCT_Input.Read_Var(var,value,Float_Def) );
}

extern "C" int hpct_input_fread_float_vec(const char *var,float *value,int nelems)
{
  return( _HPCT_Input.Read_Var_Vec(var,value,nelems,Float_Def) );
}

extern "C" int hpct_input_fread_float_ivec(const char *var,float *value,int elem)
{
  return( _HPCT_Input.Read_Var_iVec(var,value,elem,Float_Def) );
}

extern "C" int hpct_input_fread_double(const char *var,double *value)
{
  return( _HPCT_Input.Read_Var(var,value,Double_Def) );
}

extern "C" int hpct_input_fread_double_vec(const char *var,double *value,int nelems)
{
  return( _HPCT_Input.Read_Var_Vec(var,value,nelems,Double_Def) );
}

extern "C" int hpct_input_fread_double_ivec(const char *var,double *value,int elem)
{
  return( _HPCT_Input.Read_Var_iVec(var,value,elem,Double_Def) );
}

extern "C" int hpct_input_fread_int(const char *var,int *value)
{
  return( _HPCT_Input.Read_Var(var,value,Int_Def) );
}

extern "C" int hpct_input_fread_int_vec(const char *var,int *value,int nelems)
{
  return( _HPCT_Input.Read_Var_Vec(var,value,nelems,Int_Def) );
}

extern "C" int hpct_input_fread_int_ivec(const char *var,int *value,int elem)
{
  return( _HPCT_Input.Read_Var_iVec(var,value,elem,Int_Def) );
}

extern "C" int hpct_input_fread_char(const char *var, char **value)
{
  return( _HPCT_Input.Read_Var(var,value) );
}

extern "C" int hpct_input_fread_char_ivec(const char *var,char **value,int elem)
{
  return( _HPCT_Input.Read_Var_iVec(var,value,elem) );
}

// hpct_input_toggle_messages(): enable/disable error messages

extern "C" void hpct_input_toggle_messages(int flag)
{
  _HPCT_Input.MsgToggle(flag);
}

//-----------------------------------------------------------------
//                     Fortran Interfaces
//-----------------------------------------------------------------

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fopen_(char *filename,int *flag,int _namelen)
#else
extern "C" void hpct_input_fopen_(char *filename,int _namelen,int *flag)
#endif
{
  char *name = hpct_f2c_char(filename,_namelen);
  *flag = hpct_input_fopen(name);

  delete[] name;
  return;
}

extern "C" void hpct_input_fclose_()
{
  hpct_input_fclose();
  return;
}

extern "C" void hpct_input_fdump_(int *flag)
{

  *flag = hpct_input_fdump();
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fdump_delim_(char *prefix, int *flag, int _namelen )
#else
extern "C" void hpct_input_fdump_delim_(char *prefix, int _namelen, int *flag)
#endif
{
  char *name = hpct_f2c_char(prefix,_namelen);
  *flag = hpct_input_fdump_delim(name);
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fdump_file_(char *prefix, char *filename, int *flag, int _namelen, int _filelen )
#else
extern "C" void hpct_input_fdump_file_(char *prefix, int _namelen, char *filename, int _filelen, int *flag)
#endif
{
  char *name = hpct_f2c_char(prefix,  _namelen);
  char *file = hpct_f2c_char(filename,_filelen);

    *flag = hpct_input_fdump_file(name,file);
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_real_(char *var,float *value,int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_real_(char *var,int _namelen,float *value,int *flag)
#endif
{

  char *name = hpct_f2c_char(var,_namelen);
  *flag = hpct_input_fread_float(name,value);

  delete[] name;
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_double_(char *var,double *value,int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_double_(char *var,int _namelen,double *value,int *flag)
#endif
{

  char *name = hpct_f2c_char(var,_namelen);
  *flag = hpct_input_fread_double(name,value);

  delete[] name;
  return;
}


#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_int_(char *var,int *value,int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_int_(char *var,int _namelen,int *value,int *flag)
#endif
{

  char *name = hpct_f2c_char(var,_namelen);
  *flag = hpct_input_fread_int(name,value);

  delete[] name;
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_int_vec_(char *var,int *value,int *nelems,int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_int_vec_(char *var,int _namelen,int *value,int *nelems, int *flag)
#endif
{

  char *name = hpct_f2c_char(var,_namelen);
  *flag = hpct_input_fread_int_vec(name,value,*nelems);

  delete[] name;
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_int_ivec_(char *var,int *value,int *elem,
					   int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_int_ivec_(char *var,int _namelen,int *value,
					   int *elem, int *flag)
#endif
{
  int cindex;

  char *name = hpct_f2c_char(var,_namelen);

  // Convert from Fortran to C style indexing

  cindex  = *elem - 1;

  *flag = hpct_input_fread_int_ivec(name,value,cindex);

  delete[] name;
  return;
}


#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_real_vec_(char *var,float *value,int *nelems,int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_real_vec_(char *var,int _namelen,float *value,int *nelems, int *flag)
#endif
{

  char *name = hpct_f2c_char(var,_namelen);
  *flag = hpct_input_fread_float_vec(name,value,*nelems);

  delete[] name;
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_double_vec_(char *var,double *value,int *nelems,
					     int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_double_vec_(char *var,int _namelen,double *value,
					     int *nelems, int *flag)
#endif
{

  char *name = hpct_f2c_char(var,_namelen);
  *flag = hpct_input_fread_double_vec(name,value,*nelems);

  delete[] name;
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_double_ivec_(char *var,double *value,int *elem,
					     int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_double_ivec_(char *var,int _namelen,double *value,
					     int *elem, int *flag)
#endif
{
  int cindex;

  char *name = hpct_f2c_char(var,_namelen);

  // Convert from Fortran to C style indexing

  cindex  = *elem - 1;

  *flag = hpct_input_fread_double_ivec(name,value,cindex);

  delete[] name;
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_char_(char *var,char *value,int *flag,int _namelen, int _storage)
#else
extern "C" void hpct_input_fread_char_(char *var,int _namelen,char *value,int _storage,int *flag)
#endif
{

  char *tmpvar;

  // Note: tiny bit sneaky; for C/C++ we will receive a char * back
  // with the correct amount of storage space for the desired input string.
  // For Fortran, we will require that the space be allocated in advance and
  // hence, we need an extra copy here after completing the call.

  char *name = hpct_f2c_char(var,_namelen);
  *flag = hpct_input_fread_char(name,&tmpvar);

  strncpy(value,tmpvar,strlen(tmpvar));

  delete[] name;
  return;
}


#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_char_ivec_(char *var,char *value,int *elem,int *flag,
					    int _namelen, int _storage)
#else
extern "C" void hpct_input_fread_char_ivec_(char *var,int _namelen,char *value,
					    int _storage,int *elem,int *flag)
#endif
{

  char *tmpvar;
  char *name = hpct_f2c_char(var,_namelen);
  int cindex;

  // Convert from Fortran to C style indexing

  cindex  = *elem - 1;

  // Read the desired string

  *flag = hpct_input_fread_char_ivec(name,&tmpvar,cindex);

  strncpy(value,tmpvar,strlen(tmpvar));

  delete[] name;
  free(tmpvar);
  return;
}

extern "C" void hpct_input_toggle_messages_(int *flag)
{
  hpct_input_toggle_messages(*flag);
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_register_int_(char *var,int *value,int *flag,int _namelen)
#else
extern "C" void hpct_input_register_int_(char *var,int _namelen,int *value,int *flag)
#endif
{

  char *name = hpct_f2c_char(var,_namelen);
  *flag = hpct_input_register_int(name,*value);

  delete[] name;
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_register_float_(char *var,float *value,int *flag,int _namelen)
#else
extern "C" void hpct_input_register_float_(char *var,int _namelen,float *value,int *flag)
#endif
{

  char *name = hpct_f2c_char(var,_namelen);
  *flag = hpct_input_register_float(name,*value);

  delete[] name;
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_register_double_(char *var,double *value,int *flag,int _namelen)
#else
extern "C" void hpct_input_register_double_(char *var,int _namelen,double *value,int *flag)
#endif
{

  char *name = hpct_f2c_char(var,_namelen);
  *flag = hpct_input_register_double(name,*value);

  delete[] name;
  return;
}

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_register_char_(char *var,char *value,int *flag,int _namelen, int _storage)
#else
extern "C" void hpct_input_register_char_(char *var,int _namelen,char *value,int _storage, int *flag)
#endif
{

  char *name   = hpct_f2c_char(var,  _namelen);
  char *string = hpct_f2c_char(value,_storage);

  *flag = hpct_input_register_char(name,string);

  delete[] name;
  delete[] string;
  return;
}

// ----------------------------------------------------------------
// -------------------- Convenience Functions ---------------------
// ----------------------------------------------------------------

// hpct_f2c_char(): Convert evil Fortran character strings to C

#if 0
char *hpct_f2c_char(char*input,int len)
{
  char* name = new char[len+1];

  std::strncpy(name,input,len);
  name[len]='\0';
  return(name);
}
#endif

//--------------------------------
// Variable Registration Routines
//--------------------------------

extern "C" int hpct_input_register_int(const char *var,int value)
{
  _HPCT_Input.Register_Var(var,value);
  return 1;
}

extern "C" int hpct_input_register_float(const char *var,float value)
{
  _HPCT_Input.Register_Var(var,value);
  return 1;
}

extern "C" int hpct_input_register_double(const char *var,double value)
{
  _HPCT_Input.Register_Var(var,value);
  return 1;
}

extern "C" int hpct_input_register_char       (const char *var,char *value)
{
  _HPCT_Input.Register_Var(var,value);
  return 1;
}

extern "C" int hpct_input_register_get_int    (const char *var,int *value)
{
  return( _HPCT_Input.Get_Var(var,value) );
}

extern "C" int hpct_input_register_get_float  (const char *var,float *value)
{
  return( _HPCT_Input.Get_Var(var,value) );
}

extern "C" int hpct_input_register_get_double (const char *var,double *value)
{
  return( _HPCT_Input.Get_Var(var,value) );
}

extern "C" int hpct_input_register_get_char   (const char *var,char **value)
{
  return( _HPCT_Input.Get_Var(var,value) );
}
