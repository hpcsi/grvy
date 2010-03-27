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
// grvy_timer: Timing routines for performance monitoring
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

GRVY_Timer_Class *_GRVY_TimerMap;                // performance timer map

double grvy_timer ()
{
  return( _GRVY_Timers->RawTimer() );
}

// grvy_timer_begin(): Defines beginning of a specific timing region

void grvy_timer_begin(const char *id)
{
  //_GRVY_Timers->VerifyInit();
  _GRVY_Timers->BeginTimer(id);

  return;
}

// grvy_timer_end(): Defines end of a specific timing region

void grvy_timer_end(const char *id)
{
  //  _GRVY_Timers->VerifyInit();
  _GRVY_Timers->EndTimer(id);
  return;
}

// grvy_timer_elapsedseconds(): Get seconds spent between ..._begin, ..._end

double grvy_timer_elapsedseconds(const char *id)
{
  _GRVY_Timers->VerifyInit();
  return( _GRVY_Timers->ElapsedSeconds(id) );
}

// grvy_timer_elapsed_global: provides elapsed time since first init() call

double grvy_timer_elapsed_global()
{
  _GRVY_Timers->VerifyInit();
  return ( _GRVY_Timers->ElapsedGlobal() );
}


// grvy_timer_init(): Define beginning of global portion to be
// monitored

void grvy_timer_init(const char *id)
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

// grvy_timer_reset(): Reset a global counter to start at zero

void grvy_timer_reset()
{
  _GRVY_Timers->Reset();
  return;
}

// grvy_timer_finalize(): Define end of global portion to be monitored

void grvy_timer_finalize()
{
  //  _GRVY_Timers->VerifyInit();
  _GRVY_Timers->EndTimer(_GRVY_gtimer);
  return;
}

// grvy_timer_summarize(): Print a summary of all current timers

void grvy_timer_summarize()
{
  _GRVY_Timers->VerifyInit();
  _GRVY_Timers->Summarize();
  return;
}

//-----------------------------------------------------------------
//                     Fortran Interfaces
//-----------------------------------------------------------------

extern "C" void grvy_asci_time_(char *timestring,int _namelen) {
  struct tm *ptr;
  time_t tm;

  tm = time(NULL);
  ptr = localtime(&tm);

  // asctime() puts a newline at the end of the string - we won't include
  // that in the copy.

  strncpy(timestring,asctime(ptr),strlen(asctime(ptr)) - 1);

  return;
}

extern "C" void grvy_timer_init_(char *id,int _namelen) {
  char *name = grvy_f2c_char(id,_namelen);
  grvy_timer_init(name);

  delete[] name;
}

extern "C" void grvy_timer_reset_() {
  grvy_timer_reset();
}

extern "C" void grvy_timer_finalize_() {
  grvy_timer_finalize();
}

extern "C" void grvy_timer_summarize_() {
  grvy_timer_summarize();
}


extern "C" void grvy_timer_begin_(char *id,int _namelen)
{
  char *name = grvy_f2c_char(id,_namelen);
  grvy_timer_begin(name);

  delete[] name;
  return;
}

extern "C" void grvy_timer_end_(char *id,int _namelen)
{
  char *name = grvy_f2c_char(id,_namelen);
  grvy_timer_end(name);

  delete[] name;
  return;
}

extern "C" void grvy_timer_(double *value)
{
  *value = grvy_timer();
  return;
}


extern "C" void grvy_timer_elapsed_global_(double *value)
{
  *value = grvy_timer_elapsed_global();
  return;
}

