//--------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
// Copyright (C) 2008,2009,2010 The PECOS Development Team
//
// Please see http://pecos.ices.utexas.edu for more information.
//
// This file is part of the PECOS GRVY Toolkit
//
// GRVY is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GRVY is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GRVY.  If not, see <http://www.gnu.org/licenses/>.
//
// -------------------------------------------------------------------------
//
// grvy.h: Basic API Definitions
//
// $Id$
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#ifndef GRVY_H_
#define GRVY_H_

#ifdef __cplusplus
extern "C" {
#endif

  /*! \page apiC C/C++ Library Interface
   *
   * This section outlines the available GRVY functions for C/C++.  To
   * use, make sure to \#include <grvy.h> within your source code
   * files. Several simple examples using these functions are provided
   * in the examples section.  Functions which have an integer return
   * value return "0" upon success. To illustrate the organizatinal
   * flexibility of the input file parsing, a
   * \ref inputFile "sample input file" is also provided.
   * <hr>
   */

  /*
   * Basic API Definition for ASCII input file parsing
   */

  /* C Interface */

  extern int grvy_input_fopen               (const char *filename);
  extern int grvy_input_fclose              ();

  extern int grvy_input_fread_char          (const char* var, char **value);
  extern int grvy_input_fread_char_ivec     (const char* var, char **value,  int elem);

  extern int grvy_input_fread_int           (const char* var, int *value);
  extern int grvy_input_fread_int_vec       (const char* var, int *value,    int nelems);
  extern int grvy_input_fread_int_ivec      (const char* var, int *value,    int elem);

  extern int grvy_input_fread_float         (const char* var, float *value);
  extern int grvy_input_fread_float_vec     (const char* var, float *value,  int nelems);
  extern int grvy_input_fread_float_ivec    (const char *var, float *value,  int elem);

  extern int grvy_input_fread_double        (const char* var, double *value);
  extern int grvy_input_fread_double_vec    (const char* var, double *value, int nelems);
  extern int grvy_input_fread_double_ivec   (const char* var, double *value, int elem);

  extern int grvy_input_fdump               ();
  extern int grvy_input_fdump_delim         (const char* prefix);
  extern int grvy_input_fdump_file          (const char* prefix, const char *filename);

  /* \subpage input_register Default Value Registration Functions */

  /* Input variable registration functions (to provide default input
     values and backwards compatibility support) */

  extern int grvy_input_register_int        (const char *var,     int value);
  extern int grvy_input_register_float      (const char *var,   float value);
  extern int grvy_input_register_double     (const char *var,  double value);
  extern int grvy_input_register_char       (const char *var,   char *value);

  extern int grvy_input_register_get_int    (const char *var,    int *value);
  extern int grvy_input_register_get_float  (const char *var,  float *value);
  extern int grvy_input_register_get_double (const char *var, double *value);
  extern int grvy_input_register_get_char   (const char *var,  char **value);

  /* Error Message Output Control */

  extern void grvy_input_toggle_messages(int flag);

  /*
   * Basic API Definition for Performance Timers
   */

  extern double grvy_timer                ();
  extern void   grvy_timer_init           (const char *id);
  extern void   grvy_timer_reset          ();
  extern void   grvy_timer_finalize       ();
  extern void   grvy_timer_begin          (const char *id);
  extern void   grvy_timer_end            (const char *id);
  extern double grvy_timer_elapsedseconds (const char *id);
  extern double grvy_timer_elapsed_global ();
  extern void   grvy_timer_summarize      ();

  /*
   * Basic API Definition for Math Utilities
   */

  extern int grvy_double_isnan            (double *val);
  extern int grvy_double_isinf            (double *val);

  extern int grvy_float_isnan             (float  *val);
  extern int grvy_float_isinf             (float  *val);

  /*
   * Logging Utilities
   */

  /* Available logging priorities; roughly mimics syslog priority levels but with
   * fewer options. Also provides a NOLOG option to disable all log messages. 
   */
  
  typedef enum {GRVY_NOLOG  =  -1,
		GRVY_FATAL  =   0,
		GRVY_ERROR  = 100,
		GRVY_WARN   = 200,
		GRVY_INFO   = 300,
		GRVY_DEBUG  = 400,
		GRVY_ALL    = 500
  } PriorityLevel;

  extern void grvy_log_setlevel           (int priority);
  extern void grvy_log                    (int loglevel, const char *mesg);
  extern void grvy_log_int                (int loglevel, const char *func, const char *mesg, int var);
  extern void grvy_log_float              (int loglevel, const char *func, const char *mesg, float var);
  extern void grvy_log_double             (int loglevel, const char *func, const char *mesg, double var);
  extern void grvy_log_char               (int loglevel, const char *func, const char *mesg, const char *char_item);

  /*
   * Miscellaneous Utilities
   */

  extern int    grvy_check_file_path      (const char *pathname);
  extern int    grvy_create_unique_dir    (      char *name_template);
  extern int    grvy_create_scratch_dir   (      char *name_template);

  /*
   * Doxygen API Definitions
   */

  /*! \page apiC
   * \section input Input Parsing Routines
   *
   * <b>Open/close a libGRVY style input file:</b>
   * \code
   * int grvy_input_fopen               (const char *filename);
   * int grvy_input_fclose              ();
   * \endcode
   *
   * <b> Dump the contents of a libGRVY style input file :</b>
   * 
   * The following routines can be used to dump the entire contents of
   * a parsed libGRVY style input file to \e stdout or to an ASCII
   * file (note that all comments within the original input file are
   * stripped). The prefix variable can be used to include an
   * additional delimiter which prepends each line of the output.
   * These routines are normally used to save all relevant input
   * parameters in the application output for future traceability and
   * repeatability.
   *
   * \code
   * int grvy_input_fdump               ();
   * int grvy_input_fdump_delim         (const char* prefix);
   * int grvy_input_fdump_file          (const char* prefix, const char *filename);
   * \endcode
   *
   * <b>Read a single int, float, double, or char string for input keyword \e var:</b>
   * \code
   * int grvy_input_fread_int           (const char* var, int    *value);
   * int grvy_input_fread_float         (const char* var, float  *value);
   * int grvy_input_fread_double        (const char* var, double *value);
   * int grvy_input_fread_char          (const char* var, char  **value);
   * \endcode
   *
   * <b>Read a vector of ints, floats, or doubles for input keyword \e var:</b>
   * \code
   * int grvy_input_fread_int_vec       (const char* var, int    *value, int nelems);
   * int grvy_input_fread_float_vec     (const char* var, float  *value, int nelems);
   * int grvy_input_fread_double_vec    (const char* var, double *value, int nelems);
   * \endcode
   *
   * <b>Read the \e ith entry (elem) of a vector of ints, floats,
   * doubles, or characters for input keyword \e var:</b>
   *
   * \code
   * int grvy_input_fread_int_ivec      (const char* var, int *value,    int elem);
   * int grvy_input_fread_float_ivec    (const char *var, float *value,  int elem);
   * int grvy_input_fread_double_ivec   (const char* var, double *value, int elem);
   * int grvy_input_fread_char_ivec     (const char* var, char **value,  int elem);
   * \endcode
   *
   * <b> Support for the creation of backwards compatible input files:</b>
   *
   * The following routines provide a mechanism to register a default
   * value with the input parsing mechanism.  If a particular keyword
   * (\e var) has a registered default value and is subsequently
   * queried with an input file which does not contain the keyword, the
   * registered default value will be returned instead.  A convenient
   * use for these registration functions is to provide sensible
   * defaults to newly added input keywords which are not present in
   * older input file revisions.
   *
   * \code
   * int grvy_input_register_int        (const char *var,     int value);
   * int grvy_input_register_float      (const char *var,   float value);
   * int grvy_input_register_double     (const char *var,  double value);
   * int grvy_input_register_char       (const char *var,   char *value);
   * \endcode
   *
   * The following routines can be used to query the current hashed
   * values of any keyword variables which have been registered
   * previously.
   *
   * \code
   * int grvy_input_register_get_int    (const char *var,    int *value);
   * int grvy_input_register_get_float  (const char *var,  float *value);
   * int grvy_input_register_get_double (const char *var, double *value);
   * int grvy_input_register_get_char   (const char *var,  char **value);
   * \endcode
   *
   * \section timing Performance Timing Routines
   * \code
   * double grvy_timer                  ();
   * void   grvy_timer_init             (const char *id);
   * void   grvy_timer_reset            ();
   * void   grvy_timer_finalize         ();
   * void   grvy_timer_begin            (const char *id);
   * void   grvy_timer_end              (const char *id);
   * double grvy_timer_elapsedseconds   (const char *id);
   * double grvy_timer_elapsed_global   ();
   * void   grvy_timer_summarize        ();
   * \endcode

   * \section logging Logging Utilities
   *
   * The following routines provide a priority-based logging mechanism
   * to control application messages.  A set of default priority definitions are
   * provided to define messages of the following form:
   *
   * - GRVY_FATAL
   * - GRVY_ERROR
   * - GRVY_WARN 
   * - GRVY_INFO 
   * - GRVY_DEBUG
   *
   * The suggested usage of the logging mechanism is to use the
   * following routine to generate messages as opposed to writing to \e stdout
   * or \e stderr directly.  Note that to use, each message must also be assigned a 
   * priority loglevel (e.g. GRVY_WARN).
   *
   * \code void grvy_log                    (int loglevel, const char *mesg); \endcode
   * 
   * The verbosity of the application output can then be controlled by
   * setting the current loglevel as desired (for example, the GRVY_INFO
   * mode would show all messages except those set to the GRVY_DEBUG
   * loglevel).
   *
   * \code void grvy_log                    (int loglevel, const char *mesg); \endcode
   *
   * Note that two additional default priority loglevels exist,
   * GRVY_NOLOG and GRVY_ALL which can be used to disable or enable
   * all log output, respectively.
   *
   * \section math Math Utilities
   * 
   * The following routines provide unified floating-point checks for NANs and INFs.
   *
   * \code
   * int grvy_double_isnan              (double *val);
   * int grvy_double_isinf              (double *val);
   *   				  
   * int grvy_float_isnan               (float  *val);
   * int grvy_float_isinf               (float  *val);
   * \endcode
   *
   * \section misc Miscellaneous Routines
   * <b>grvy_check_file_path:</b>
   * This routine checks a desired output \e pathname and creates any non-existing parent directories.
   * \code
   * int    grvy_check_file_path        (const char *pathname);
   * \endcode
   * <b>grvy_create_unique_dir:</b>
   *
   * This routine creates a new, unique directory with 0700 permissions using the
   * provided template name.  The last six characters of name_template
   * must be XXXXXX, and name_template is modified with the unique
   * name upon return.
   *
   * \code
   * int    grvy_create_unique_dir      (char *name_template);
   * \endcode
   *
   * <b>grvy_create_scratch_dir:</b>
   *
   * Identical to grvy_create_unique_dir(), except that the new directory and
   * all of its contents are deleted when the process exits.
   *
   * \code
   * int    grvy_create_scratch_dir     (char *name_template);
   * \endcode
   */


#ifdef __cplusplus
}
#endif

#endif