#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <string.h>

char * build_example_file_path()
{
  const char filename[] = "input-example.txt";
  char *retval;
  char *input_dir = getenv("HPCT_INPUT_EXAMPLE_DIR");

  if (input_dir == NULL)
    {
      retval = strdup(filename);
      if (retval == NULL)
        {
          error_at_line(1, ENOMEM, __FILE__, __LINE__, "A");
        }
    }
  else
    {
      const int len = strlen(filename) + strlen(input_dir)
                     + 1 /* for path separator */ + 1 /* for null byte */;
      retval = malloc(len*sizeof(char));
      if (retval == NULL)
        {
          error_at_line(1, ENOMEM, __FILE__, __LINE__, "A");
        }
      strcpy(retval, input_dir);
      strcat(retval, "/");
      strcat(retval, filename);
    }

  return retval;
}
