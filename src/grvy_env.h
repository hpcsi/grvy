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
// grvy_env.h: Header for internal environment class
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#ifndef GRVY_ENV_H
#define GRVY_ENV_H

#include <grvy.h>
#include <string>
#include<vector>

class GRVY_Hostenv_Class 
{
  friend class GRVY::GRVY_Timer_Class;
  
public:
  GRVY_Hostenv_Class();
  void Print();
  std::vector<std::string> Getenv(char *envp[]);
  std::string Hostname();
  
private:
  std::string hostname;
  std::string os_sysname;
  std::string os_release;
  std::string os_version;
  std::string domainname;
  std::string cputype;
};

#endif
