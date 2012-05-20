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
// gadd.cpp: Utility for adding external performance data to HDF
// historical data.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<iostream>
#include<vector>
#include<boost/lexical_cast.hpp>

#include<grvy_classes.h>
#include<grvy_env.h>
#include<grvy.h>
#include<grvy_int.h>

#define GETPOT_NAMESPACE GRVYGetPot
#define GETPOT_DISABLE_MUTEX
#include<getpot.h>

using namespace std;

namespace GRVY_gadd
{
  void summarize_usage()
  {
    grvy_printf(GRVY_INFO,"\nUsage: %s [OPTIONS] time name file\n\n","gadd");
    grvy_printf(GRVY_INFO,"where \"time\" is the performancing timing in seconds, \"name\" is the\n"
		"experiment identifier, and \"file\" is the path to a libGRVY style historical\n"
		"performance database.\n");
    grvy_printf(GRVY_INFO,"\nThis utility is used to store application runtime measurements from various\n"
		"compute resources to provide historical monitoring during application development,\n"
		"perform system benchmarking, identify performance outliers, and facilitate\n"
		"cross-platform comparisons.\n\n");
    grvy_printf(GRVY_INFO,"OPTIONS:\n");
    grvy_printf(GRVY_INFO,"  --help                  generate help message and exit\n");
    grvy_printf(GRVY_INFO,"  --version               output version information and exit\n");
    grvy_printf(GRVY_INFO,"  -q [ --quiet ]          suppress normal stdout messages\n");
    grvy_printf(GRVY_INFO,"  -e [ --env ]            store runtime environment with timing\n");
    grvy_printf(GRVY_INFO,"  -c [ --comment ]  arg   additional comment string for the measurement\n");
    grvy_printf(GRVY_INFO,"  -m [ --machine ]  arg   machine name (default=local hostname)\n");
    grvy_printf(GRVY_INFO,"  -j [ --jobid ]    arg   batch job identifier (default = -1)\n");
    grvy_printf(GRVY_INFO,"  -p [ --numprocs ] arg   number of processors (default = 1)\n");
    grvy_printf(GRVY_INFO,"  -r [ --revision ] arg   application/code revision (default = unknown)\n");
    grvy_printf(GRVY_INFO,"  -f [ --flops ]    arg   measured FLOPs (default = 0.0)\n");
    grvy_printf(GRVY_INFO,"\n");
								    
    return;
  }

  void pop_argument(vector<string> &inputs,string arg)
  {
    for(size_t i=0;i<inputs.size();i++) // pop this arg from remaining inputs
      if(inputs[i] == arg)
	{
	  grvy_printf(GRVY_DEBUG,"Erasing delimiter = %s\n",arg.c_str());
	  inputs.erase(inputs.begin()+i);
	}
  }

  // support routines to read values for optional command-line arguments
  
  void read_optional_argument(GRVYGetPot::GetPot &cl, const char *opt_text1, const char *opt_text2, 
			      string &varname, vector<string> &arguments)
  {
    if(cl.search(2,opt_text1,opt_text2))
      {
	varname = cl.next(varname);
	grvy_printf(GRVY_DEBUG,"User requested %s option = %s\n",opt_text1,varname.c_str());
	pop_argument(arguments,varname);
      }
  }

  template <typename T> void read_optional_argument(GRVYGetPot::GetPot &cl, 
						    const char *opt_text1, const char *opt_text2, 
						    T &varname, vector<string> &arguments)
  {
    if(cl.search(2,opt_text1,opt_text2))
      {
	varname = cl.next(varname);

	if(grvy_log_getlevel() == GRVY_DEBUG)
	  cout << "User requested " << opt_text1 << " = " << varname << endl;

	pop_argument(arguments,boost::lexical_cast<string>(varname));
      }
  }

  void parse_supported_options(int argc, char *argv[], char *env[], GRVY::GRVY_Timer_Class *gt)
  {

    // Required inputs

    double timing=0;		 // user provided performance timing (secs)
    string name; 		 // user provided experiment name
    string input_file;		 // path to desired HDF5 storage file
    
    // Optional inputs
    
    string machine;		 // machine name (defaults to local hostname)
    string comment;		 // additional comment (defaults to empty string)
    int    jobId    =  -1;	 // default jobId
    int    numprocs =   1;	 // default processor count
    string revision = "unknown"; // default processor count
    double flops    = 0.0;	 // default FLOPs
    
    //-------------------
    // parse command line
    //-------------------

    GRVYGetPot::GetPot cl(argc,argv);

    if(cl.search("--version"))
      {
	grvy_version_stdout();
	return;
      }

    if(cl.search(2,"--help","-h"))
      {
	GRVY_gadd::summarize_usage();
	return;
      }

    vector<string> inputs = cl.nominus_vector();

    if(cl.search("--debug"))
      {
	grvy_log_setlevel(GRVY_DEBUG);
      }

    if(cl.search(2,"--quiet","-q"))
      {
	gt->SetOption("output_stdout",false);
	grvy_log_setlevel(GRVY_ERROR);
	
	grvy_printf(GRVY_DEBUG,"User requested --quiet option\n");
      }

    if(cl.search(2,"--with-env","-e"))
      {
	gt->SetOption("output_printenv",true);
	grvy_printf(GRVY_DEBUG,"User requested --env option\n");
      }

    // Optional arguments

    grvy_printf(GRVY_DEBUG,"\n%s: Parsing optional arguments:\n\n",__func__);
    GRVY_Hostenv_Class myenv;

    read_optional_argument(cl,"--comment", "-c",comment, inputs);
    read_optional_argument(cl,"--revision","-r",revision,inputs);
    read_optional_argument(cl,"--machine", "-m",machine, inputs);
    read_optional_argument(cl,"--jobId",   "-j",jobId,   inputs);
    read_optional_argument(cl,"--numprocs","-p",numprocs,inputs);
    read_optional_argument(cl,"--flops",   "-f",flops,   inputs);

    // Error on unsupported options

    vector<string> ufos = cl.unidentified_options();

    if(ufos.size() > 0)
      {
	grvy_printf(GRVY_ERROR,"\n");
	for(size_t i = 0;i<ufos.size();i++)
	  grvy_printf(GRVY_ERROR,"%s: Unsupported command-line argument detected (%s)\n","[*] Error",
		    ufos[i].c_str());
	GRVY_gadd::summarize_usage();
	exit(1);
      }

    // Required arguments 

    grvy_printf(GRVY_DEBUG,"\n%s: Parsing required arguments:\n\n",__func__);

    if(inputs.size() != 3)
      {
	GRVY_gadd::summarize_usage();
	exit(1);
      } 
    else
      {
	timing     = boost::lexical_cast<double>(inputs[0]);
	name       = inputs[1];
	input_file = inputs[2];
      }

    // If we have the pleasure of making it this far, then we have the
    // minimum required to save some performance data; fire in the hole...

    if(gt->SaveHistTiming(timing,machine,name,comment,numprocs,jobId,revision,flops,input_file) == 0)
      grvy_printf(GRVY_INFO,"\nSuccessfully stored timing information to file %s.\n\n",input_file.c_str());
    else
      grvy_printf(GRVY_ERROR,"\n[ERROR]: Unable to store timing information to file %s.\n\n",input_file.c_str());

    return;
}

} // end namespace GRVY_gadd

//------
// Main
//------

int main(int argc, char* argv[], char *env[])
{
  GRVY::GRVY_Timer_Class gt;  // a GRVY performance timer

  GRVY_gadd::parse_supported_options(argc,argv,env,&gt);

  return 0;
}










