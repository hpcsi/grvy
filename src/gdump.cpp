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
// gdump.cpp: Utility for querying GRVY generated performance history data.
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

namespace GRVY_gdump
{
  void summarize_usage()
  {
    //grvy_printf(GRVY_INFO,"\ngdump: a utility for querying historical performance data\n\n");
    grvy_printf(GRVY_INFO,"\nUsage: %s [OPTIONS] file\n\n","gdump");
    grvy_printf(GRVY_INFO,"where \"file\" is the path to a libGRVY style historical performance database.\n\n");
    grvy_printf(GRVY_INFO,"OPTIONS:\n");
    return;
  }

  void parse_supported_options(int argc, char *argv[],  GRVY::GRVY_Timer_Class *gt)
  {

    // define supported options

    string input_file;
    string output_dir;
      //    string output_dir("./gdump_data");

    bo::options_description visible;
    bo::options_description hidden;
    bo::options_description desc;
    bo::variables_map vmap;
    bo::positional_options_description p;

    visible.add_options()
      ("help",              "generate help message and exit")
      ("version",           "output libGRVY version information and exit")
      ("enable-global,G",   "include global timers per host in output")
      ("enable-subtimers,A","include all individual subtimer(s) in output "
       "(automatically enables -G)")
      ("output-dir,O",bo::value<string>()->implicit_value("./gdump_data"),"enable raw data dump to files in "
      "output directory");
      ;

    hidden.add_options()
      ("input-file",bo::value< string >(),"input file")
      ("debug","verbose debugging output")
      ;

    desc.add(hidden).add(visible); // combined option set
    p.add("input-file",1);	   // required input-file name

    // parse command line

    bo::parsed_options parsed = 
      bo::command_line_parser(argc,argv).options(desc).positional(p).allow_unregistered().run();

    bo::store(parsed,vmap);
    bo::notify(vmap);

    if(vmap.count("help")  || !vmap.count("input-file") || (argc == 1) ) 
      {
	GRVY_gdump::summarize_usage();
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

    if(vmap.count("enable-global"))
      {
	gt->SetOption("output_totaltimer_raw",true);
      }

    if(vmap.count("enable-subtimers"))
      {
	gt->SetOption("output_subtimer_raw",  true);
	gt->SetOption("output_totaltimer_raw",true);
      }

    if(vmap.count("output-dir"))
      {
	output_dir = vmap["output-dir"].as<string>();
	grvy_printf(GRVY_DEBUG,"User provided output dir = %s\n",output_dir.c_str());
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

	GRVY_gdump::summarize_usage();
	cout << visible << "\n";
	exit(1);
      }

    // If we have the pleasure of making it this far, then we have the
    // minimum required to query some performance data; fire in the hole...

    gt->SummarizeHistTiming(input_file,output_dir);

    return;
}

} // end namespace GRVY_gdump

//---------------------------------------------------------------------------
// Main
//---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
  GRVY::GRVY_Timer_Class gt;  // a GRVY performance timer

  GRVY_gdump::parse_supported_options(argc,argv,&gt);

  return 0;
}










