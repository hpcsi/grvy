// -*-c++-*-
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008-2013,2018-2021 The PECOS Development Team
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
// HDF table data structure for performance logging.
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#ifndef GRVY_PTABLE_H_
#define GRVY_PTABLE_H_

//--------------------------------------------------------------------
// Packet table data structures for historical HDF performance logging
//--------------------------------------------------------------------

typedef struct Env_PTable_V1 {
  const char *env_string;	// environment variable setting
} Env_PTable_V1;

typedef struct SubTimer_PTable_V1 {
  const char *timer_name;	// subtimer name
  double measurement;		// subtimer wallclock time (secs)
  double mean;			// mean time over all subtimer evaluations
  double variance;		// variance over all subtimer evaluations
  size_t count;			// total number of subtimer evaluations
} SubTimer_PTable_V1;

typedef struct TimerPTable_V1 {
  const char *experiment;	// user's experiment name
  const char *timestamp;	// date/time of experiment
  const char *user_comment;	// generic user comment
  double total_time;		// total wallclock time (secs)
  int num_procs;		// number of processors
  int job_Id;			// batch scheduler jobID
  const char *code_revision;	// specific code revision (e.g. svn revision)
  double flops;			// total application FLOPS
  hvl_t vl_env_variables;	// variable number of runtime environment variables
  hvl_t vl_subtimers;		// inclusion of a variable number of subtimers
} Timer_PTable_V1;

//--------------------------------------------------------------------
// Controls related to historical HDF performance logging
//--------------------------------------------------------------------

const int PERF_HIST_CHUNKSIZE = 100; // chunk size for HDF performance datatype
const int PERF_HIST_PACK_FREQ = 100; // frequency for repacking of performance data

#endif
