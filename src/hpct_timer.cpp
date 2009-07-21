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
// hpct_timer: Timing routines for performance monitoring
//
// $Id: hpct_input.cpp 9 2008-12-14 02:10:50Z karl $
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<stdio.h>
#include<hpct_classes.h>
#include<hpct_int.h>
#include<hpct.h>
#include<string>

using namespace std;
using namespace HPCT;

HPCT_Timer_Class *_HPCT_TimerMap;                // performance timer map

double hpct_timer ()
{
  return( _HPCT_Timers->RawTimer() );
}


void hpct_timer_begin(const char *id)
{
  //_HPCT_Timers->VerifyInit();
  _HPCT_Timers->BeginTimer(id);

  return;
}


void hpct_timer_end(const char *id)
{
  //  _HPCT_Timers->VerifyInit();
  _HPCT_Timers->EndTimer(id);
  return;
}


// hpct_timer_elapsedseconds(): Get seconds spent between ..._begin, ..._end

double hpct_timer_elapsedseconds(const char *id)
{
  _HPCT_Timers->VerifyInit();
  return( _HPCT_Timers->ElapsedSeconds(id) );
}


// hpct_timer_init(): Define beginning of global portion to be monitored

void hpct_timer_init(const char *id)
{

  // create new timer on 1st call

  if(_HPCT_Timers == NULL)
    _HPCT_Timers = new HPCT_Timer_Class();

  // register the name for the timer

  _HPCT_Timers->SaveTimerName(id);

  // initialize global timer region

  _HPCT_Timers->BeginTimer(_HPCT_gtimer);

  return;
}

void hpct_timer_reset()
{
  _HPCT_Timers->Reset();
  return;
}

// hpct_timer_finalize(): Define end of global portion to be monitored

void hpct_timer_finalize()
{
  //  _HPCT_Timers->VerifyInit();
  _HPCT_Timers->EndTimer(_HPCT_gtimer);
  return;
}

// hpct_timer_summarize(): Print a summary of all current timers

void hpct_timer_summarize()
{
  _HPCT_Timers->VerifyInit();
  _HPCT_Timers->Summarize();
  return;
}

//-----------------------------------------------------------------
//                     Fortran Interfaces
//-----------------------------------------------------------------

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
  char *name = hpct_f2c_char(id,_namelen);
  hpct_timer_init(name);

  delete[] name;
}

extern "C" void hpct_timer_reset_() {
  hpct_timer_reset();
}

extern "C" void hpct_timer_finalize_() {
  hpct_timer_finalize();
}

extern "C" void hpct_timer_summarize_() {
  hpct_timer_summarize();
}


extern "C" void hpct_timer_begin_(char *id,int _namelen)
{
  char *name = hpct_f2c_char(id,_namelen);
  hpct_timer_begin(name);

  delete[] name;
  return;
}

extern "C" void hpct_timer_end_(char *id,int _namelen)
{
  char *name = hpct_f2c_char(id,_namelen);
  hpct_timer_end(name);

  delete[] name;
  return;
}

extern "C" void hpct_timer_(double *value)
{
  *value = hpct_timer();
  return;
}

