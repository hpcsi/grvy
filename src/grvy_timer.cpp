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
#include<stdio.h>
#include<algorithm>
#include<time.h>
#include<stack>
#include<grvy_classes.h>
#include<grvy_env.h>
#include<grvy.h>
#include<grvy_int.h>

#include<boost/accumulators/accumulators.hpp>
#include<boost/accumulators/statistics/mean.hpp>
#include<boost/accumulators/statistics/stats.hpp>
#include<boost/accumulators/statistics/count.hpp>
#include<boost/accumulators/statistics/max.hpp>
#include<boost/accumulators/statistics/min.hpp>
#include<boost/accumulators/statistics/variance.hpp>
#include<boost/math/special_functions.hpp>

using namespace boost::accumulators;
using namespace std;
using namespace GRVY;

#define PTABLE_VERSION 1	// default version beginning Oct. 2010

#ifdef HAVE_HDF5
#include "grvy_ptable.h"
#endif

//--------------------------------
// Individual timer data structure
//--------------------------------

typedef struct GRVY_Timer_Data {
  double timings[2];
  accumulator_set <double,features<tag::mean,tag::count,tag::variance> > stats;
} tTimer_Data;

typedef struct minmax {
  double value;
  size_t index;
} minmax;

typedef std::map <std::string, GRVY_Timer_Data > _GRVY_Type_TimerMap2;
typedef accumulator_set <double,features<tag::mean,tag::variance,tag::count> > perf_stats;

//-------------------------------------------------------
// private implementation class definition for GRVY_Timer
//-------------------------------------------------------

namespace GRVY {

class GRVY_Timer_Class::GRVY_Timer_ClassImp
{
public:
  GRVY_Timer_ClassImp    () {}
 ~GRVY_Timer_ClassImp    () {}
		         
  void   VerifyInit      ();
  void   BeginTimer      (const char *, bool); 
  void   EndTimer        (const char *, bool);
  double RawTimer        ();

#ifdef HAVE_HDF5
  hid_t  CreateHistType  (int version); 
  int    AppendHistData  (double timing, string experiment, string comment, int num_procs, int jobId,
			  string code_revision, double flops, int version, hid_t tableId, bool save_internal_timer);
  int    ReadAllHostData (GRVY_HDF5_Class *h5, hid_t tableId, vector <TimerPTable_V1> *data);
  int    ReadPTable      (string host);
  void   SummarizeHost   (string host);
  void   WriteHeaderInfo (FILE *fp, const char *delim);
#endif
  int    SaveHistTiming  (double timiing, string machinename, string experiment, string comment, int num_procs, 
			  int jobId, string code_revision, double flops, string filename, bool save_internal_timer );


  bool        initialized;            // initialized?
  double      timer_finalize;         // raw timer value at time of finalize()
  std::string timer_name;             // user name supplied for the timer
  int         num_begins;	      // number of active begin timers (used for callgraph determination)
  std::stack <std::string> callgraph; // callgraph to support embedded timers
  bool        beginTrigger;           // a trigger used for embedded timers
  _GRVY_Type_TimerMap2     TimerMap;  // map used to store performance timers for each defined key

  std::map<std::string,bool> options; // runtime options

  int    default_jobId;
  double default_flops;
  //int default_revision;
  string default_revision;

  GRVY_Timer_Class *self;	      // back pointer to public class

  accumulator_set <double,features<tag::mean,tag::count,tag::variance> > stats_empty; // empty accumulator
};

} // matches namespace GRVY

namespace GRVY {

int show_statistics = 1;

GRVY_Timer_Class::GRVY_Timer_Class() :m_pimpl(new GRVY_Timer_ClassImp() )
{
  m_pimpl->initialized           = false;
  m_pimpl->timer_finalize        = -1;
  m_pimpl->num_begins            = 0;	
  m_pimpl->beginTrigger          = false;
  m_pimpl->self                  = this;

  // default historical timing values

  m_pimpl->default_jobId         = -1;
  m_pimpl->default_flops         = 0.0;   
  m_pimpl->default_revision      = "unknown";
  //m_pimpl->default_revision      = -1;

  // set default options for gdump

  m_pimpl->options["output_stdout"        ] = true;
  m_pimpl->options["output_totaltimer_raw"] = true;
  m_pimpl->options["output_subtimer_raw"  ] = false;
  m_pimpl->options["dump_files"           ] = true;
}

GRVY_Timer_Class::~GRVY_Timer_Class()
{
  // using auto_ptr for proper cleanup
}

void GRVY_Timer_Class::Init(string name)
{
  m_pimpl->initialized = true;
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
	  //Ginfo("begin: ending previous timer %s (newtimer = %s)\n",callgraph.top().c_str(),id);
	  EndTimer(callgraph.top().c_str(),true);
	}
	
      if(num_begins > callgraph.size())
	{
	  callgraph.push(id);
	  //Ginfo("begin: num_begins = %i (%s)\n",num_begins,id);
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

	  //Ginfo("end: popping id %s (size = %i)\n",callgraph.top().c_str(),callgraph.size());
	  //Ginfo("end: num_begins = %i\n",num_begins);

	  if(callgraph.size() >= 1)
	    callgraph.pop();
	  else
	    grvy_printf(GRVY_ERROR,"GRVY (%s): no callgraph left for %s\n",__func__,id);

	  if(num_begins > 2)
	    {
	      BeginTimer(callgraph.top().c_str(),true);
	      beginTrigger = true;
	      //Ginfo("end: re-beginning timer for %s\n",callgraph.top().c_str());
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

double GRVY_Timer_Class:: ElapsedSeconds(string id)
{
  double elapsedseconds = 0.0;

  m_pimpl->VerifyInit();

  _GRVY_Type_TimerMap2 :: const_iterator index = m_pimpl->TimerMap.find(id);

  if ( index == m_pimpl->TimerMap.end() )
    _GRVY_message(GRVY_ERROR,__func__,"No timer data available for",id.c_str());
  else if( (index->second).timings[1] != -1)
    _GRVY_message(GRVY_ERROR,__func__,"Timer still active for",id.c_str());
  else
    elapsedseconds = (index->second).timings[0];

  return elapsedseconds;
}

size_t GRVY_Timer_Class:: StatsCount(string id)
{
  _GRVY_Type_TimerMap2 :: const_iterator index = m_pimpl->TimerMap.find(id);

  if ( index == m_pimpl->TimerMap.end() )
    {
      _GRVY_message(GRVY_WARN,__func__,"No stats data available for",id.c_str());
      return(0);
    }
  else
    {
      return(boost::accumulators::count((index->second).stats));
    }
}

double GRVY_Timer_Class:: StatsMean(string id)
{
  _GRVY_Type_TimerMap2 :: const_iterator index = m_pimpl->TimerMap.find(id);

  if ( index == m_pimpl->TimerMap.end() )
    {
      _GRVY_message(GRVY_ERROR,__func__,"No stats data available for",id.c_str());
      return -1.0;
    }
  else
    {
      return(boost::accumulators::mean((index->second).stats));
    }
}

double GRVY_Timer_Class:: StatsVariance(string id)
{
  _GRVY_Type_TimerMap2 :: const_iterator index = m_pimpl->TimerMap.find(id);

  if ( index == m_pimpl->TimerMap.end() )
    {
      _GRVY_message(GRVY_ERROR,__func__,"No stats data available for",id.c_str());
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
	  grvy_printf(GRVY_WARN,"\n[*] Warning: Profile percentages do not sum to 100 %%.\n");
	  grvy_printf(GRVY_WARN,"This likely means that you defined timer keys which are\n");
	  grvy_printf(GRVY_WARN,"not mutually exclusive.\n");
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

//--------------------------------------------------------------------
// SaveHistTime(): used to save the current profiled timer to an HDF5
// file for historical monitoring purposed.  Should be called after
// the global timer has been Finalized.
//--------------------------------------------------------------------

// SaveHistTiming wrapper -> save_internal_timer

int GRVY_Timer_Class::SaveHistTiming(string experiment, string comment, int num_procs, string filename)
{
  GRVY_Hostenv_Class myenv;
  bool save_internal_timer = true;
  double timing = 0.0;

  return(m_pimpl->SaveHistTiming(timing,myenv.Hostname(),experiment,comment,num_procs,
				 m_pimpl->default_jobId,m_pimpl->default_revision,m_pimpl->default_flops,filename,
				 save_internal_timer));
}

// SaveHistTiming wrapper -> save_internal_timer

int GRVY_Timer_Class::SaveHistTiming(string experiment, string comment, int num_procs, 
				     int jobId, string code_revision, double flops, string filename )
{
  GRVY_Hostenv_Class myenv;
  bool save_internal_timer = true;
  double timing = 0.0;

  return(m_pimpl->SaveHistTiming(timing,myenv.Hostname(),experiment,comment,num_procs,
				 jobId,code_revision,flops,filename,save_internal_timer));
}

// SaveHistTiming wrapper -> save external timing

int GRVY_Timer_Class::SaveHistTiming(double timing, string machine, string experiment, string comment, int num_procs, 
				     int jobId, string code_revision, double flops, string filename )
{
  bool save_internal_timer = false;

  return(m_pimpl->SaveHistTiming(timing,machine,experiment,comment,num_procs,
				 jobId,code_revision,flops,filename,save_internal_timer));
}

int GRVY_Timer_Class::GRVY_Timer_ClassImp::SaveHistTiming(double timing, string machinename, string experiment, 
							  string comment, int num_procs, int jobId,
							  string code_revision, double flops, string filename,
							  bool save_internal_timer )
{

  GRVY_HDF5_Class h5;

#ifndef HAVE_HDF5
  return 1;  // above h5 ctor will error if HDF5 is not available
#else

  // Make sure we have timer data to save when we are in the mode of
  // saving internal timer data 

  if(save_internal_timer && !initialized)
    {
      grvy_printf(GRVY_ERROR,"\n**Error (%s): Performance timer not initialized - unable to save timing\n",__func__);
      exit(1);
    }

  // Open existing file or create new one if not present

  if( h5.Exists(filename) )
    h5.Open(filename,false);
  else
    h5.Create(filename,false);

  grvy_printf(GRVY_DEBUG,"%s: hdf5 file opened/created for %s\n",__func__,filename.c_str());

  // Open/create GRVY timer group

  string toplevel("GRVY/Performance_timings/");

  if (h5.GroupExists(toplevel))
    h5.GroupOpen(toplevel);
  else
    h5.GroupCreate(toplevel);



  // Open/create group for this host/machine

  //  string hostlevel = toplevel+myenv.Hostname();
  string hostlevel = toplevel + machinename;

  if (h5.GroupExists(hostlevel))
    h5.GroupOpen(hostlevel);
  else
    h5.GroupCreate(hostlevel);

  // Setup packet table datatype for storing timer information

  hid_t timers_type = CreateHistType(PTABLE_VERSION);

  // Open/create packet table for this host/machine using variable
  // length packets

  string tablename("PTable");
  hid_t  tableId;

  if (h5.m_pimpl->PTableExists(hostlevel,tablename))
    {
      tableId = h5.m_pimpl->PTableOpen(hostlevel,tablename);
    }
  else
    {
      if( (tableId = H5PTcreate_fl(h5.m_pimpl->groupIds[hostlevel],tablename.c_str(),
				   timers_type,(hsize_t)256,-1)) == H5I_BADID)
	{
	  grvy_printf(GRVY_FATAL,"%s: Unable to create HDF packet table (%s)\n",__func__,tablename.c_str());
	  exit(1);
	}

      // Query runtime host environment

      GRVY_Hostenv_Class myenv;

      // Save environment attributes and assign local packet table
      // version in case we ever need to make a change in the future

      int format_version = PTABLE_VERSION;

      h5.AttributeWrite(hostlevel,"format_version", format_version);
      h5.AttributeWrite(hostlevel,"os_sysname",     myenv.os_sysname);
      h5.AttributeWrite(hostlevel,"os_release",     myenv.os_release);
      h5.AttributeWrite(hostlevel,"os_version",     myenv.os_version);
      h5.AttributeWrite(hostlevel,"cputype",        myenv.cputype   );
      
    }
    
  // Append performance data to HDF packat table

  AppendHistData(timing,experiment,comment,num_procs,jobId,code_revision,flops,
		 PTABLE_VERSION,tableId,save_internal_timer);
  
  // Clean up shop

  H5Tclose(timers_type);
  h5.m_pimpl->PTableClose(tableId);

  h5.Close();

  return 0;
#endif
}

void GRVY_Timer_Class::SummarizeHistTiming(string filename,string delimiter, string outdir)
{

  GRVY_HDF5_Class h5;

#ifndef HAVE_HDF5
  return;  // above h5 ctor will error if HDF5 is not available
#else

  // Cache user options

  bool dump_stdout        = m_pimpl->options["output_stdout"];
  bool dump_files         = m_pimpl->options["dump_files"];
  bool output_totaltimers = m_pimpl->options["output_totaltimer_raw"];
  bool output_subtimers   = m_pimpl->options["output_subtimer_raw"];

  // Open existing file

  h5.Open(filename,true);

  // Open GRVY timer group

  string toplevel("GRVY/Performance_timings/");
  h5.GroupOpen(toplevel);

  // Scan for all available host timings

  vector<string> machines = h5.ListSubGroups(toplevel);

  // Summarize results/stats for each host/machine; note that we
  // compute statistics on a per-experiment basis.

  int Ptable_Version;

  // ------------------------------------------
  // Main loop over all available host timings
  // ------------------------------------------

  if(dump_stdout) 
    {
      if(dump_files)
	{
	  grvy_printf(GRVY_INFO,"\nDumping historical performance data to ascii file(s)\n\n");
	}
      else
	{
	  grvy_printf(GRVY_INFO,"\nSummaring available historical performance:\n\n");
	}
    }

  for(int imach=0;imach<machines.size();imach++) 
    {
      map <string,perf_stats> statistics;
      string machine = toplevel+machines[imach];

      // read machine attributes

      string os_sysname, os_release, os_version, cputype;
      
      h5.AttributeRead(machine,"format_version",Ptable_Version);
      h5.AttributeRead(machine,"os_sysname",    os_sysname);
      h5.AttributeRead(machine,"os_release",    os_release);
      h5.AttributeRead(machine,"os_version",    os_version);
      h5.AttributeRead(machine,"cputype",       cputype);

      hid_t timers_type = m_pimpl->CreateHistType(Ptable_Version);
      hid_t     tableId = h5.m_pimpl->PTableOpen(toplevel+machines[imach],"PTable");

      map<string,minmax> max_vals;
      map<string,minmax> min_vals;
      int max_revision_width = 0;

      // -------------------------------------------------
      // Read performance data in specified version format
      // -------------------------------------------------

      switch(Ptable_Version) {

      case 1:
	{
	  vector<TimerPTable_V1> data;
	  m_pimpl->ReadAllHostData(&h5,tableId,&data);
	  h5.m_pimpl->PTableClose(tableId);

	  grvy_printf(GRVY_DEBUG,"%s: Number of experiments = %i\n",__func__,data.size());	  

	  //------------------------------------------
	  // accumulate basic statistics for the host
	  // -----------------------------------------

	  for(int i=0;i<data.size();i++)
	    {
	      
	      double runtime    = data[i].total_time;
	      string experiment = data[i].experiment;
	  
	      statistics[experiment](runtime);
	  
	      if(max_vals.find(experiment) == max_vals.end()) 
		{
		  max_vals[experiment].value = -HUGE_VAL;
		  min_vals[experiment].value =  HUGE_VAL;
		}
	      
	      if(runtime < min_vals[experiment].value) // min check
		{
		  min_vals[experiment].value = runtime;
		  min_vals[experiment].index = i;
		}
	      
	      if(runtime > max_vals[experiment].value) // max check
		{
		  max_vals[experiment].value = runtime;
		  max_vals[experiment].index = i;
		}

	      // cache max revision width

	      if(strlen(data[i].code_revision) > max_revision_width)
		max_revision_width = strlen(data[i].code_revision);

	    }

	  // ------------------------------
	  // Prep output dirs/ for results
	  // ------------------------------

	  map<string,FILE *> fp_experiments;

	  const char *cdelim = delimiter.c_str(); // desired comment delimiter

	  if(dump_files)	
	    {
	      for(map <string,perf_stats>::iterator ii=statistics.begin();ii != statistics.end(); ++ii)
		{
		  string exp_name = (ii->first);
		  string outfile = outdir + "/" + machines[imach] + "/" + exp_name.c_str();

		  grvy_printf(GRVY_DEBUG,"%s: Verifying existence of path for file %s\n",__func__,outfile.c_str());
		  grvy_check_file_path(outfile.c_str());

		  fp_experiments[exp_name] = fopen(outfile.c_str(),"w");
		  
		  if(fp_experiments[exp_name] == NULL)
		    {
		      grvy_printf(GRVY_ERROR,"%s: Unable to open file for writing (%s)\n",__func__,outfile.c_str());
		      exit(1);
		    }
		}
	    }
	  
	  //------------------------
	  // Dump statistics summary
	  //------------------------

	  map<string,MAP_string_to_double> aggregate_subtimers;


	      
	  for(map <string,perf_stats>::iterator ii=statistics.begin();ii != statistics.end(); ++ii)
	    {
		  
	      string ename = (ii->first); // current experiment name
	      
	      if(dump_stdout)
		{
		  if(dump_files)
		    {
		      grvy_printf(GRVY_INFO,"--> File path: %s/%s/%s  ",outdir.c_str(),
				  machines[imach].c_str(),ename.c_str());
		    }
		  else 
		    {
		      grvy_printf(GRVY_INFO,"--> Host=%s, Experiment = %s  ",machines[imach].c_str(),ename.c_str());
		    }

		  grvy_printf(GRVY_INFO,"(%i total samples)\n",boost::accumulators::count(ii->second));
		  grvy_printf(GRVY_INFO,"\n");
		  grvy_printf(GRVY_INFO,"    --> Mean time = %.8e (secs), Variance = %.8e\n",
			mean(ii->second),variance(ii->second));
		  grvy_printf(GRVY_INFO,"    --> Min  time = %.8e on %s\n",
			min_vals[ename].value,data[min_vals[ename].index].timestamp);
		  grvy_printf(GRVY_INFO,"    --> Max  time = %.8e on %s\n\n",
			max_vals[ename].value,data[max_vals[ename].index].timestamp);
		  
#if 0
		  grvy_printf(GRVY_INFO,"  --> Mean time = %.8e (secs)\n",mean(ii->second));
		  grvy_printf(GRVY_INFO,"  --> Variance  = %.8e\n",variance(ii->second));
		  gry_printf(GRVY_INFO,"  --> Min  time = %.8e on %s\n",
			     min_vals[ename].value,data[min_vals[ename].index].timestamp);
		  grvy_printf(GRVY_INFO,"  --> Max  time = %.8e on %s\n\n",
			      max_vals[ename].value,data[max_vals[ename].index].timestamp);
#endif
		}
		  
	      if(dump_files)
		{
		  FILE *fp_mach = fp_experiments[ename];

		  fprintf(fp_mach,"%s --\n",cdelim);
		  fprintf(fp_mach,"%s Historical Performance Timing Records\n",cdelim);
		  fprintf(fp_mach,"%s libGRVY: Version = %s",cdelim,GRVY_LIB_VERSION); 
		  fprintf(fp_mach," (%i)\n",GRVY_get_numeric_version());
		  fprintf(fp_mach,"%s\n",cdelim);

		  fprintf(fp_mach,"%s Host = %s\n",cdelim,machines[imach].c_str());
		  fprintf(fp_mach,"%s Sysname  = %s\n",cdelim, os_sysname.c_str());
		  fprintf(fp_mach,"%s Release  = %s\n",cdelim, os_release.c_str());
		  fprintf(fp_mach,"%s Version  = %s\n",cdelim, os_version.c_str());
		  fprintf(fp_mach,"%s CPU Type = %s\n",cdelim, cputype.c_str()   );
		  fprintf(fp_mach,"%s --\n",cdelim);

		  fprintf(fp_mach,"%s Experiment: %s (%i total samples)\n",cdelim,
			  ename.c_str(),boost::accumulators::count(ii->second));
		  fprintf(fp_mach,"%s\n",cdelim);
		  fprintf(fp_mach,"%s  --> Mean time = %.8e (secs)\n",cdelim,mean(ii->second));
		  fprintf(fp_mach,"%s  --> Variance  = %.8e\n",cdelim,variance(ii->second));
		  fprintf(fp_mach,"%s  --> Min  time = %.8e on %s\n",cdelim,
			      min_vals[ename].value,data[min_vals[ename].index].timestamp);
		  fprintf(fp_mach,"%s  --> Max  time = %.8e on %s\n",cdelim,
			      max_vals[ename].value,data[max_vals[ename].index].timestamp);
		  fprintf(fp_mach,"%s\n",cdelim);
		  fprintf(fp_mach,"%s --\n",cdelim);

		  // header for global output

		  if(max_revision_width < 8)
		    max_revision_width = 8;

		  fprintf(fp_mach,"%s    Index     Experiment-Date     Total Time(sec)    # Procs      JobId %*s      Flops",
			  cdelim,max_revision_width,"Revision");

		  // header for subtimer(s)

		  if(output_subtimers)
		    {

		      fprintf(fp_mach," |  ");

		      // first, we generate a list of all possible
		      // subtimer names (stored on a per experiment
		      // name basis); note that there is no guarantee
		      // that the same set of subtimers are defined
		      // for each experiment sample and consequently,
		      // we use this aggregate map as a means to
		      // provide standardized output

		      for(int i=0;i<data.size();i++)
			{
			  hvl_t subtimers = data[i].vl_subtimers;
			  SubTimer_PTable_V1 *subtimer2 = (SubTimer_PTable_V1*)subtimers.p;
			  
			  for(int j=0;j<subtimers.len;j++)
			    {
			      if(aggregate_subtimers[ename].find(subtimer2[j].timer_name) == 
				 aggregate_subtimers[ename].end() )
				{
				  aggregate_subtimers[ename][subtimer2[j].timer_name] = 0.0;
				}
			    }

			}

		      grvy_printf(GRVY_DEBUG,"%s total number of defined subtimers = %i\n",__func__,
				  aggregate_subtimers[ename].size());

		      // now, we can list all the possible subtimer(s) in the header

		      for(MAP_string_to_double::iterator it_sub=aggregate_subtimers[ename].begin(); 
			  it_sub != aggregate_subtimers[ename].end(); ++it_sub)
			{
			  int width = std::max(14,(int)strlen( (it_sub->first).c_str()));
			  int padL  = (width-strlen( (it_sub->first).c_str()))/2;
			  int padR  = (width-strlen( (it_sub->first).c_str()))-padL;
			  
			  fprintf(fp_mach,"%*s%s%*s ",padL,"",(it_sub->first).c_str(),padR,"");
			}
		    }

		  fprintf(fp_mach,"\n");
		  fprintf(fp_mach,"%s --\n",cdelim);
		} 

	    } // end loop over statistics for defined experiments - completes the header for this host


	  //----------------------
	  // Dump raw sample data
	  //----------------------

	  if(output_totaltimers && dump_files)
	    {
	      for(int i=0;i<data.size();i++)
		{

		  string ename  = data[i].experiment;     // experiment name for current data sample
		  FILE *fp_mach = fp_experiments[ename];  // corresponding open file pointer for the host

		  fprintf(fp_mach,"%10i %s  %.8e %10i %10i %*s %.4e",i+1,
			  data[i].timestamp,data[i].total_time,
			  data[i].num_procs,data[i].job_Id,max_revision_width,data[i].code_revision,data[i].flops);
		  
		  if(output_subtimers)
		    {

		      fprintf(fp_mach,"    ");

		      hvl_t subtimers = data[i].vl_subtimers;
		      SubTimer_PTable_V1 *subtimer2 = (SubTimer_PTable_V1*)subtimers.p;

		      set_map_constant_value(aggregate_subtimers[ename],0.0);

		      // cache subtimers for this measurement in aggregate order 

		      for(int j=0;j<subtimers.len;j++)
			aggregate_subtimers[ename][subtimer2[j].timer_name] = subtimer2[j].measurement;

		      for(MAP_string_to_double::iterator it_sub=aggregate_subtimers[ename].begin();
			  it_sub != aggregate_subtimers[ename].end(); ++it_sub)
			{
			  int width = std::max(14,(int)strlen( (it_sub->first).c_str()));
			  fprintf(fp_mach,"%*-.8e ",width,(it_sub)->second);
			}

		    }

		  fprintf(fp_mach,"\n");

		} // end loop over data samples for the host

	      
	      // Close open files for this host

	      for(map<string,FILE *> ::iterator ii=fp_experiments.begin();ii != fp_experiments.end(); ++ii)
		{
		  grvy_printf(GRVY_DEBUG,"%s Closing open file for host:%s -> experiment:%s\n",__func__,
			      machines[imach].c_str(),(ii->first).c_str());
		  fclose((ii->second));
		}
	      
	    }

	  //grvy_printf(GRVY_INFO,"\n[End]   Performance Statistics for: %s\n",machines[imach].c_str());
	  
	  break;
	}
      default:

	// TODO: leaving this for now, but will likely want to alter logic if there a change in 
	// the packet table version is required

	grvy_printf(GRVY_FATAL,"%s: Unknown timer packet version read (%i)\n",__func__,Ptable_Version);
	exit(1);
      }

    } // end loop over machines

  h5.Close();

  return;
#endif
}

int GRVY_Timer_Class::SetOption(string option, bool flag)
{
  map<string,bool> :: iterator index;

  index = m_pimpl->options.find(option);

  if(index == m_pimpl->options.end() )
    {
      grvy_printf(GRVY_WARN,"%s: unknown option provided (%s) -> ignoring set request\n",__func__,option.c_str());
      return 0;
    }
  else
    {
      index->second = flag;
      grvy_printf(GRVY_DEBUG,"%s: option %s -> %i\n",__func__,option.c_str(),flag);
    }

  return 0;
}

// TODO: if/when we need a new ptable version, this can be templated
// based on the ptable version

#ifdef HAVE_HDF5

int GRVY_Timer_Class::GRVY_Timer_ClassImp::ReadAllHostData(GRVY_HDF5_Class *h5, 
							   hid_t tableId, vector <TimerPTable_V1> *data)
{

  grvy_printf(GRVY_DEBUG,"%s: Reading performance data (format = V1)\n",__func__);

  hsize_t nrecords = h5->m_pimpl->PTableNumPackets(tableId);

  grvy_printf(GRVY_DEBUG,"%s: %i packets are available for reading \n",__func__,nrecords);

  data->reserve(nrecords);

  // Loop over all packet entries and stash results in (data)

  Timer_PTable_V1 packet_data;

  for(int i=0;i<nrecords;i++)
    {
      if (H5PTget_next(tableId,1,&packet_data) < 0)
	{
	  grvy_printf(GRVY_FATAL,"%s: Unable to read %i packet (%i)\n",__func__,i);
	  exit(1);
	}
      else
	{
	  data->push_back(packet_data);
	}
    }

  grvy_printf(GRVY_DEBUG,"%s: Finished reading performance data (format = V1)\n",__func__);

  return(0);
}


//--------------------------------------------------------------------
// CreateHistType(): create a compound HDF datatype as the basis
// for storing historical performance data in a packet table.
//--------------------------------------------------------------------


hid_t GRVY_Timer_Class::GRVY_Timer_ClassImp::CreateHistType(int version)
{

  grvy_printf(GRVY_DEBUG,"Creating HDF TimerHist packet datatype version %i\n",version);

  switch(version) {
  
  case 1:
      hid_t ptable_type;
      hid_t strtype;
      hid_t varlen_strtype;
      
      // Create our basic subtimer packet...

      strtype = H5Tcopy(H5T_C_S1);
      H5Tset_strpad(strtype,H5T_STR_NULLTERM);
      H5Tset_size(strtype,H5T_VARIABLE);

      if( (ptable_type = H5Tcreate (H5T_COMPOUND, sizeof(SubTimer_PTable_V1))) < 0 )
	{
	  grvy_printf(GRVY_FATAL,"%s: Unable to create compound HDF datatype\n",__func__);
	  exit(1);
	}

      // We provide specific datatypes during the table creation to
      // ensure that all tables are consistent across platforms.  

      H5Tinsert(ptable_type, "timer name",  HOFFSET(SubTimer_PTable_V1, timer_name),  strtype );
      H5Tinsert(ptable_type, "measurement", HOFFSET(SubTimer_PTable_V1, measurement), H5T_IEEE_F64LE );
      H5Tinsert(ptable_type, "mean",        HOFFSET(SubTimer_PTable_V1, mean),        H5T_IEEE_F64LE );
      H5Tinsert(ptable_type, "variance",    HOFFSET(SubTimer_PTable_V1, variance),    H5T_IEEE_F64LE );
      H5Tinsert(ptable_type, "count",       HOFFSET(SubTimer_PTable_V1, count),       H5T_STD_U64LE  );
      
      hid_t subtimer_type;
      
      subtimer_type = H5Tvlen_create(ptable_type);
      
      // Now, create a combined complex datatype with the header and VL subtimer(s)
      
      hid_t timers_type;
      
      if ( (timers_type = H5Tcreate(H5T_COMPOUND,sizeof(TimerPTable_V1))) < 0)
      {
	grvy_printf(GRVY_FATAL,"%s: Unable to create compound HDF datatype\n",__func__);
	exit(1);
      }

      H5Tinsert(timers_type,"Experiment Name",HOFFSET(TimerPTable_V1,experiment),    strtype        );
      H5Tinsert(timers_type,"Timestamp      ",HOFFSET(TimerPTable_V1,timestamp) ,    strtype        );
      H5Tinsert(timers_type,"Comment",        HOFFSET(TimerPTable_V1,user_comment),  strtype        );
      H5Tinsert(timers_type,"Total Time",     HOFFSET(TimerPTable_V1,total_time),    H5T_IEEE_F64LE );
      H5Tinsert(timers_type,"Num Processors", HOFFSET(TimerPTable_V1,num_procs),     H5T_STD_I32LE  );
      H5Tinsert(timers_type,"Job Id",         HOFFSET(TimerPTable_V1,job_Id),        H5T_STD_I32LE  );
      H5Tinsert(timers_type,"Flops",          HOFFSET(TimerPTable_V1,flops),         H5T_IEEE_F64LE );
      H5Tinsert(timers_type,"Code Revision"  ,HOFFSET(TimerPTable_V1,code_revision), strtype        );
      H5Tinsert(timers_type,"SubTimers",      HOFFSET(TimerPTable_V1,vl_subtimers),  subtimer_type  );

      H5Tclose(strtype);
      H5Tclose(ptable_type);
    
      return(timers_type);
      break;

  default:
    grvy_printf(GRVY_FATAL,"%s: Unknown timer packet version requested (%i)\n",__func__,version);
    exit(1);
  }
}

//--------------------------------------------------------------------
// WriteHeaderInfo(): write common header info for ascii output
//--------------------------------------------------------------------

void GRVY_Timer_Class::GRVY_Timer_ClassImp::WriteHeaderInfo(FILE *fp, const char *delim)
{

  fprintf(fp,"%s Historical Performance Timing History\n",delim);
  fprintf(fp,"%s libGRVY: Version = %s",delim,GRVY_LIB_VERSION); 
  fprintf(fp," (%i)\n",GRVY_get_numeric_version());
  fprintf(fp,"%s\n",delim);
  fprintf(fp,"%s -----------------------------------------------------------------------------\n",delim);

#if 0
  fprintf(fp,"%s Experiment: %s (%i total samples)\n",delim,
	  (ii->first).c_str(),boost::accumulators::count(ii->second));
  fprintf(fp,"%s\n",delim);
  fprintf(fp,"%s  --> Mean time = %.8e (secs)\n",delim,mean(ii->second));
  fprintf(fp,"%s  --> Variance  = %.8e\n",delim,variance(ii->second));
  fprintf(fp,"%s  --> Min  time = %.8e on %s\n",delim,
	  min_vals[ii->first].value,data[min_vals[ii->first].index].timestamp);
  fprintf(fp,"%s  --> Max  time = %.8e on %s\n",delim,
	  max_vals[ii->first].value,data[max_vals[ii->first].index].timestamp);
  fprintf(fp,"%s\n",delim);
  fprintf(fp,"%s ----------------------------------------------------------------------------------\n",delim);
  fprintf(fp,"%s  Experiment-Date        Runtime(secs)    # Procs      JobId     SW Revision  Flops\n",delim);
  fprintf(fp,"%s ----------------------------------------------------------------------------------\n",delim);
#endif

  return;

}

//--------------------------------------------------------------------
// AppendHistData(): append performance timings to HDF packet table
//--------------------------------------------------------------------

int GRVY_Timer_Class::GRVY_Timer_ClassImp::AppendHistData(double timing, string experiment, string comment, 
							  int num_procs,int jobId, string code_revision, double flops,
							  int version, hid_t tableId, bool save_internal_timers)
{
  grvy_printf(GRVY_DEBUG,"Appending historical timer data for PTable version %i\n",version);
  grvy_printf(GRVY_DEBUG,"--> save_internal_timer = %i\n",save_internal_timers);

  int num_subtimers;		// total number of defined subtimers

  if(save_internal_timers)
    num_subtimers = TimerMap.size() - 1;  // -1 since global timer goes in header
  else
    num_subtimers = 0;		         // no subtimers if user is providing a global timing

  // timestamp the data

  time_t t=time(NULL);
  struct tm *timeinfo = localtime(&t);
  
  char mytimestamp[25];

  sprintf(mytimestamp,"%04i-%02i-%02i %02i:%02i:%02i %3s",timeinfo->tm_year+1900,timeinfo->tm_mon+1,
	  timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec,timeinfo->tm_zone);

  switch(version) {
  
  case 1:
    {
      Timer_PTable_V1            header;
      vector<SubTimer_PTable_V1> subtimers;

      subtimers.reserve(num_subtimers);
      
      header.experiment       = experiment.c_str();
      header.timestamp        = mytimestamp;
      header.user_comment     = comment.c_str();

      if(save_internal_timers)
	header.total_time      = self->ElapsedSeconds(_GRVY_gtimer);
      else
	header.total_time     = timing;

      header.num_procs        = num_procs;
      header.job_Id           = jobId;	      
      header.flops            = flops;
      header.code_revision    = code_revision.c_str();
      header.vl_subtimers.p   = &subtimers[0];
      header.vl_subtimers.len = num_subtimers;

    _GRVY_Type_TimerMap2 :: iterator index;

    SubTimer_PTable_V1 data_tmp;

    for(index=TimerMap.begin(); index != TimerMap.end(); ++index)
      {
	if(index->first != _GRVY_gtimer)
	  {

	    string timer_name = index->first;

	    data_tmp.timer_name  = timer_name.c_str();

	    data_tmp.measurement = self->ElapsedSeconds(index->first);
	    data_tmp.mean        = self->StatsMean     (index->first);
	    data_tmp.variance    = self->StatsVariance (index->first);

	    data_tmp.count       = self->StatsCount    (index->first);
	    
	    subtimers.push_back(data_tmp);
	  }
      }

    assert(subtimers.size() == num_subtimers);

    // That was fun, now we can append the packet data

    assert(H5PTappend( tableId, 1, &(header) ) >= 0);
    }
    break;

  default:
    grvy_printf(GRVY_FATAL,"%s: Unknown timer packet version requested (%i)\n",__func__,version);
    exit(1);
  }

  return 0;
}

#endif

}
