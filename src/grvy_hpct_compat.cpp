// -*-c++-*-
//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
//
// grvy_hpct_compat: HPCT calls passed thru to GRVY
//
// $Id: grvy_input.cpp 8716 2010-03-27 14:57:25Z karl $
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

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

static GRVY_Input_Class _GRVY_Input;     // input class

extern "C" int hpct_input_fopen(const char *filename)
{
  return( _GRVY_Input.Open(filename) );
}

extern "C" int hpct_input_fdump()
{
  return( _GRVY_Input.Fdump() );
}

extern "C" int hpct_input_fdump_delim(const char *prefix)
{
  return( _GRVY_Input.Fdump(prefix) );
}

extern "C" int hpct_input_fdump_file(const char *prefix, const char *filename)
{
  return( _GRVY_Input.Fdump(prefix,filename) );
}

extern "C" int hpct_input_fclose()
{
  return( _GRVY_Input.Close() );
}

extern "C" int hpct_input_fread_float(const char *var,float *value)
{
  return( _GRVY_Input.Read_Var(var,value,Float_Def) );
}

extern "C" int hpct_input_fread_float_vec(const char *var,float *value,int nelems)
{
  return( _GRVY_Input.Read_Var_Vec(var,value,nelems,Float_Def) );
}

extern "C" int hpct_input_fread_float_ivec(const char *var,float *value,int elem)
{
  return( _GRVY_Input.Read_Var_iVec(var,value,elem,Float_Def) );
}

extern "C" int hpct_input_fread_double(const char *var,double *value)
{
  return( _GRVY_Input.Read_Var(var,value,Double_Def) );
}

extern "C" int hpct_input_fread_double_vec(const char *var,double *value,int nelems)
{
  return( _GRVY_Input.Read_Var_Vec(var,value,nelems,Double_Def) );
}

extern "C" int hpct_input_fread_double_ivec(const char *var,double *value,int elem)
{
  return( _GRVY_Input.Read_Var_iVec(var,value,elem,Double_Def) );
}

extern "C" int hpct_input_fread_int(const char *var,int *value)
{
  return( _GRVY_Input.Read_Var(var,value,Int_Def) );
}

extern "C" int hpct_input_fread_int_vec(const char *var,int *value,int nelems)
{
  return( _GRVY_Input.Read_Var_Vec(var,value,nelems,Int_Def) );
}

extern "C" int hpct_input_fread_int_ivec(const char *var,int *value,int elem)
{
  return( _GRVY_Input.Read_Var_iVec(var,value,elem,Int_Def) );
}

extern "C" int hpct_input_fread_char(const char *var, char **value)
{
  return( _GRVY_Input.Read_Var(var,value) );
}

extern "C" int hpct_input_fread_char_ivec(const char *var,char **value,int elem)
{
  return( _GRVY_Input.Read_Var_iVec(var,value,elem) );
}

extern "C" void hpct_input_toggle_messages(int flag)
{
  _GRVY_Input.MsgToggle(flag);
}

//-----------------------------------------------------------------
//                     Fortran Interfaces
//-----------------------------------------------------------------

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fopen_(char *filename,int *flag,int _namelen)
#else
extern "C" void hpct_input_fopen_(char *filename,int _namelen,int *flag)
#endif
{
  char *name = grvy_f2c_char(filename,_namelen);
  *flag = grvy_input_fopen(name);

  delete[] name;
  return;
}

extern "C" void hpct_input_fclose_()
{
  grvy_input_fclose();
  return;
}

extern "C" void hpct_input_fdump_(int *flag)
{

  *flag = grvy_input_fdump();
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fdump_delim_(char *prefix, int *flag, int _namelen )
#else
extern "C" void grvy_input_fdump_delim_(char *prefix, int _namelen, int *flag)
#endif
{
  char *name = grvy_f2c_char(prefix,_namelen);
  *flag = grvy_input_fdump_delim(name);
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fdump_file_(char *prefix, char *filename, int *flag, int _namelen, int _filelen )
#else
extern "C" void hpct_input_fdump_file_(char *prefix, int _namelen, char *filename, int _filelen, int *flag)
#endif
{
  char *name = grvy_f2c_char(prefix,  _namelen);
  char *file = grvy_f2c_char(filename,_filelen);

    *flag = grvy_input_fdump_file(name,file);
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_real_(char *var,float *value,int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_real_(char *var,int _namelen,float *value,int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_float(name,value);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_double_(char *var,double *value,int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_double_(char *var,int _namelen,double *value,int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_double(name,value);

  delete[] name;
  return;
}


#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_int_(char *var,int *value,int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_int_(char *var,int _namelen,int *value,int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_int(name,value);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_int_vec_(char *var,int *value,int *nelems,int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_int_vec_(char *var,int _namelen,int *value,int *nelems, int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_int_vec(name,value,*nelems);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_int_ivec_(char *var,int *value,int *elem,
					   int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_int_ivec_(char *var,int _namelen,int *value,
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
extern "C" void hpct_input_fread_real_vec_(char *var,float *value,int *nelems,int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_real_vec_(char *var,int _namelen,float *value,int *nelems, int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_float_vec(name,value,*nelems);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_double_vec_(char *var,double *value,int *nelems,
					     int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_double_vec_(char *var,int _namelen,double *value,
					     int *nelems, int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_double_vec(name,value,*nelems);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_double_ivec_(char *var,double *value,int *elem,
					     int *flag,int _namelen)
#else
extern "C" void hpct_input_fread_double_ivec_(char *var,int _namelen,double *value,
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

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_fread_char(name,&tmpvar);

  strncpy(value,tmpvar,strlen(tmpvar));

  delete[] name;
  return;
}


#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_fread_char_ivec_(char *var,char *value,int *elem,int *flag,
					    int _namelen, int _storage)
#else
extern "C" void hpct_input_fread_char_ivec_(char *var,int _namelen,char *value,
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

extern "C" void hpct_input_toggle_messages_(int *flag)
{
  grvy_input_toggle_messages(*flag);
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_register_int_(char *var,int *value,int *flag,int _namelen)
#else
extern "C" void hpct_input_register_int_(char *var,int _namelen,int *value,int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_register_int(name,*value);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_register_float_(char *var,float *value,int *flag,int _namelen)
#else
extern "C" void hpct_input_register_float_(char *var,int _namelen,float *value,int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_register_float(name,*value);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_register_double_(char *var,double *value,int *flag,int _namelen)
#else
extern "C" void hpct_input_register_double_(char *var,int _namelen,double *value,int *flag)
#endif
{

  char *name = grvy_f2c_char(var,_namelen);
  *flag = grvy_input_register_double(name,*value);

  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_input_register_char_(char *var,char *value,int *flag,int _namelen, int _storage)
#else
extern "C" void hpct_input_register_char_(char *var,int _namelen,char *value,int _storage, int *flag)
#endif
{

  char *name   = grvy_f2c_char(var,  _namelen);
  char *string = grvy_f2c_char(value,_storage);

  *flag = grvy_input_register_char(name,string);

  delete[] name;
  delete[] string;
  return;
}

//--------------------------------
// Variable Registration Routines
//--------------------------------

extern "C" int hpct_input_register_int(const char *var,int value)
{
  _GRVY_Input.Register_Var(var,value);
  return 1;
}

extern "C" int hpct_input_register_float(const char *var,float value)
{
  _GRVY_Input.Register_Var(var,value);
  return 1;
}

extern "C" int hpct_input_register_double(const char *var,double value)
{
  _GRVY_Input.Register_Var(var,value);
  return 1;
}

extern "C" int hpct_input_register_char       (const char *var,char *value)
{
  _GRVY_Input.Register_Var(var,value);
  return 1;
}

extern "C" int hpct_input_register_get_int    (const char *var,int *value)
{
  return( _GRVY_Input.Get_Var(var,value) );
}

extern "C" int hpct_input_register_get_float  (const char *var,float *value)
{
  return( _GRVY_Input.Get_Var(var,value) );
}

extern "C" int hpct_input_register_get_double (const char *var,double *value)
{
  return( _GRVY_Input.Get_Var(var,value) );
}

extern "C" int hpct_input_register_get_char   (const char *var,char **value)
{
  return( _GRVY_Input.Get_Var(var,value) );
}

/* Timers */

//GRVY_Timer_Class *_GRVY_TimerMap;                // performance timer map

double hpct_timer ()
{
  return( _GRVY_Timers->RawTimer() );
}

// hpct_timer_begin(): Defines beginning of a specific timing region

extern "C" void hpct_timer_begin(const char *id)
{
  //_GRVY_Timers->VerifyInit();
  _GRVY_Timers->BeginTimer(id);

  return;
}

// hpct_timer_end(): Defines end of a specific timing region

extern "C" void hpct_timer_end(const char *id)
{
  //  _GRVY_Timers->VerifyInit();
  _GRVY_Timers->EndTimer(id);
  return;
}

// hpct_timer_elapsedseconds(): Get seconds spent between ..._begin, ..._end

extern "C" double hpct_timer_elapsedseconds(const char *id)
{
  _GRVY_Timers->VerifyInit();
  return( _GRVY_Timers->ElapsedSeconds(id) );
}

// hpct_timer_elapsed_global: provides elapsed time since first init() call

extern "C" double hpct_timer_elapsed_global()
{
  _GRVY_Timers->VerifyInit();
  return ( _GRVY_Timers->ElapsedGlobal() );
}


// hpct_timer_init(): Define beginning of global portion to be
// monitored

extern "C" void hpct_timer_init(const char *id)
{

  // create new timer on 1st call

  if(_GRVY_Timers == NULL)
    _GRVY_Timers = new GRVY_Timer_Class();

  // register the name for the timer

  _GRVY_Timers->SaveTimerName(id);

  // initialize global timer region

  _GRVY_Timers->BeginTimer(_GRVY_gtimer);

  return;
}

// hpct_timer_reset(): Reset a global counter to start at zero

extern "C" void hpct_timer_reset()
{
  _GRVY_Timers->Reset();
  return;
}

// hpct_timer_finalize(): Define end of global portion to be monitored

extern "C" void hpct_timer_finalize()
{
  //  _GRVY_Timers->VerifyInit();
  _GRVY_Timers->EndTimer(_GRVY_gtimer);
  return;
}

// hpct_timer_summarize(): Print a summary of all current timers

extern "C" void hpct_timer_summarize()
{
  _GRVY_Timers->VerifyInit();
  _GRVY_Timers->Summarize();
  return;
}

/* Fotran Timers */

extern "C" void hpct_asci_time_(char *timestring,int _namelen) {
  struct tm *ptr;
  time_t tm;

  tm = time(NULL);
  ptr = localtime(&tm);

  // asctime() puts a newline at the end of the string - we won't include
  // that in the copy.

  strncpy(timestring,asctime(ptr),strlen(asctime(ptr)) - 1);

  return;
}

extern "C" void hpct_timer_init_(char *id,int _namelen) {
  char *name = grvy_f2c_char(id,_namelen);
  grvy_timer_init(name);

  delete[] name;
}

extern "C" void hpct_timer_reset_() {
  grvy_timer_reset();
}

extern "C" void hpct_timer_finalize_() {
  grvy_timer_finalize();
}

extern "C" void hpct_timer_summarize_() {
  grvy_timer_summarize();
}


extern "C" void hpct_timer_begin_(char *id,int _namelen)
{
  char *name = grvy_f2c_char(id,_namelen);
  grvy_timer_begin(name);

  delete[] name;
  return;
}

extern "C" void hpct_timer_end_(char *id,int _namelen)
{
  char *name = grvy_f2c_char(id,_namelen);
  grvy_timer_end(name);

  delete[] name;
  return;
}

extern "C" void hpct_timer_(double *value)
{
  *value = grvy_timer();
  return;
}


extern "C" void hpct_timer_elapsed_global_(double *value)
{
  *value = grvy_timer_elapsed_global();
  return;
}

/* Math functions */

GRVY_Math_Class _GRVY_Math2; //math class

extern "C" int hpct_double_isnan(double *val)
{
  return( _GRVY_Math2.isnan<double>( *val ) );
}

extern "C" int hpct_double_isinf(double *val)
{
  return( _GRVY_Math2.isinf<double>( *val ) );
}

extern "C" int hpct_float_isnan(float *val)
{
  return( _GRVY_Math2.isnan<float>( *val ) );
}

extern "C" int hpct_float_isinf(float *val)
{
  return( _GRVY_Math2.isinf<float>( *val ) );
}

//-----------------------------------------------------------------
//                     Fortran Interfaces
//-----------------------------------------------------------------

extern "C" int hpct_double_isnan_( double *val )
{
  return( _GRVY_Math2.isnan<double>( *val ) );
}

extern "C" int hpct_double_isinf_( double *val )
{
  return( _GRVY_Math2.isinf<double>( *val ) );
}

/* File-system utils */

extern "C" int hpct_create_unique_dir(char *name_template)
{
  return(grvy_create_unique_dir(name_template));
}

extern "C" int hpct_create_scratch_dir(char *name_template)
{
  return(grvy_create_scratch_dir(name_template));
}

extern "C" int hpct_check_file_path(const char *pathname)
{
  return(grvy_check_file_path(pathname));
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_check_file_path_(char *pathname,int *flag,int _namelen)
#else
extern "C" void hpct_check_file_path_(char *pathname,int _namelen,int *flag)
#endif
{
  char *name = grvy_f2c_char(pathname,_namelen);
  
  *flag = grvy_check_file_path(name);
  
  delete[] name;
  return;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_create_unique_dir_(char *name_template,int *flag,int _namelen)
#else
extern "C" void hpct_create_unique_dir_(char *name_template,int _namelen,int *flag)
#endif
{
  char *c_name_template = grvy_f2c_char(name_template,_namelen);
  
  *flag = grvy_create_unique_dir(c_name_template);
  strncpy(name_template,c_name_template,_namelen);
  
  delete[] c_name_template;
}

#ifdef _GRVY_FORTRAN_STRING_ORDER1
extern "C" void hpct_create_scratch_dir_(char *name_template,int *flag,int _namelen)
#else
extern "C" void hpct_create_scratch_dir_(char *name_template,int _namelen,int *flag)
#endif
{
  char *c_name_template = grvy_f2c_char(name_template,_namelen);
  
  *flag = grvy_create_scratch_dir(c_name_template);
  strncpy(name_template,c_name_template,_namelen);
  
  delete[] c_name_template;
}

/* Logging */

extern "C" void hpct_log_setlevel(int priority)
{
  _GRVY_Log.change_priority(priority);
  return;
}

extern "C" void hpct_log(int loglevel, const char *mesg)
{
  _GRVY_Log.msg(loglevel,mesg);
}

extern "C" void hpct_log_setlevel_(int *priority) {
  grvy_log_setlevel(*priority);
}

extern "C" void hpct_log_(int *loglevel, char *mesg, int _namelen)
{
  char *message = grvy_f2c_char(mesg,_namelen);
  _GRVY_Log.msg(*loglevel,message);
  delete[] message;
}
