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
#include<map>

#ifdef HAVE_MPI
#include<mpi.h>
#endif

using namespace std;
using namespace GRVY;

#define info  GRVY_INFO
#define warn  GRVY_WARN
#define error GRVY_ERROR
#define debug GRVY_DEBUG

#define prefix "mpi_ocore"

// Local datatypes

enum OCORE_MSG_TYPES {
  OCORE_WRITE_NEW,
  OCORE_UPDATE_OLD,
  OCORE_READ,
  OCORE_EXIT,
};

typedef struct MPI_Ocore_datagram {
  bool in_mem;			// data resides in memory or disk?
  //  size_t read_count;		// read  frequency
  //  size_t write_count;		// write frequency
  size_t index;			// data index location (for mem or disk)
} MPI_Ocore_datagram;

typedef struct MPI_Ocore_owners {
  //  size_t user_index;		// global user-provided offset index
  int data_hostId;		// MPI rank Id assigned to store data
  size_t read_count;		// read frequency
  size_t write_count;		// write frequency
} MPI_Ocore_owners;

//------------------------------------------------------------
// private implementation class definition for GRVY_MPI_Ocore
//------------------------------------------------------------

namespace GRVY {

  class GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp 
  {
  public:
    GRVY_MPI_Ocore_ClassImp () {}
   ~GRVY_MPI_Ocore_ClassImp () {}

#ifdef HAVE_MPI
    int    Initialize       (string input_file, int blocksize);
    int    AssignOwnership  (size_t sparse_index);
    //    size_t GetRecord        (size_t sparse_index);
    int    PullData         (size_t sparse_index);
    int    StoreData        (size_t sparse_index, bool new_data);
    int    Read_from_Pool   (int mpi_task, size_t sparse_index,double *data);
    int    Write_to_Pool    (int mpi_task, bool new_data, size_t sparse_index,double *data);
    void   PollForWork      ();
    void   Summarize        ();
    void   Abort	    ();		
  
    bool use_mpi_ocore;		        // flag: enable use of MPI ocore?
    bool initialized;			// flag: Init functioned complete?
    bool mpi_initialized_by_ocore;	// flag: did we have to init MPI locally?
    bool master;			// flag: master MPI process?

    string inputfile;			// input file with runtime controls

    int max_poolsize_MBs;		  // max ramdisk size (in MBs)
    int max_mapsize_MBs;		  // max mapsize (in MBs)
    int word_size;			  // individual word size in bytes
    int blocksize;			  // data element blocksize 
    int mpi_rank;			  // rank of local processs
    int mpi_nprocs;			  // total number of procs available
    int sendtag;		          // MPI message sendtag
    int recvtag;			  // MPI message recvtag
    int num_smap_records;		  // max number of mappable sparse record indices (user index)
    int max_num_records;		  // max number of sequential ocore records (ocore index)
    int num_active_records;	          // number of currently active records
    int distrib_rank;			  // incremental counter used for distributing ocore data

    vector< vector <double>  > pool;       // raw data pool storage
    map<size_t,MPI_Ocore_datagram> smap;   // sparse data map (sparse indices -> contiguous pool indices)
    map<size_t,MPI_Ocore_owners> rank_map; // map for rank 0 to identify which child rank owns the data

    MPI_Comm MYCOMM;			   // MPI communicator for ocore activity

    GRVY_MPI_Ocore_Class *self;	          // back pointer to public class

  private:
    GRVY_Timer_Class ptimer;		  // Local performance timer
#endif    
  };

} // matches namespace GRVY

#ifdef HAVE_MPI    

namespace GRVY {

  GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_Class() :m_pimpl(new GRVY_MPI_Ocore_ClassImp() )
  {

    m_pimpl->initialized           = false;

    // Default settings 

    m_pimpl->use_mpi_ocore         = true;
    m_pimpl->max_mapsize_MBs       = 10;
    m_pimpl->max_poolsize_MBs      = 200;
    m_pimpl->word_size             = 8;
    m_pimpl->blocksize             = 8192;
    m_pimpl->sendtag               = 1001;
    m_pimpl->recvtag               = 2001;
    m_pimpl->num_active_records    = 0;
    m_pimpl->self                  = this;

    //--------------------------
    // MPI setup/initialization
    //--------------------------

    m_pimpl->MYCOMM                = MPI_COMM_WORLD;
    m_pimpl->distrib_rank          = 0;

    int initialized;
    int init_argc = 1;

    MPI_Initialized(&initialized);

    if(!initialized)
      {
	grvy_printf(info,"%s (%5i): Performing MPI_Init()\n",prefix,m_pimpl->mpi_rank);
	MPI_Init(&init_argc,NULL);
	m_pimpl->mpi_initialized_by_ocore = true;
      }

    MPI_Comm_size(m_pimpl->MYCOMM,&m_pimpl->mpi_nprocs);
    MPI_Comm_rank(m_pimpl->MYCOMM,&m_pimpl->mpi_rank);

    if(m_pimpl->mpi_rank == 0)
      m_pimpl->master = true;

    if(m_pimpl->mpi_nprocs == 1)
      {
	m_pimpl->use_mpi_ocore = false;
	grvy_printf(info,"%s: Disabling MPI_ocore - more than 1 MPI task is required\n",prefix);
      }

    // set default options for ocore

    //    m_pimpl->options["output_stdout"        ] = true;
    //    m_pimpl->options["output_totaltimer_raw"] = true;

  }

  GRVY_MPI_Ocore_Class::~GRVY_MPI_Ocore_Class()
  {
    // using auto_ptr for proper cleanup


    // tear down MPI if we initialized 

    if(m_pimpl->mpi_initialized_by_ocore)
      {
	grvy_printf(info,"%s (%5i): Performing MPI_Finalize()\n",prefix,m_pimpl->mpi_rank);
	MPI_Finalize();
      }
  }

  int GRVY_MPI_Ocore_Class::Initialize(string input_file,int blocksize)
  {
    return(m_pimpl->Initialize(input_file,blocksize));
  }

  // ---------------------------------------------------------------------------------------
  // Finalize(): Public finalize -> sends notice to children to exit polling loop
  // ---------------------------------------------------------------------------------------

  void GRVY_MPI_Ocore_Class::Finalize()
  {

    if(m_pimpl->master)
      {
	grvy_printf(info,"%s: master task initiating finalize\n",prefix);

	int hbuf[4] = {OCORE_EXIT,0,0,m_pimpl->blocksize};

	MPI_Bcast(hbuf,4,MPI_INTEGER,0,m_pimpl->MYCOMM);
      }

    fflush(NULL);
    MPI_Barrier(m_pimpl->MYCOMM);

    m_pimpl->Summarize();

    return;
  }

  // ---------------------------------------------------------------------------------------
  // Write(): Public write data function
  // ---------------------------------------------------------------------------------------

  int GRVY_MPI_Ocore_Class::Write(size_t offset, double *data)
  {
    if(!m_pimpl->use_mpi_ocore)
      {
	grvy_printf(info,"\n%s: Use of MPI_ocore disabled, unable to complete write request\n",prefix);
	return(1);
      }

    map<size_t,MPI_Ocore_owners> :: iterator it = m_pimpl->rank_map.find(offset);
    int rank_owner;
    bool new_data = false;

    if(it == m_pimpl->rank_map.end())	// new record
      {
	rank_owner = m_pimpl->AssignOwnership(offset);
	new_data   = true;
      }
    else			// old record
      {
	rank_owner = it->second.data_hostId;
	it->second.write_count++;
      }
    
    assert(rank_owner > 0 && rank_owner < m_pimpl->mpi_nprocs );

    grvy_printf(info,"%s: Writing data for offset %i to task %i\n",prefix,offset,rank_owner);

    return(m_pimpl->Write_to_Pool(rank_owner,new_data,offset,data));
  }

  // ---------------------------------------------------------------------------------------
  // Write(): Public read data function
  // ---------------------------------------------------------------------------------------

  int GRVY_MPI_Ocore_Class::Read(size_t offset, double *data)
  {
    if(!m_pimpl->use_mpi_ocore)
      {
	grvy_printf(info,"\n%s: Use of MPI_ocore disabled, unable to complete read request\n",prefix);
	return(1);
      }

    map<size_t,MPI_Ocore_owners> :: iterator it = m_pimpl->rank_map.find(offset);
    int rank_owner;

    if(it == m_pimpl->rank_map.end())	// unknown record
      {
	grvy_printf(error,"%s (%5i): Error - unknown record index requested for read (%i)\n",prefix,m_pimpl->mpi_rank,offset);
	m_pimpl->Abort();
      }
    else			// old record
      {
	rank_owner = it->second.data_hostId;
	it->second.read_count++;
      }

    assert(rank_owner > 0 && rank_owner < m_pimpl->mpi_nprocs );

    grvy_printf(info,"%s: Reading data for offset %i from task %i\n",prefix,offset,rank_owner);

    return(m_pimpl->Read_from_Pool(rank_owner,offset,data));
  }

  // ---------------------------------------------------------------------------------------
  // PollForWork(): Work poller for ocore slaves
  // ---------------------------------------------------------------------------------------

  void GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: PollForWork()
  {
    int hbuf[4];		// header buffer (includes info about next message)
    int msg_destination;	// message destination
    int msg_offset;		// message offset
    int msg_size;		// message size

    int recv_request;

    // ---------------------------------------------------------
    // note: header buffer layout
    // 
    //  hbuf(0) -> next message type: (WRITE,UPDATE,READ,EXIT)
    //  hbuf(1) -> message destination
    //  hbuf(2) -> storage offset
    //  hbuf(3) -> message size
    // ---------------------------------------------------------

    assert(mpi_rank > 0);

    grvy_printf(info,"%s (%5i): Child task entering polling mode\n",prefix,mpi_rank);

    while(1)
      {
	// get next item of work from rank 0 (distributed to all tasks)

	MPI_Bcast(hbuf,4,MPI_INTEGER,0,MYCOMM);

	// Based on received header buffer, decide what to do next

	switch(hbuf[0])
	  {
	  
	  case OCORE_WRITE_NEW:		
	    if(mpi_rank == hbuf[1] )
	      {
		grvy_printf(info,"\n%s (%5i): Polling -> NEW write request received\n",prefix,mpi_rank);

		bool new_data = true;
		StoreData(hbuf[2],new_data);
	      }
	    break;
	  case OCORE_UPDATE_OLD: 
	    if(mpi_rank == hbuf[1] )
	      {
		grvy_printf(info,"\n%s (%5i): Polling -> UPDATE write request received\n",prefix,mpi_rank);

		bool new_data = false;
		StoreData(hbuf[2],new_data);
	      }
	    break;
	  case OCORE_READ:
	    if(mpi_rank == hbuf[1] )
	      {
		grvy_printf(info,"\n%s (%5i): polling -> READ request received\n",prefix,mpi_rank);
		PullData(hbuf[2]);
	      }
	    break;
	  case OCORE_EXIT:		// exit: task 0 has requested us to finish
	    grvy_printf(info,"\n%s (%5i): polling -> EXIT request received\n",prefix,mpi_rank);
	    return;
	  default:
	    grvy_printf(error,"\n%s: Internal Error - unknown work request received (%i)\n",prefix,hbuf[0]);
	    Abort();
	  }

      }	// end infinite polling while(1) loop

    return;
  }

  // ---------------------------------------------------------------------------------------
  // Summarize(): Summarize read/write statistics and performance
  // ---------------------------------------------------------------------------------------

  void GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: Summarize()
  {

    if(master)
      {
	size_t total_written    = ptimer.StatsCount("write_to_pool" )*blocksize*word_size;
	size_t total_read       = ptimer.StatsCount("read_from_pool")*blocksize*word_size;
	
	double aggr_write_speed = total_written/ptimer.ElapsedSeconds("write_to_pool");
	double aggr_read_speed  = total_read/ptimer.ElapsedSeconds("read_from_pool");
	
	grvy_printf(info,"\n%s: Final MPI Ocore Read/Write Stats\n",prefix);
	grvy_printf(info,"%s:   --> Total Written = %12.5e GBs; Avg. = %12.5e MB/sec\n",prefix,
		    1.0*total_written/(1024*1024*1024),aggr_write_speed/(1024*1024));
	grvy_printf(info,"%s:   --> Total Read    = %12.5e GBs; Avg. = %12.5e MB/sec\n\n",prefix,
		    1.0*total_read/(1024*1024*1024),aggr_read_speed/(1024*1024));

      }

    // Memory Usage

    int *num_active_per_task;

    if(master)
      {
	num_active_per_task = new int[mpi_nprocs];
	MPI_Gather(&num_active_records,1,MPI_INTEGER,num_active_per_task,1,MPI_INTEGER,0,MYCOMM);

	for(int i=1;i<mpi_nprocs;i++)
	  grvy_printf(info,"%s:   --> Proc %5i - Memory Used = %12.5e MBs (Max Req. = %i MBs)\n",prefix,i,
		      1.0*num_active_per_task[i]*blocksize*word_size/(1024*1024),max_poolsize_MBs);

	delete [] num_active_per_task;
      }
    else
      MPI_Gather(&num_active_records,1,MPI_INTEGER,num_active_per_task,1,MPI_INTEGER,0,MYCOMM);
    
    return;
  }

  // ---------------------------------------------------------------------------------------
  // Write_to_Pool(): Internal write method - use MPI to xfer desired data
  // ---------------------------------------------------------------------------------------

  int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: Write_to_Pool(int dest_task, bool new_data, 
								    size_t offset, double *data)
  {

    ptimer.BeginTimer("write_to_pool");

    // create msg header

    int hbuf[4];

    if(new_data)
      hbuf[0] = OCORE_WRITE_NEW;   
    else
      hbuf[0] = OCORE_UPDATE_OLD;  

    hbuf[1] = dest_task;
    hbuf[2] = offset;
    hbuf[3] = blocksize;
    
    // Notify children of upcoming write request

    grvy_printf(info,"%s (%5i): Performing mpi_bcast to prep rank %i\n",prefix,mpi_rank,dest_task);

    MPI_Bcast(hbuf,4,MPI_INTEGER,0,MYCOMM);

    // Perform the write (sync for now)

    grvy_printf(info,"%s (%5i): Performing mpi_send to rank %i\n",prefix,mpi_rank,dest_task);

    MPI_Send(data,blocksize,MPI_DOUBLE,dest_task,sendtag,MYCOMM);

    ptimer.EndTimer("write_to_pool");
  }

  // ---------------------------------------------------------------------------------------
  // Read_from_Pool(): Internal read method - use MPI to xfer desired data
  // ---------------------------------------------------------------------------------------

  int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: Read_from_Pool(int recv_task, size_t offset, double *data)
  {
    MPI_Status status;
    ptimer.BeginTimer("read_from_pool");

    // create msg header

    int hbuf[4] = {OCORE_READ,recv_task,offset,blocksize};

    // Notify children of upcoming read request

    grvy_printf(info,"%s (%5i): Performing mpi_bcast to prep rank %i\n",prefix,mpi_rank,recv_task);

    MPI_Bcast(hbuf,4,MPI_INTEGER,0,MYCOMM);

    // Perform the read (sync for now)

    grvy_printf(info,"%s (%5i): Performing mpi_recv from rank %i\n",prefix,mpi_rank,recv_task);

    MPI_Recv(data,blocksize,MPI_DOUBLE,recv_task,recvtag,MYCOMM,&status);

    ptimer.EndTimer("read_from_pool");
  }

  // ---------------------------------------------------------------------------------------
  // Initialize(): Initialize data structures for MPI Ocore
  // ---------------------------------------------------------------------------------------

  int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: Initialize(string input_file,int blocksize)
  {

    if(!use_mpi_ocore) return 0;

    if(master)
      grvy_printf(info,"\n%s: Initializing on %i processors...\n",prefix,mpi_nprocs);

    // Initialize timer
    
    ptimer.Init("GRVY MPI_Ocore");

    // todo: read input variables/bcast from rank 0

    num_smap_records = max_mapsize_MBs*1024*1024/sizeof(int);
    max_num_records  = max_poolsize_MBs*1024*1024/(blocksize*word_size);

    if(master)
      {
	grvy_printf(info,"\n");
	grvy_printf(info,"%s: --> Individual record word size         = %15i (Bytes )\n",prefix,word_size);
	grvy_printf(info,"%s: --> Individual record blocksize         = %15i (Words )\n",prefix,blocksize);
	grvy_printf(info,"\n");
	grvy_printf(info,"%s: --> Max sparse map memory size          = %15i (MBytes)\n",prefix,max_mapsize_MBs );
	grvy_printf(info,"%s: --> MPI ramdisk memory size (per proc)  = %15i (MBytes)\n",prefix,max_poolsize_MBs);
	grvy_printf(info,"\n");
	grvy_printf(info,"%s: --> Max number of mappable records      = %15i\n",         prefix,num_smap_records);
	grvy_printf(info,"%s: --> Max number of ocore ramdisk records = %15i\n",         prefix,max_num_records );
	grvy_printf(info,"\n");
      }
    else 			// children
      {
    
	// -------------------------------
	// allocation for raw storage pool
	// -------------------------------
	
	// \todo: allow for non-fixed size pool

#if 1

	try 
	  {
	    vector<double> data(blocksize);
	    
	    for(int i=0;i<max_num_records;i++)
	      pool.push_back(data);
	  }
	
	catch(const std::bad_alloc& ex) 
	  {
	    grvy_printf(error,"%s (p%5i): Unable to allocate raw storage pool...exiting.\n",prefix,mpi_rank);
	    exit(1);
	  }
	
	catch(...)
	  {
	    grvy_printf(error,"%s (p%5i): Unknown exception...exiting\n",prefix,mpi_rank);
	    exit(1);
	  }
	
	grvy_printf(info,"%s (%5i): Successfully initialized ramdisk storage pool of %8i MBs\n",
		    prefix,mpi_rank,max_poolsize_MBs);
	
#endif
	
      }

	initialized = true;    
	
	// Put children tasks to work in polling mode
	
	if(!master)
	  PollForWork();

	fflush(NULL);
	return 0;
  }

  // --------------------------------------------------------------------------------
  // AssignOwnership(): maps user-provided sparse index to hosting MPI rank
  // --------------------------------------------------------------------------------

  int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: AssignOwnership(size_t sparse_index)
  {

    
    // Assign mpi task ownership based on round-robin 
    
    MPI_Ocore_owners owner;
    
    distrib_rank++;
    if(distrib_rank == mpi_nprocs)
      distrib_rank = 1;
   
    owner.data_hostId = distrib_rank;
    owner.write_count = 1;
    owner.read_count  = 0;
    
    try {
      rank_map[sparse_index] = owner;
    }
    
    catch(const std::bad_alloc& ex) 
      {
	grvy_printf(error,"%s (%5i): Unable to allocate rank_map storage...exiting.\n",prefix,mpi_rank);
	Abort();	    
      }
    catch(...)
      {
	grvy_printf(error,"%s (%5i): Unknown exception...exiting\n",prefix,mpi_rank);
	Abort();	    
      }

    grvy_printf(info,"\n%s: Assigned task ownership for new record %i to task %i\n",prefix,sparse_index,distrib_rank);

    return(distrib_rank);
  }

  // ---------------------------------------------------------------------------------------
  // StoreData(): Recv and store ocore data from rank 0
  // ---------------------------------------------------------------------------------------

  int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: StoreData(size_t sparse_index, bool new_data)
  {

    MPI_Status status;

    grvy_printf(info,"%s (%5i): Entering StoreData()\n",prefix,mpi_rank);

    if(!initialized)
      {
	grvy_printf(info,"%s (%5i): Ocore must be initialized prior to storing data\n",prefix,mpi_rank);
	Abort();
      }

    assert(mpi_rank > 0);

    map<size_t,MPI_Ocore_datagram>:: iterator it = smap.find(sparse_index);
    size_t offset;
    bool   in_mem;

    if(it == smap.end())	// new record
      {
	MPI_Ocore_datagram datagram;	
	datagram.in_mem      = true;
	datagram.index       = num_active_records;

	offset               = num_active_records;
	in_mem               = true;

	try {
	  smap[sparse_index] = datagram;
	}

	catch(const std::bad_alloc& ex) 
	  {
	    grvy_printf(error,"%s (%5i): Unable to allocate smap storage...exiting.\n",prefix,mpi_rank);
	    Abort();	    
	  }
	catch(...)
	  {
	    grvy_printf(error,"%s (%5i): Unknown exception...exiting\n",prefix,mpi_rank);
	    Abort();	    
	  }
	num_active_records++;
      }
    else
      {
	offset = smap[sparse_index].index;
	in_mem = smap[sparse_index].in_mem;
	
      }

    grvy_printf(info,"%s (%5i): Prepping to recv data (user index %i -> ocore index %i)\n",prefix,mpi_rank,
		sparse_index,offset);

    if(in_mem)
      {
	MPI_Recv(&pool[offset][0],blocksize,MPI_DOUBLE,0,sendtag,MYCOMM,&status);
	grvy_printf(info,"%s (%5i): Data received into memory pool\n",prefix,mpi_rank);
      }
    else
      {
	grvy_printf(error,"%s (%5i): disk-transaction not yet enabled\n",prefix,mpi_rank);
	Abort();
      }
    
      return(0);
  }

  // ---------------------------------------------------------------------------------------
  // PullData(): Read and send ocore data record to rank 0
  // ---------------------------------------------------------------------------------------

  int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: PullData(size_t sparse_index)
  {

    MPI_Status status;

    grvy_printf(info,"%s (%5i): Entering PullData()\n",prefix,mpi_rank);

    if(!initialized)
      {
	grvy_printf(info,"%s (%5i): Ocore must be initialized prior to pulling data\n",prefix,mpi_rank);
	Abort();
      }

    assert(mpi_rank > 0);

    map<size_t,MPI_Ocore_datagram>:: iterator it = smap.find(sparse_index);

    assert(it != smap.end());

    size_t offset = it->second.index;
    bool   in_mem = it->second.in_mem;

    grvy_printf(info,"%s (%5i): Prepping to send data (user index %i -> ocore index %i)\n",prefix,mpi_rank,
		sparse_index,offset);

    if(in_mem)
      {
	MPI_Send(&pool[offset][0],blocksize,MPI_DOUBLE,0,recvtag,MYCOMM);
	grvy_printf(info,"%s (%5i): Data sent from memory pool\n",prefix,mpi_rank);
      }
    else
      {
	grvy_printf(error,"%s (%5i): disk-transaction not yet enabled\n",prefix,mpi_rank);
	Abort();
      }
    
      return(0);
  }

  // --------------------------------------------------------------------------------
  // GetRecord(): map sparse index to Ocore index
  // --------------------------------------------------------------------------------

#if 0
  size_t GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: GetRecord(size_t sparse_index)
  {

    map<size_t,size_t>:: iterator it = smap.find(sparse_index);

    if(it == smap.end())	// new record
      {
	// store mapping and allocate raw storage for new record

#if 0
	try {
	  smap[sparse_index] = ++num_active_records;;

	  vector<double> data(blocksize);
	  pool.push_back(data);
	}

	catch(const std::bad_alloc& ex) 
	  {
	    grvy_printf(error,"%s (%5i): Unable to allocate raw storage pool...exiting.\n",prefix,mpi_rank);
	    Abort();	    
	  }

	catch(...)
	  {
	    grvy_printf(error,"%s (%5i): Unknown exception...exiting\n",prefix,mpi_rank);
	    Abort();	    
	  }
#endif

	grvy_printf(info,"%s (%5i): Registered new active record (record = %15i)\n",prefix,num_active_records);

	return(num_active_records);
      }
    else			// an old record
      return( it->second );
  }
#endif

  void GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: Abort()
  {
    grvy_printf(error,"%s: Aborting due to application error\n",prefix);
    MPI_Abort(MPI_COMM_WORLD,42);
    return;
  }



} // matches namespace GRVY

#else

//----------------------------------------------------------
// NOOP public interface for case when MPI is not linked in
//----------------------------------------------------------

namespace GRVY {
#if 1
GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_Class()
{
  grvy_printf(GRVY_FATAL,"\n\nlibGRVY not built with MPI support\n\n");
  grvy_printf(GRVY_FATAL,"Please enable support using the \"--with-mpi\" option to configure \n");
  grvy_printf(GRVY_FATAL,"and reinstall if you desire to use MPI out-of-core related functionality.\n\n");
  exit(1);
}


GRVY_MPI_Ocore_Class::~GRVY_MPI_Ocore_Class()
{
  grvy_printf(GRVY_FATAL,"\n\nlibGRVY not built with MPI support\n\n");
  grvy_printf(GRVY_FATAL,"Please enable support using the \"--with-mpi\" option to configure \n");
  grvy_printf(GRVY_FATAL,"and reinstall if you desire to use MPI out-of-core related functionality.\n\n");
  exit(1);
}
#endif

}

int GRVY_MPI_Ocore_Class::Initialize(string input_file,int blocksize) { return 0;}

#endif


