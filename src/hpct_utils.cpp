//--------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
// Copyright (C) 2008,2009 The PECOS Development Team
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
// hpct_utils: Internal Utility Functions
//
// $Id: hpct_input.cpp 9 2008-12-14 02:10:50Z karl $
//
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

// Notes: these are primarily for internal use by HPCT functions.  Hint: these
// may not be the external droids you are looking for.


#include<errno.h>
#include<ftw.h>
#include<libgen.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

#include<stack>

#include<hpct_classes.h>
#include<hpct_int.h>

#include"fortran_string_order.h"

using namespace std;

namespace HPCT {

  //-------------------------
  // Miscellaneous utilities
  //-------------------------

  extern "C" int hpct_check_file_path(const char *pathname)
  {
    const int MAX_DEPTH = 50;

    char *pathlocal;
    char *parents;
    char *dirstring;
    char *token;
    int depth = 0;

    // Save a copy of pathname and look for the parent directories.

    pathlocal = strdup(pathname);
    dirstring = strdup(pathname);
    parents   = dirname(pathlocal);

    if(strcmp(parents,".") == 0)
      return(1);

    if( (token = strtok(parents,"/")) != NULL )
      {
	if ( !_HPCT_CheckDir(token) )
	  return(-1);

	// Now, search for any remaining parent directories.

	sprintf(dirstring,"%s",token);

	while ( (token = strtok(0,"/")) && (depth < MAX_DEPTH) )
	  {
	    dirstring = strcat(dirstring,"/");
	    if(!_HPCT_CheckDir(strcat(dirstring,token)))
	      return(-1);
	    depth++;
	  };

	if(depth >= MAX_DEPTH )
	  {
	    _HPCT_message(_HPCT_emask,__func__,"Max directory depth exceeded, limit =",MAX_DEPTH);
	    return(-1);
	  }

      }

    // Clean Up

    free(pathlocal);
    free(dirstring);

    return(1);
  }

  int _HPCT_CheckDir(const char *dirname)
  {
    struct stat st;
    int status = 0;

    if(stat(dirname,&st) != 0)
      {
	if( mkdir(dirname,0700) != 0 )
	  {
	    printf("** Error: unable to create local directory %s\n",dirname);
	    return(-1);
	  }
      }

    return(1);
  }

  extern "C" int hpct_create_unique_dir(char *name_template)
  {
    if (name_template == NULL)
      {
	errno = EINVAL;
	return -1;
      }
    else if (mkdtemp(name_template) == NULL)  /* mkdtemp sets errno */
      return -1;
    else
      return 0;
  }

#ifdef TLS /* Use thread local storage for the stack, if possible */
  static TLS std::stack<char *> _hpct_create_scratch_dir_paths;
#else
  static     std::stack<char *> _hpct_create_scratch_dir_paths;
#endif

  extern "C" int hpct_create_scratch_dir(char *name_template)
  {
    int status = hpct_create_unique_dir(name_template);
    if (status) return status;

    char * name_copy = strdup(name_template);
    if (name_copy == NULL) {
      errno = ENOMEM;
      return -1;
    }

    if (_hpct_create_scratch_dir_paths.empty())
      {
	if (status = atexit(_HPCT_create_scratch_dir_atexit_handler))
	  {
	    free(name_copy);
	    return status;
	  }
      }

    _hpct_create_scratch_dir_paths.push(name_copy);

    return status;
  }

  void _HPCT_create_scratch_dir_atexit_handler()
  {
    while(!_hpct_create_scratch_dir_paths.empty())
    {
      char * path = _hpct_create_scratch_dir_paths.top();
      if (_HPCT_RemoveAll(path)) perror(path);
      free(path);
      _hpct_create_scratch_dir_paths.pop();
    }
  }

  int _HPCT_RemoveAll(const char *path)
  {
    struct stat st;
    int status;

    if (0 != lstat(path,&st) )
      {
	/* Succeed silently on "removal" of non-existent path */
	status = (errno == ENOENT) ? 0 : -1; /* errno set by lstat */
      }
    else if (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode))
      {
	status = unlink(path);               /* errno set by unlink */
      }
    else if (S_ISDIR(st.st_mode))
      {
	status = nftw(path, _HPCT_RemoveAll_nftw_helper, _POSIX_OPEN_MAX,
	              FTW_DEPTH | FTW_PHYS); /* errno set by nftw */
      }
    else
      {
	/* unable to handle st.st_mode */
        status = -1;
	errno = EINVAL;
      }

    return status;
  }

  int _HPCT_RemoveAll_nftw_helper(const char *path,
                                  const struct stat * st,
				  int flag,
				  struct FTW *f)
  {
    int status;

    switch (flag) {
    case FTW_F:   /* File */
    case FTW_SL:  /* Symlink */
    case FTW_SLN: /* Symlink (dangling) */
      status = unlink(path);                 /* errno set by unlink */
      if (status) perror(path);
      break;
    case FTW_D:   /* Directory */
    case FTW_DP:  /* Directory that used to have subdirectories */
      status = rmdir(path);                  /* errno set by rmdir */
      if (status) perror(path);
      break;
    case FTW_DNR: /* Unreadable directory */
    case FTW_NS:  /* Permissions failed */
    default:      /* Unknown flag from nftw */
      /* Presumably related errors will arise on later rmdir attempts */
      status = 0;
    }

    return status;
  }


  //------------------------------------
  // Basic stdout warning/error messages
  //------------------------------------

  void _HPCT_message(char *message)
  {
    printf("\n%s\n",message);
    return;
  }

  void _HPCT_message(const char *mask, const char *func, const char *message)
  {
    printf("\n%s (%s): %s\n",mask,func,message);
    return;
  }

  void _HPCT_message(const char *mask, const char *func, const char *message, const char *char_item)
  {
    printf("\n%s (%s): %s -> %s \n",mask,func,message,char_item);
    return;
  }

  void _HPCT_message(const char *mask, const char *func, const char *message, int int_item)
  {
    printf("\n%s (%s): %s -> %i \n",mask,func,message,int_item);
    return;
  }

  void _HPCT_message(const char *mask, const char *func, const char *message, float float_item)
  {
    printf("\n%s (%s): %s -> %f \n",mask,func,message,float_item);
    return;
  }

  void _HPCT_message(const char *mask, const char *func, const char *message, double double_item)
  {
    printf("\n%s (%s): %s -> %e \n\n",mask,func,message,double_item);
    return;
  }

  //-----------------------------------------------------------------
  //                     Fortran Interfaces
  //-----------------------------------------------------------------

  extern "C" void hpct_check_file_path_(char *pathname,int _namelen)
  {
    char *name = hpct_f2c_char(pathname,_namelen);

    hpct_check_file_path(name);

    delete[] name;
    return;
  }

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_create_unique_dir_(char *name_template,int *flag,int _namelen)
#else
extern "C" void hpct_create_unique_dir_(char *name_template,int _namelen,int *flag)
#endif
  {
    char *c_name_template = hpct_f2c_char(name_template,_namelen);

    *flag = hpct_create_unique_dir(c_name_template);
    strncpy(name_template,c_name_template,_namelen);

    delete[] c_name_template;
  }

#ifdef _HPCT_FORTRAN_STRING_ORDER1
extern "C" void hpct_create_scratch_dir_(char *name_template,int *flag,int _namelen)
#else
extern "C" void hpct_create_scratch_dir_(char *name_template,int _namelen,int *flag)
#endif
  {
    char *c_name_template = hpct_f2c_char(name_template,_namelen);

    *flag = hpct_create_scratch_dir(c_name_template);
    strncpy(name_template,c_name_template,_namelen);

    delete[] c_name_template;
  }

  // ----------------------------------------------------------------
  // -------------------- Convenience Functions ---------------------
  // ----------------------------------------------------------------

  // hpct_f2c_char(): Convert evil Fortran character strings to C

  char *hpct_f2c_char(char*input,int len)
  {
    char* output = new char[len+1];
    strncpy(output,input,len);
    output[len]='\0';

    return(output);
  }


}
