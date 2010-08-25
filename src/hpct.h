//-----------------------------------------------------------------------bl-
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
//-----------------------------------------------------------------------el-
//
// hpct.h: Basic API Definitions - compatibility with previous HPCT name.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#ifndef HPCT_H_
#define HPCT_H_

#ifdef __cplusplus
extern "C" {
#endif

  /*
   * Basic API Definition for ASCII input file parsing
   */

  /* C Interface */

  extern int hpct_input_fopen               (const char *filename);
  extern int hpct_input_fclose              ();

  extern int hpct_input_fread_char          (const char* var, char **value);
  extern int hpct_input_fread_char_ivec     (const char* var, char **value,  int elem);

  extern int hpct_input_fread_int           (const char* var, int *value);
  extern int hpct_input_fread_int_vec       (const char* var, int *value,    int nelems);
  extern int hpct_input_fread_int_ivec      (const char* var, int *value,    int elem);

  extern int hpct_input_fread_float         (const char* var, float *value);
  extern int hpct_input_fread_float_vec     (const char* var, float *value,  int nelems);
  extern int hpct_input_fread_float_ivec    (const char *var, float *value,  int elem);

  extern int hpct_input_fread_double        (const char* var, double *value);
  extern int hpct_input_fread_double_vec    (const char* var, double *value, int nelems);
  extern int hpct_input_fread_double_ivec   (const char* var, double *value, int elem);

  extern int hpct_input_fdump               ();
  extern int hpct_input_fdump_delim         (const char* prefix);
  extern int hpct_input_fdump_file          (const char* prefix, const char *filename);

  /* Input variable registration functions (to provide default input
     values and backwards compatibility support) */

  extern int hpct_input_register_int        (const char *var,     int value);
  extern int hpct_input_register_float      (const char *var,   float value);
  extern int hpct_input_register_double     (const char *var,  double value);
  extern int hpct_input_register_char       (const char *var,   char *value);

  extern int hpct_input_register_get_int    (const char *var,    int *value);
  extern int hpct_input_register_get_float  (const char *var,  float *value);
  extern int hpct_input_register_get_double (const char *var, double *value);
  extern int hpct_input_register_get_char   (const char *var,  char **value);

  /* Error Message Output Control */

  extern void hpct_input_toggle_messages(int flag);

  /*
   * Basic API Definition for Performance Timers
   */

  extern double hpct_timer                ();
  extern void   hpct_timer_init           (const char *id);
  extern void   hpct_timer_reset          ();
  extern void   hpct_timer_finalize       ();
  extern void   hpct_timer_begin          (const char *id);
  extern void   hpct_timer_end            (const char *id);
  extern double hpct_timer_elapsedseconds (const char *id);
  extern double hpct_timer_elapsed_global ();
  extern void   hpct_timer_summarize      ();

  /*
   * Basic API Definition for Math Utilities
   */

  extern int hpct_double_isnan            (double *val);
  extern int hpct_double_isinf            (double *val);

  extern int hpct_float_isnan             (float  *val);
  extern int hpct_float_isinf             (float  *val);

  /*
   * Logging Utilities
   */

  /* Available logging priorities; roughly mimics syslog priority levels but with
   * fewer options. Also provides a NOLOG option to disable all log messages. 
   */
  
  typedef enum {HPCT_NOLOG  =  -1,
		HPCT_FATAL  =   0,
		HPCT_ERROR  = 100,
		HPCT_WARN   = 200,
		HPCT_INFO   = 300,
		HPCT_DEBUG  = 400,
		HPCT_ALL    = 500
  } PriorityLevel;

  extern void hpct_log_setlevel           (int priority);
  extern void hpct_log                    (int loglevel, const char *mesg);

  /*
   * Miscellaneous Utilities
   */

  extern int    hpct_check_file_path      (const char *pathname);
  extern int    hpct_create_unique_dir    (      char *name_template);
  extern int    hpct_create_scratch_dir   (      char *name_template);

#ifdef __cplusplus
}
#endif

#endif
