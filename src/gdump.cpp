// -*-c++-*-
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
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
// gdump.cpp: Utility for querying GRVY generated performance history data.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<iostream>
#include<vector>

#define GETPOT_NAMESPACE GRVYGetPot
#define GETPOT_DISABLE_MUTEX
#include<getpot.h>

#include<grvy_classes.h>
#include<grvy.h>
#include<grvy_int.h>

using namespace std;

namespace GRVY_gdump
{
  void summarize_usage()
  {
    grvy_printf(GRVY_INFO,"\nUsage: %s [OPTIONS] file\n\n","gdump");
    grvy_printf(GRVY_INFO,"where \"file\" is the path to a libGRVY style historical performance database.\n\n");
    grvy_printf(GRVY_INFO,"OPTIONS:\n");

    grvy_printf(GRVY_INFO,"  --help                     generate help message and exit\n");
    grvy_printf(GRVY_INFO,"  --version                  output version information and exit\n");
    grvy_printf(GRVY_INFO,"  -q [ --quiet ]             suppress normal stdout messages\n");
    grvy_printf(GRVY_INFO,"  -a [ --enable-subtimers ]  include all individual subtimer(s) in output\n");
    grvy_printf(GRVY_INFO,"  -s [ --summarize-only ]    summarize timer statistics but do not dump files\n");
    grvy_printf(GRVY_INFO,"  -c [ --with-comment ]      include user comment string in output\n");
    grvy_printf(GRVY_INFO,"  -e [ --with-env ]          include runtime environment variables in output\n");
    grvy_printf(GRVY_INFO,"  -d [ --delimiter ]  arg    override default comment delimiter (default=#)\n");
    grvy_printf(GRVY_INFO,"  -o [ --output-dir ] arg    override default dump directory (default=./gdata)\n");

    grvy_printf(GRVY_INFO,"\n");
    return;
  }

  void parse_supported_options(int argc, char *argv[],  GRVY::GRVY_Timer_Class *gt)
  {

    string input_file;
    string delimiter ("#");
    string output_dir("./gdata");

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
	GRVY_gdump::summarize_usage();
	return;
      }

    // store inputs (variables with no leading minus sign); because
    // some of our command-line options have arguments, this vector
    // can be longer than one. We will prune values from this vector
    // as we parse known options; you're probably thinking to yourself
    // that Boost program options handles this a little nicer - you're
    // probably right. But GetPot is header only, and isn't that
    // much more work, so we're switching to minimize the number of
    // external library linkages.

    vector<string> inputs = cl.nominus_vector();

    if(cl.search(2,"--quiet","-q"))
      {
	gt->SetOption("output_stdout",false);
	grvy_printf(GRVY_DEBUG,"User requested --quiet option\n");
      }

    if(cl.search("--debug"))
      {
	grvy_log_setlevel(GRVY_DEBUG);
      }

    if(cl.search(2,"--summarize-only","-s"))
      {
	gt->SetOption("dump_files",false);
	grvy_printf(GRVY_DEBUG,"User requested --sumarize-only option\n");
      }

    if(cl.search(2,"--with-comment","-c"))
      {
	gt->SetOption("output_comments",true);
	grvy_printf(GRVY_DEBUG,"User requested --with-comment option\n");
      }

    if(cl.search(2,"--with-env","-e"))
      {
	gt->SetOption("output_printenv",true);
	grvy_printf(GRVY_DEBUG,"User requested --with-env option\n");
      }

    if(cl.search(2,"--enable-subtimers","-a"))
      {
	gt->SetOption("output_subtimer_raw",  true);
	gt->SetOption("output_totaltimer_raw",true);
	grvy_printf(GRVY_DEBUG,"User requested --enable-subtimer option\n");
      }

    if(cl.search(2,"--delimiter","-d"))
      {
	delimiter = cl.next(delimiter);
	grvy_printf(GRVY_DEBUG,"User requested --delimiter option with %s\n",delimiter.c_str());

	for(size_t i=0;i<inputs.size();i++) // pop this arg from remaining inputs
	  if(inputs[i] == delimiter)
	    {
	      grvy_printf(GRVY_DEBUG,"Erasing delimiter = %s\n",delimiter.c_str());
	      inputs.erase(inputs.begin()+i);
	    }
      }

    if(cl.search(2,"--output-dir","-o"))
      {
	output_dir = cl.next(output_dir);
	grvy_printf(GRVY_DEBUG,"User requested --output-dir option with %s\n",output_dir.c_str());

	for(size_t i=0;i<inputs.size();i++) // pop this arg from remaining inputs
	  if(inputs[i] == output_dir)
	    {
	      grvy_printf(GRVY_DEBUG,"Erasing output_dir = %s\n",output_dir.c_str());
	      inputs.erase(inputs.begin()+i);
	    }
      }

    // error on unknown options....

    vector<string> ufos = cl.unidentified_options();

    if(ufos.size() > 0)
      {
	grvy_printf(GRVY_ERROR,"\n");
	for(size_t i = 0;i<ufos.size();i++)
	  grvy_printf(GRVY_ERROR,"%s: Unsupported command-line argument detected (%s)\n","[*] Error",
		    ufos[i].c_str());
	GRVY_gdump::summarize_usage();
	exit(1);
      }

    // Required arguments

    if(inputs.size() != 1)
      {
	GRVY_gdump::summarize_usage();
	exit(1);
      } 
    else
      input_file = inputs[0];



    // If we have the pleasure of making it this far, then we have the
    // minimum required to query some performance data; fire in the hole...

    gt->SummarizeHistTiming(input_file,delimiter,output_dir);

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










