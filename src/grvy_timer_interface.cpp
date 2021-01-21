// -*-c++-*-
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
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
// grvy_timer_interface: C interface for performance timing routines
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<stdio.h>
#include<string.h>
#include<grvy_classes.h>
#include<grvy_int.h>
#include<grvy.h>
#include<string>

using namespace std;
using namespace GRVY;

// Utility function to create new timer class on 1st call for C
// interface

std::unique_ptr<GRVY_Timer_Class> _GRVY_Timers;

namespace GRVY_Internal {
  void verify_C_timer_allocated();
}

void GRVY_Internal::verify_C_timer_allocated()
{

  if(_GRVY_Timers.get() == NULL)
    _GRVY_Timers.reset (new GRVY_Timer_Class());

  return;
}

double grvy_timer ()
{
  GRVY_Internal::verify_C_timer_allocated();
  return( _GRVY_Timers->RawTimer() );
}

// grvy_timer_begin(): Defines beginning of a specific timing region

void grvy_timer_begin(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  _GRVY_Timers->BeginTimer(id);
  return;
}

// grvy_timer_end(): Defines end of a specific timing region

void grvy_timer_end(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  _GRVY_Timers->EndTimer(id);
  return;
}

// grvy_timer_elapsedseconds(): Get (exclusive) seconds spent between ..._begin, ..._end

double grvy_timer_elapsedseconds(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  return( _GRVY_Timers->ElapsedSeconds(id) );
}

// grvy_timer_elapsedseconds(): Get (inclusive) seconds spent between ..._begin, ..._end

double grvy_timer_elapsedseconds_inc(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  return( _GRVY_Timers->ElapsedSeconds_inc(id) );
}

// grvy_timer_elapsed_global: provides elapsed time since first init() call

double grvy_timer_elapsed_global()
{
  GRVY_Internal::verify_C_timer_allocated();
  return ( _GRVY_Timers->ElapsedGlobal() );
}

// grvy_timer_init(): Define beginning of global portion to be
// monitored

void grvy_timer_init(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();

  // initialize global timer region

  _GRVY_Timers->Init(id);

  return;
}

// grvy_timer_reset(): Reset a global counter to start at zero

void grvy_timer_reset()
{
  GRVY_Internal::verify_C_timer_allocated();
  _GRVY_Timers->Reset();
  return;
}

// grvy_timer_finalize(): Define end of global portion to be monitored

void grvy_timer_finalize()
{
  GRVY_Internal::verify_C_timer_allocated();
  _GRVY_Timers->Finalize();
  return;
}

// grvy_timer_summarize(): Print a summary of all current timers

void grvy_timer_summarize()
{
  GRVY_Internal::verify_C_timer_allocated();
  _GRVY_Timers->Summarize();
  return;
}

// grvy_timer_summarize(): Print a summary of all current timers

int grvy_timer_set_summarize_width(const int maxwidth)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->SetSummarizeWidth(maxwidth));
}

// grvy_timer_stat_count(): provides number of calls to a timer

int grvy_timer_stats_count(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->StatsCount(id));
}

// grvy_timer_stat_mean(): provides mean timing for a timer

double grvy_timer_stats_mean(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->StatsMean(id));
}

// grvy_timer_stat_mean(): provides mean (inclusive) timing for a timer

double grvy_timer_stats_mean_inc(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->StatsMean_inc(id));
}

// grvy_timer_stat_variance(): provides variance for a timer

double grvy_timer_stats_variance(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->StatsVariance(id));
}

// grvy_timer_stat_variance(): provides (inclusive) variance for a timer

double grvy_timer_stats_variance_inc(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->StatsVariance_inc(id));
}

// grvy_timer_stat_min(): provides min value for a timer

double grvy_timer_stats_min(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->StatsMin(id));
}

// grvy_timer_stat_min(): provides (inclusive) min value for a timer

double grvy_timer_stats_min_inc(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->StatsMin_inc(id));
}

// grvy_timer_stat_max(): provides min value for a timer

double grvy_timer_stats_max(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->StatsMax(id));
}

// grvy_timer_stat_max(): provides (inclusive) min value for a timer

double grvy_timer_stats_max_inc(const char *id)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->StatsMax_inc(id));
}

// grvy_summarize_hist_timing(): summarize and dump historical timing information to ascii files
void grvy_summarize_hist_timing(const char *filename, const char *delimiter, const char *outdir)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->SummarizeHistTiming(filename,delimiter,outdir));
}

// grvy_timer_save_hist(): save internal GRVY timer to historical repository (short)

int grvy_timer_save_hist(const char*experiment, const char *comment, int num_procs,const char *filename)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->SaveHistTiming(experiment,comment,num_procs,filename));
}

// grvy_timer_save_hist(): save internal GRVY timer to historical repository (expanded)

int grvy_timer_save_hist_exp(const char*experiment, const char *comment, 
			     int num_procs, int jobId, const char *code_revision, double flops,
			     const char *filename)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->SaveHistTiming(experiment,comment,num_procs,jobId,code_revision,flops,filename));
}

// grvy_timer_save_hist_ext(): save external performance data to historical repository (short)

int grvy_timer_save_hist_ext(double timing, const char *machinename, const char*experiment, 
			     const char *comment, int num_procs,const char *filename)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->SaveHistTiming(timing,machinename,experiment,comment,num_procs,filename));
}

// grvy_timer_save_hist_ext_exp(): save external performance data to historical repository (expanded)

int grvy_timer_save_hist_ext_exp(double timing, const char *machinename, const char*experiment, const char *comment, 
				 int num_procs, int jobId, const char *code_revision, double flops,
				 const char *filename)
{
  GRVY_Internal::verify_C_timer_allocated();
  return(_GRVY_Timers->SaveHistTiming(timing,machinename,experiment,comment,num_procs,
				      jobId,code_revision,flops,filename));
}

//-----------------------------------------------------------------
// grvy_asci_time_fortran
// 
// C/C++ folks can call asctime directly, but let's give the 
// Fortran folks a convenience utility
//-----------------------------------------------------------------

extern "C" void grvy_asci_time_fortran(char *timestring) {
  struct tm *ptr;
  time_t tm;

  tm = time(NULL);
  ptr = localtime(&tm);

  // asctime() puts a newline at the end of the string - we won't include
  // that in the copy.

  strncpy(timestring,asctime(ptr),strlen(asctime(ptr)) - 1);
  return;
}


