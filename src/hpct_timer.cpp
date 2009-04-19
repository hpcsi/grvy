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

// Notes: for portability concerns, we will go with a simple timeofday
// based timer.  This should give resolution that is order
// micro-seconds on desired systems.  Note that this also avoids any
// issues that cycle-based counters face due to cpu freqeuncy
// throttling or SMP issues due to potential thread migration.
//
// A check is included to potentially warn against accuracy
// problems when the timer is used at a level below anticipated 
// threshold accuracy.

using namespace std;

#include<sys/time.h>
#include<time.h>
#include<stdio.h>
#include<hpct_int.h>
#include<iostream>
#include<math.h>

const static double HPCT_TIMER_THRESH = 9.0e-8;  // low water mark for expected timer usage deltas
const static double HPCT_PERC_TOL     = 1e-3;    // tolerance for defining acceptable global percentages
static double _HPCT_Timer_Last        = 0.;      // timer value at last call
static _HPCT_Type_TimerMap _HPCT_TimerMap;       // performance timer map

extern "C" double hpct_timer           ();
extern "C" void   hpct_timer_init      ();
extern "C" void   hpct_timer_finalize  ();
extern "C" void   hpct_timer_begin     (const char *id);
extern "C" void   hpct_timer_end       (const char *id);
extern "C" void   hpct_timer_summarize ();

double hpct_timer (void)
{
  int rc;
  struct timeval tv;

  rc = gettimeofday (&tv, NULL);
  if (rc == -1) {
    printf("tmrc: gettimeofday\n");
    _HPCT_message(_HPCT_emask,__func__,"Unable to use gettimeofday");
    return -1.;
  }

  double t1 =  ((double) tv.tv_sec) + 1.e-6*((double) tv.tv_usec);

  if( (t1 - _HPCT_Timer_Last) <= HPCT_TIMER_THRESH )
      _HPCT_message(_HPCT_wmask,__func__,"Timer accuracy may be insufficient - just measured:",
		    t1-_HPCT_Timer_Last);

  _HPCT_Timer_Last = t1;
  return t1;
}


void hpct_timer_begin(const char *id)
{
  vector <double> timings(2);
  double mytime;
  _HPCT_Type_TimerMap :: const_iterator index;

  mytime = hpct_timer();

  // Is this the first call for this id?

  index = _HPCT_TimerMap.find(id);

  if ( index == _HPCT_TimerMap.end() )
    {
      timings[0] = 0.0;	       // stores accumulated time
      timings[1] = mytime;     // stores latest timestamp
    }
  else
    {
      timings[0] = index->second[0];
      timings[1] = mytime;	
    }

  _HPCT_TimerMap[id] = timings;

  return;
}

void hpct_timer_end(const char *id)
{
  vector <double> timings(2);
  double mytime;
  _HPCT_Type_TimerMap :: const_iterator index;

  mytime = hpct_timer();
  index  = _HPCT_TimerMap.find(id);

  if ( index == _HPCT_TimerMap.end() )
    _HPCT_message(_HPCT_emask,__func__,"No timer data available for",id);
  else if(index->first[1] < 0)
    _HPCT_message(_HPCT_emask,__func__,"No matching begin timer call for",id);
  else
    {

      timings[0] = index->second[0] +  (mytime - index->second[1]);
      timings[1]  = -1.;

      _HPCT_TimerMap[id] = timings;
    }

  return;
}

// hpct_timer_init(): Define beginning of global portion to be monitored

void hpct_timer_init()
{

  _HPCT_Type_TimerMap :: iterator index;

  // first: reset accumulated time for any counters that have been 
  // defined previously

  for(index=_HPCT_TimerMap.begin(); index != _HPCT_TimerMap.end(); ++index)
    index->second[0] = 0.0;

  // initialize global timer region

  hpct_timer_begin(_HPCT_gtimer);
  return;
}

// hpct_timer_init(): Define beginning of global portion to be monitored

void hpct_timer_finalize()
{
  hpct_timer_end(_HPCT_gtimer);
  return;
}

// hpct_timer_summarize(): Print a summary of all current timers

void hpct_timer_summarize()
{
  vector <double> timings(2);
  double totaltime,subtime;
  double local_percentage, total_percentage;
  int global_time_defined = 0;

  _HPCT_Type_TimerMapSortLH _HPCT_TimerMapSortLH;
  _HPCT_Type_TimerMapSortHL _HPCT_TimerMapSortHL;

  _HPCT_Type_TimerMapSortLH :: iterator indexLH;
  _HPCT_Type_TimerMapSortHL :: iterator indexHL;

  _HPCT_Type_TimerMap :: iterator index,gindex;

  // Was a global timing region defined via HPCT_timer_init() and
  // HPCT_timer_end()?  If so, use the total time to define runtime
  // percentages.

  gindex = _HPCT_TimerMap.find(_HPCT_gtimer);
  if ( gindex != _HPCT_TimerMap.end() )
    {
      totaltime = gindex->second[0];
      global_time_defined = 1;

      // Sum the timings from all subordinate keys

      subtime = 0.0;

      for(index=_HPCT_TimerMap.begin(); index != _HPCT_TimerMap.end(); ++index)
	if(index != gindex)
	  subtime += index->second[0];

      // Reset the global key to store the exclusive cumulative time 

      timings[0] = totaltime - subtime;
      timings[1] = gindex->second[1];
      _HPCT_TimerMap[_HPCT_gtimer] = timings;

    }
  
  // sort the list via timings for output to the masses

  for(index=_HPCT_TimerMap.begin(); index != _HPCT_TimerMap.end(); ++index)
    {
      timings = index->second;
      _HPCT_TimerMapSortHL[timings] = index->first;
    }

  total_percentage = 0.0;

  printf("\n-------------------------------------------------------\n");
  printf("HPCT Wall Clock Performance Timings:\n");
  
  for(indexHL=_HPCT_TimerMapSortHL.begin(); indexHL != _HPCT_TimerMapSortHL.end(); ++indexHL)
    {
      printf("--> %-20s: %10.5e secs",indexHL->second.c_str(),indexHL->first[0]);
      if(global_time_defined)
	{
	  local_percentage  = 100.*indexHL->first[0]/(totaltime);
	  total_percentage += local_percentage;
	  printf(" (%8.4f %)\n",local_percentage);
	}
      else
	printf("\n");
    }

  if(global_time_defined)
    {
      printf("\n %22s = %10.5e secs (%8.4f %)\n","Total Measured Time",totaltime,total_percentage);

      if( fabs(total_percentage - 100.0) > HPCT_PERC_TOL )
	{
	  printf("\n%s: Profile percentages do not sum to 100 %%.\n",_HPCT_wmask);
	  printf("This likely means that you defined timer keys which are\n");
	  printf("not mutually exclusive.\n");
	}

    }

  printf("-------------------------------------------------------\n\n");


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

  std::strncpy(timestring,asctime(ptr),strlen(asctime(ptr)));
 
  return;

}


extern "C" void hpct_timer_init_() {
  hpct_timer_init();
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

