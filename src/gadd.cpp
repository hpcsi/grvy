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
// $Id: grvy_classes.cpp 11224 2010-07-02 06:40:52Z karl $
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<iostream>
#include<vector>
#include<boost/program_options.hpp>
#include<boost/program_options/parsers.hpp>
#include<grvy_classes.h>
#include<grvy_env.h>
#include<grvy.h>
#include<grvy_int.h>

using namespace std;


namespace GRVY_gadd
{
  void summarize_usage()
  {
    grvy_printf(GRVY_INFO,"\nUsage: %s [OPTIONS] <time> <name> <file>\n\n","gadd");
    grvy_printf(GRVY_INFO,"where \"time\" is the performancing timing in seconds, \"name\" is the\n"
		"experiment identifier, and \"file\" is the path to a libGRVY style historical\n"
		"performance database.\n");
    grvy_printf(GRVY_INFO,"\nThis utility is used to store application runtime measurements from various\n"
		"compute resources to provide historical monitoring during application development,\n"
		"perform system benchmarking, identify performance outliers, and facilitate\n"
		"cross-platform comparisons.\n\n");
    grvy_printf(GRVY_INFO,"OPTIONS:\n");
    return;
  }

  void parse_supported_options(int argc, char *argv[],  GRVY::GRVY_Timer_Class *gt)
  {

    // Required inputs

    double timing;		// user provided performance timing (secs)
    string name; 		// user provided experiment name
    string input_file;		// path to desired HDF5 storage file
    
    // Optional inputs
    
    string machine;		// machine name (defaults to local hostname)
    string comment;		// additional comment (defaults to empty string)
    int    jobId    =  -1;	// default jobId
    int    numprocs =   1;	// default processor count
    string revision =  "-r0";	// default processor count
    double flops    = 0.0;	// default FLOPs
    
    // Define supported options for Boost
    
    namespace bo = boost::program_options;
    
    bo::options_description            visible;
    bo::options_description            hidden;
    bo::options_description            desc;
    bo::variables_map                  vmap;
    bo::positional_options_description p;
    
    visible.add_options()
      ("help",                            "generate help message and exit")
      ("version",                         "output version information and exit")
      ("quiet,q",                         "suppress normal stdout messages")
      ("comment,c", bo::value<string>(),  "additional comment string for the measurement")
      ("machine,m", bo::value<string>(),  "machine name (default=local hostname)")
      ("jobid,j",   bo::value<int>(),     "batch job identifier (default = -1)")
      ("numprocs,p",bo::value<int>(),     "number of processors (default = 1)")
      ("revision,r",bo::value<string>(),  "application/code revision (default = -r0)")
      ("flops,f",   bo::value<double>(),  "measured FLOPs (default = 0.0)")
      ;

    hidden.add_options()
      ("timing"    ,bo::value<double>(),  "timing")
      ("name"      ,bo::value<string>(),  "name")
      ("input-file",bo::value<string>(),  "input file")
      ("debug",                           "verbose debugging output")
      ;

    desc.add(hidden).add(visible); // combined option set

    p.add("timing"    , 1);	   // required measurement timing (secs)
    p.add("name"      , 1);	   // required measurement name
    p.add("input-file", 1);	   // required path to hdf5 storage file

    //-------------------
    // parse command line
    //-------------------

    bo::parsed_options parsed = 
      bo::command_line_parser(argc,argv).options(desc).positional(p).allow_unregistered().run();

    bo::store(parsed,vmap);
    bo::notify(vmap);

    if(vmap.count("help")  || !vmap.count("input-file") || !vmap.count("timing") || !vmap.count("name") )
      {
	GRVY_gadd::summarize_usage();
	cout << visible << "\n";
	return;
      }

    if(vmap.count("version"))
      {
	grvy_version_stdout();
	return;
      }

    if(vmap.count("debug"))
      {
	grvy_log_setlevel(GRVY_DEBUG);
      }

    if(vmap.count("quiet"))
      {
	gt->SetOption("output_stdout",false);
	grvy_printf(GRVY_DEBUG,"User requested --quiet option\n");
      }

    // Parse optional arguments

    grvy_printf(GRVY_DEBUG,"\n%s: Parsing optional arguments:\n\n",__func__);

    GRVY_Hostenv_Class myenv;

    machine  = GRVY::read_boost_option(vmap,"machine", myenv.Hostname());
    comment  = GRVY::read_boost_option(vmap,"comment", comment);
    jobId    = GRVY::read_boost_option(vmap,"jobid",   jobId);
    numprocs = GRVY::read_boost_option(vmap,"numprocs",numprocs);
    revision = GRVY::read_boost_option(vmap,"revision",revision);
    flops    = GRVY::read_boost_option(vmap,"flops",   flops);
    timing   = GRVY::read_boost_option(vmap,"timing",  timing);

    // Parse required arguments

    grvy_printf(GRVY_DEBUG,"\n%s: Parsing required arguments:\n\n",__func__);

    if(vmap.count("timing"))
      {
	timing = vmap["timing"].as<double>();
	grvy_printf(GRVY_DEBUG,"User provided timing value = %.6e (secs)\n",timing);
      }

    if(vmap.count("input-file"))
      {
	input_file = vmap["input-file"].as<string>();
	grvy_printf(GRVY_DEBUG,"User provided input file = %s\n",input_file.c_str());
      }

    // Erorr on unsupported options

    vector<string> unknown = bo::collect_unrecognized(parsed.options,bo::exclude_positional);

    if(unknown.size() > 0)
      {
	grvy_printf(GRVY_ERROR,"\n");

	for(int i=0;i<unknown.size();i++)
	  {
	    grvy_printf(GRVY_ERROR,"%s: Unsupported command-line argument detected (%s)\n","[*] Error:",
			unknown[i].c_str());
	  }

	GRVY_gadd::summarize_usage();
	cout << visible << "\n";
	exit(1);
      }

    // If we have the pleasure of making it this far, then we have the
    // minimum required to save some performance data; fire in the hole...

    //    gt->SaveHistTiming(timing,machine,name,comment,numprocs,jobId,-1,flops,input_file);



    return;
}

} // end namespace GRVY_gadd

//---------------------------------------------------------------------------
// Main
//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
  GRVY::GRVY_Timer_Class gt;  // a GRVY performance timer

  GRVY_gadd::parse_supported_options(argc,argv,&gt);

  return 0;
}










