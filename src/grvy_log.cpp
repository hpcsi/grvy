// -*-c++-*-
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008-2013,2018-2021 The PECOS Development Team
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
// grvy_log: Application logging related routines.
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<stdio.h>
#include<grvy_classes.h>
#include<grvy_int.h>
#include<grvy.h>
#include<string>
#include<stdarg.h>

using namespace std;
using namespace GRVY;

void grvy_log_setlevel(int priority)
{
  _GRVY_Log.change_priority(priority);
  return;
}

int grvy_log_getlevel()
{
  return(_GRVY_Log.get_priority());
}

void grvy_log_setmask(int loglevel,const char *delimiter)
{
  _GRVY_Log.change_mask(loglevel,delimiter);
  return;
}

void grvy_log(int loglevel, const char *mesg)
{
  _GRVY_Log.msg(loglevel,mesg);
}

int grvy_printf(int loglevel, const char *format,...)
{
  va_list arg;
  int status;

  if(_GRVY_Log.isLog(loglevel))
    {
      va_start(arg,format);
      status = _GRVY_Log.msg_printf(loglevel,format,arg);
      va_end(arg);
      return(status);
    }
    else
      return 0;
}

// the following 5 routines are not terribly useful (compared to
// _printf) except for potentially exposing to Fortran. Disabling for
// now since they are not exposed to Fortran and it would be super
// cool to have a va printf in Fortran somewhoe.

# if 0 

void grvy_log_int(int loglevel, const char *func, const char *mesg, int var)
{
  _GRVY_message(loglevel,func,mesg,var);
}

void grvy_log_float(int loglevel, const char *func, const char *mesg, float var)
{
  _GRVY_message(loglevel,func,mesg,var);
}

void grvy_log_double(int loglevel, const char *func, const char *mesg, double var)
{
  _GRVY_message(loglevel,func,mesg,var);
}

void grvy_log_char(int loglevel, const char *func, const char *mesg, const char *char_item)
{
  _GRVY_message(loglevel,func,mesg,char_item);
}

#endif

//-----------------------------------------------------------------
//                     Fortran Interfaces
//-----------------------------------------------------------------

extern "C" void grvy_log_setlevel_(int *priority) {
  grvy_log_setlevel(*priority);
}

extern "C" void grvy_log_setmask_(int *loglevel, char *mesg, int _namelen)
{
  char *message = grvy_f2c_char(mesg,_namelen);
  grvy_log_setmask(*loglevel,message);
  delete[] message;
}

extern "C" void grvy_log_(int *loglevel, char *mesg, int _namelen)
{
  char *message = grvy_f2c_char(mesg,_namelen);
  _GRVY_Log.msg(*loglevel,message);
  delete[] message;
}




