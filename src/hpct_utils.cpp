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
// hpct_utils: Internal Utility Functions
//
// $Id: hpct_input.cpp 9 2008-12-14 02:10:50Z karl $
//
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

// Notes: these are for internal use by HPCT functions.  Hint: these
// are probably not the external droids you are looking for.


#include<stdio.h>
#include<string.h>
#include<hpct_classes.h>
#include<hpct_int.h>

using namespace std;

namespace HPCT {

//------------------------------------
// Basic stdout warning/error messages                      
//------------------------------------

void _HPCT_message(char *message)
{
  printf("\n%s\n",message);
  return;
}

void _HPCT_message(const char *mask, const char *func, const char *message)
{
  printf("\n%s (%s): %s\n",mask,func,message);
  return;
}

void _HPCT_message(const char *mask, const char *func, const char *message, const char *char_item)
{
  printf("\n%s (%s): %s -> %s \n",mask,func,message,char_item);
  return;
}

void _HPCT_message(const char *mask, const char *func, const char *message, int int_item)
{
  printf("\n%s (%s): %s -> %i \n",mask,func,message,int_item);
  return;
}

void _HPCT_message(const char *mask, const char *func, const char *message, float float_item)
{
  printf("\n%s (%s): %s -> %f \n",mask,func,message,float_item);
  return;
}

void _HPCT_message(const char *mask, const char *func, const char *message, double double_item)
{
  printf("\n%s (%s): %s -> %e \n\n",mask,func,message,double_item);
  return;
}

// ----------------------------------------------------------------
// -------------------- Convenience Functions ---------------------
// ----------------------------------------------------------------

// hpct_f2c_char(): Convert evil Fortran character strings to C

char *hpct_f2c_char(char*input,int len)
{
  char* name = new char[len+1];
  
  strncpy(name,input,len);
  name[len]='\0';
  return(name);
}


}
