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

/*
 * Basic API Definition for ASCII input file parsing 
 */

/* C Interface */

extern int hpct_input_fopen       (const char *filename);
extern int hpct_input_fclose      ();
extern int hpct_input_fdump       ();
extern int hpct_input_fread_int   (const char* var, int *value);
extern int hpct_input_fread_float (const char* var, float *value);
extern int hpct_input_fread_double(const char* var, double *value);
extern int hpct_input_fread_char  (const char* var, char **value);

extern int hpct_input_fread_float_vec  (const char* var, float *value,  int nelems);
extern int hpct_input_fread_double_vec (const char* var, double *value, int nelems);
extern int hpct_input_fread_int_vec    (const char* var, int *value,    int nelems);

extern int hpct_input_fread_double_ivec(const char* var, double *value, int elem);
extern int hpct_input_fread_int_ivec   (const char* var, int *value,    int elem);
extern int hpct_input_fread_char_ivec  (const char* var, char **value,  int elem);

/* Output Control */

extern void hpct_input_toggle_messages(int flag);

/*
 * Basic API Definition for Performance Timers
 */

extern double hpct_timer             (void);
extern void   hpct_timer_init        ();
extern void   hpct_timer_finalize    ();
extern void   hpct_timer_begin       (const char *id);
extern void   hpct_timer_end         (const char *id);
extern void   hpct_timer_summarize   ();


#ifdef __cplusplus
}
#endif
