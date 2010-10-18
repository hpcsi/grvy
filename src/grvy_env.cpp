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
// grvy_env.cpp: Utilities for querying runtime environment.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<sys/utsname.h>
#include<grvy.h>
#include<grvy_env.h>
#include<boost/regex.hpp>
#include<iostream> 

GRVY_Hostenv_Class::GRVY_Hostenv_Class()
{
  struct utsname uts;

  uname (&uts);

  hostname   = uts.nodename;
  os_sysname = uts.sysname;
  os_release = uts.release;
  os_version = uts.version;
  
  // Cull domainname from nodename

  boost::regex re("(\\S+)\\.(\\S+)");
  boost::cmatch regex_match;

  //  const std::string Host(uts.nodename);

  if(boost::regex_match(uts.nodename,regex_match,re))
    {
      domainname = regex_match[2];
    }
  else
    grvy_printf(GRVY_WARN,"unable to determine domainname\n");

  cputype = "Unknown";
  
  return;
}

void GRVY_Hostenv_Class::Print()
{
  grvy_printf(GRVY_INFO,"hostname = %s\n",hostname.c_str()    );
  grvy_printf(GRVY_INFO,"sysname  = %s\n",os_sysname.c_str()  );
  grvy_printf(GRVY_INFO,"release  = %s\n",os_release.c_str()  );
  grvy_printf(GRVY_INFO,"version  = %s\n",os_version.c_str()  );
  grvy_printf(GRVY_INFO,"domainname = %s\n",domainname.c_str());
  grvy_printf(GRVY_INFO,"cputype    = %s\n",cputype.c_str()   );

  return;
}



