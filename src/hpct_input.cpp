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

using namespace std;
#include <GetPot>		
#include<stdlib.h>

// Define a Fortran string length argument 
// list order.  Online docs indicate that SYSV
// should be different then Linux; but this does
// not necessarily seem to be true.  If the Fortran
// interface blows up on a new platform, it is most
// likely due to this ordering.

#if defined __APPLE_CC__ || defined __linux__
#define FORTRAN_ORDER1
#endif

// GetPot constants to define typedefs and delimiters

const float     Float_Def = -9999999.0f;
const double   Double_Def = -9999999.0e1;
const int         Int_Def = -9999999;
const long       Long_Def = -9999999;
const char*      Char_Def = "unknown";
const char *comment_start = "#";
const char *comment_end   = "\n";

static GetPot _hpct_ifile;               // input file 
static int _HPCT_Initialized;	         // input file initialized?
const char *_Error_Mask   = "[*] Error"; // default error notification

// Convention is to assume that user wants error messages.
// Thou shalt turn them off otherwise.

static int _HPCT_Input_Output_Errors=1;  // flag to control output

char *hpct_f2c_char   (char *input,int len);
void hpct_input_error (const char *func_name,const char *var_name);

//-----------------------------------------------------------------
//                       ye ol' C Interfaces
//-----------------------------------------------------------------

extern "C" int hpct_input_fopen(const char *filename)
{
  _hpct_ifile = GetPot(filename,comment_start,comment_end);

  if(_hpct_ifile.size() <= 1)
    {
      printf("\n%s (%s): non-existent or empty file -> %s\n",_Error_Mask,__func__,filename);
      return 0;
    }
  else 
    {
      _HPCT_Initialized=1;
      return 1;
    }

}

extern "C" int hpct_input_fdump()
{

  if(_HPCT_Initialized)
    {
      _hpct_ifile.print();
      return 1;
    }
  else
    {
      printf("\n%s (%s): unitialized file - verify file has been opened\n",
	     _Error_Mask,__func__);
      return 0;
    }

}

extern "C" int hpct_input_fclose()
{
  _HPCT_Initialized=0;
  return 1;

}

extern "C" int hpct_input_fread_float(const char *var,float *value)
{
  *value = _hpct_ifile(var,Float_Def);
  if(*value == Float_Def)
    {
      hpct_input_error("fread_float",var);
      return 0;
    }
  else
    return 1;
}

extern "C" int hpct_input_fread_float_vec(const char *var,float *value,int nelems)
{
  int i;

  for(i=0;i<nelems;i++)
    {
      value[i] = _hpct_ifile(var,Float_Def,i);

      if(value[i] == Float_Def)
	{
	  hpct_input_error("fread_float_vec",var);
	  return 0;
	}
    }
  return 1;
}

extern "C" int hpct_input_fread_double(const char *var,double *value)
{
  *value = _hpct_ifile(var,Double_Def);
  if(*value == Double_Def)
    {
      hpct_input_error("fread_double",var);
      return 0;
    }
  else
    return 1;
}


extern "C" int hpct_input_fread_double_vec(const char *var,double *value,int nelems)
{
  int i;

  for(i=0;i<nelems;i++)
    {
      value[i] = _hpct_ifile(var,Double_Def,i);

      if(value[i] == Double_Def)
	{
	  hpct_input_error("fread_double_vec",var);
	  return 0;
	}
    }
  return 1;
}

extern "C" int hpct_input_fread_double_ivec(const char *var,double *value,int elem)
{
  int i;

  *value = _hpct_ifile(var,Double_Def,elem);

  if(*value == Double_Def)
    {
      hpct_input_error("fread_double_ivec",var);
      return 0;
    }

  return 1;
}

extern "C" int hpct_input_fread_int(const char *var,int *value)
{
  *value = _hpct_ifile(var,Int_Def);
  if(*value == Int_Def)
    {
      hpct_input_error("fread_int",var);
      return 0;
    }
  else
    return 1;
}

extern "C" int hpct_input_fread_int_vec(const char *var,int *value,int nelems)
{
  int i;

  for(i=0;i<nelems;i++)
    {
      value[i] = _hpct_ifile(var,Int_Def,i);

      if(value[i] == Int_Def)
	{
	  hpct_input_error("fread_int_vec",var);
	  return 0;
	}
    }
  return 1;
}

extern "C" int hpct_input_fread_int_ivec(const char *var,int *value,int elem)
{
  int i;

  *value = _hpct_ifile(var,Int_Def,elem);

  if(*value == Double_Def)
    {
      hpct_input_error("fread_int_ivec",var);
      return 0;
    }

  return 1;
}


#if 0
extern "C" int hpct_input_fread_long(const char *var,int *value)
{
  *value = _hpct_ifile(var,Long_Def);
  if(*value == Long_Def)
    {
      hpct_input_error("fread_long",var);
      return 0;
    }
  else
    return 1;
}
#endif

extern "C" int hpct_input_fread_char(const char *var, char **value)
{
  string tstring;

  tstring = _hpct_ifile(var,Char_Def);
  *value = (char *) malloc(tstring.length()*sizeof(char)+1);
  std::strcpy(value[0],tstring.c_str());

  if(strcmp(*value,Char_Def) == 0)
    {
      hpct_input_error(__func__,var);
      return 0;
    }
  else
      return 1;
}

extern "C" int hpct_input_fread_char_ivec(const char *var,char **value,int elem)
{

  string tstring;

  tstring = _hpct_ifile(var,Char_Def,elem);
  *value = (char *) malloc(tstring.length()*sizeof(char)+1);
  std::strcpy(value[0],tstring.c_str());

  if(strcmp(*value,Char_Def) == 0)
    {
      hpct_input_error(__func__,var);
      return 0;
    }
  else
    return 1;
}

// hpct_input_toggle_messages(): enable/disable error messages

extern "C" void hpct_input_toggle_messages(int flag)
{
  if(flag == 0)
    _HPCT_Input_Output_Errors = 0;
  else
    _HPCT_Input_Output_Errors = 1;
}

//-----------------------------------------------------------------
//                     Fortran Interfaces
//-----------------------------------------------------------------

#ifdef FORTRAN_ORDER1
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

#ifdef FORTRAN_ORDER1
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

#ifdef FORTRAN_ORDER1
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


#ifdef FORTRAN_ORDER1
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

#ifdef FORTRAN_ORDER1
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

#ifdef FORTRAN_ORDER1
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


#ifdef FORTRAN_ORDER1
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

#ifdef FORTRAN_ORDER1
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

#ifdef FORTRAN_ORDER1
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

#ifdef FORTRAN_ORDER1
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

  std::strncpy(value,tmpvar,strlen(tmpvar));

  delete[] name;
  return;
}


#ifdef FORTRAN_ORDER1
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

  std::strncpy(value,tmpvar,strlen(tmpvar));

  delete[] name;
  free(tmpvar);
  return;
}

extern "C" void hpct_input_toggle_messages_(int *flag)
{
  if(*flag == 0)
    _HPCT_Input_Output_Errors = 0;
  else
    _HPCT_Input_Output_Errors = 1;
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

// hpct_input_error(): output error message if desired

void hpct_input_error(const char *func_name, const char *var_name)
{
  if(_HPCT_Input_Output_Errors)
    printf("\n%s (%s): unable to query variable -> %s\n",_Error_Mask,func_name,var_name);
}




