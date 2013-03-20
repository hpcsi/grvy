//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
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

#include<grvy.h>
#include<stdio.h>

void post_log_messages()
{
  grvy_printf(GRVY_FATAL,"This is a fatal message\n");
  grvy_printf(GRVY_ERROR,"This is an error message\n");
  grvy_printf(GRVY_WARN, "This is a warning message\n");
  grvy_printf(GRVY_INFO, "This is an informational message\n");
  grvy_printf(GRVY_DEBUG,"This is a debug message\n");
}

int main()
{

  printf("\nMessages with default setting\n\n");
  post_log_messages();

  /* The above will not include debug messages by default; now
     increasing loglevel to debug mode to get all messages. */

  printf("\nMessages with debug output setting\n\n");

  grvy_log_setlevel(GRVY_DEBUG);
  post_log_messages();

  /* Set priority to warning */

  printf("\nMessages with warn output setting\n\n");
  grvy_log_setlevel(GRVY_WARN);
  post_log_messages();

  /* Provide application specific message delimiters (they will prepend logging output) */

  printf("\nMessages with custom delimiters\n\n");

  grvy_log_setlevel(GRVY_ALL);

  grvy_log_setmask(GRVY_FATAL,"[*] Fatal: ");
  grvy_log_setmask(GRVY_ERROR,"[*] Error: ");
  grvy_log_setmask(GRVY_WARN, "[*]  Warn: ");
  grvy_log_setmask(GRVY_INFO, "[*]  Info: ");
  grvy_log_setmask(GRVY_DEBUG,"[*] Debug: ");

  post_log_messages();

  /* Additional printf style logging example */

  printf("\nC printf-style messages\n\n");

  grvy_printf(GRVY_INFO,"An informational message, the answer is %i.\n",42);

  return 0;
}

