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
// grvy_env.cpp: Utilities for querying runtime environment.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<sys/utsname.h>
#include<grvy.h>
#include<grvy_env.h>
#include<netdb.h>
#include<iostream> 
#include<unistd.h>
#include<cstdio>
#include<cstring>

//#include<boost/regex.hpp>

GRVY_Hostenv_Class::GRVY_Hostenv_Class()
{
  struct utsname uts;
  char *begin;

  uname (&uts);

  hostname   = uts.nodename;
  os_sysname = uts.sysname;
  os_release = uts.release;
  os_version = uts.version;
  //domainname = uts.domainname;

  // Cull domainname from nodename. 

  begin = strstr(uts.nodename,".");
  if(begin != NULL)
    {
      begin++;
      domainname = begin;
    }
  else
    domainname = "unknown";
  
  // 3/25/12 - doing away with Boost regex parsing of hostname in
  // favor of simple C string parsing above. Removes dependency on
  // regex library linkage.
  
#if 0
  boost::regex re("^([\\w\\-]+)\\.(\\S+)$");
  boost::cmatch regex_match;

  if(boost::regex_match(uts.nodename,regex_match,re))
      domainname = regex_match[2];
  else
    {
      // use gethostbyname as a last resort to gleen domainname

      struct hostent *hp = gethostbyname(hostname.c_str());

      if(hp != NULL)
	{
	  std::string myname(hp->h_name); 

	  grvy_printf(GRVY_DEBUG,"%s: hostbyname = %s\n",__func__,myname.c_str());

	  if(boost::regex_match(hp->h_name,regex_match,re))
	    {
	      domainname = regex_match[2];
	    }
	  else
	    {
	      fprintf(stderr,"unable to determine domainname\n");
	    }

	} 
      else
	{
	  //grvy_printf(GRVY_WARN,"unable to run gethostbyname()\n");
	  fprintf(stderr,"unable to run gethostbyname()\n");
	  //herror("foofoo");
	}
    }
#endif

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

std::vector<std::string> GRVY_Hostenv_Class::Getenv()
{

  extern char **environ;

  std::vector<std::string> AllEnv;

  for(int i=0; environ[i] != NULL; i++)
    {
      AllEnv.push_back(environ[i]);
      grvy_printf(GRVY_DEBUG,"User environ: %s\n",environ[i]);
    }

  return(AllEnv);
}

std::string GRVY_Hostenv_Class::Hostname()
{
  return(hostname);
}



