// -*-c++-*-
//-----------------------------------------------------------------------bl-
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
//-----------------------------------------------------------------------el-
//
// grvy_timer: Timing routines for performance monitoring
//
// $Id: grvy_timer_interface.cpp 15042 2010-10-17 15:39:22Z karl $
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Notes: for portability concerns, we will go with a simple timeofday
// based timer.  This should give resolution that is order
// micro-seconds on desired systems.  Note that this also avoids any
// issues that cycle-based counters face due to cpu freqeuncy
// throttling or SMP issues due to potential thread migration.
//
// A check is included to potentially warn against accuracy problems
// when the timer is used at a level below anticipated threshold
// accuracy.
//--------------------------------------------------------------------------

#include<sys/time.h>
#include<stdarg.h>
#include<time.h>
#include<stack>
#include "grvy_classes.h"
#include "grvy.h"
#include "grvy_int.h"
#include "grvy_env.h"

#include<boost/accumulators/accumulators.hpp>
#include<boost/accumulators/statistics/mean.hpp>
#include<boost/accumulators/statistics/stats.hpp>
#include<boost/accumulators/statistics/count.hpp>
#include<boost/accumulators/statistics/variance.hpp>
#include<boost/math/special_functions.hpp>

using namespace boost::accumulators;
using namespace std;
using namespace GRVY;

// Historical logging packet table type for HDF

#define MAX_TIMER_WIDTH_V1 120
#define PTABLE_VERSION     1

typedef struct Timer_PTable_V1 {
  char timer_name[MAX_TIMER_WIDTH_V1];
  double measurement;
  double mean;
  double variance;
  int count;
} Timer_PTable_V1;

// Individual timer data structure

typedef struct GRVY_Timer_Data {
  double timings[2];
  accumulator_set <double,features<tag::mean,tag::count,tag::variance> > stats;
} tTimer_Data;

typedef std::map <std::string, GRVY_Timer_Data > _GRVY_Type_TimerMap2;

//-------------------------------------------------------
// private implementation class definition for GRVY_Timer
//-------------------------------------------------------

namespace GRVY {

class GRVY_Timer_Class::GRVY_Timer_ClassImp
{
public:
  GRVY_Timer_ClassImp() {}
 ~GRVY_Timer_ClassImp() {}

  void   VerifyInit  ();
  void   BeginTimer  (const char *,bool); 
  void   EndTimer    (const char *,bool);
  double RawTimer    ();

  short int   initialized;            // initialized?
  double      timer_finalize;         // raw timer value at time of finalize()
  std::string timer_name;             // user name supplied for the timer
  int         num_begins;	      // number of active begin timers (used for callgraph determination)
  std::stack <std::string> callgraph; // callgraph to support embedded timers
  bool        beginTrigger;           // a trigger used for embedded timers
  _GRVY_Type_TimerMap2     TimerMap;  // map used to store performance timers for each defined key

  accumulator_set <double,features<tag::mean,tag::count,tag::variance> > stats_empty; // empty accumulator
};

} // matches namespace GRVY

namespace GRVY {

int show_statistics = 1;

GRVY_Timer_Class::GRVY_Timer_Class() :m_pimpl(new GRVY_Timer_ClassImp() )
{
  //m_pimpl->timer_name     = name;
  m_pimpl->initialized    = 1;
  m_pimpl->timer_finalize = -1;
  m_pimpl->num_begins     = 0;	
  m_pimpl->beginTrigger   = false;
}

GRVY_Timer_Class::~GRVY_Timer_Class()
{
  // using auto_ptr for proper cleanup
}

void GRVY_Timer_Class::Init(string name)
{
  m_pimpl->timer_name = name;
  BeginTimer(_GRVY_gtimer);
  return;
}

void GRVY_Timer_Class::Finalize()
{
  EndTimer(_GRVY_gtimer);
  return;
}

void GRVY_Timer_Class::GRVY_Timer_ClassImp:: VerifyInit ()
{
  if( !initialized )
    {
      grvy_printf(GRVY_ERROR,"%s: timer uninitialized\n",__func__);
      exit(1);
    }
}

void GRVY_Timer_Class::BeginTimer (const char *id)
{
  return(m_pimpl->BeginTimer(id,false));
}

void GRVY_Timer_Class::EndTimer (const char *id)
{
  return(m_pimpl->EndTimer(id,false));
}

void GRVY_Timer_Class::GRVY_Timer_ClassImp::BeginTimer (const char *id, bool embeddedFlag)
{
  double mytime;
  _GRVY_Type_TimerMap2 :: iterator index;

  tTimer_Data Data;

  //  VerifyInit();

  // ------------------
  // Get current time
  // ------------------

  mytime = RawTimer();

  // ----------------------------------------------------------------
  // Embedded Timer Support:
  // 
  // Maintain callgraph for embedded timers; note that we detect an
  // embedded timer when num_begins exceeds a value of 2 as we
  // always have one global catch-all timer created during the init()
  // process.  When we detect an embedded timer, we use the
  // callgraph information to automatically EndTimer() the surrounding
  // timer. Since the EndTimer routine will have to do the opposite
  // to restart the timer, an embeddedFlag is used to distinguish
  // embedded begin/end calls from normal entries.
  // ----------------------------------------------------------------

  if(!embeddedFlag)
    {
      num_begins++; 

      if(num_begins > 2)
	{
	  //grvy_printf(GRVY_INFO,"begin: ending previous timer %s (newtimer = %s)\n",callgraph.top().c_str(),id);
	  EndTimer(callgraph.top().c_str(),true);
	}
	
      if(num_begins > callgraph.size())
	{
	  callgraph.push(id);
	  //grvy_printf(GRVY_INFO,"begin: num_begins = %i (%s)\n",num_begins,id);
	}
    }

  // Is this the first call for this id?

  index = TimerMap.find(id);

  if ( index == TimerMap.end() )
    {
      Data.timings[0] = 0.0;	                      // stores accumulated time
      Data.timings[1] = mytime;                       // stores latest timestamp

      TimerMap[id] = Data;
    }
  else
    {
      (index->second).timings[1] = mytime;           // stores latest timestamp
    }
  
}

void GRVY_Timer_Class::GRVY_Timer_ClassImp::EndTimer (const char *id, bool embeddedFlag)
{
  double      mytime, increment;
  tTimer_Data Data;
  char temp_string[120];

  _GRVY_Type_TimerMap2 :: iterator index;

  mytime = RawTimer();
  index  = TimerMap.find(id);

  if ( index == TimerMap.end() )
    _GRVY_message(GRVY_ERROR,__func__,"No timer data available for",id);
  else if(index->first[1] < 0)
    _GRVY_message(GRVY_ERROR,__func__,"No matching begin timer call for",id);
  else
    {
      // update map with latest increment info

      increment = mytime - (index->second).timings[1];

      // warn against potential measurements that are too small

      if(!beginTrigger)
	{
	  if( increment <= _GRVY_TIMER_THRESH )
	    {
	      grvy_printf(GRVY_WARN,"Timer acuracy may be insufficient (%.30s) -> measured %le secs\n",
			  id,increment);
	    }
	}
      else
	{
	  beginTrigger = false;
	}

      (index->second).timings[0] += increment;
      (index->second).timings[1]  = -1.;

      // TODO: with support for embedded timers, we need to retool
      // the boost accumulators a bit.  In order to not over-count
      // the timer calls, we have to avoid calling the accumulator
      // when this is an embedded closure; however, we will lose
      // some of the statistics coverage -> probably going to have
      // to switch to an increment value which is accumulated until
      // the final endtimer.

      if(!embeddedFlag)
	(index->second).stats(increment);

      // ----------------------------------------------------------------
      // Embedded Timer Support:
      // 
      // Pop curent timer off the callgraph stack and restart
      // surrounding timer if necessary.
      // ----------------------------------------------------------------

      if(!embeddedFlag)
	{

	  //grvy_printf(GRVY_INFO,"end: popping id %s (size = %i)\n",callgraph.top().c_str(),callgraph.size());
	  //grvy_printf(GRVY_INFO,"end: num_begins = %i\n",num_begins);

	  if(callgraph.size() >= 1)
	    callgraph.pop();
	  else
	    grvy_printf(GRVY_ERROR,"GRVY (%s): no callgraph left for %s\n",__func__,id);

	  if(num_begins > 2)
	    {
	      BeginTimer(callgraph.top().c_str(),true);
	      beginTrigger = true;
	      //grvy_printf(GRVY_INFO,"end: re-beginning timer for %s\n",callgraph.top().c_str());
	    }

	  num_begins--;
	}
    }

  // Store the latest raw timer snapshot for the global timer region
  // to allow users to query the global elapsed time after 
  // grvy_timer_finalize()

  if ( strcmp(id,_GRVY_gtimer) == 0 )
    timer_finalize = RawTimer();

  return;
}

void GRVY_Timer_Class:: Reset()
{
  _GRVY_Type_TimerMap2 :: iterator index;

  if(!m_pimpl->initialized)
    return;

  // Reset All timers

  for(index=m_pimpl->TimerMap.begin(); index != m_pimpl->TimerMap.end(); ++index)
    {
      (index->second).timings[0] = 0.0;                  // reset raw timing info
      (index->second).stats      = m_pimpl->stats_empty; // reset accumulator (no reset in Boost currently)
    }

  return;
}

double GRVY_Timer_Class::RawTimer() 
{
  return(m_pimpl->RawTimer());
}

double GRVY_Timer_Class::GRVY_Timer_ClassImp::RawTimer()
{
  int rc;
  struct timeval tv;

  rc = gettimeofday (&tv, NULL);
#if 0
  if (rc == -1) {
    printf("tmrc: gettimeofday\n");
    return -1.;
  }
#endif

  double t1 =  ((double) tv.tv_sec) + 1.e-6*((double) tv.tv_usec);

  return(t1);
}

double GRVY_Timer_Class:: ElapsedSeconds(const char *id)
{
  double elapsedseconds = 0.0;

  m_pimpl->VerifyInit();

  _GRVY_Type_TimerMap2 :: const_iterator index = m_pimpl->TimerMap.find(id);

  if ( index == m_pimpl->TimerMap.end() )
    _GRVY_message(GRVY_ERROR,__func__,"No timer data available for",id);
  else if( (index->second).timings[1] != -1)
    _GRVY_message(GRVY_ERROR,__func__,"Timer still active for",id);
  else
    elapsedseconds = (index->second).timings[0];

  return elapsedseconds;
}

int GRVY_Timer_Class:: StatsCount(const char *id)
{
  _GRVY_Type_TimerMap2 :: const_iterator index = m_pimpl->TimerMap.find(id);

  if ( index == m_pimpl->TimerMap.end() )
    {
      _GRVY_message(GRVY_ERROR,__func__,"No stats data available for",id);
      return(-1);
    }
  else
    {
      return(boost::accumulators::count((index->second).stats));
    }
}

double GRVY_Timer_Class:: StatsMean(const char *id)
{
  _GRVY_Type_TimerMap2 :: const_iterator index = m_pimpl->TimerMap.find(id);

  if ( index == m_pimpl->TimerMap.end() )
    {
      _GRVY_message(GRVY_ERROR,__func__,"No stats data available for",id);
      return -1.0;
    }
  else
    {
      return(boost::accumulators::mean((index->second).stats));
    }
}

double GRVY_Timer_Class:: StatsVariance(const char *id)
{
  _GRVY_Type_TimerMap2 :: const_iterator index = m_pimpl->TimerMap.find(id);

  if ( index == m_pimpl->TimerMap.end() )
    {
      _GRVY_message(GRVY_ERROR,__func__,"No stats data available for",id);
      return -1.0;
    }
  else
    {
      return(boost::accumulators::variance((index->second).stats));
    }
}

double GRVY_Timer_Class:: ElapsedGlobal()
{
  double elapsedseconds = 0.0;
  double mytime;

  m_pimpl->VerifyInit();

  mytime = RawTimer();

  _GRVY_Type_TimerMap2 :: const_iterator index = m_pimpl->TimerMap.find(_GRVY_gtimer);

  if ( index == m_pimpl->TimerMap.end() )
    _GRVY_message(GRVY_ERROR,__func__,"No timer data available for",_GRVY_gtimer);
  else if( (index->second).timings[1] != -1)     // inside active timer
    elapsedseconds = mytime - (index->second).timings[1];
  else if( (index->second).timings[1] == -1)     // outside active timer
    elapsedseconds = (index->second).timings[0];

  // if we are being called after a finalize(), include the elapsed
  // time since the last global timer was updated

  if (m_pimpl->timer_finalize > 0)
    elapsedseconds += mytime - m_pimpl->timer_finalize;

  return elapsedseconds;
}

void GRVY_Timer_Class:: Summarize()
{
  std::vector <double> timings(2);
  double totaltime,subtime;
  double local_percentage, total_percentage;
  int    global_time_defined = 0;
  int    max_timer_name_width;
  size_t display_id_width = 20;
  size_t timer_name_width;
  const size_t max_stdout_width = 120;

  _GRVY_Type_TimerMapSortLH _GRVY_TimerMapSortLH;
  _GRVY_Type_TimerMapSortHL _GRVY_TimerMapSortHL;

  _GRVY_Type_TimerMapSortLH :: iterator indexLH;
  _GRVY_Type_TimerMapSortHL :: iterator indexHL;

  _GRVY_Type_TimerMap2 :: iterator index,gindex;

  m_pimpl->VerifyInit();

  // Was a global timing region defined via GRVY_timer_init() and
  // GRVY_timer_end()?  If so, use the total time to define runtime
  // percentages.

  gindex = m_pimpl->TimerMap.find(_GRVY_gtimer);

  if ( gindex != m_pimpl->TimerMap.end() )
    {
      totaltime = (gindex->second).timings[0];
      global_time_defined = 1;

      // Sum the timings from all subordinate keys

      subtime = 0.0;

      for(index=m_pimpl->TimerMap.begin(); index != m_pimpl->TimerMap.end(); ++index)
	if(index != gindex)
	  subtime += (index->second).timings[0];

      // Temporarily reset the global key to store the exclusive cumulative time only.

      (gindex->second).timings[0] = totaltime - subtime;
      
    }
  
  // sort the list via timings for output to the masses

  for(index=m_pimpl->TimerMap.begin(); index != m_pimpl->TimerMap.end(); ++index)
    {

      if( index->first != gindex->first)
	{
	  timings[0] = (index->second).timings[0];
	  timings[1] = (index->second).timings[1];

	  _GRVY_TimerMapSortHL[timings] = index->first;

	  // Update display width if this identifier is longer than default

	  display_id_width = std::max(display_id_width, index->first.length()+1);
	  display_id_width = std::min(display_id_width, max_stdout_width - 35);
	}
    }

  total_percentage = 0.0;

  printf("\n");
  for(int i=0;i<display_id_width+35;i++)
    printf("-");

  if(show_statistics)
    for(int i=0;i<40;i++)
      printf("-");

  printf("\n");

  max_timer_name_width = display_id_width + 13;

  std::string varstring = m_pimpl->timer_name.substr(0,max_timer_name_width-1);

  printf("%s - %-*s ",varstring.c_str(),(int)(display_id_width+32-varstring.length()),"Performance Timings:");

  if(show_statistics)
    printf("|      Mean      Variance       Count");

  printf("\n");

  // Print results for all user-defined timer keys

  for(indexHL=_GRVY_TimerMapSortHL.begin(); indexHL != _GRVY_TimerMapSortHL.end(); ++indexHL)
    {

      std::string varstring = indexHL->second.substr(0,display_id_width-1);
      printf("--> %-*s: %10.5e secs",(int)display_id_width,varstring.c_str(),indexHL->first[0]);

      if(global_time_defined)
	{
	  local_percentage  = 100.*indexHL->first[0]/(totaltime);
	  total_percentage += local_percentage;
	  printf(" (%8.4f %%)",local_percentage);
	}
      
      if(show_statistics)
	{
	  if(indexHL->second !=  _GRVY_gtimer )
	    {
	      gindex = m_pimpl->TimerMap.find(indexHL->second);
	      if(boost::accumulators::count((gindex->second).stats) > 0)
		{
		  printf(" | [%10.5e  %10.5e  %9zi]",
			 boost::accumulators::mean    ((gindex->second).stats),
			 boost::accumulators::variance((gindex->second).stats),
			 boost::accumulators::count   ((gindex->second).stats));
		}
	      else
		{
		  printf(" | [%10s  %10.5e  %9zi]","   N/A   ",
			 boost::accumulators::variance((gindex->second).stats),
			 boost::accumulators::count   ((gindex->second).stats));
		}
	    }
	}
      
      printf("\n");
      
    }

  // Print results for left-over contribution in main timer

  gindex = m_pimpl->TimerMap.find(_GRVY_gtimer);

  printf("--> %-*s: %10.5e secs",(int)display_id_width,_GRVY_gtimer,
	 (gindex->second).timings[0]);

  if(global_time_defined)
    {
      local_percentage  = 100.*(gindex->second).timings[0]/(totaltime);
      total_percentage += local_percentage;
      printf(" (%8.4f %%)",local_percentage);
    }

  printf("\n");

  // A little sanity checking on the results
      
  if(global_time_defined)
    {
      printf("\n %*s = %10.5e secs (%8.4f %%)\n",(int)display_id_width+2,"Total Measured Time",
	     totaltime,total_percentage);

      if( fabs(total_percentage - 100.0) > _GRVY_PERC_TOL )
	{
	  printf("\n%s: Profile percentages do not sum to 100 %%.\n",_GRVY_wmask);
	  printf("This likely means that you defined timer keys which are\n");
	  printf("not mutually exclusive.\n");
	}
      
      // Restore the global key timing to store inclusive cumulative time 

      (gindex->second).timings[0] += subtime;

    }

  for(int i=0;i<display_id_width+35;i++)
    printf("-");

  if(show_statistics)

    for(int i=0;i<40;i++)
      printf("-");

  printf("\n\n");
}

// SaveHistTime(): used to save the current profiled timer to an HDF5 file for
// historical monitoring purposed.  Should be called after the global
// timer has been Finalized.

int GRVY_Timer_Class::SaveHistTiming(const char *filename)
{

  GRVY_HDF5_Class h5;

#ifndef HAVE_HDF5
  return 1;
#endif

  m_pimpl->VerifyInit();

  // Open existing file or create new one if not present

  if( h5.Exists(filename) )
    h5.Open(filename,false);
  else
    h5.Create(filename,false);

  grvy_printf(GRVY_DEBUG,"%s: hdf5 file opened/created for %s\n",__func__,filename);

  // Open/create GRVY timer group

  string toplevel("GRVY/Performance_timings/");

  if (h5.GroupExists(toplevel))
    h5.GroupOpen(toplevel);
  else
    h5.GroupCreate(toplevel);

  // Query runtime host environment

  GRVY_Hostenv_Class myenv;

  // Open/create group for this host/machine

  string hostlevel = toplevel+myenv.Hostname();

  if (h5.GroupExists(hostlevel))
    h5.GroupOpen(hostlevel);
  else
    h5.GroupCreate(hostlevel);

  // Setup packet table datatype for storing timer information

  hid_t ptable_type;

  if(PTABLE_VERSION == 1 )
    {
      hid_t strtype;
      Timer_PTable_V1 *data;

      strtype = H5Tcopy(H5T_C_S1);

      H5Tset_size(strtype,(size_t)MAX_TIMER_WIDTH_V1);
      H5Tset_strpad(strtype,H5T_STR_NULLTERM);

      grvy_printf(GRVY_DEBUG,"creating compound datatype\n");

      if( (ptable_type = H5Tcreate (H5T_COMPOUND, sizeof(Timer_PTable_V1))) < 0 )
	{
	  grvy_printf(GRVY_FATAL,"%s: Unable to create compound HDF datatype\n",__func__);
	  exit(1);
	}

      H5Tinsert(ptable_type, "timer_name", HOFFSET(Timer_PTable_V1, timer_name), strtype);
      H5Tinsert(ptable_type, "measurement",HOFFSET(Timer_PTable_V1, measurement),H5T_NATIVE_DOUBLE);
      H5Tinsert(ptable_type, "mean",       HOFFSET(Timer_PTable_V1, mean),       H5T_NATIVE_DOUBLE);
      H5Tinsert(ptable_type, "variance",   HOFFSET(Timer_PTable_V1, variance),   H5T_NATIVE_DOUBLE);
      H5Tinsert(ptable_type, "count",      HOFFSET(Timer_PTable_V1, count),      H5T_NATIVE_INT);

    }
  else
    {
      grvy_printf(GRVY_FATAL,"%s: unknown timer packet table version\n",__func__);
    }

  grvy_printf(GRVY_DEBUG,"creating variable-length datatype\n");

  hid_t timers_type;

  // Open/create packet table for this host/machine using variable
  // length packets

  timers_type = H5Tvlen_create( ptable_type );

  hid_t tableId;

  if( (tableId = H5PTcreate_fl(h5.m_pimpl->groupIds[hostlevel],"PTable",timers_type,(hsize_t)256,-1)) == H5I_BADID)
    {
      grvy_printf(GRVY_FATAL,"%s: Unable to create HDF packet table (%s)\n",__func__,"PTable");
      exit(1);
    }

  // TODO: assign local packet table version as attribute in case we ever need to change.
  
  // TODO: write grvy performance data

  // TODO: provide mechanism to embed batch system job number

  // Clean up shop

  h5.Close();

  return 0;
}

int GRVY_Timer_Class::SaveHistTiming(const char *filename, const char *id, double timing)
{
    
  return 0;
}

void GRVY_Timer_Class::SummarizeHistTiming(const char *filename)
{
  return;
}
  
}
