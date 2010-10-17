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

#include<sys/time.h>
#include<stdarg.h>
#include<time.h>
#include<stack>
#include "grvy_classes.h"
#include "grvy.h"
#include "grvy_int.h"

#include<boost/accumulators/accumulators.hpp>
#include<boost/accumulators/statistics/mean.hpp>
#include<boost/accumulators/statistics/stats.hpp>
#include<boost/accumulators/statistics/count.hpp>
#include<boost/accumulators/statistics/variance.hpp>
#include<boost/math/special_functions.hpp>

using namespace boost::accumulators;

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

using namespace std;
using namespace GRVY;

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

  void VerifyInit();

  short int   initialized;            // initialized?
  double      timer_last;             // raw timer value of last call
  double      timer_finalize;         // raw timer value at time of finalize()
  std::string timer_name;             // user name supplied for the timer
  int         num_begins;	      // number of active begin timers (used for callgraph determination)
  std::stack <std::string> callgraph; // callgraph to support embedded timers
  bool        beginTrigger;           // a trigger used for embedded timers
  _GRVY_Type_TimerMap2     TimerMap;  // map used to store performance timers for each defined key

  accumulator_set <double,features<tag::mean,tag::count,tag::variance> > stats_empty; // empty accumulator
};

} // matches namespace GRVY

int show_statistics = 1;

GRVY_Timer_Class::GRVY_Timer_Class(string name) :m_pimpl(new GRVY_Timer_ClassImp() )
{
  m_pimpl->timer_name     = name;
  m_pimpl->initialized    = 1;
  m_pimpl->timer_last     = 0.0;
  m_pimpl->timer_finalize = -1;
  m_pimpl->num_begins     = 0;	
  m_pimpl->beginTrigger   = false;
}

GRVY_Timer_Class::~GRVY_Timer_Class()
{
  // using auto_ptr for proper cleanup
}

void GRVY_Timer_Class::GRVY_Timer_ClassImp:: VerifyInit ()
{
  if( (_GRVY_Timers == NULL) || !initialized )
    {
      grvy_printf(GRVY_ERROR,"%s: timer uninitialized\n",__func__);
      exit(1);
    }
}

void GRVY_Timer_Class:: BeginTimer (const char *id, bool embeddedFlag)
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
      m_pimpl->num_begins++; 

      if(m_pimpl->num_begins > 2)
	{
	  //grvy_printf(GRVY_INFO,"begin: ending previous timer %s (newtimer = %s)\n",callgraph.top().c_str(),id);
	  EndTimer(m_pimpl->callgraph.top().c_str(),true);
	}
	
      if(m_pimpl->num_begins > m_pimpl->callgraph.size())
	{
	  m_pimpl->callgraph.push(id);
	  //grvy_printf(GRVY_INFO,"begin: m_pimpl->num_begins = %i (%s)\n",m_pimpl->num_begins,id);
	}
    }

  // Is this the first call for this id?

  index = m_pimpl->TimerMap.find(id);

  if ( index == m_pimpl->TimerMap.end() )
    {
      Data.timings[0] = 0.0;	                      // stores accumulated time
      Data.timings[1] = mytime;                       // stores latest timestamp

      m_pimpl->TimerMap[id] = Data;
    }
  else
    {
      (index->second).timings[1] = mytime;           // stores latest timestamp
    }
  
}

void GRVY_Timer_Class:: EndTimer (const char *id, bool embeddedFlag)
{
  double      mytime, increment;
  tTimer_Data Data;
  char temp_string[120];

  _GRVY_Type_TimerMap2 :: iterator index;

  mytime = RawTimer();
  index  = m_pimpl->TimerMap.find(id);

  if ( index == m_pimpl->TimerMap.end() )
    _GRVY_message(GRVY_ERROR,__func__,"No timer data available for",id);
  else if(index->first[1] < 0)
    _GRVY_message(GRVY_ERROR,__func__,"No matching begin timer call for",id);
  else
    {
      // update map with latest increment info

      increment = mytime - (index->second).timings[1];

      // warn against potential measurements that are too small

      if(!m_pimpl->beginTrigger)
	{
	  if( increment <= _GRVY_TIMER_THRESH )
	    {
	      grvy_printf(GRVY_WARN,"Timer acuracy may be insufficient (%.30s) -> measured %le secs\n",
			  id,increment);
	    }
	}
      else
	{
	  m_pimpl->beginTrigger = false;
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
	  //grvy_printf(GRVY_INFO,"end: m_pimpl->num_begins = %i\n",m_pimpl->num_begins);

	  if(m_pimpl->callgraph.size() >= 1)
	    m_pimpl->callgraph.pop();
	  else
	    grvy_printf(GRVY_ERROR,"GRVY (%s): no callgraph left for %s\n",__func__,id);

	  if(m_pimpl->num_begins > 2)
	    {
	      BeginTimer(m_pimpl->callgraph.top().c_str(),true);
	      m_pimpl->beginTrigger = true;
	      //grvy_printf(GRVY_INFO,"end: re-beginning timer for %s\n",callgraph.top().c_str());
	    }

	  m_pimpl->num_begins--;
	}
    }

  // Store the latest raw timer snapshot for the global timer region
  // to allow users to query the global elapsed time after 
  // grvy_timer_finalize()

  if ( strcmp(id,_GRVY_gtimer) == 0 )
    m_pimpl->timer_finalize = RawTimer();

  return;
}

void GRVY_Timer_Class:: Reset()
{
  _GRVY_Type_TimerMap2 :: iterator index;

  if(_GRVY_Timers == NULL)
    return;

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

double GRVY_Timer_Class:: RawTimer()
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

  m_pimpl->timer_last = t1;
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

int GRVY_Timer_Class::InitHistDB(const char *filename)
{
  GRVY::GRVY_HDF5_Class h5_file;
  //return(h5_file.file_create(filename,true));
}

int GRVY_Timer_Class::SaveHistTiming()
{
    
  return 0;
}

int GRVY_Timer_Class::SaveHistTiming(const char *id, double timing)
{
    
  return 0;
}

void GRVY_Timer_Class::SummarizeHistTiming(const char *filename)
{
  return;
}
  
