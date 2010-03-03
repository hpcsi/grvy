//--------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
// Copyright (C) 2008,2009,2010 The PECOS Development Team
//
// Please see http://pecos.ices.utexas.edu for more information.
//
// This file is part of the PECOS HPC Toolkit (HPCT)
//
// HPCT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// HPCT is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HPCT.  If not, see <http://www.gnu.org/licenses/>.
//
// -------------------------------------------------------------------------
//
// HPCT Test Suite
//
// $Id$
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "hpct.h"

const char *test_name = "C logging: unit tests";

void post_log_messages();

int main(int argc, char **argv)
{
  int flag = 1;

  printf("\nPriority = ALL\n");
  hpct_log_setlevel(HPCT_ALL);
  post_log_messages();

  printf("\nPriority = DEBUG\n");
  hpct_log_setlevel(HPCT_DEBUG);
  post_log_messages();

  printf("\nPriority = INFO\n");
  hpct_log_setlevel(HPCT_INFO);
  post_log_messages();

  printf("\nPriority = WARN\n");
  hpct_log_setlevel(HPCT_WARN);
  post_log_messages();

  printf("\nPriority = ERROR\n");
  hpct_log_setlevel(HPCT_ERROR);
  post_log_messages();

  printf("\nPriority = FATAL\n");
  hpct_log_setlevel(HPCT_FATAL);
  post_log_messages();

  printf("\nPriority = NOLOG\n");
  hpct_log_setlevel(HPCT_NOLOG);
  post_log_messages();

  printf("\n[End of Test]\n");

  if(flag == 0)
      exit (1);
  else
    exit (0);
}

void post_log_messages()
{
  hpct_log(HPCT_FATAL,"a fine fatal message"    );
  hpct_log(HPCT_ERROR,"an astute error message" );
  hpct_log(HPCT_WARN, "a fabulous warn message");
  hpct_log(HPCT_INFO, "a useful info message"  );
  hpct_log(HPCT_DEBUG,"a clever debug message"  );

  return;
}


