// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
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
// -------------------------------------------------------------------------
//
// hpct.h: Basic class member functions.
//
// $Id: hpct.h 2422 2009-04-19 19:33:42Z karl $
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include<sys/time.h>
#include<time.h>
#include<hpct_classes.h>
#include<hpct.h>
#include<hpct_int.h>


namespace HPCT {

int show_statistics = 1;

//-------------------------------------
// HPCT_Input_Class:: Member Functions
//-------------------------------------

HPCT_Input_Class::HPCT_Input_Class()   // default constructor
{
  Float_Def     = -9999999.0f;
  Double_Def    = -9999999.0e1;
  Int_Def       = -9999999;
  Long_Def      = -9999999;
  Char_Def      = "unknown";
  comment_start = "#";
  comment_end   = "\n";

  initialized   = 0;

  // Convention is to assume that user wants error messages.
  // Thou shalt turn them off otherwise.

  silent        = 0; 

}

int HPCT_Input_Class:: VerifyInit()
{
  if(!initialized)
    {
      printf("\n%s (%s): uninitialized input file - verify file has been opened\n",
             _HPCT_emask,__func__);
      return 0;
    }
  else
    return 1;

}

int HPCT_Input_Class:: Open(const char *filename)
{
  ifile = GetPot(filename,comment_start,comment_end);

  if(ifile.size() <= 1)
    {
      if(!silent)
	printf("\n%s (%s): non-existent or empty file -> %s\n",_HPCT_emask,__func__,filename);
      return 0;
    }
  else
    {
      initialized=1;
      return 1;
    }
}

int HPCT_Input_Class:: Close()
{
  initialized=0;
  return 1;
}

int HPCT_Input_Class:: Fdump()
{
  if(! VerifyInit()) return 0;

  ifile.print();		// dump the raw file contents
  PrintRegVars("");		// include any registered defaults

  return 1;
}

int HPCT_Input_Class:: Fdump(const char *prefix)
{

  if(! VerifyInit()) return 0;

  ifile.print(prefix);		// dump the raw file contents
  PrintRegVars(prefix);		// include any registered defaults

  return 1;
}

int HPCT_Input_Class:: Fdump(const char *prefix, const char *filename)
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

void HPCT_Input_Class::PrintRegVars(const char *prefix)
{

  map<std::string, int    > :: const_iterator int_index;
  map<std::string, float  > :: const_iterator flt_index;
  map<std::string, double > :: const_iterator dbl_index;
  map<std::string, string > :: const_iterator str_index;

  cout << prefix << "[HPCT Registered Variable Default Values]" << endl;

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

void HPCT_Input_Class:: ErrorMsg(const char *func_name,const char *var_name)
{
  if(!silent)
    printf("\n%s (%s): unable to query variable -> %s\n",_HPCT_emask,func_name,var_name);
}

void HPCT_Input_Class:: MsgToggle(int flag)
{
  if(flag == 0)
    silent = 1;
  else
    silent = 0;
}

//--------------
// Scalar Reads
//--------------

template <typename T> int HPCT_Input_Class:: Read_Var(const char *var, T *value, T Var_Def)
{
  if(! VerifyInit()) return 0;

  *value = ifile(var,Var_Def);

  if(*value == Var_Def)
    {
      if( !Get_Var(var,value) )
	{
	  ErrorMsg("fread",var);
	  return 0;
	}
      else
	{
	  if(!silent)
	    _HPCT_message(_HPCT_imask,"fread","Using pre-registered value for variable",var);
	}
    }
  
  return 1;
}

//--------------
// Vector Reads
//--------------

template <typename T> int HPCT_Input_Class:: Read_Var_Vec(const char *var, T *value, int nelems,T Var_Def)
{
  int i;

  if(! VerifyInit()) return 0;

  for(i=0;i<nelems;i++)
    {
      value[i] = ifile(var,Var_Def,i);

      if(value[i] == Var_Def)
        {
	  ErrorMsg("fread_ivec",var);
          return 0;
        }
    }
 
 return 1;
}

//------------------
// ith Vector Reads
//------------------

template <typename T> int HPCT_Input_Class:: Read_Var_iVec(const char *var, T *value, int elem,T Var_Def)
{
  int i;

  if(! VerifyInit()) return 0;

  *value = ifile(var,Var_Def,elem);

  if(*value == Var_Def)
    {
      ErrorMsg("fread_ivec",var);
      return 0;
    }
 
 return 1;
} 

//------------------------
// Character String Reads
//------------------------

int HPCT_Input_Class:: Read_Var(const char *var, char **value)
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
	  ErrorMsg("fread_char",var);
	  return 0;
	}
      else 
	{
	  if(!silent)
	    _HPCT_message(_HPCT_imask,"fread_char","Using pre-registered value for variable",var);
	}
    }
  //  else
  return 1;
}

int HPCT_Input_Class:: Read_Var_iVec(const char *var, char **value, int elem)
{
  string tstring;

  if(! VerifyInit()) return 0;
  
  tstring = ifile(var,Char_Def,elem);
  *value = (char *) malloc(tstring.length()*sizeof(char)+1);
  strcpy(value[0],tstring.c_str());

  if(strcmp(*value,Char_Def) == 0)
    {
      ErrorMsg("fread_char_ivec",var);
      return 0;
    }
  else
    return 1;
}

//---------------------------------------
// Default Variable Value Registrations
//---------------------------------------

void HPCT_Input_Class:: Register_Var (const char *varname, int var)
{
  default_ints[varname] = var;
  return;
}

void HPCT_Input_Class:: Register_Var (const char *varname, float var)
{
  default_floats[varname] = var;
  return;
}

void HPCT_Input_Class:: Register_Var (const char *varname, double var)
{
  default_doubles[varname] = var;
  return;
}

void HPCT_Input_Class:: Register_Var (const char *varname, char *var)
{
  default_strings[varname] = var;
  return;
}

int HPCT_Input_Class:: Get_Var (const char *varname, int *var)
{
  map<std::string, int > :: const_iterator index;
  
  index = default_ints.find(varname);

  if( index == default_ints.end() )
    {
      if(!silent)
	_HPCT_message(_HPCT_imask,"register_get","No registered variable named",varname);
      return(0);
    }
  else
    {
      *var = index->second;
      return(1);
    }

}

int HPCT_Input_Class:: Get_Var (const char *varname, float *var)
{
  map<std::string, float > :: const_iterator index;
  
  index = default_floats.find(varname);

  if( index == default_floats.end() )
    {
      if(!silent)
	_HPCT_message(_HPCT_imask,"register_get","No registered variable named",varname);
      return(0);
    }
  else
    {
      *var = index->second;
      return(1);
    }

}

int HPCT_Input_Class:: Get_Var (const char *varname, double *var)
{
  map<std::string, double > :: const_iterator index;
  
  index = default_doubles.find(varname);

  if( index == default_doubles.end() )
    {
      if(!silent)
	_HPCT_message(_HPCT_imask,"register_get","No registered variable named",varname);
      return(0);
    }
  else
    {
      *var = index->second;
      return(1);
    }

}

int HPCT_Input_Class:: Get_Var (const char *varname, char **var)
{
  map<std::string, string > :: const_iterator index;
  string tstring;
  
  index = default_strings.find(varname);

  if( index == default_strings.end() )
    {
      if(!silent)
	_HPCT_message(_HPCT_imask,"register_get","No registered variable named",varname);
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

template int HPCT_Input_Class::Read_Var <int>          (const char *var, int    *value, int    vardef);
template int HPCT_Input_Class::Read_Var <float>        (const char *var, float  *value, float  vardef);
template int HPCT_Input_Class::Read_Var <double>       (const char *var, double *value, double vardef);

template int HPCT_Input_Class:: Read_Var_Vec <int>     (const char *var, int    *value, int nelem, int    Var_Def);
template int HPCT_Input_Class:: Read_Var_Vec <float>   (const char *var, float  *value, int nelem, float  Var_Def);
template int HPCT_Input_Class:: Read_Var_Vec <double>  (const char *var, double *value, int nelem, double Var_Def);

template int HPCT_Input_Class:: Read_Var_iVec <int>    (const char *var, int    *value, int elem,  int    Var_Def);
template int HPCT_Input_Class:: Read_Var_iVec <float>  (const char *var, float  *value, int elem,  float  Var_Def);
template int HPCT_Input_Class:: Read_Var_iVec <double> (const char *var, double *value, int elem,  double Var_Def);


//-------------------------------------
// HPCT_Timer_Class:: Member Functions
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

HPCT_Timer_Class::HPCT_Timer_Class()   // default constructor
{
  initialized = 1;
  timer_last  = 0.0;
}

void HPCT_Timer_Class:: VerifyInit ()
{

  if(_HPCT_Timers == NULL)
    {
      printf("\n%s (%s): timer uninitialized\n",_HPCT_emask,__func__);
      exit(1);
    }

  if(!initialized)
    {
      printf("\n%s (%s): timer uninitialized\n",_HPCT_emask,__func__);
      exit(1);
    }

}

void HPCT_Timer_Class:: SaveTimerName (const char *id)
{
  timer_name = id;
  return;
}

void HPCT_Timer_Class:: BeginTimer (const char *id)
{
  double mytime;
  _HPCT_Type_TimerMap2 :: iterator index;

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

void HPCT_Timer_Class:: EndTimer (const char *id)
{
  double      mytime, increment;
  tTimer_Data Data;

  _HPCT_Type_TimerMap2 :: iterator index;

  mytime = RawTimer();
  index  = TimerMap.find(id);

  if ( index == TimerMap.end() )
    _HPCT_message(_HPCT_emask,__func__,"No timer data available for",id);
  else if(index->first[1] < 0)
    _HPCT_message(_HPCT_emask,__func__,"No matching begin timer call for",id);
  else
    {
      // update map with latest increment info

      increment = mytime - (index->second).timings[1];

      (index->second).timings[0] += increment;
      (index->second).timings[1]  = -1.;

      (index->second).stats(increment);
    }

  return;
}

void HPCT_Timer_Class:: Reset()
{
  _HPCT_Type_TimerMap2 :: iterator index;

  if(_HPCT_Timers == NULL)
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

double HPCT_Timer_Class:: RawTimer()
{
  int rc;
  struct timeval tv;

  rc = gettimeofday (&tv, NULL);
  if (rc == -1) {
    printf("tmrc: gettimeofday\n");
    return -1.;
  }

  double t1 =  ((double) tv.tv_sec) + 1.e-6*((double) tv.tv_usec);

  if( (t1 - timer_last) <= _HPCT_TIMER_THRESH )
    _HPCT_message(_HPCT_wmask,__func__,"Timer accuracy may be insufficient - just measured:",
		  t1-timer_last);
  
  timer_last = t1;
  return(t1);

}

double HPCT_Timer_Class:: ElapsedSeconds(const char *id)
{
  double elapsedseconds = 0.0;

  _HPCT_Type_TimerMap2 :: const_iterator index = TimerMap.find(id);

  if ( index == TimerMap.end() )
    _HPCT_message(_HPCT_emask,__func__,"No timer data available for",id);
  else if( (index->second).timings[1] != -1)
    _HPCT_message(_HPCT_emask,__func__,"Timer still active for",id);
  else
    elapsedseconds = (index->second).timings[0];

  return elapsedseconds;
}

void HPCT_Timer_Class:: Summarize()
{
  vector <double> timings(2);
  double totaltime,subtime;
  double local_percentage, total_percentage;
  int    global_time_defined = 0;
  int    max_timer_name_width;
  size_t display_id_width = 20;
  size_t timer_name_width;
  const size_t max_stdout_width = 120;

  _HPCT_Type_TimerMapSortLH _HPCT_TimerMapSortLH;
  _HPCT_Type_TimerMapSortHL _HPCT_TimerMapSortHL;

  _HPCT_Type_TimerMapSortLH :: iterator indexLH;
  _HPCT_Type_TimerMapSortHL :: iterator indexHL;

  _HPCT_Type_TimerMap2 :: iterator index,gindex;

  // Was a global timing region defined via HPCT_timer_init() and
  // HPCT_timer_end()?  If so, use the total time to define runtime
  // percentages.

  gindex = TimerMap.find(_HPCT_gtimer);

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

	  _HPCT_TimerMapSortHL[timings] = index->first;

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

  printf("%s - %-*s ",varstring.c_str(),display_id_width+32-varstring.length(),"Performance Timings:");

  if(show_statistics)
    printf("|      Mean      Variance       Count");

  printf("\n");

  // Print results for all user-defined timer keys

  for(indexHL=_HPCT_TimerMapSortHL.begin(); indexHL != _HPCT_TimerMapSortHL.end(); ++indexHL)
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
	  if(indexHL->second !=  _HPCT_gtimer )
	    {
	      gindex = TimerMap.find(indexHL->second);
	      printf(" | [%10.5e  %10.5e  %9i]",
		     boost::accumulators::mean    ((gindex->second).stats),
		     boost::accumulators::variance((gindex->second).stats),
		     boost::accumulators::count   ((gindex->second).stats));
	    }
	}
      
      printf("\n");
      
    }

  // Print results for left-over contribution in main timer

  gindex = TimerMap.find(_HPCT_gtimer);

  printf("--> %-*s: %10.5e secs",(int)display_id_width,_HPCT_gtimer,
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

      if( fabs(total_percentage - 100.0) > _HPCT_PERC_TOL )
	{
	  printf("\n%s: Profile percentages do not sum to 100 %%.\n",_HPCT_wmask);
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

}
