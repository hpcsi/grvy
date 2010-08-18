//--------------------------------------------------------------------------
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
//--------------------------------------------------------------------------
//
// grvy_env.cpp: Utilities for querying runtime environment.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<sys/utsname.h>
#include<grvy.h>
//#include<stdio.h>
//#include<grvy_int.h>
#include<grvy_env.h>
#include<boost/regex.hpp>
#include<iostream> 

GRVY_Hostenv_Class::GRVY_Hostenv_Class()
{
  struct utsname uts;

  uname (&uts);
  
  grvy_printf(GRVY_INFO,"nodename = %s\n",uts.nodename);
  grvy_printf(GRVY_INFO,"sysname  = %s\n",uts.sysname);
  grvy_printf(GRVY_INFO,"release  = %s\n",uts.release);
  grvy_printf(GRVY_INFO,"version  = %s\n",uts.version);

  // Cull domainname from nodename

  boost::regex re("(\\S+)\\.(\\S+)");
  boost::cmatch regex_match;

  //  const std::string Host(uts.nodename);

  if(boost::regex_match(uts.nodename,regex_match,re))
    {
      domainname = regex_match[2];
      grvy_printf(GRVY_INFO,"domainname  = %s\n",domainname.c_str());
    }
  else
    grvy_printf(GRVY_WARN,"unable to determine domainname\n");
  
  return;
}



