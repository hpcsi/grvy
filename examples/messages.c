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
// messages.c: Example illustrating log message generation via libGRVY.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<grvy.h>
#include<stdio.h>

void post_log_messages()
{
  grvy_log(GRVY_FATAL,"This is a fatal message");
  grvy_log(GRVY_ERROR,"This is an error message");
  grvy_log(GRVY_WARN, "This is a warning message");
  grvy_log(GRVY_INFO, "This is an informational message");
  grvy_log(GRVY_DEBUG,"This is a debug message");
}

int main()
{

  printf("\nMessages with default setting\n\n");
  post_log_messages();

  /* above will not include debug messages by default; now increasing
   * to debug mode to get all messages. */

  printf("\nMessages with debug output setting\n\n");

  grvy_log_setlevel(GRVY_DEBUG);
  post_log_messages();

  /* above will not include debug messages by default; now increasing
   * to debug mode to get all messages. */

  printf("\nMessages with warn output setting\n\n");
  grvy_log_setlevel(GRVY_WARN);
  post_log_messages();

  // Provide application specific message delimiters

  printf("\nMessages with custom delimiters\n\n");

  grvy_log_setlevel(GRVY_ALL);

  grvy_log_setmask(GRVY_FATAL,"[*] Fatal: ");
  grvy_log_setmask(GRVY_ERROR,"[*] Error: ");
  grvy_log_setmask(GRVY_WARN, "[*]  Warn: ");
  grvy_log_setmask(GRVY_INFO, "[*]  Info: ");
  grvy_log_setmask(GRVY_DEBUG,"[*] Debug: ");

  post_log_messages();

  // Exmample of printf style messages 

  printf("\nC printf-style messages\n\n");

  grvy_printf(GRVY_INFO,"An informational message, the answer is %i\n",42);

  return 0;
}

