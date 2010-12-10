// -*-c++-*-
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
// Packet table data structure for performance logging.
//
// $Id: grvy_timer_interface.cpp 15042 2010-10-17 15:39:22Z karl $
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#ifndef GRVY_PTABLE_H_
#define GRVY_PTABLE_H_

//--------------------------------------------------------------------
// Packet table data structures for historical HDF performance logging
//--------------------------------------------------------------------

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
  int code_revision;		// specific code revision (e.g. svn revision)
  double flops;			// total application FLOPS
  hvl_t vl_subtimers;		// inclusion of a variable number of subtimers
} Timer_PTable_V1;

#endif
