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
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <grvy.h>

int main(int argc, char **argv)
{

  int flag = 0;
  int    igot;
  float  fgot;
  double dgot;
  char  *sgot;

  grvy_log_setlevel(GRVY_NOLOG);

  /* Verify error on unknown variable query */

  flag = grvy_input_register_get_int   ("sec1/aint",&igot);     if(flag != 0 ) exit(1);
  flag = grvy_input_register_get_float ("sec1/afloat",&fgot);   if(flag != 0 ) exit(1);
  flag = grvy_input_register_get_double("sec1/adouble",&dgot);  if(flag != 0 ) exit(1);
  flag = grvy_input_register_get_char  ("sec1/astring",&sgot);  if(flag != 0 ) exit(1);

  return 0;
}
