//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011 The PECOS Development Team
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
// messages.c: Example illustrating log message generation via libGRVY.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<grvy_classes.h>
#include<grvy.h>
#include<grvy_int.h>
#include<iostream>

using namespace std;
using namespace GRVY;

GRVY_Log_Class logger2;

#if 0
class myclass {
public:
  myclass();
  template <typename T> inline myclass operator <<(const T &val) {std::cout << val << std::endl;};
};


myclass::myclass()
{
  cout << "inside ctor" << endl;
}

#endif

myclass logger;

void print_messages();

int main()
{

  cout << endl << "Messages with default setting" << endl;

  print_messages();

#if 0
  /* above will not include debug messages by default; now increasing
   * to debug mode to get all messages. */

  cout << endl << "Messages with debug output setting" << endl;

  grvy_log_setlevel(GRVY_DEBUG);
  print_messages();

  cout << endl << "Messages with warn output setting" << endl;

  grvy_log_setlevel(GRVY_WARN);
  print_messages();
#endif

  return 0;
}

void print_messages()
{
  //  cout << "top of messages" << endl;
  int i = 3;

  //  logger.msg_priority(GRVY_INFO);
  //  logger.msg(GRVY_INFO,"koomie foo");


  logger << "koomie foo2";
  logger << "koomie foo3";

  logger2 << "ok";

  //  int i = 3;

  //  _GRVY_Log.msg_priority(GRVY_INFO);
  //  cout << "testing i";
  //  logger << 123;
  //logger << endl;
  //  _GRVY_Log << &i;
  //  cout << endl;
  //  _GRVY_Log.msg(GRVY_INFO,"koomie foo");
  //  _GRVY_Log << "koomie foo";
  //GRVY_LOG(GRVY_FATAL) << "This is a fatal message from mymodel"          << endl;
  //   GRVY_LOG(GRVY_ERROR) << "This is an error message from mymodel"         << endl;
  //   GRVY_LOG(GRVY_WARN)  << "This is an warning message from mymodel"       << endl;
  //  GRVY_LOG(GRVY_INFO)  << "This is an informational message from mymodel" << endl;
  //   GRVY_LOG(GRVY_DEBUG) << "This is a debug message from mymodel"          << endl;
}
