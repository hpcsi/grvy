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

#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include "grvy.h"

using namespace std;
namespace bo = boost::program_options;

namespace GRVY_gadd
{
  void summarize_usage()
  {
    grvy_printf(GRVY_INFO,"\nUsage: %s [OPTIONS] file\n\n","gadd");
    grvy_printf(GRVY_INFO,"where \"file\" is the path to a libGRVY style historical performance database.\n");
    grvy_printf(GRVY_INFO,"\nThis utility is used to store application runtime measurements from various\n"
		"compute resources to provide historical monitoring during application development,\n"
		"perform system benchmarking, identify performance outliers, and facilitate\n"
		"cross-platform comparisons.\n\n");
    grvy_printf(GRVY_INFO,"OPTIONS:\n");
    return;
  }

  void parse_supported_options(int argc, char *argv[],  GRVY::GRVY_Timer_Class *gt)
  {

    string input_file;
    string delimiter ("#");
    string output_dir("./gdata");

    // define supported options

    bo::options_description visible;
    bo::options_description hidden;
    bo::options_description desc;
    bo::variables_map vmap;
    bo::positional_options_description p;

    visible.add_options()
      ("help",                            "generate help message and exit")
      ("version",                         "output version information and exit")
      ("quiet,q",                         "suppress normal stdout messages")
      ("machine,m",                       "machine name")
      ("name,n",	                  "experiment name")	
      ("jobid,j",                         "batch job identifier")
      ("numprocs,p",                      "number of processors")
      ("revision,r",                      "application/code revision")
      ("flops,f",                         "measured FLOPs")
      ;

    hidden.add_options()
      ("input-file",bo::value< string >(),"input file")
      ("debug",                           "verbose debugging output")
      ;

    desc.add(hidden).add(visible); // combined option set
    p.add("input-file",1);	   // required input-file name

    //-------------------
    // parse command line
    //-------------------

    bo::parsed_options parsed = 
      bo::command_line_parser(argc,argv).options(desc).positional(p).allow_unregistered().run();

    bo::store(parsed,vmap);
    bo::notify(vmap);

    if(vmap.count("help")  || !vmap.count("input-file") || (argc == 1) ) 
      {
	GRVY_gadd::summarize_usage();
	cout << visible << "\n";
	return;
      }

    if(vmap.count("debug"))
      {
	grvy_log_setlevel(GRVY_DEBUG);
      }

    if(vmap.count("version"))
      {
	grvy_version_stdout();
	return;
      }

    if(vmap.count("quiet"))
      {
	gt->SetOption("output_stdout",false);
	grvy_printf(GRVY_DEBUG,"User requested --quiet option\n");
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
    // minimum required to query some performance data; fire in the hole...

    gt->SummarizeHistTiming(input_file,delimiter,output_dir);

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










