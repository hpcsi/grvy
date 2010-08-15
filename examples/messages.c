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

int main()
{
  char *mask = "mymodel";	/* mask to include in message output */
  int std  = 0;			/* standard message (always include) */
  int info = 1;			/* info message                      */
  int dbg  = 2;			/* debug message                     */

  printf("\nMessages with default setting\n");

  grvy_log(GRVY_FATAL,"This is a fatal message from mymodel");
  grvy_log(GRVY_ERROR,"This is an error message from mymodel");
  grvy_log(GRVY_WARN, "This is an warning message from mymodel");
  grvy_log(GRVY_INFO, "This is an informational message from mymodel");
  grvy_log(GRVY_DEBUG,"This is a debug message from mymodel");

  /* above will not include debug messages by default; now increasing
   * to debug mode to get all messages. */

  printf("\nMessages with debug output setting\n");

  grvy_log_setlevel(GRVY_DEBUG);

  grvy_log(GRVY_FATAL,"This is a fatal message from mymodel");
  grvy_log(GRVY_ERROR,"This is an error message from mymodel");
  grvy_log(GRVY_WARN, "This is an warning message from mymodel");
  grvy_log(GRVY_INFO, "This is an informational message from mymodel");
  grvy_log(GRVY_DEBUG,"This is a debug message from mymodel");


  /* above will not include debug messages by default; now increasing
   * to debug mode to get all messages. */

  printf("\nMessages with warn output setting\n");

  grvy_log_setlevel(GRVY_WARN);

  grvy_log(GRVY_FATAL,"This is a fatal message from mymodel");
  grvy_log(GRVY_ERROR,"This is an error message from mymodel");
  grvy_log(GRVY_WARN, "This is an warning message from mymodel");
  grvy_log(GRVY_INFO, "This is an informational message from mymodel");
  grvy_log(GRVY_DEBUG,"This is a debug message from mymodel");

  /* Exmample of printf style messages */

  grvy_log_setlevel(GRVY_INFO);

  printf("\nprintf style messages\n");

  grvy_printf(GRVY_INFO,"An informational message, result = %i\n",42);

  return 0;
}

