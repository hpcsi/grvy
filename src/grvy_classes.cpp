// -*-c++-*-
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
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
// -------------------------------------------------------------------------
//
// grvy_classes.cpp: Basic class member functions.
//
// $Id$
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include<sys/time.h>
#include<stdarg.h>
#include<time.h>
#include<grvy_classes.h>
#include<grvy.h>
#include<grvy_int.h>

namespace GRVY {

  int show_statistics = 1;

  //-------------------------------------
  // GRVY_Input_Class:: Member Functions
  //-------------------------------------

  GRVY_Input_Class::GRVY_Input_Class()   // default constructor
  {
    Float_Def     = -9999999.0f;
    Double_Def    = -9999999.0e1;
    Int_Def       = -9999999;
    Long_Def      = -9999999;
    Char_Def      = "unknown";
    comment_start = "#";
    comment_end   = "\n";

    initialized   = 0;
  }

  int GRVY_Input_Class:: VerifyInit()
  {
    if(!initialized)
      {
	_GRVY_message(GRVY_ERROR,__func__,"uninitialized input file - verify file has been opened");
	return 0;
      }
    else
      return 1;

  }

  int GRVY_Input_Class:: Open(const char *filename)
  {
    ifile = GRVYGetPot::GetPot(filename,comment_start,comment_end);

    if(ifile.size() <= 1)
      {
	_GRVY_message(GRVY_ERROR,__func__,"non-existent or empty file -> ",filename);
	return 0;
      }
    else
      {
	initialized=1;
	return 1;
      }
  }

  int GRVY_Input_Class:: Close()
  {
    initialized=0;
    return 1;
  }

  int GRVY_Input_Class:: Fdump()
  {
    if(! VerifyInit()) return 0;

    ifile.print();		// dump the raw file contents
    PrintRegVars("");		// include any registered defaults

    return 1;
  }

  int GRVY_Input_Class:: Fdump(const char *prefix)
  {

    if(! VerifyInit()) return 0;

    ifile.print(prefix);		// dump the raw file contents
    PrintRegVars(prefix);		// include any registered defaults

    return 1;
  }

  int GRVY_Input_Class:: Fdump(const char *prefix, const char *filename)
  {

    if(! VerifyInit()) return 0;

    std::streambuf *cout_sbuf = std::cout.rdbuf();   // save original stdout buff
    std::ofstream fout(filename,ios::app|ios::out);  // file for append
    std::cout.rdbuf(fout.rdbuf());                   // redirect cout to file        

    ifile.print(prefix);				   // dumpe the raw file contents
    PrintRegVars(prefix);		                   // include any registered defaults
    std::cout.rdbuf(cout_sbuf);                      // restore cout stream

    return 1;
  }

  void GRVY_Input_Class::PrintRegVars(const char *prefix)
  {

    map<std::string, int    > :: const_iterator int_index;
    map<std::string, float  > :: const_iterator flt_index;
    map<std::string, double > :: const_iterator dbl_index;
    map<std::string, string > :: const_iterator str_index;

    cout << prefix << "[GRVY Registered Variable Default Values]" << endl;

    for(int_index=default_ints.begin(); int_index != default_ints.end(); ++int_index)
      cout << prefix << (int_index->first).c_str() << "=" << int_index->second << endl;

    for(flt_index=default_floats.begin(); flt_index != default_floats.end(); ++flt_index)
      cout << prefix << (flt_index->first).c_str() << "=" << flt_index->second << endl;

    for(dbl_index=default_doubles.begin(); dbl_index != default_doubles.end(); ++dbl_index)
      cout << prefix << (dbl_index->first).c_str() << "=" << dbl_index->second << endl;

    for(str_index=default_strings.begin(); str_index != default_strings.end(); ++str_index)
      cout << prefix << (str_index->first).c_str() << "=" << str_index->second << endl;

    cout << endl;
  }

  void GRVY_Input_Class:: MsgToggle(int flag)
  {
    if(flag == 0)
      {
	_GRVY_Log.change_priority(GRVY_NOLOG);
      }
    else
      {
	_GRVY_Log.change_priority(GRVY_INFO);
      }

    return;
  }

  //--------------
  // Scalar Reads
  //--------------

  template <typename T> int GRVY_Input_Class:: Read_Var(const char *var, T *value, T Var_Def)
  {
    if(! VerifyInit()) return 0;

    *value = ifile(var,Var_Def);

    if(*value == Var_Def)
      {
	if( !Get_Var(var,value) )
	  {
	    _GRVY_message(GRVY_ERROR,"fread","Unable to query variable -> ",var);
	    return 0;
	  }
	else
	  {
	    _GRVY_message(GRVY_INFO,"fread: Using pre-registered value for variable",var,*value);
	  }
      }
  
    return 1;
  }

  //--------------
  // Vector Reads
  //--------------

  template <typename T> int GRVY_Input_Class:: Read_Var_Vec(const char *var, T *value, int nelems,T Var_Def)
  {
    int i;

    if(! VerifyInit()) return 0;

    for(i=0;i<nelems;i++)
      {
	value[i] = ifile(var,Var_Def,i);

	if(value[i] == Var_Def)
	  {
	    _GRVY_message(GRVY_ERROR,"fread_ivec","Unable to query variable -> ",var);
	    return 0;
	  }
      }
 
    return 1;
  }

  //------------------
  // ith Vector Reads
  //------------------

  template <typename T> int GRVY_Input_Class:: Read_Var_iVec(const char *var, T *value, int elem,T Var_Def)
  {
    int i;

    if(! VerifyInit()) return 0;

    *value = ifile(var,Var_Def,elem);

    if(*value == Var_Def)
      {
	_GRVY_message(GRVY_ERROR,"fread_ivec","Unable to query variable -> ",var);
	return 0;
      }
 
    return 1;
  } 

  //------------------------
  // Character String Reads
  //------------------------

  int GRVY_Input_Class:: Read_Var(const char *var, char **value)
  {
    string tstring;

    if(! VerifyInit()) return 0;
  
    tstring = ifile(var,Char_Def);
    *value = (char *) malloc(tstring.length()*sizeof(char)+1);
    strcpy(value[0],tstring.c_str());

    if(strcmp(*value,Char_Def) == 0)
      {

	if( !Get_Var(var,value) )
	  {
	    _GRVY_message(GRVY_ERROR,"fread_char","Unable to query variable -> ",var);
	    return 0;
	  }
	else 
	  {
	    _GRVY_message(GRVY_INFO,"fread_char","Using pre-registered value for variable",var);
	  }
      }
    return 1;
  }

  int GRVY_Input_Class:: Read_Var_iVec(const char *var, char **value, int elem)
  {
    string tstring;

    if(! VerifyInit()) return 0;
  
    tstring = ifile(var,Char_Def,elem);
    *value = (char *) malloc(tstring.length()*sizeof(char)+1);
    strcpy(value[0],tstring.c_str());

    if(strcmp(*value,Char_Def) == 0)
      {
	_GRVY_message(GRVY_ERROR,"fread_char_ivec","Unable to query variable -> ",var);
	return 0;
      }
    else
      return 1;
  }

  //---------------------------------------
  // Default Variable Value Registrations
  //---------------------------------------

  void GRVY_Input_Class:: Register_Var (const char *varname, int var)
  {
    default_ints[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, float var)
  {
    default_floats[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, double var)
  {
    default_doubles[varname] = var;
    return;
  }

  void GRVY_Input_Class:: Register_Var (const char *varname, char *var)
  {
    default_strings[varname] = var;
    return;
  }

  int GRVY_Input_Class:: Get_Var (const char *varname, int *var)
  {
    map<std::string, int > :: const_iterator index;
  
    index = default_ints.find(varname);

    if( index == default_ints.end() )
      {
	_GRVY_message(GRVY_INFO,"register_get","No registered variable named",varname);
	return(0);
      }
    else
      {
	*var = index->second;
	return(1);
      }

  }

  int GRVY_Input_Class:: Get_Var (const char *varname, float *var)
  {
    map<std::string, float > :: const_iterator index;
  
    index = default_floats.find(varname);

    if( index == default_floats.end() )
      {
	_GRVY_message(GRVY_INFO,"register_get","No registered variable named",varname);
	return(0);
      }
    else
      {
	*var = index->second;
	return(1);
      }

  }

  int GRVY_Input_Class:: Get_Var (const char *varname, double *var)
  {
    map<std::string, double > :: const_iterator index;
  
    index = default_doubles.find(varname);

    if( index == default_doubles.end() )
      {
	_GRVY_message(GRVY_INFO,"register_get","No registered variable named",varname);
	return(0);
      }
    else
      {
	*var = index->second;
	return(1);
      }

  }

  int GRVY_Input_Class:: Get_Var (const char *varname, char **var)
  {
    map<std::string, string > :: const_iterator index;
    string tstring;
  
    index = default_strings.find(varname);

    if( index == default_strings.end() )
      {
	_GRVY_message(GRVY_INFO,"register_get","No registered variable named",varname);
	return(0);
      }
    else
      {
	tstring = index->second;
	*var = (char *) malloc(tstring.length()*sizeof(char)+1);
	strcpy(var[0],tstring.c_str());
	return(1);
      }

  }

  //------------------------------
  // Supported Function Templates
  //------------------------------

  template int GRVY_Input_Class::Read_Var <int>          (const char *var, int    *value, int    vardef);
  template int GRVY_Input_Class::Read_Var <float>        (const char *var, float  *value, float  vardef);
  template int GRVY_Input_Class::Read_Var <double>       (const char *var, double *value, double vardef);

  template int GRVY_Input_Class:: Read_Var_Vec <int>     (const char *var, int    *value, int nelem, int    Var_Def);
  template int GRVY_Input_Class:: Read_Var_Vec <float>   (const char *var, float  *value, int nelem, float  Var_Def);
  template int GRVY_Input_Class:: Read_Var_Vec <double>  (const char *var, double *value, int nelem, double Var_Def);

  template int GRVY_Input_Class:: Read_Var_iVec <int>    (const char *var, int    *value, int elem,  int    Var_Def);
  template int GRVY_Input_Class:: Read_Var_iVec <float>  (const char *var, float  *value, int elem,  float  Var_Def);
  template int GRVY_Input_Class:: Read_Var_iVec <double> (const char *var, double *value, int elem,  double Var_Def);


  //-------------------------------------
  // GRVY_Timer_Class:: Member Functions
  //-------------------------------------

  // Notes: for portability concerns, we will go with a simple timeofday
  // based timer.  This should give resolution that is order
  // micro-seconds on desired systems.  Note that this also avoids any
  // issues that cycle-based counters face due to cpu freqeuncy
  // throttling or SMP issues due to potential thread migration.
  //
  // A check is included to potentially warn against accuracy problems
  // when the timer is used at a level below anticipated threshold
  // accuracy.

  GRVY_Timer_Class::GRVY_Timer_Class()   // default constructor
  {
    initialized    = 1;
    timer_last     = 0.0;
    timer_finalize = -1;
  }

  void GRVY_Timer_Class:: VerifyInit ()
  {

    if(_GRVY_Timers == NULL)
      {
	_GRVY_message(GRVY_ERROR,__func__,"timer uninitialized");
	exit(1);
      }

    if(!initialized)
      {
	_GRVY_message(GRVY_ERROR,__func__,"timer uninitialized");
	exit(1);
      }

  }

  void GRVY_Timer_Class:: SaveTimerName (const char *id)
  {
    timer_name = id;
    return;
  }

  void GRVY_Timer_Class:: BeginTimer (const char *id)
  {
    double mytime;
    _GRVY_Type_TimerMap2 :: iterator index;

    tTimer_Data Data;

    //  VerifyInit();

    mytime = RawTimer();

    // Is this the first call for this id?

    index = TimerMap.find(id);

    if ( index == TimerMap.end() )
      {
	Data.timings[0] = 0.0;	                        // stores accumulated time
	Data.timings[1] = mytime;                         // stores latest timestamp

	TimerMap[id] = Data;
      }
    else
      {
	(index->second).timings[1] = mytime;             // stores latest timestamp
      }

  
  }

  void GRVY_Timer_Class:: EndTimer (const char *id)
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

	if( increment <= _GRVY_TIMER_THRESH )
	  {
	    sprintf(temp_string,"Timer accuracy may be insufficient (%.30s) - just measured",id);
	    _GRVY_message(GRVY_WARN,__func__,temp_string,increment);
	  }

	(index->second).timings[0] += increment;
	(index->second).timings[1]  = -1.;

	(index->second).stats(increment);
      }

    // Store the latest raw timer snapshot for the global timer region
    // to allow users to query the global elapsed time after an
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

    if(!initialized)
      return;

    // Reset All timers

    for(index=TimerMap.begin(); index != TimerMap.end(); ++index)
      {
	(index->second).timings[0] = 0.0;             // reset raw timing info
	(index->second).stats      = stats_empty;     // reset accumulator (no reset in Boost currently)
      }

    return;
  }

  double GRVY_Timer_Class:: RawTimer()
  {
    int rc;
    struct timeval tv;

    rc = gettimeofday (&tv, NULL);
    if (rc == -1) {
      printf("tmrc: gettimeofday\n");
      return -1.;
    }

    double t1 =  ((double) tv.tv_sec) + 1.e-6*((double) tv.tv_usec);

    // moved warning check to EndTimer
    //
    //  if( (t1 - timer_last) <= _GRVY_TIMER_THRESH )
    //    _GRVY_message(GRVY_WARN,__func__,"Timer accuracy may be insufficient - just measured:",
    //		  t1-timer_last);
  
    timer_last = t1;
    return(t1);

  }

  double GRVY_Timer_Class:: ElapsedSeconds(const char *id)
  {
    double elapsedseconds = 0.0;

    _GRVY_Type_TimerMap2 :: const_iterator index = TimerMap.find(id);

    if ( index == TimerMap.end() )
      _GRVY_message(GRVY_ERROR,__func__,"No timer data available for",id);
    else if( (index->second).timings[1] != -1)
      _GRVY_message(GRVY_ERROR,__func__,"Timer still active for",id);
    else
      elapsedseconds = (index->second).timings[0];

    return elapsedseconds;
  }

  int GRVY_Timer_Class:: StatsCount(const char *id)
  {
    _GRVY_Type_TimerMap2 :: const_iterator index = TimerMap.find(id);
    if ( index == TimerMap.end() )
      _GRVY_message(GRVY_ERROR,__func__,"No stats data available for",id);
    else
      return(boost::accumulators::count((index->second).stats));
  }

  double GRVY_Timer_Class:: StatsMean(const char *id)
  {
    _GRVY_Type_TimerMap2 :: const_iterator index = TimerMap.find(id);
    if ( index == TimerMap.end() )
      _GRVY_message(GRVY_ERROR,__func__,"No stats data available for",id);
    else
      return(boost::accumulators::mean((index->second).stats));
  }

  double GRVY_Timer_Class:: StatsVariance(const char *id)
  {
    _GRVY_Type_TimerMap2 :: const_iterator index = TimerMap.find(id);
    if ( index == TimerMap.end() )
      _GRVY_message(GRVY_ERROR,__func__,"No stats data available for",id);
    else
      return(boost::accumulators::variance((index->second).stats));
  }

  double GRVY_Timer_Class:: ElapsedGlobal()
  {
    double elapsedseconds = 0.0;
    double mytime;

    mytime = RawTimer();

    _GRVY_Type_TimerMap2 :: const_iterator index = TimerMap.find(_GRVY_gtimer);

    if ( index == TimerMap.end() )
      _GRVY_message(GRVY_ERROR,__func__,"No timer data available for",_GRVY_gtimer);
    else if( (index->second).timings[1] != -1)     // inside active timer
      elapsedseconds = mytime - (index->second).timings[1];
    else if( (index->second).timings[1] == -1)     // outside active timer
      elapsedseconds = (index->second).timings[0];

    // if we are being called after a finalize(), include the elapsed
    // time since the last global timer was updated

    if (timer_finalize > 0)
      elapsedseconds += mytime - timer_finalize;

    return elapsedseconds;
  }

  void GRVY_Timer_Class:: Summarize()
  {
    vector <double> timings(2);
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

    // Was a global timing region defined via GRVY_timer_init() and
    // GRVY_timer_end()?  If so, use the total time to define runtime
    // percentages.

    gindex = TimerMap.find(_GRVY_gtimer);

    if ( gindex != TimerMap.end() )
      {
	totaltime = (gindex->second).timings[0];
	global_time_defined = 1;

	// Sum the timings from all subordinate keys

	subtime = 0.0;

	for(index=TimerMap.begin(); index != TimerMap.end(); ++index)
	  if(index != gindex)
	    subtime += (index->second).timings[0];

	// Temporarily reset the global key to store the exclusive cumulative time only.

	(gindex->second).timings[0] = totaltime - subtime;
      
      }
  
    // sort the list via timings for output to the masses

    for(index=TimerMap.begin(); index != TimerMap.end(); ++index)
      {

	if( index->first != gindex->first)
	  {
	    timings[0] = (index->second).timings[0];
	    timings[1] = (index->second).timings[1];

	    _GRVY_TimerMapSortHL[timings] = index->first;

	    // Update display width if this identifier is longer than default

	    display_id_width = max(display_id_width, index->first.length()+1);
	    display_id_width = min(display_id_width, max_stdout_width - 35);
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

    string varstring = timer_name.substr(0,max_timer_name_width-1);

    printf("%s - %-*s ",varstring.c_str(),(int)(display_id_width+32-varstring.length()),"Performance Timings:");

    if(show_statistics)
      printf("|      Mean      Variance       Count");

    printf("\n");

    // Print results for all user-defined timer keys

    for(indexHL=_GRVY_TimerMapSortHL.begin(); indexHL != _GRVY_TimerMapSortHL.end(); ++indexHL)
      {

	string varstring = indexHL->second.substr(0,display_id_width-1);
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
		gindex = TimerMap.find(indexHL->second);
		printf(" | [%10.5e  %10.5e  %9zi]",
		       boost::accumulators::mean    ((gindex->second).stats),
		       boost::accumulators::variance((gindex->second).stats),
		       boost::accumulators::count   ((gindex->second).stats));
	      }
	  }
      
	printf("\n");
      
      }

    // Print results for left-over contribution in main timer

    gindex = TimerMap.find(_GRVY_gtimer);

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

  //-------------------------------------
  // GRVY_Math_Class:: Member Functions
  //-------------------------------------

  GRVY_Math_Class::GRVY_Math_Class( ) // default constructor
  {
    return;
  }

  //-------------------------------------
  // GRVY_Log_Class:: Member Functions
  //-------------------------------------

  GRVY_Log_Class::GRVY_Log_Class()
  {

    // set default log level
    
    log_level = GRVY_INFO;

    // set default log masks for each priority level

    LogMask[GRVY_FATAL] = "[*] Fatal: ";
    LogMask[GRVY_ERROR] = "[*] Error: ";
    LogMask[GRVY_WARN ] = "[*]  Warn: ";
    LogMask[GRVY_INFO ] = "[*]  Info: ";
    LogMask[GRVY_DEBUG] = "[*] Debug: ";

    return;
  }

  void GRVY_Log_Class::change_priority(int priority)
  {
    log_level = priority;
    return;
  }

  void GRVY_Log_Class::msg(int priority, const string msg)
  {
    if(isLog(priority))
      {
	cout << LogMask[priority];
	cout << msg << endl;
      }

    return;
  }

  int GRVY_Log_Class::msg_printf(int priority, const char *format,va_list arg)
  {
    int done;

    printf("%s",LogMask[priority].c_str());
    done = vprintf (format, arg);
    return done;
  }

}
