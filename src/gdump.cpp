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
#include <boost/program_options.hpp>
#include "grvy.h"

namespace GRVY_gdump
{
  void summarize_usage()
  {
    grvy_printf(GRVY_INFO,"\ngdump: a utility for querying historical performance history data\n\n");
    grvy_printf(GRVY_INFO,"Usage: %s [OPTIONS] file\n\n","gdump");
    grvy_printf(GRVY_INFO,"where \"file\" is the path to a libGRVY style performance database.\n\n");
    grvy_printf(GRVY_INFO,"OPTIONS:\n");
  }

}

int main(int argc, char* argv[])
{

  using namespace std;
  namespace bo = boost::program_options;

  bo::options_description desc;

  desc.add_options()
    ("help","generate help message and exit")
    ("version", "output libGRVY version information and exit")
    ("summarize,s","summarize available performance data per host (default)")
    //    ("summarize,s","summarize available performance data per host (default)")
    ;

  bo::variables_map vmap;
  bo::store(bo::parse_command_line(argc,argv,desc),vmap);
  bo::notify(vmap);

  if(vmap.count("help")) 
    {
      GRVY_gdump::summarize_usage();
      cout << desc << "\n";
      return 0;
    }

  if(vmap.count("version"))
    {
      grvy_version_stdout();
      return 0;
    }
    
  return 0;
}








