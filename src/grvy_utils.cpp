// -*-c++-*-
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
// grvy_utils: Internal Utility Functions
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

// Notes: these are primarily for internal use by GRVY functions.  Hint: these
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

#include<grvy_classes.h>
#include<grvy_int.h>
#include<grvy.h>

#include"boost/format.hpp"

#define BOOST_FILESYSTEM_VERSION 3
#ifdef HAVE_BOOST_FILESYSTEM_PATH_HPP
#include"boost/filesystem.hpp"
#endif

#include"fortran_string_order.h"

using namespace std;
using boost::format;

namespace GRVY {

  //-------------------------
  // Miscellaneous utilities
  //-------------------------

  extern "C" int grvy_check_file_path(const char *pathname)
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
      {
	free(pathlocal);
	free(dirstring);
	return 0;
      }

    // Deal with the possibility of an absolute path being provided

    bool abs_path = false;

    std::string leading_char("");
    std::string path_to_check;

    if(strncmp(parents,"/",1) == 0)
      {
	leading_char = "/";
	abs_path     = true;
      }

    // Verify existence of top-level directory

    if( (token = strtok(parents,"/")) != NULL )
      {
	path_to_check += leading_char + token;

	if ( _GRVY_CheckDir(path_to_check.c_str()) )
	  {
	    free(pathlocal);
	    free(dirstring);
	    return -1;
	  }

	// Now, search for any remaining parent directories.

	if(abs_path)
	  sprintf(dirstring,"/%s",token);
	else
	  sprintf(dirstring,"%s",token);

	while ( (token = strtok(0,"/")) && (depth < MAX_DEPTH) )
	  {
	    dirstring = strcat(dirstring,"/");

	    if(_GRVY_CheckDir(strcat(dirstring,token)))
	      {
		free(pathlocal);
		free(dirstring);
		return -1;
	      }
	    depth++;
	  };

	if(depth >= MAX_DEPTH )
	  {
	    _GRVY_message(GRVY_ERROR,__func__,"Max directory depth exceeded, limit =",MAX_DEPTH);
	    free(pathlocal);
	    free(dirstring);
	    return -1;
	  }
      }

    // Clean Up
    free(pathlocal);
    free(dirstring);

    return 0;
  }

  int _GRVY_CheckDir(const char *dirname)
  {
    struct stat st;

    if(stat(dirname,&st) != 0)
      {
	if( mkdir(dirname,0700) != 0 )
	  {
	    _GRVY_message(GRVY_ERROR,__func__,"unable to create directory",dirname);
	    return -1;
	  }
      }
    else if (!S_ISDIR(st.st_mode))
      {
	_GRVY_message(GRVY_ERROR,__func__,"entry exists but is not a directory",dirname);
	return -1;
      }

    return 0;
  }

  extern "C" int grvy_create_unique_dir(char *name_template)
  {

    int status = verify_string_ends_with_6_Xs(name_template);
    if (status) return status;

    if (mkdtemp(name_template) == NULL)  /* mkdtemp sets errno */
      return -1;
    else
      return 0;
  }

  // ----------------------------------
  // Recursive directory copy utility
  // ----------------------------------

  extern "C" int grvy_copy_dir(const char *from_dir, const char *to_dir)
  {



#ifdef HAVE_BOOST_FILESYSTEM_PATH_HPP
    boost::filesystem::path source_dir(from_dir);
    boost::filesystem::path dest_dir  (to_dir);

    // verify existence of from_dir

    if(!boost::filesystem::is_directory(source_dir))
      {
	grvy_printf(GRVY_ERROR,"%s: %s is not a valid directory\n",__func__,from_dir);
	return(1);
      }

    // verify sneaky user didn't give us identical from/to paths

    if(strcmp(from_dir,to_dir) == 0)
      {
	grvy_printf(GRVY_ERROR,"%s: from/to dirs are identical (%s)\n",__func__,from_dir);
	return(1);
      }

    // verify destination path is a directory if it exists

    if(boost::filesystem::exists(to_dir) && !boost::filesystem::is_directory(to_dir))
      {
	grvy_printf(GRVY_ERROR,"%s: destination path is not a directory\n",__func__,to_dir);
	return(1);
      }

    // create destination if necessary and verify empty contents

    boost::system::error_code ec;

    if(boost::filesystem::is_directory(dest_dir))
      {
	if(!boost::filesystem::is_empty(dest_dir))
	  {
	    grvy_printf(GRVY_ERROR,"%s: %s target directory exists but is not empty\n",__func__,to_dir);
	    return(1);
	  }
      }
    else
      {
	if(!boost::filesystem::create_directory(to_dir,ec))
	  {
	    grvy_printf(GRVY_ERROR,"%s: %s unable to create target directory\n",__func__,to_dir);
	    return(1);
	  }
      }

    // ks (5/17/12): slight kludge for boost::filesystem; strange runtime errors 
    // can be obtained when LC_ALL is not set to "C"; see example relevant discussion at
    // 
    // https://svn.boost.org/trac/boost/ticket/5928

    char *lc_original;
    char *lc_setting = getenv("LC_ALL");

    if(lc_setting != NULL)
      lc_original = strdup(lc_setting);

    setenv("LC_ALL","C",1);

    // now we have a target dir, copy over contents recursively

    for ( boost::filesystem::recursive_directory_iterator end, dir(from_dir);dir != end; ++dir )
      {

	std::string path_in(dir->path().string());
	boost::filesystem::path target(to_dir);

	target /= path_in.substr(path_in.find(from_dir)+strlen(from_dir),string::npos);
	
	grvy_printf(GRVY_DEBUG,"\n%s: copying %s to %s\n",__func__,dir->path().string().c_str(),
		    target.c_str());

	copy(dir->path(),target,ec);

	if(ec)
	  {
	    grvy_printf(GRVY_ERROR,"%s: unable to complete copy (%s -> %s)\n",__func__,
			dir->path().string().c_str(),target.c_str());
	    grvy_printf(GRVY_ERROR,"%s: %s\n",__func__,ec.message().c_str());
	    return(1);
	  }

	// if we just created a directory, make sure it is at least
	// user writable; this is a reasonably odd corner case which
	// is motivated by the fact that "make distcheck" in autotools
	// runs in a read-only environment.

	if(boost::filesystem::is_directory(target))
	  {
	    grvy_printf(GRVY_DEBUG,"%s: ensuring directory is user-writable (%s)\n",__func__,target.string().c_str());
	
	    struct stat statbuf;
	    if(stat(target.string().c_str(),&statbuf))
	      {
		grvy_printf(GRVY_ERROR,"%s: Unable to stat directory (%s)\n",__func__);
		return(1);
	      }
	    if(chmod(target.string().c_str(), (statbuf.st_mode) | S_IWUSR))
	      {
		grvy_printf(GRVY_ERROR,"%s: Unable to stat directory (%s)\n",__func__,target.string().c_str());
		return(1);
	      }
	  }

	grvy_printf(GRVY_DEBUG,"%s: done with copy\n",__func__);
      }

    // restore original environment

    if(lc_setting != NULL)
      setenv("LC_ALL",lc_original,1);
    else
      unsetenv("LC_ALL");
      
    return(0);

#else
    grvy_printf(GRVY_ERROR,"\nThis GRVY library installation was built with Boost headers-only:\n");
    grvy_printf(GRVY_ERROR,"--> grvy_copy_dir() function is unsupported as a result.\n");
    grvy_printf(GRVY_ERROR,"\nPlease rebuild GRVY with full Boost library linkage in order to "
		"enable this functionality.\n\n");
    return(1);
#endif

  }

  // Utility to provide a unique scratch directory for the user - note
  // that this utility has a very specific requirement on the input
  // string char template to include 6 trailing X's. We check to make
  // sure this is the case as some OS's (eg. OSX) will create the
  // directory without the trailing X's, but it will not have any
  // derived "uniqueness" at runtime.  Linux does not do this, so we
  // check explicitly to hopefully avoid confusion for the user.

#ifdef TLS_TODO /* Use thread local storage for the stack, if possible */
  static TLS std::stack<char *> _grvy_create_scratch_dir_paths;
#else
  static     std::stack<char *> _grvy_create_scratch_dir_paths;
#endif

  extern "C" int grvy_create_scratch_dir(char *name_template)
  {

    int status = verify_string_ends_with_6_Xs(name_template);
    if (status) return status;
    
    status = grvy_create_unique_dir(name_template);
    if (status) return status;
    
    char * name_copy = strdup(name_template);
    if (name_copy == NULL) {
      errno = ENOMEM;
      return -1;
    }
    
    if (_grvy_create_scratch_dir_paths.empty())
      {
	if ( (status = atexit(_GRVY_create_scratch_dir_atexit_handler)) )
	  {
	    free(name_copy);
	    return status;
	  }
      }

    _grvy_create_scratch_dir_paths.push(name_copy);

    return status;
  }

  void _GRVY_create_scratch_dir_atexit_handler()
  {
    while(!_grvy_create_scratch_dir_paths.empty())
    {
      char * path = _grvy_create_scratch_dir_paths.top();
      if (_GRVY_RemoveAll(path)) perror(path);
      free(path);
      _grvy_create_scratch_dir_paths.pop();
    }
  }

  int _GRVY_RemoveAll(const char *path)
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
	status = nftw(path, _GRVY_RemoveAll_nftw_helper, _POSIX_OPEN_MAX,
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

  int _GRVY_RemoveAll_nftw_helper(const char *path,
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

  void _GRVY_message(int LogLevel, const char *func, const char *message)
  {
    static string log_message;

    log_message  = str(format("(%1%): %2% ") % func % message);
    _GRVY_Log.msg(LogLevel,log_message);
    return;
  }

  template <typename T> void _GRVY_message (int LogLevel, const char *func, const char *message, T var)
  {
    static string log_message;

    log_message  = str(format("(%1%): %2% -> %3% ") % func % message % var);
    _GRVY_Log.msg(LogLevel,log_message);
  }

  void _GRVY_message(int LogLevel, const char *func, const char *message, const char *char_item)
  {
    static string log_message;
    
    log_message = str(format("(%1%): %2% -> %3%") % func % message % char_item);
    _GRVY_Log.msg(LogLevel,log_message);
    return;
  }

  //------------------------------
  // Message Templates
  //------------------------------

  template void _GRVY_message (int LogLevel, const char *func, const char *message, int    var);
  template void _GRVY_message (int LogLevel, const char *func, const char *message, float  var);
  template void _GRVY_message (int LogLevel, const char *func, const char *message, double var);

  //-----------------------------------------------------------------
  //                     Fortran Interfaces
  //-----------------------------------------------------------------


  // ----------------------------------------------------------------
  // -------------------- Convenience Functions ---------------------
  // ----------------------------------------------------------------



  // grvy_f2c_char(): Convert evil Fortran character strings to C

  char *grvy_f2c_char(char*input,int len)
  {
    char* output = new char[len+1];
    strncpy(output,input,len);
    output[len]='\0';
    
    return(output);
  }

#if 0

  // next functions deprecated with iso_c implementation 

  char *grvy_f2c_char_no_spaces(char*input,int len)
  {
    char* output = new char[len+1];
    strncpy(output,input,len);
    output[len]='\0';

    output = grvy_trim_whitespace(output);

    return(output);
  }


  // C style whitespace trimmer - mod'd example from Stack Overflow
  // primarily used to trim Fortran provided strings (you said trim...)

  char *grvy_trim_whitespace(char *str)
  {
    char *end;

    // let's punt if we start with a space...

    if(isspace(*str))
      return str;

    // trim trailing space

    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;

    // write new null terminator

    *(end+1) = '\0';

    return(str);
  }

#endif
  
  // verify_string_ends_with_6_Xs(): 
  //
  // Verify that the template satisfies the necessary requirements to
  // end with 6 X's for use with mkdtemp() system function.

  int verify_string_ends_with_6_Xs(char *name_template)
  {
    const int NUM_REQUIRED_TRAILING_X = 6;
    int length_template;
    
    if(name_template == NULL)
      {
	_GRVY_message(GRVY_ERROR,__func__,"Invalid directory template name (null) ");
	return 1;
      }

    length_template = strlen(name_template);
    
    for(int i=length_template-NUM_REQUIRED_TRAILING_X;i<length_template;i++)
      {
	if(name_template[i] != 'X')
	  {
	    _GRVY_message(GRVY_ERROR,__func__,
			  "Invalid directory template (must end with XXXXXX) -",name_template);
	    return 1;
	  }
      }
    
    return 0;
  }

} // end namespace GRVY
