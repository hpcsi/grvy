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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * build_example_file_path(char *filename)
{
  //const char filename[] = "input-example.txt";
  char *retval;
  char *input_dir = getenv("GRVY_INPUT_EXAMPLE_DIR");

  if (input_dir == NULL)
    {
      retval = strdup(filename);
      if (retval == NULL)
        {
          fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__, strerror(ENOMEM));
          fflush(stderr);
          exit(1);
        }
    }
  else
    {
      const int len = strlen(filename) + strlen(input_dir)
                     + 1 /* for path separator */ + 1 /* for null byte */;
      retval = malloc(len*sizeof(char));
      if (retval == NULL)
        {
          fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__, strerror(ENOMEM));
          fflush(stderr);
          exit(1);
        }
      strcpy(retval, input_dir);
      strcat(retval, "/");
      strcat(retval, filename);
    }

  return retval;
}

