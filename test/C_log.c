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
// Regression/Test Suite for libGRVY.
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <grvy.h>

const char *test_name = "C logging: unit tests";

void post_log_messages();

int main(int argc, char **argv)
{

  int flag = 1;

  printf("\nPriority = ALL\n");
  grvy_log_setlevel(GRVY_ALL);
  post_log_messages();

  printf("\nPriority = DEBUG\n");
  grvy_log_setlevel(GRVY_DEBUG);
  post_log_messages();

  printf("\nPriority = INFO\n");
  grvy_log_setlevel(GRVY_INFO);
  post_log_messages();

  printf("\nPriority = WARN\n");
  grvy_log_setlevel(GRVY_WARN);
  post_log_messages();

  printf("\nPriority = ERROR\n");
  grvy_log_setlevel(GRVY_ERROR);
  post_log_messages();

  printf("\nPriority = FATAL\n");
  grvy_log_setlevel(GRVY_FATAL);
  post_log_messages();

  printf("\nPriority = NOLOG\n");
  grvy_log_setlevel(GRVY_NOLOG);
  post_log_messages();

  // Repeat with non-default logmask

  grvy_log_setmask(GRVY_FATAL,"[*] Fatal: ");
  grvy_log_setmask(GRVY_ERROR,"[*] Error: ");
  grvy_log_setmask(GRVY_WARN, "[*]  Warn: ");
  grvy_log_setmask(GRVY_INFO, "[*]  Info: ");
  grvy_log_setmask(GRVY_DEBUG,"[*] Debug: ");

  printf("\nPriority = ALL\n");
  grvy_log_setlevel(GRVY_ALL);
  post_log_messages();

  printf("\nPriority = DEBUG\n");
  grvy_log_setlevel(GRVY_DEBUG);
  post_log_messages();

  printf("\nPriority = INFO\n");
  grvy_log_setlevel(GRVY_INFO);
  post_log_messages();

  printf("\nPriority = WARN\n");
  grvy_log_setlevel(GRVY_WARN);
  post_log_messages();

  printf("\nPriority = ERROR\n");
  grvy_log_setlevel(GRVY_ERROR);
  post_log_messages();

  printf("\nPriority = FATAL\n");
  grvy_log_setlevel(GRVY_FATAL);
  post_log_messages();

  printf("\nPriority = NOLOG\n");
  grvy_log_setlevel(GRVY_NOLOG);
  post_log_messages();

  printf("\n[End of Test]\n");

  if(flag == 0)
      exit (1);
  else
    exit (0);
}

void post_log_messages()
{
  grvy_printf(GRVY_FATAL,"a fine fatal message\n"    );
  grvy_printf(GRVY_ERROR,"an astute error message\n" );
  grvy_printf(GRVY_WARN, "a fabulous warn message\n" );
  grvy_printf(GRVY_INFO, "a useful info message\n"   );
  grvy_printf(GRVY_DEBUG,"a clever debug message\n"  );

  return;
}


