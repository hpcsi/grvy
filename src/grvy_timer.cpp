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
// grvy_timer: Timing routines for performance monitoring
//
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

typedef accumulator_set <double,features<tag::mean,tag::count,tag::variance,tag::min,tag::max> > perf_stats;

typedef struct GRVY_Timer_Data {
  double timings[2];		// data for exclusive timing measurements
  double timings_inc[2];	// data for inclusive timing measurements
  double accumulated;		// exclusive accumulated time for embedded timer
  perf_stats stats;		// for exclusive data statistics
  perf_stats stats_inc;		// for inclusive data statistics
} tTimer_Data;

typedef struct grvy_minmax {
  double value;
  size_t index;
} grvy_minmax;

typedef std::map <std::string, GRVY_Timer_Data > _GRVY_Type_TimerMap2;

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
    void   BeginTimer      (std::string); 
    void   EndTimer        (std::string);

    void   PauseTimer      (std::string id); 
    void   ResumeTimer     (std::string id); 

    double RawTimer        ();

    void   AddEllipsis     (std::string &);
    double ElapsedSeconds  (std::string id, bool exclusive);

    double Stats_Exclusive (std::string id, int STAT);
    double Stats_Inclusive (std::string id, int STAT);

#ifdef HAVE_HDF5
    hid_t  CreateHistType   (int version); 
    int    AppendHistData   (double timing, string experiment, string comment, int num_procs, int jobId,
			     string code_revision, double flops, int version, hid_t tableId, hid_t timers_type,
			     bool save_internal_timer, int num_measurements);
    int    ReadAllHostData  (GRVY_HDF5_Class *h5, string machine, hid_t tableId, hid_t timers_type, 
			     vector <TimerPTable_V1> *data);
    int    RepackAllHostData(GRVY_HDF5_Class *h5,  int version, hid_t tableId, hid_t timers_type, string machine);
    int    ReadPTable       (string host);
    void   SummarizeHost    (string host);
    void   WriteHeaderInfo  (FILE *fp, const char *delim);
#endif
    int    SaveHistTiming   (double timing, string machinename, string experiment, string comment, int num_procs, 
			     int jobId, string code_revision, double flops, string filename, bool save_internal_timer );

    bool        initialized;            // user called initialize?
    bool        finalized;		// finalized?
    double      timer_finalize;         // raw timer value at time of finalize()

    std::string timer_name;             // user name supplied for the timer
    int         num_begins;	        // number of active begin timers (used for callgraph determination)
    std::stack <std::string> callgraph; // callgraph to support embedded timers
    _GRVY_Type_TimerMap2     TimerMap;  // map used to store performance timers for each defined key

    std::map<std::string,bool> options; // runtime options

    int    default_jobId;
    double default_flops;
    string default_revision;
    size_t max_stdout_width;	       // max output width for timer information
    perf_stats stats_empty;            // empty accumulator

    GRVY_Timer_Class *self;	       // back pointer to public class

  private:
    bool new_performance_table;

  };

} // matches namespace GRVY

namespace GRVY {

  int show_statistics = 1;

  GRVY_Timer_Class::GRVY_Timer_Class() :m_pimpl(new GRVY_Timer_ClassImp() )
  {
    m_pimpl->initialized           = false;
    m_pimpl->finalized             = false;
    m_pimpl->timer_finalize        = -1;
    m_pimpl->num_begins            = 0;	
    m_pimpl->self                  = this;

    // default historical timing values

    m_pimpl->default_jobId         = -1;
    m_pimpl->default_flops         = 0.0;   
    m_pimpl->default_revision      = "unknown";
    m_pimpl->max_stdout_width      = 120;

    // set default options for gdump

    m_pimpl->options["output_stdout"        ] = true;
    m_pimpl->options["output_totaltimer_raw"] = true;
    m_pimpl->options["output_subtimer_raw"  ] = false;
    m_pimpl->options["dump_files"           ] = true;
    m_pimpl->options["output_comments"      ] = false;
    m_pimpl->options["output_printenv"      ] = false;

    // start global timer...

    BeginTimer(_GRVY_gtimer);
    m_pimpl->timer_name = "Performance Timings:";
  }

  GRVY_Timer_Class::~GRVY_Timer_Class()
  {
    // using auto_ptr for proper cleanup
  }

  void GRVY_Timer_Class::Init(string name)
  {

    if(!m_pimpl->initialized)
      m_pimpl->initialized = true;

    m_pimpl->timer_name  = name;

    // reset any other existing timers (only an issue if Init() is called multiple times

    Reset();
    return;
  }

  void GRVY_Timer_Class::Finalize()
  {
    EndTimer(_GRVY_gtimer);
    m_pimpl->finalized = true;
    return;
  }

  void GRVY_Timer_Class::GRVY_Timer_ClassImp:: VerifyInit ()
  {
    if( !initialized )
      {
	//self->Init("GRVY Default");
	grvy_printf(GRVY_DEBUG,"%s: initialzing timer calls for user\n",__func__);
	//no_user_init = true;
	//exit(1);
      }
  }

  void GRVY_Timer_Class::BeginTimer (std::string id)
  {
    return(m_pimpl->BeginTimer(id));
  }

  void GRVY_Timer_Class::EndTimer (std::string id)
  {
    return(m_pimpl->EndTimer(id));
  }

  void GRVY_Timer_Class::GRVY_Timer_ClassImp::BeginTimer (std::string id)
  {
    double mytime;
    _GRVY_Type_TimerMap2 :: iterator index;

    tTimer_Data Data;

    // ------------------
    // Get current time
    // ------------------

    mytime = RawTimer();

    // ------------------------------------------------------------------
    // Embedded Timer Support:
    // 
    // Maintain callgraph for embedded timers; note that we detect an
    // embedded timer when num_begins exceeds a value of 2 as we
    // always have one global catch-all timer created during the
    // init() process.  When we detect an embedded timer, we use the
    // callgraph information to automatically PauseTimer() the
    // surrounding timer. The callgraph information will also be used
    // to restart the outer timer once the nested timer calls Endtimer().
    // ------------------------------------------------------------------

    num_begins++; 
    
    if(num_begins > 2)
      PauseTimer(callgraph.top());
    
    if(num_begins > callgraph.size())
      callgraph.push(id);

    // Is this the first call for this id?

    index = TimerMap.find(id);

    if ( index == TimerMap.end() )
      {
	Data.timings[0]     = 0.0;	            // storage for accumulated time (exclusive)
	Data.timings[1]     = mytime;               // storage for latest timestamp
	
	Data.timings_inc[0] = 0.0;                  // storage for accumulated time (inclusive)
	Data.timings_inc[1] = mytime;               // timestamp at timer begin

	Data.accumulated    = 0.0;		    // storage for accumulated time (used
                                                    // when a timer is paused due to presence of nested timer)
	TimerMap[id] = Data;
      }
    else
      {
	(index->second).timings[1]      = mytime;    // stores latest timestamp (used for exclusive timings)
	(index->second).timings_inc[1]  = mytime;    // stores latest timestamp (used for inclusive timings)
	(index->second).accumulated     = 0.0;       // nullify any accumulations
      }
  
  } 

  // ----------------------------------------------------------------
  // PauseTimer(): 
  //
  // Used to pause outer timer on call graph when the beginnig of a
  // nested timer is detected.  Accumlates incremental time since the
  // last Begin()/Resume() call for the timer.
  // ----------------------------------------------------------------
  // ----------------------------------------------------------------

  void GRVY_Timer_Class::GRVY_Timer_ClassImp::PauseTimer (std::string id)
  {
    double mytime = RawTimer();
    _GRVY_Type_TimerMap2::iterator index = TimerMap.find(id);

    double increment = mytime - (index->second).timings[1];

    (index->second).accumulated += increment;

    return;
  }

  // ----------------------------------------------------------------
  // ResumeTimer(): 
  //
  // Used to resume outer timer on call graph when the end of a nested
  // timer is detected.  
  // ----------------------------------------------------------------

  void GRVY_Timer_Class::GRVY_Timer_ClassImp::ResumeTimer (std::string id)
  {
    _GRVY_Type_TimerMap2::iterator index = TimerMap.find(id);

    (index->second).timings[1] = RawTimer(); // stores latest timestamp

    //grvy_printf(GRVY_DEBUG,"Resuming timer %s\n",index->first.c_str());

    return;
  }

  void GRVY_Timer_Class::GRVY_Timer_ClassImp::EndTimer (string id)
  {
    double      mytime, increment;
    double      inclusive_increment;
    tTimer_Data Data;

    _GRVY_Type_TimerMap2 :: iterator index;

    mytime = RawTimer();
    index  = TimerMap.find(id);

    if ( index == TimerMap.end() )
      _GRVY_message(GRVY_ERROR,__func__,"No timer data available for",id.c_str());
    else if(index->first[1] < 0)
      _GRVY_message(GRVY_ERROR,__func__,"No matching begin timer call for",id.c_str());
    else
      {
	// update timer with total accumulated time between begin/end
	// demarcation.  If the timer was paused/resumed due to the
	// presence of an embedded timer, it will have a non-zero
	// accumulation contribution.

	increment = (mytime - (index->second).timings[1]) + (index->second).accumulated;

	(index->second).timings[0] += increment; // exclusiving timing
	(index->second).timings[1]  = -1.;
	(index->second).stats(increment);

	// update inclusive timing measurements

	inclusive_increment = mytime - (index->second).timings_inc[1];

	(index->second).timings_inc[0] += inclusive_increment; 
	(index->second).timings_inc[1]  = -1.;
	(index->second).stats_inc(inclusive_increment);

	// ----------------------------------------------------------------
	// Embedded Timer Support:
	// 
	// Pop curent timer off the callgraph stack and resume
	// surrounding timer if necessary.
	// ----------------------------------------------------------------

	//Ginfo("end: popping id %s (size = %i)\n",callgraph.top().c_str(),callgraph.size());
	//Ginfo("end: num_begins = %i\n",num_begins);
	
	if(callgraph.size() >= 1)
	  callgraph.pop();
	else
	  grvy_printf(GRVY_ERROR,"GRVY (%s): no callgraph left for %s\n",__func__,id.c_str());

	if(num_begins > 2)
	  ResumeTimer(callgraph.top());
	
	num_begins--;

      }

    // Store the latest raw timer snapshot for the global timer region
    // to allow users to query the global elapsed time after 
    // grvy_timer_finalize()

    if ( id == _GRVY_gtimer )
      timer_finalize = RawTimer();

    return;
  }

  void GRVY_Timer_Class::Reset()
  {
    _GRVY_Type_TimerMap2 :: iterator index;

    m_pimpl->num_begins            = 0;	

    // Reset all currently defined timers

    for(index=m_pimpl->TimerMap.begin(); index != m_pimpl->TimerMap.end(); ++index)
      {
	(index->second).timings[0]     = 0.0;                  // reset raw timing info
	(index->second).timings_inc[0] = 0.0;                  // reset raw timing info
	(index->second).stats          = m_pimpl->stats_empty; // reset accumulator (no reset in Boost currently)
	(index->second).stats_inc      = m_pimpl->stats_empty; // reset inclusive timer accumulator 
      }

    // Restart global timer

    BeginTimer(_GRVY_gtimer);

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

  // ----------------------------------------
  // Timer count statistics
  // ----------------------------------------

  size_t GRVY_Timer_Class::StatsCount(string id)
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

  // ----------------------------------------
  // Wrappers for Exclusive Timer Properties
  // ----------------------------------------

  double GRVY_Timer_Class:: ElapsedSeconds   (string id)  { return(m_pimpl->ElapsedSeconds(id,true)); }

  double GRVY_Timer_Class:: StatsMean        (string id)  { return(m_pimpl->Stats_Exclusive(id,TIMER_MEAN    )); }
  double GRVY_Timer_Class:: StatsVariance    (string id)  { return(m_pimpl->Stats_Exclusive(id,TIMER_VARIANCE)); }
  double GRVY_Timer_Class:: StatsMin         (string id)  { return(m_pimpl->Stats_Exclusive(id,TIMER_MIN     )); }
  double GRVY_Timer_Class:: StatsMax         (string id)  { return(m_pimpl->Stats_Exclusive(id,TIMER_MAX     )); }

  // ----------------------------------------
  // Wrappers for Inclusive Timer Properties
  // ----------------------------------------

  double GRVY_Timer_Class:: ElapsedSeconds_inc(string id)  { return(m_pimpl->ElapsedSeconds(id,false)); }

  double GRVY_Timer_Class:: StatsMean_inc     (string id)  { return(m_pimpl->Stats_Inclusive(id,TIMER_MEAN    )); }
  double GRVY_Timer_Class:: StatsVariance_inc (string id)  { return(m_pimpl->Stats_Inclusive(id,TIMER_VARIANCE)); }
  double GRVY_Timer_Class:: StatsMin_inc      (string id)  { return(m_pimpl->Stats_Inclusive(id,TIMER_MIN     )); }
  double GRVY_Timer_Class:: StatsMax_inc      (string id)  { return(m_pimpl->Stats_Inclusive(id,TIMER_MAX     )); }

  // ------------------------------------------------------
  // Support Function to Recall Exclusive Timer Statistics
  // ------------------------------------------------------

  double GRVY_Timer_Class::GRVY_Timer_ClassImp::Stats_Exclusive(string id, int STAT)
  {
    _GRVY_Type_TimerMap2 :: const_iterator index = TimerMap.find(id);

    if ( index == TimerMap.end() )
      {
	_GRVY_message(GRVY_ERROR,__func__,"No stats data available for",id.c_str());
	return -1.0;
      }
    else
      {
	switch(STAT)
	  {
	  case TIMER_MEAN:
	    return(boost::accumulators::mean((index->second).stats));
	    break;
	  case TIMER_VARIANCE:
	    return(boost::accumulators::variance((index->second).stats));
	    break;
	  case TIMER_MIN:
	    return(boost::accumulators::min((index->second).stats));
	    break;
	  case TIMER_MAX:
	    return(boost::accumulators::max((index->second).stats));
	    break;
	  default:
	    _GRVY_message(GRVY_ERROR,__func__,"Unsupported stat requested for",id.c_str());
	    return(-1.0);
	  }

      }
  }

  // ------------------------------------------------------
  // Support Function to Recall Inclusive Timer Statistics
  // ------------------------------------------------------

  double GRVY_Timer_Class::GRVY_Timer_ClassImp::Stats_Inclusive(string id, int STAT)
  {
    _GRVY_Type_TimerMap2 :: const_iterator index = TimerMap.find(id);

    if ( index == TimerMap.end() )
      {
	_GRVY_message(GRVY_ERROR,__func__,"No stats data available for",id.c_str());
	return -1.0;
      }
    else
      {
	switch(STAT)
	  {
	  case TIMER_MEAN:
	    return(boost::accumulators::mean((index->second).stats_inc));
	    break;
	  case TIMER_VARIANCE:
	    return(boost::accumulators::variance((index->second).stats_inc));
	    break;
	  case TIMER_MIN:
	    return(boost::accumulators::min((index->second).stats_inc));
	    break;
	  case TIMER_MAX:
	    return(boost::accumulators::max((index->second).stats_inc));
	    break;
	  default:
	    _GRVY_message(GRVY_ERROR,__func__,"Unsupported stat requested for",id.c_str());
	    return(-1.0);
	  }

      }
  }

  // ------------------------------------------------------
  // Support Function to Recall elapsed time for a timer
  // ------------------------------------------------------

  double GRVY_Timer_Class::GRVY_Timer_ClassImp::ElapsedSeconds(string id,bool exclusive)
  {
    double elapsedseconds = 0.0;

    VerifyInit();

    _GRVY_Type_TimerMap2 :: const_iterator index = TimerMap.find(id);

    if ( index == TimerMap.end() )
      _GRVY_message(GRVY_ERROR,__func__,"No timer data available for",id.c_str());
    else if( (index->second).timings[1] > 0. )
      _GRVY_message(GRVY_ERROR,__func__,"Timer still active for",id.c_str());
    else
      {
      if(exclusive)
	elapsedseconds = (index->second).timings[0];
      else
	elapsedseconds = (index->second).timings_inc[0];
      }

    return elapsedseconds;
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
    else if( (index->second).timings[1] > 0.)     // inside active timer
      elapsedseconds = mytime - (index->second).timings[1];
    else                                             // outside active timer
      elapsedseconds = (index->second).timings[0];

    // if we are being called after a finalize(), include the elapsed
    // time since the last global timer was updated

    if (m_pimpl->timer_finalize > 0)
      elapsedseconds += mytime - m_pimpl->timer_finalize;

    return elapsedseconds;
  }

  int GRVY_Timer_Class::SetSummarizeWidth(const int width)
  {
    const int min_width = 120;

    if(width <= min_width )
      {
	grvy_printf(GRVY_ERROR,"\n** Error (%s): stdout minimum width is %i characters \n",__func__,min_width);
	return(1);
      }

    m_pimpl->max_stdout_width = width;

    return(0);
  }

  void GRVY_Timer_Class::Summarize()
  {
    std::vector <double> timings(2);
    double totaltime,subtime;
    double local_percentage, total_percentage;
    int    global_time_defined = 0;
    int    max_timer_name_width;
    size_t display_id_width = 20;
    size_t timer_name_width;

    _GRVY_Type_TimerMapSortLH _GRVY_TimerMapSortLH;
    _GRVY_Type_TimerMapSortHL _GRVY_TimerMapSortHL;

    _GRVY_Type_TimerMapSortLH :: iterator indexLH;
    _GRVY_Type_TimerMapSortHL :: iterator indexHL;

    _GRVY_Type_TimerMap2 :: iterator index,gindex;

    m_pimpl->VerifyInit();

    if(!m_pimpl->finalized)
      Finalize();

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
	    display_id_width = std::min(display_id_width, m_pimpl->max_stdout_width - 35 - 40);
	  }
      }

    // check to see if user Init() string is longer than any of the
    // timer IDs and adjust widths accordingly

    if( (m_pimpl->timer_name.length() + 23) > (display_id_width + 35) )
      {
	int local_width = std::min(m_pimpl->max_stdout_width-40,
				   m_pimpl->timer_name.length()+23);

	display_id_width = local_width - 35;
      }

    // display header

    total_percentage = 0.0;

    grvy_printf(GRVY_INFO,"\n");
    for(int i=0;i<display_id_width+35;i++)
      grvy_printf(GRVY_INFO,"-");

    if(show_statistics)
      for(int i=0;i<40;i++)
	grvy_printf(GRVY_INFO,"-");

    grvy_printf(GRVY_INFO,"\n");

    max_timer_name_width = display_id_width + 13;

    std::string varstring = m_pimpl->timer_name.substr(0,max_timer_name_width-1);

    // add ellipsis if string is shortened to help alert the user that we 
    // shortened on purpose.

    if(m_pimpl->timer_name.length() > max_timer_name_width)
      m_pimpl->AddEllipsis(varstring);
    
    grvy_printf(GRVY_INFO,"%s - %-*s ",varstring.c_str(),(int)(display_id_width+32-varstring.length()),
		"Performance Timings:");

    if(show_statistics)
      grvy_printf(GRVY_INFO,"|      Mean      Variance       Count");

    grvy_printf(GRVY_INFO,"\n");

    // Print results for all user-defined timer keys

    for(indexHL=_GRVY_TimerMapSortHL.begin(); indexHL != _GRVY_TimerMapSortHL.end(); ++indexHL)
      {

	std::string varstring = indexHL->second.substr(0,display_id_width-1);

	// add ellipsis if string is shortened to help alert the user that we 
	// shortened on purpose.

	if(indexHL->second.length() > max_timer_name_width)
	  m_pimpl->AddEllipsis(varstring);

	grvy_printf(GRVY_INFO,"--> %-*s: %10.5e secs",(int)display_id_width,varstring.c_str(),indexHL->first[0]);

	if(global_time_defined)
	  {
	    local_percentage  = 100.*indexHL->first[0]/(totaltime);
	    total_percentage += local_percentage;
	    grvy_printf(GRVY_INFO," (%8.4f %%)",local_percentage);
	  }
      
	if(show_statistics)
	  {
	    if(indexHL->second !=  _GRVY_gtimer )
	      {
		gindex = m_pimpl->TimerMap.find(indexHL->second);
		if(boost::accumulators::count((gindex->second).stats) > 0)
		  {
		    grvy_printf(GRVY_INFO," | [%10.5e  %10.5e  %9zi]",
			   boost::accumulators::mean    ((gindex->second).stats),
			   boost::accumulators::variance((gindex->second).stats),
			   boost::accumulators::count   ((gindex->second).stats));
		  }
		else
		  {
		    grvy_printf(GRVY_INFO," | [%10s  %10.5e  %9zi]","   N/A   ",
			   boost::accumulators::variance((gindex->second).stats),
			   boost::accumulators::count   ((gindex->second).stats));
		  }
	      }
	  }
      
	grvy_printf(GRVY_INFO,"\n");
      
      }

    // Print results for left-over contribution in main timer

    gindex = m_pimpl->TimerMap.find(_GRVY_gtimer);

    grvy_printf(GRVY_INFO,"--> %-*s: %10.5e secs",(int)display_id_width,_GRVY_gtimer,
	   (gindex->second).timings[0]);

    if(global_time_defined)
      {
	local_percentage  = 100.*(gindex->second).timings[0]/(totaltime);
	total_percentage += local_percentage;
	grvy_printf(GRVY_INFO," (%8.4f %%)",local_percentage);
      }

    grvy_printf(GRVY_INFO,"\n");

    // A little sanity checking on the results
      
    if(global_time_defined)
      {
	grvy_printf(GRVY_INFO,"\n %*s = %10.5e secs (%8.4f %%)\n",(int)display_id_width+2,"Total Measured Time",
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
      grvy_printf(GRVY_INFO,"-");

    if(show_statistics)

      for(int i=0;i<40;i++)
	grvy_printf(GRVY_INFO,"-");

    grvy_printf(GRVY_INFO,"\n\n");

    // Warn against potential timer inaccuracy
    bool small_time_found = false;

    for(gindex=m_pimpl->TimerMap.begin(); gindex != m_pimpl->TimerMap.end(); ++gindex)
      {
	if( (boost::accumulators::count((gindex->second).stats) > 0) &&
	    (boost::accumulators::min  ((gindex->second).stats) < _GRVY_TIMER_THRESH ) )
	  {
	    grvy_printf(GRVY_WARN,"[Warning]: Timer accuracy may be insufficient (%.30s) -> minimum measured =  %le secs\n",
			gindex->first.c_str(),boost::accumulators::min((gindex->second).stats));
	    small_time_found = true;
	  }
      }
    
    if(small_time_found)
      grvy_printf(GRVY_INFO,"\n");

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

  // SaveHistTiming wrapper -> save_internal_timer (expanded)

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

  int GRVY_Timer_Class::SaveHistTiming(double timing, string machine, string experiment, 
				       string comment, int num_procs, string filename )
  {
    bool save_internal_timer = false;
    return(m_pimpl->SaveHistTiming(timing,machine,experiment,comment,num_procs,
				   m_pimpl->default_jobId,m_pimpl->default_revision,m_pimpl->default_flops,
				   filename,save_internal_timer));
  }

  // SaveHistTiming wrapper -> save external timing (expanded version)

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

    // Query runtime host environment
  
    GRVY_Hostenv_Class myenv;

#ifdef USE_HDF5_PTABLE

    if (h5.m_pimpl->PTableExists(hostlevel,tablename))
      {
	new_performance_table = false;
	tableId = h5.m_pimpl->PTableOpen(hostlevel,tablename);
      }
    else
      {

	int compression_level = -1;   // value between 0 and 9 (or -1 for no compression)
	hsize_t chunk_size    =  1;   
	new_performance_table = true;

	if( (tableId = H5PTcreate_fl(h5.m_pimpl->groupIds[hostlevel],tablename.c_str(),
				     timers_type,chunk_size,compression_level)) == H5I_BADID)
	  {
	    grvy_printf(GRVY_FATAL,"%s: Unable to create HDF packet table (%s)\n",__func__,tablename.c_str());
	    exit(1);
	  }

      }
#else
    if (h5.DatasetExists(hostlevel,tablename))
      {
	new_performance_table = false;

	h5.DatasetOpen(hostlevel,tablename);
	tableId = h5.m_pimpl->datasetIds[tablename];
      }
    else
      {
	new_performance_table = true;

	// create 1D hdf data type with unlimited dimension

	hsize_t dims_current[1] = {PERF_HIST_CHUNKSIZE};
	hsize_t dims_max[1]     = {H5S_UNLIMITED};
	hid_t space_id          = H5Screate_simple(1,dims_current,dims_max);

	// enable chunking for this performance logging dataset

	hid_t chunk_props = H5Pcreate(H5P_DATASET_CREATE);

	// enable compression? (experimentals indicate not useful for VL data)
	//H5Pset_deflate(chunk_props,9);

	H5Pset_chunk(chunk_props,1,dims_current);

	tableId = H5Dcreate(h5.m_pimpl->groupIds[hostlevel],tablename.c_str(),
			    timers_type, space_id ,H5P_DEFAULT,chunk_props,H5P_DEFAULT);

	H5Sclose(space_id);
	H5Pclose(chunk_props);
      }
#endif

    if(new_performance_table)
      {

	// Save environment attributes and assign local packet table
	// version in case we ever need to make a change in the future

	int format_version = PTABLE_VERSION;

	h5.AttributeWrite(hostlevel,"format_version",   format_version);
	h5.AttributeWrite(hostlevel,"os_sysname",       myenv.os_sysname);
	h5.AttributeWrite(hostlevel,"os_release",       myenv.os_release);
	h5.AttributeWrite(hostlevel,"os_version",       myenv.os_version);
	h5.AttributeWrite(hostlevel,"cputype",          myenv.cputype   );
	h5.AttributeWrite(hostlevel,"num_measurements", 0);
      
      }

    // --------------------------------------------------
    // Append performance data to HDF performance table 
    // --------------------------------------------------

    // we query the current number of measurements to decide where to
    // place new data.  After the append, we increment the measurement
    // counter.

    int num_measurements;

    h5.AttributeRead(hostlevel,"num_measurements", num_measurements );

    AppendHistData(timing,experiment,comment,num_procs,jobId,code_revision,flops,
		   PTABLE_VERSION,tableId,timers_type,save_internal_timer,num_measurements);

    h5.AttributeWrite(hostlevel,"num_measurements", ++num_measurements );

    // -----------------------------------------------------
    // Repack data if we have reached the NPACK threshold 
    // -----------------------------------------------------

    // The reason for this addition is that HDF file size can bloat
    // with frequent file writes when using VL datatypes (which is
    // exactly what we do, and our use case is to write one
    // performance timing result at a time).  Consequently, we attempt
    // to recreate the effect of an h5repack periodically.
    // 
    // *update*: this work great for first repack, but then loses it's 
    // effectiveness for subsquent repacks. Users should use h5repack
    // periodically if file get's too big and hopefully hdf5 will provide either 
    // a dataset delete capability or repack API at some point.....

#if 0
    if( (num_measurements % PERF_HIST_PACK_FREQ) == 0) 
      {
	//int ibegin = NPACK*((num_measurements/NPACK)-1);
	//int iend   = ibegin + NPACK - 1;

	grvy_printf(GRVY_INFO,"\n%s: Repacking performance data to minimize file size\n",__func__);

	RepackAllHostData(&h5,PTABLE_VERSION,tableId,timers_type,hostlevel);
      }
#endif

    // -----------------------------------------------------
    // Extend the data set if we are on a chunksize boundary
    // -----------------------------------------------------

    if( (num_measurements % PERF_HIST_CHUNKSIZE) == 0 )
      {
	hsize_t dims_current[1] = {num_measurements + PERF_HIST_CHUNKSIZE};
	hsize_t dims_max[1]     = {H5S_UNLIMITED};

	grvy_printf(GRVY_DEBUG,"\n%s: Adding additional chunksize space for performance data (%i)\n",
		    __func__,PERF_HIST_CHUNKSIZE);

	H5Dset_extent(tableId,dims_current);
      }
  
    // Clean up shop

    H5Tclose(timers_type);

#ifdef USE_HDF5_PTABLE
    h5.m_pimpl->PTableClose(tableId);
#else
    H5Dclose(tableId);
#endif

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
    bool output_comments    = m_pimpl->options["output_comments"];
    bool output_env         = m_pimpl->options["output_printenv"];

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
       	hid_t     tableId = h5.m_pimpl->PTableOpen(machine,"PTable");

	map<string,grvy_minmax> max_vals;
	map<string,grvy_minmax> min_vals;

	int max_revision_width = 0; 	// column width variables for pretty-printing
	int max_index_width    = 0;
	int max_comment_width  = 0;
	int max_env_width      = 0;

	// -------------------------------------------------
	// Read performance data in specified version format
	// -------------------------------------------------

	switch(Ptable_Version) {

	case 1:
	  {
	    vector<TimerPTable_V1> data;
	    m_pimpl->ReadAllHostData(&h5,machine,tableId,timers_type,&data);

#if USE_HDF5_PTABLE
	    h5.m_pimpl->PTableClose(tableId);
#else
	    assert(H5Dclose(tableId) >= 0);
#endif

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

		// cache max revision and comment column widths for pretty print

		max_revision_width = std::max((int)strlen(data[i].code_revision),max_revision_width);

		if(output_comments)
		  {
		    max_comment_width = std::max((int)strlen(data[i].user_comment),max_comment_width);
		    max_comment_width +=2; // include quotes around comment
		  }

#if 1
		if(output_env)
		  {
		    Env_PTable_V1 *env_variables = (Env_PTable_V1*)data[i].vl_env_variables.p;

		    for(int j=0;j<data[i].vl_env_variables.len;j++)
		      max_env_width = std::max((int)strlen(env_variables[j].env_string),max_env_width);
		  }
#endif

	      }

	    grvy_printf(GRVY_DEBUG,"%s: max revision width = %i\n",__func__,max_revision_width);

	    if(output_comments)
	      grvy_printf(GRVY_DEBUG,"%s: max comment width  = %i\n",__func__,max_comment_width);

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
		  
		  }
		  
		if(dump_files)
		  {
		    FILE *fp_mach = fp_experiments[ename];

		    fprintf(fp_mach,"%s --\n",cdelim);
		    fprintf(fp_mach,"%s Historical Performance Timing Records\n",cdelim);
		    fprintf(fp_mach,"%s libGRVY: Version = %s",cdelim,GRVY_LIB_VERSION); 
		    fprintf(fp_mach," (%i)\n",GRVY_get_numeric_version());
		    fprintf(fp_mach,"%s\n",cdelim);

		    fprintf(fp_mach,"%s Host     = %s\n",cdelim,machines[imach].c_str());
		    fprintf(fp_mach,"%s Sysname  = %s\n",cdelim, os_sysname.c_str());
		    fprintf(fp_mach,"%s Release  = %s\n",cdelim, os_release.c_str());
		    fprintf(fp_mach,"%s Version  = %s\n",cdelim, os_version.c_str());
		    fprintf(fp_mach,"%s CPU Type = %s\n",cdelim, cputype.c_str()   );
		    fprintf(fp_mach,"%s --\n",cdelim);

		    fprintf(fp_mach,"%s Experiment: %s (%zu total samples)\n",cdelim,
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

		    max_index_width = numDigits_positive(data.size());
		    if(max_index_width < 5)
		      max_index_width = 5;

		    fprintf(fp_mach,"%s %*s     Experiment-Date     Total Time(sec)    # Procs      "
			    "JobId %*s      Flops", cdelim,max_index_width,"Index",max_revision_width,"Revision");

		    // header for subtimer(s)

		    if(output_subtimers)
		      {

			fprintf(fp_mach," ");

			// first, we generate a list of all possible
			// subtimer names (stored on a per experiment
			// name basis); note that there is no
			// guarantee that the same set of subtimers
			// are defined for each experiment sample and
			// consequently, we use this aggregate map as
			// a means to provide standardized output

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
		      }	// end if(output_subtimers)

		    if(output_comments)
		      {
			max_comment_width = std::max(7,max_comment_width);
			fprintf(fp_mach," %*s ",max_comment_width,"Comment");
		      }

		    if(output_env)
		      fprintf(fp_mach," Environment Variables");

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

		    fprintf(fp_mach,"  %*i %s  %.8e %10i %10i %*s %.4e",max_index_width,i+1,
			    data[i].timestamp,data[i].total_time,
			    data[i].num_procs,data[i].job_Id,max_revision_width,data[i].code_revision,data[i].flops);
		  
		    if(output_subtimers)
		      {

			fprintf(fp_mach," ");

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
			    fprintf(fp_mach,"%*.8e ",width,(it_sub)->second);
			  }

		      }	// end if(output_subtimers)

		    if(output_comments)
		      {
			if(strlen(data[i].user_comment) > 0)
			  {
			    //			    int mylength = max_comment_width-strlen(data[i].user_comment)-1;
			    fprintf(fp_mach," %*s%s\"",(int)(max_comment_width-strlen(data[i].user_comment)-1),
				    "\"",data[i].user_comment);
				    //			    fprintf(fp_mach," %*s%s\"",mylength,
			  }
			else
			  {
			      fprintf(fp_mach," %*sN/A\"",(int)(max_comment_width-strlen("N/A")-1),"\"");
			  }
		      }

		    if(output_env)
		      {
			Env_PTable_V1 *env_variables = (Env_PTable_V1*)data[i].vl_env_variables.p;
			for(int j=0;j<data[i].vl_env_variables.len;j++)
			  {
			    fprintf(fp_mach," %s",env_variables[j].env_string);
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

  int GRVY_Timer_Class::GRVY_Timer_ClassImp::ReadAllHostData(GRVY_HDF5_Class *h5, string machine,
							     hid_t tableId, hid_t timers_type, 
							     vector <TimerPTable_V1> *data)
  {

    grvy_printf(GRVY_DEBUG,"%s: Reading performance data (format = V1)\n",__func__);

#ifdef USE_HDF5_PTABLE
    hsize_t nrecords = h5->m_pimpl->PTableNumPackets(tableId);
#else
    int num_measurements;
    h5->AttributeRead(machine,"num_measurements", num_measurements );
    hsize_t nrecords = num_measurements;
#endif

    grvy_printf(GRVY_DEBUG,"%s: %i packets are available for reading \n",__func__,nrecords);

    data->reserve(nrecords);

    // Initialize mem/file space for subsquent data reads

    hsize_t dims_current[1] = {1};
    hsize_t dims_max[1]     = {1};
    hsize_t coord[1];
    hid_t   filespace       = H5Dget_space(tableId);
    hid_t   space_id        = H5Screate_simple(1,dims_current,dims_max);

    // Loop over all packet entries and stash results in (data)

    Timer_PTable_V1 packet_data;

    for(int i=0;i<nrecords;i++)
      {
#ifdef USE_HDF5_PTABLE
	if (H5PTget_next(tableId,1,&packet_data) < 0)
	  {
	    grvy_printf(GRVY_FATAL,"%s: Unable to read %i packet (%i)\n",__func__,i);
	    exit(1);
	  }
#else

	coord[0] = i;
	H5Sselect_elements(filespace,H5S_SELECT_SET,1,coord);

	if (H5Dread(tableId,timers_type,space_id,filespace,H5P_DEFAULT,&packet_data) < 0)
	  {
	    grvy_printf(GRVY_FATAL,"%s: Unable to read %i performance packet (%i)\n",__func__,i);
	    exit(1);
	  }
#endif
	else
	  {
	    data->push_back(packet_data);
	  }
      }

    grvy_printf(GRVY_DEBUG,"%s: Finished reading performance data (format = V1)\n",__func__);

    return(0);
  }

  int GRVY_Timer_Class::GRVY_Timer_ClassImp::RepackAllHostData(GRVY_HDF5_Class *h5, int version,
							       hid_t tableId, hid_t timers_type, string machine)
  {
    vector<TimerPTable_V1> data;
    ReadAllHostData(h5,machine,tableId,timers_type,&data);

    grvy_printf(GRVY_INFO,"%s: Repacking %i measurements for host %s\n",__func__,data.size(),machine.c_str());

    // Initialize mem/file space for subsquent data reads

    hsize_t dims_current[1] = {1};
    hsize_t dims_max[1]     = {H5S_UNLIMITED};
    hsize_t coord[1];
    hid_t   filespace       = H5Dget_space(tableId);

#if 0

    // method to write one at a time

    hid_t   space_id        = H5Screate_simple(1,dims_current,dims_max);
    for(int i=0;i<data.size();i++)
      {
	coord[0] = i;
	H5Sselect_elements(filespace,H5S_SELECT_SET,1,coord);

	H5Dwrite(tableId,timers_type,space_id,filespace,H5P_DEFAULT,&(data)[i]);
      }
#else

    // method to write all data at once

    hsize_t  offset[1] = {0};
    dims_current[0]    = data.size();

    hid_t   space_id   = H5Screate_simple(1,dims_current,dims_max);

    H5Sselect_hyperslab(filespace,H5S_SELECT_SET,offset,NULL,dims_current,NULL);

    H5Dwrite(tableId,timers_type,space_id,filespace,H5P_DEFAULT,&(data)[0]);

    grvy_printf(GRVY_INFO,"%s: rewrote %i packets\n",__func__,data.size());
#endif

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
      hid_t ptable_env_type;
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

      hid_t runtime_env_type;

      if( (ptable_env_type = H5Tcreate (H5T_COMPOUND, sizeof(Env_PTable_V1))) < 0 )
	{
	  grvy_printf(GRVY_FATAL,"%s: Unable to create compound HDF datatype\n",__func__);
	  exit(1);
	}

      H5Tinsert(ptable_env_type, "environment variable", HOFFSET(Env_PTable_V1, env_string),  strtype );
      runtime_env_type = H5Tvlen_create(ptable_env_type);

      // Now, create a combined complex datatype with the header and VL subtimer(s)
      
      hid_t timers_type;
      
      if ( (timers_type = H5Tcreate(H5T_COMPOUND,sizeof(TimerPTable_V1))) < 0)
	{
	  grvy_printf(GRVY_FATAL,"%s: Unable to create compound HDF datatype\n",__func__);
	  exit(1);
	}

      H5Tinsert(timers_type,"Experiment Name",HOFFSET(TimerPTable_V1,experiment),      strtype         );
      H5Tinsert(timers_type,"Timestamp      ",HOFFSET(TimerPTable_V1,timestamp) ,      strtype         );
      H5Tinsert(timers_type,"Comment",        HOFFSET(TimerPTable_V1,user_comment),    strtype         );
      H5Tinsert(timers_type,"Total Time",     HOFFSET(TimerPTable_V1,total_time),      H5T_IEEE_F64LE  );
      H5Tinsert(timers_type,"Num Processors", HOFFSET(TimerPTable_V1,num_procs),       H5T_STD_I32LE   );
      H5Tinsert(timers_type,"Job Id",         HOFFSET(TimerPTable_V1,job_Id),          H5T_STD_I32LE   );
      H5Tinsert(timers_type,"Flops",          HOFFSET(TimerPTable_V1,flops),           H5T_IEEE_F64LE  );
      H5Tinsert(timers_type,"Code Revision"  ,HOFFSET(TimerPTable_V1,code_revision),   strtype         );
      H5Tinsert(timers_type,"Env Variables",  HOFFSET(TimerPTable_V1,vl_env_variables),runtime_env_type);
      H5Tinsert(timers_type,"SubTimers",      HOFFSET(TimerPTable_V1,vl_subtimers),    subtimer_type   );

      H5Tclose(strtype);
      H5Tclose(ptable_type);
      H5Tclose(ptable_env_type);
    
      // Pack the datatype for mem alignment

      H5Tpack(timers_type);

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
							    int num_procs,int jobId, string code_revision, 
							    double flops, int version, hid_t tableId, 
							    hid_t timers_type, bool save_internal_timers,
							    int num_measurements)
  {
    grvy_printf(GRVY_DEBUG,"Appending historical timer data for PTable version %i\n",version);
    grvy_printf(GRVY_DEBUG,"--> save_internal_timer = %i\n",save_internal_timers);

    int num_subtimers;		// total number of defined subtimers

    if(save_internal_timers)
      num_subtimers = TimerMap.size() - 1;  // -1 since global timer goes in header
    else
      num_subtimers = 0;		    // no subtimers if user is providing a global timing

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
	vector<Env_PTable_V1>      env_variables;

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

	// Environment variable inclusion

	GRVY_Hostenv_Class myenv;
	vector<string> runtimeEnv;

	if(options["output_printenv"])
	  {
	    Env_PTable_V1 env_tmp;

	    runtimeEnv = myenv.Getenv();

	    for(vector<string>::iterator it=runtimeEnv.begin();it != runtimeEnv.end(); ++it)
	      {
		env_tmp.env_string = (*it).c_str();
		env_variables.push_back(env_tmp);
	      }

	    header.vl_env_variables.len = runtimeEnv.size();
	    header.vl_env_variables.p   = &env_variables[0];
	  }
#if 1
	else
	  {
	    header.vl_env_variables.p = NULL;
	    header.vl_env_variables.len = 0;
	  }
#endif

	_GRVY_Type_TimerMap2 :: iterator index;

	SubTimer_PTable_V1 data_tmp;

	if(save_internal_timers)
	  {
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
	  }

	// ------------------------------------------------
	// That was fun, now we can append the packet data
	// ------------------------------------------------

#ifdef USE_HDF5_PTABLE
	assert(H5PTappend( tableId, 1, &(header) ) >= 0);
#else
	hsize_t dims_current[1] = {1};
	//	hsize_t dims_max[1]     = {1};
	hsize_t dims_max[1]     = {H5S_UNLIMITED};
	hsize_t coord[1]        = {num_measurements};

	hid_t filespace         = H5Dget_space(tableId);
	hid_t space_id          = H5Screate_simple(1,dims_current,dims_max);

	H5Sselect_elements(filespace,H5S_SELECT_SET,1,coord);
	assert( H5Dwrite(tableId,timers_type,space_id,filespace,H5P_DEFAULT,&(header)) >= 0);

	grvy_printf(GRVY_DEBUG,"%s: Just wrote performance packet data)\n",__func__);
#endif
    
      }
      break;

    default:
      grvy_printf(GRVY_FATAL,"%s: Unknown timer packet version requested (%i)\n",__func__,version);
      exit(1);
    }

    return 0;
  }


  
#endif


// AddEllipsis() - replace last 3 characters in a string with "..." to signify
// that a string has been shortened on output

void GRVY_Timer_Class::GRVY_Timer_ClassImp::AddEllipsis(std::string &name)
{
  int length = name.length();

  name[length-1] = '.';
  name[length-2] = '.';
  name[length-3] = '.';

  return;
}

}
