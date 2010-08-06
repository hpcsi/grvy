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
// grvy_log: Application logging related routines.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<stdio.h>
#include<grvy_classes.h>
#include<grvy_int.h>
#include<grvy.h>
#include<string>

using namespace std;
using namespace GRVY;

void grvy_log_setlevel(int priority)
{
  _GRVY_Log.change_priority(priority);
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

//-----------------------------------------------------------------
//                     Fortran Interfaces
//-----------------------------------------------------------------

extern "C" void grvy_log_setlevel_(int *priority) {
  grvy_log_setlevel(*priority);
}

extern "C" void grvy_log_(int *loglevel, char *mesg, int _namelen)
{
  char *message = grvy_f2c_char(mesg,_namelen);
  _GRVY_Log.msg(*loglevel,message);
  delete[] message;
}




