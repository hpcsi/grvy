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

//GRVY_Timer_Class *_GRVY_TimerMap;                // performance timer map

double grvy_timer ()
{
  return( _GRVY_Timers->RawTimer() );
}

// grvy_timer_begin(): Defines beginning of a specific timing region

void grvy_timer_begin(const char *id)
{
  _GRVY_Timers->BeginTimer(id,false);

  return;
}

// grvy_timer_end(): Defines end of a specific timing region

void grvy_timer_end(const char *id)
{
  _GRVY_Timers->EndTimer(id,false);
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

  _GRVY_Timers->BeginTimer(_GRVY_gtimer,false);

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
  _GRVY_Timers->EndTimer(_GRVY_gtimer,false);
  return;
}

// grvy_timer_summarize(): Print a summary of all current timers

void grvy_timer_summarize()
{
  _GRVY_Timers->VerifyInit();
  _GRVY_Timers->Summarize();
  return;
}

// grvy_timer_stat_count(): provides number of calls to a timer

int grvy_timer_stats_count(const char *id)
{
  return(_GRVY_Timers->StatsCount(id));
}

// grvy_timer_stat_mean(): provides mean timing for a timer

double grvy_timer_stats_mean(const char *id)
{
  return(_GRVY_Timers->StatsMean(id));
}

// grvy_timer_stat_variance(): provides variance for a timer

double grvy_timer_stats_variance(const char *id)
{
  return(_GRVY_Timers->StatsVariance(id));
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

extern "C" double grvy_timer_elapsedseconds_(char *id, int _namelen)
{
  double elapsed_time;
  char *name = grvy_f2c_char(id,_namelen);
  
  elapsed_time = grvy_timer_elapsedseconds(name);

  delete[] name;
  return(elapsed_time);
}

extern "C" int grvy_timer_stats_count_(char *id, int _namelen)
{
  int result;
  char *name = grvy_f2c_char(id,_namelen);
  
  result = grvy_timer_stats_count(name);

  delete[] name;
  return(result);
}

extern "C" double grvy_timer_stats_mean_(char *id, int _namelen)
{
  double result;
  char *name = grvy_f2c_char(id,_namelen);
  
  result = grvy_timer_stats_mean(name);

  delete[] name;
  return(result);
}

extern "C" double grvy_timer_stats_variance_(char *id, int _namelen)
{
  double result;
  char *name = grvy_f2c_char(id,_namelen);
  
  result = grvy_timer_stats_variance(name);

  delete[] name;
  return(result);
}
