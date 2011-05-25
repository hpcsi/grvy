// -*-c++-*-
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011 The PECOS Development Team
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
// grvy_ramdisk: Routines to implement ramdisk over MPI
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<grvy_classes.h>
#include<grvy.h>
#include<grvy_int.h>

using namespace std;
using namespace GRVY;

#define info  GRVY_INFO
#define warn  GRVY_WARN
#define error GRVY_ERROR
#define debug GRVY_DEBUG

typedef struct MPI_Ocore_datagram {
  bool in_mem;			// data resides in memory or disk?
  size_t read_counts;		// read  frequency
  size_t write_counts;		// write frequency
  size_t index;			// data index location (for mem or disk)
} MPI_Ocore_datagram;

//------------------------------------------------------------
// private implementation class definition for GRVY_MPI_Ocore
//------------------------------------------------------------

namespace GRVY {

  class GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp 
  {
  public:
    GRVY_MPI_Ocore_ClassImp () {}
   ~GRVY_MPI_Ocore_ClassImp () {}
    
    int Initialize(int blocksize);
  
    bool use_mpi_ocore;
    bool initialized;			
    int max_poolsize_GBs;		// max ramdisk size (in GB)
    int max_mapsize_MBs;		// max mapsize (in MB)
    int word_size;			// individual word size in bytes
    int blocksize;			// data element blocksize 
    int mpi_rank;			// rank of local processs
    int mpi_nprocs;			// total number of procs available
    int sendtag;		        // MPI message sendtag
    int recvtag;			// MPI message recvtag
    int num_map_records;		// max number of mappable record indices
    int num_records;			// max number of raw records
    int num_active;		        // active number of raw records

    vector< vector <double>  > pool;    // raw data pool storage
    vector<MPI_Ocore_datagram> map;	// data map

    GRVY_MPI_Ocore_Class *self;	        // back pointer to public class

  private:
};

} // matches namespace GRVY

namespace GRVY {

  GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_Class() :m_pimpl(new GRVY_MPI_Ocore_ClassImp() )
  {
    m_pimpl->use_mpi_ocore         = true;
    m_pimpl->initialized           = false;
    m_pimpl->word_size             = 8;
    m_pimpl->blocksize             = 8192;
    m_pimpl->sendtag               = 1001;
    m_pimpl->recvtag               = 2001;
    m_pimpl->self                  = this;

    // set default options for ocore

//    m_pimpl->options["output_stdout"        ] = true;
//    m_pimpl->options["output_totaltimer_raw"] = true;
//    m_pimpl->options["output_subtimer_raw"  ] = false;
//    m_pimpl->options["dump_files"           ] = true;
//    m_pimpl->options["output_comments"      ] = false;
//    m_pimpl->options["output_printenv"      ] = false;

  }

  GRVY_MPI_Ocore_Class::~GRVY_MPI_Ocore_Class()
  {
    // using auto_ptr for proper cleanup
  }

  int GRVY_MPI_Ocore_Class::Initialize(string name)
  {
    m_pimpl->initialized = true;

    if(! m_pimpl->use_mpi_ocore) return 0;

    grvy_printf(GRVY_INFO,"mpi_ocore Initialize\n");

    m_pimpl->num_records = m_pimpl->max_poolsize_GBs*1024*1024*1024/sizeof(int);

    // -------------------------------
    // allocation for raw storage pool
    // -------------------------------

    try 
      {
	vector<double> data(m_pimpl->blocksize);

	for(int i=0;i<m_pimpl->num_records;i++)
	  m_pimpl->pool.push_back(data);
      }

    catch(const std::bad_alloc& ex)
      {
	grvy_printf(error,"grvy_ocore (proc %5i): Unable to allocate raw storage pool...exiting.\n",m_pimpl->mpi_rank);
	exit(1);
      }

    catch(...)
      {
	grvy_printf(error,"grvy_ocore (proc %5i): Unknown exception...exiting\n");
	exit(1);
      }

    grvy_printf(info,"grvy_ocore (proc %5i): Allocated ramdisk storage pool = %8i GBs\n",
		m_pimpl->mpi_rank,m_pimpl->max_poolsize_GBs);
    
    return 0;
  }

} // matches namespace GRVY
