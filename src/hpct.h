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
//
// hpct.h: Basic API Definitions
//
// $Id$
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

  /*! \page apiC C/C++ Library Interface
   *
   * This section outlines the available HPCT functions for C/C++.  To
   * use, make sure to \#include <hpct.h> within your source code
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

  extern int hpct_input_fopen               (const char *filename);
  extern int hpct_input_fclose              ();
					    
  extern int hpct_input_fread_char          (const char* var, char **value);
  extern int hpct_input_fread_char_ivec     (const char* var, char **value,  int elem);
					    
  extern int hpct_input_fread_int           (const char* var, int *value);
  extern int hpct_input_fread_int_vec       (const char* var, int *value,    int nelems);
  extern int hpct_input_fread_int_ivec      (const char* var, int *value,    int elem);
					    
  extern int hpct_input_fread_float         (const char* var, float *value);
  extern int hpct_input_fread_float_vec     (const char* var, float *value,  int nelems);
					    
  extern int hpct_input_fread_double        (const char* var, double *value);
  extern int hpct_input_fread_double_vec    (const char* var, double *value, int nelems);
  extern int hpct_input_fread_double_ivec   (const char* var, double *value, int elem);
					    
  extern int hpct_input_fdump               ();
  extern int hpct_input_fdump_delim         (const char* prefix);
  extern int hpct_input_fdump_file          (const char* prefix, const char *filename);

  /* \subpage input_register Default Value Registration Functions */

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

  // paul, these are some ideas; modify as desired.....

  extern int hpct_double_isnan            (double val);
  extern int hpct_double_isinf            (double val);

  extern int hpct_float_isnan             (float  val);
  extern int hpct_float_isinf             (float  val);

  /*
   * Miscellaneous Utilities
   */

  extern int    hpct_check_file_path      (const char *pathname);
  extern int    hpct_create_temp_dir      (      char *name_template);

  /* 
   * Doxygen API Definitions 
   */

  /*! \page apiC 
   * \section input Input Parsing Routines
   *
   * <b>Open/close HPCT input file:</b>
   * \code 
   * int hpct_input_fopen             (const char *filename);
   * int hpct_input_fclose            ();
   * \endcode
   *
   * <b>Read a single int, float, or double for input keyword \e var:</b>
   * \code
   * int hpct_input_fread_int         (const char* var, int    *value);
   * int hpct_input_fread_float       (const char* var, float  *value);
   * int hpct_input_fread_double      (const char* var, double *value);
   * \endcode
   *
   * <b>Read a vector of  ints, floats, or doubles for input keyword \e var:</b>
   * \code
   * int hpct_input_fread_int_vec     (const char* var, int    *value, int nelems);
   * int hpct_input_fread_float_vec   (const char* var, float  *value, int nelems);
   * int hpct_input_fread_double_vec  (const char* var, double *value, int nelems);
   * \endcode
   *
   * \section timing Performance Timing Routines
   * \code
   * double hpct_timer                ();
   * void   hpct_timer_init           (const char *id);
   * void   hpct_timer_reset          ();
   * void   hpct_timer_finalize       ();
   * void   hpct_timer_begin          (const char *id);
   * void   hpct_timer_end            (const char *id);
   * double hpct_timer_elapsedseconds (const char *id);
   * double hpct_timer_elapsed_global ();
   * void   hpct_timer_summarize      ();
   * \endcode
   *
   * \section misc Miscellaneous Routines
   * <b>hpct_check_file_path: Check desired output \e pathname and create any non-existing parent directories.</b>
   * \code
   * int    hpct_check_file_path      (const char *pathname);
   * \endcode
   * <b>hpct_create_temp_dir: Create a unique temporary directory with 0700 permissions using the provided template name.  The last six characters of name_template must be XXXXXX, and name_template is modified with the unique name upon return.</b>
   * int    hpct_create_temp_dir      (      char *name_template);
   */


#ifdef __cplusplus
}
#endif
