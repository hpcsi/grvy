// -*-c++-*-
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011,2012 The PECOS Development Team
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
#include<grvy_mpi.h>
#include<map>
#include<queue>
#include<stack>
#include <boost/type_traits/is_floating_point.hpp>

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
  size_t index;			// ocore data index location (for mem or disk)
  size_t read_count;		// read frequency
  size_t write_count;		// write frequency
} MPI_Ocore_datagram;

typedef struct MPI_Ocore_owners {
  int data_hostId;		// MPI rank Id assigned to store data

} MPI_Ocore_owners;

struct NodePriority
{
    size_t priority;		// record priority (based on read access frequency)
    size_t global_index;	// sparse storage index 
    size_t ocore_index;	        // ocore data index
}; 

class ComparePriority 
  {
  public:
    bool operator() (NodePriority &n1, NodePriority &n2) 
    {
      if(n1.priority > n2.priority)
	return true;
      else
	return false;
    }
};

//------------------------------------------------------------
// private implementation class definition for GRVY_MPI_Ocore
//------------------------------------------------------------

namespace GRVY {

// Supported Function Templates for Ocore Read/Write

template    int GRVY_MPI_Ocore_Class::Write     <       double> (size_t offset,        double *data);
template    int GRVY_MPI_Ocore_Class::Write     <        float> (size_t offset,         float *data);
template    int GRVY_MPI_Ocore_Class::Write     <          int> (size_t offset,           int *data);
template    int GRVY_MPI_Ocore_Class::Write     <long long int> (size_t offset, long long int *data);
							        
template    int GRVY_MPI_Ocore_Class::Read      <       double> (size_t offset,        double *data);
template    int GRVY_MPI_Ocore_Class::Read      <        float> (size_t offset,         float *data);
template    int GRVY_MPI_Ocore_Class::Read      <          int> (size_t offset,           int *data);
template    int GRVY_MPI_Ocore_Class::Read      <long long int> (size_t offset, long long int *data);
							        
template size_t GRVY_MPI_Ocore_Class::PopRecord <       double> (       double *data);
template size_t GRVY_MPI_Ocore_Class::PopRecord <        float> (        float *data);
template size_t GRVY_MPI_Ocore_Class::PopRecord <          int> (          int *data);
template size_t GRVY_MPI_Ocore_Class::PopRecord <long long int> (long long int *data);

class GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp 
{
public:
  GRVY_MPI_Ocore_ClassImp () {}
 ~GRVY_MPI_Ocore_ClassImp () {}
  
#ifdef HAVE_MPI
  int    Initialize       (string input_file  );
  int    AssignOwnership  (size_t sparse_index);
  int    DumptoDisk       ();
  size_t GetFreeRecord    (size_t sparse_index);
  int    PullData         (size_t sparse_index);
  int    StoreData        (size_t sparse_index, bool new_data);

  template <typename T> int Read_from_Pool (int mpi_task, size_t sparse_index,T *data);
  template <typename T> int  Write_to_Pool (int mpi_task, bool new_data, size_t sparse_index,T *data);

  void   Abort	          ();		  
  void   PollForWork      ();
  void   Summarize        ();
  void   DumpStatistics   (string statfile);
  
  bool use_mpi_ocore;		          // enable use of MPI ocore?
  bool initialized;			  // init function complete?
  bool use_disk_overflow;		  // use disk-based storage for overflow?
  bool master;			          // master MPI process?
  bool mpi_initialized_by_ocore;	  // did we have to init MPI locally?
  bool overflow_triggered;                // track whether ramdisk overflow has been triggered
  bool allow_empty_records;               // allow return of empty record if not previously written
  bool dump_stats;			  // dump raw record read/wrote statistics on finalize
  
  double dump_watermark_ratio;            // watermark for how much data to dump to disk when ramdisk is full
  
  string inputfile;			  // input file with runtime controls
  string tmpdir;			  // parent directory to store overflow ocore data
  string statsfile;			  // output filename to store raw record statistics
  char *tmp_unique_path;		  // unique path to overflow ocore data 
  FILE *fp_tmp;			          // file pointer to disk-based storage
  
  int max_poolsize_MBs;		          // max ramdisk size (in MBs)
  int max_mapsize_MBs;		          // max mapsize (in MBs)
  int word_size;			  // individual word size in bytes
  int blocksize;			  // data element blocksize 
  int mpi_rank;			          // rank of local processs
  int mpi_nprocs;			  // total number of procs available
  int sendtag;		                  // MPI message sendtag
  int recvtag;			          // MPI message recvtag
  int num_smap_records;		          // max number of mappable sparse record indices (user index)
  int max_num_records;		          // max number of sequential ocore records (ocore index)
  int num_active_records;	          // number of currently active records (ramdisk)
  int distrib_rank;			  // incremental counter used for distributing ocore data
  int num_empty_reads;	                  // number of empty reads encountered
  
  vector< vector <double>  > pool;        // raw data pool storage
  map<size_t,MPI_Ocore_datagram> smap;    // sparse data map (sparse user indices -> contiguous pool indices)
  map<size_t,MPI_Ocore_owners> rank_map;  // map for rank 0 to identify which child rank owns the data
  double* data_tmp;		          // temporary buffer for disk_overflow block storage
  
  MPI_Comm MYCOMM;			  // MPI communicator for ocore activity (comm_world)
  MPI_Comm MPI_COMM_OCORE;		  // MPI communicator with task 0 removed
  
  GRVY_MPI_Ocore_Class *self;	          // back pointer to public class
  
private:
  GRVY_Timer_Class ptimer;		  // Local performance timer
  size_t num_active_disk_records;         // number of currently active disk records
  stack<size_t> free_records;             // list of currently unoccupied ramdisk records
  
#endif    
};

} // matches namespace GRVY

#ifdef HAVE_MPI    

namespace GRVY {

GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_Class() :m_pimpl(new GRVY_MPI_Ocore_ClassImp() )
{
  
  m_pimpl->initialized              = false;
  
  // Initial values
  
  m_pimpl->master                   = false;
  m_pimpl->overflow_triggered       = false;
  m_pimpl->mpi_initialized_by_ocore = false;
  m_pimpl->sendtag                  = 1001;
  m_pimpl->recvtag                  = 2001;
  m_pimpl->num_active_records       = 0;
  m_pimpl->num_empty_reads          = 0;
  
  // Default settings 
  
  m_pimpl->use_mpi_ocore            = true;
  m_pimpl->use_disk_overflow        = true;
  m_pimpl->allow_empty_records      = true;
  m_pimpl->max_mapsize_MBs          = 10;
  m_pimpl->max_poolsize_MBs         = 20;
  m_pimpl->word_size                = 8;
  m_pimpl->blocksize                = 8192;
  m_pimpl->tmpdir                   = "/tmp";
  m_pimpl->dump_watermark_ratio     = 0.1;
  
  m_pimpl->self                     = this;
  
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
      grvy_printf(debug,"%s: Performing MPI_Init()\n",prefix,m_pimpl->mpi_rank);
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

  // create convenience communicator for Ocore tasks

  MPI_Group group_world;
  MPI_Group group_ocore;
    
  int excl_ranks[1] = {0};	// exclude global rank 0 from ocore subcommunicator

  assert(MPI_Comm_group(MPI_COMM_WORLD,&group_world)  == MPI_SUCCESS);
  assert(MPI_Group_excl(group_world,1,excl_ranks,&group_ocore) == MPI_SUCCESS);

  MPI_Comm_create(MPI_COMM_WORLD,group_ocore,&m_pimpl->MPI_COMM_OCORE);

}

GRVY_MPI_Ocore_Class::~GRVY_MPI_Ocore_Class()
{
  // using auto_ptr for proper cleanup
}

int GRVY_MPI_Ocore_Class::Initialize(string input_file)
{
  return(m_pimpl->Initialize(input_file));
}

// ---------------------------------------------------------------------------------------
// GetNumRecords(): Public function to return number of currently active records
// ---------------------------------------------------------------------------------------

size_t GRVY_MPI_Ocore_Class::NumActive()
{
  return(m_pimpl->rank_map.size());
}

// ---------------------------------------------------------------------------------------
// PopRecord(): Public function to return data record and pop/remove from ocore pool
// ---------------------------------------------------------------------------------------

//size_t GRVY_MPI_Ocore_Class::PopRecord(double *data)
template <typename T> size_t GRVY_MPI_Ocore_Class::PopRecord(T *data)
{
  map<size_t,MPI_Ocore_owners> :: iterator it = m_pimpl->rank_map.begin();
  
  if(it == m_pimpl->rank_map.end() )
    {
      grvy_printf(error,"%s: No Ocore records remain for PopRecord()\n",prefix);
      m_pimpl->Abort();
    }
  
  assert( Read(it->first,data) == 0);
  m_pimpl->rank_map.erase(it);
  
  return(it->first);
}

// ---------------------------------------------------------------------------------------
// Finalize(): Public finalize -> sends notice to children to exit polling loop
// ---------------------------------------------------------------------------------------

void GRVY_MPI_Ocore_Class::Finalize()
{
  if(!m_pimpl->use_mpi_ocore) return;
  
  if(m_pimpl->master)
    {
      grvy_printf(debug,"%s: master task initiating finalize\n",prefix);
      
      int hbuf[4] = {OCORE_EXIT,0,0,m_pimpl->blocksize};
      
      MPI_Bcast(hbuf,4,MPI_INTEGER,0,m_pimpl->MYCOMM);
    }
  
  fflush(NULL);
  MPI_Barrier(m_pimpl->MYCOMM);
  
  // Close temporary ramdisk overflow files and storage
  
  if(!m_pimpl->master && m_pimpl->use_disk_overflow)
    {
      fclose(m_pimpl->fp_tmp);	    
      delete [] m_pimpl->data_tmp;
    }

  // Dump raw statistics

  if(!m_pimpl->master && m_pimpl->dump_stats)
    m_pimpl->DumpStatistics(m_pimpl->statsfile);
  
  // Free memory pools
  
  if(!m_pimpl->master)
    for(int i=0;i<m_pimpl->max_num_records;i++)
      m_pimpl->pool[i].clear();
  
  // Summarize statistics
  
  m_pimpl->Summarize();
  
  // Tear down MPI if we initialized 
  
  if(m_pimpl->mpi_initialized_by_ocore)
    {
      grvy_printf(debug,"%s (%5i): Performing MPI_Finalize()\n",prefix,m_pimpl->mpi_rank);
      MPI_Finalize();
    }
  
  return;
}

// ---------------------------------------------------------------------------------------
// Write(): Public write data function
// ---------------------------------------------------------------------------------------

template <typename T> int GRVY_MPI_Ocore_Class::Write(size_t offset, T *data)
{
  if(!m_pimpl->use_mpi_ocore)
    {
      grvy_printf(info,"\n%s: Use of MPI_ocore disabled, unable to complete write request\n",prefix);
      return(1);
    }
  
  assert(sizeof(data[0]) == m_pimpl->word_size);
  
  map<size_t,MPI_Ocore_owners> :: iterator it = m_pimpl->rank_map.find(offset);
  int rank_owner;
  bool new_data = false;
  
  if(it == m_pimpl->rank_map.end())	// new record
    {
      rank_owner = m_pimpl->AssignOwnership(offset);
      new_data   = true;
    }
  else			                // old record
    {
      rank_owner = it->second.data_hostId;
    }
  
  assert(rank_owner > 0 && rank_owner < m_pimpl->mpi_nprocs );
  
  grvy_printf(debug,"%s: Writing data for offset %i to task %i\n",prefix,offset,rank_owner);
  
  return(m_pimpl->Write_to_Pool(rank_owner,new_data,offset,data));
}

// ---------------------------------------------------------------------------------------
// Read(): Public read data function
// ---------------------------------------------------------------------------------------

template <typename T> int GRVY_MPI_Ocore_Class::Read(size_t offset, T *data)
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
      // allow for user to return back record of all zero's if
      // desired when the record has not been written previously
      
      if(m_pimpl->allow_empty_records)
	{
	  grvy_printf(debug,"\n%s: Returning previously unwritten empty record (index = %li)\n",prefix,offset);

	  if(boost::is_floating_point<T>::value)
	    for(size_t i=0;i<m_pimpl->blocksize;i++)
	      data[i] = 0.0;
	  else
	    for(size_t i=0;i<m_pimpl->blocksize;i++)
	      data[i] = 0;

	  m_pimpl->num_empty_reads++;
	  return(0);
	}
      else
	{
	  grvy_printf(error,"%s (%5i): Error - unknown record index requested for read (%i)\n",
		      prefix,m_pimpl->mpi_rank,offset);
	  m_pimpl->Abort();
	}
    }
  else			                // old record
    {
      rank_owner = it->second.data_hostId;
    }
  
  assert(rank_owner > 0 && rank_owner < m_pimpl->mpi_nprocs );
  
  grvy_printf(debug,"%s (%5i): Reading data for offset %i from task %i\n",prefix,m_pimpl->mpi_rank,offset,rank_owner);
  
  return(m_pimpl->Read_from_Pool(rank_owner,offset,data));
}

// ---------------------------------------------------------------------------------------
// isMaster(): is calling process the MPI master task?
// ---------------------------------------------------------------------------------------

bool GRVY_MPI_Ocore_Class::isMaster() { return(m_pimpl->master); }

// ---------------------------------------------------------------------------------------
// isEnabled(): is MPI_Ocore enabled (controlled via input file)
// ---------------------------------------------------------------------------------------

bool GRVY_MPI_Ocore_Class::isEnabled() 
{ 
  // this check returns false if user has not initialized mpi_ocore
  
  if(_GRVY_Ocore == NULL)	
    return(false);

  // normal check to query runtime option 

  return(m_pimpl->use_mpi_ocore); 
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
  
  grvy_printf(debug,"%s (%5i): Child task entering polling mode\n",prefix,mpi_rank);
  
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
	      grvy_printf(debug,"\n%s (%5i): Polling -> NEW write request received\n",prefix,mpi_rank);
	      
	      bool new_data = true;
	      StoreData(hbuf[2],new_data);
	    }
	  break;
	case OCORE_UPDATE_OLD: 
	  if(mpi_rank == hbuf[1] )
	    {
	      grvy_printf(debug,"\n%s (%5i): Polling -> UPDATE write request received\n",prefix,mpi_rank);
	      
	      bool new_data = false;
	      StoreData(hbuf[2],new_data);
	    }
	  break;
	case OCORE_READ:
	  if(mpi_rank == hbuf[1] )
	    {
	      grvy_printf(debug,"\n%s (%5i): polling -> READ request received\n",prefix,mpi_rank);
	      PullData(hbuf[2]);
	    }
	  break;
	case OCORE_EXIT:		// exit: task 0 has requested us to finish
	  grvy_printf(debug,"\n%s (%5i): polling -> EXIT request received\n",prefix,mpi_rank);
	  return;
	default:
	  grvy_printf(error,"\n%s: Internal Error - unknown work request received (%i)\n",prefix,hbuf[0]);
	  Abort();
	}
      
    }	// end infinite polling loop
  
  return;
}

// ---------------------------------------------------------------------------------------
// DumpStatistics(): Dump individual read/write statistics to file
// ---------------------------------------------------------------------------------------

void GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp::DumpStatistics(std::string statfile)
{
  FILE *fp; 

  map<size_t,MPI_Ocore_datagram> :: iterator it;   

  // Simple header 

  if(mpi_rank == 1)
    {
      fp = fopen(statfile.c_str(),"w");
      if(fp == NULL)
	{
	  grvy_printf(error,"%s: Unable to create stats file - %s\n",prefix,statfile.c_str());
	  Abort();
	}

      fprintf(fp,"#\n");
      fprintf(fp,"# libGRVY Out-of-Core Record Statistics\n");
      fprintf(fp,"#\n");
      fprintf(fp,"# Record Index, Write Count, Read Count, MPI Rank, In-Ram?\n");
      fprintf(fp,"#\n");
      fclose(fp);
    }

  // Dump statistics serially from each ocore subprocess

  MPI_Barrier(MPI_COMM_OCORE);

  for(int i=1;i<mpi_nprocs;i++)
    {
      if(mpi_rank == i)
	{
	  grvy_printf(info,"%s: Dumping individual read/write statistics for proc %i to %s\n",
		      prefix,mpi_rank,statsfile.c_str());
	  assert(smap.size() > 0);
	  fp = fopen(statfile.c_str(),"a");
	  
	  if(fp == NULL)
	    {
	      grvy_printf(error,"%s: Unable to append to stats file - %s\n",prefix,statfile.c_str());
	      Abort();
	    }
	  
	  for(it = smap.begin(); it != smap.end(); it++)
	    fprintf(fp,"%12li %12li %12li %9i %7i\n",it->first,it->second.write_count,it->second.read_count,
		    mpi_rank,it->second.in_mem);
	  
	  fclose(fp);
	}
      MPI_Barrier(MPI_COMM_OCORE);
    }

  if(mpi_rank == 1)
    grvy_printf(info,"\n");

  return;
}

// ---------------------------------------------------------------------------------------
// Summarize(): Summarize read/write statistics and performance
// ---------------------------------------------------------------------------------------

void GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp::Summarize()
{
  
  if(!use_mpi_ocore) return;
  
  // ----------------
  // Global Stats
  // ----------------
  
  size_t total_written;
  size_t total_read;
  
  if(master)
    {
      total_written    = ptimer.StatsCount("write_to_pool" );
      total_read       = ptimer.StatsCount("read_from_pool");
      
      double aggr_write_speed = total_written*blocksize*word_size/ptimer.ElapsedSeconds("write_to_pool");
      double aggr_read_speed  = total_read   *blocksize*word_size/ptimer.ElapsedSeconds("read_from_pool");

      grvy_printf(info,"\n---------------------------------------------------------------------------------------------\n\n");
      grvy_printf(info,"%s: Final Overall MPI Ocore Read/Write Stats:\n",prefix);
      grvy_printf(info,"\n");
      grvy_printf(info,"%s:   --> Number of Records Written = %15i\n",prefix,total_written);
      grvy_printf(info,"%s:   --> Total Data Written        = %15.5e GBs\n",prefix,
		  1.0*total_written*blocksize*word_size/(1024*1024*1024));
      grvy_printf(info,"%s:   --> Aggregate Write Speed     = %15.5e MB/sec\n",prefix,
		  aggr_write_speed/(1024*1024));

      grvy_printf(info,"\n");
      grvy_printf(info,"%s:   --> Number of Records Read    = %15i\n",prefix,total_read);
      grvy_printf(info,"%s:   --> Total Data Read           = %15.5e GBs\n",prefix,
		  1.0*total_read*blocksize*word_size/(1024*1024*1024));
      grvy_printf(info,"%s:   --> Aggregate Read Speed      = %15.5e MB/sec\n",prefix,
		  aggr_read_speed/(1024*1024));
      grvy_printf(info,"\n");

    }

  // ----------------
  // Disk-based Usage
  // ----------------

  int    local_disk_written = 0;    // values local to each ocore slave
  int    local_disk_read    = 0;
  double local_disk_wspeed  = 0.0;
  double local_disk_rspeed  = 0.0;

  int    *all_disk_written;	    // results to be gathered on ocore master
  int    *all_disk_read;
  double *all_disk_wspeed;
  double *all_disk_rspeed;

  if(master)
    {
      all_disk_written = new    int[mpi_nprocs];
      all_disk_read    = new    int[mpi_nprocs];
      all_disk_wspeed  = new double[mpi_nprocs];
      all_disk_rspeed  = new double[mpi_nprocs];
    }
  else
    {
      if(overflow_triggered)
	{
	  local_disk_written = ptimer.StatsCount    ("write_to_disk");
	  local_disk_wspeed  = local_disk_written*blocksize*word_size/ptimer.ElapsedSeconds("write_to_disk");

	  local_disk_read    = ptimer.StatsCount    ("read_from_disk");
	  local_disk_rspeed  = local_disk_read*blocksize*word_size/ptimer.ElapsedSeconds("read_from_disk");
	}
    }

  MPI_Gather(&local_disk_written,1,MPI_INTEGER,all_disk_written,1,MPI_INTEGER,0,MYCOMM);
  MPI_Gather(&local_disk_read   ,1,MPI_INTEGER,all_disk_read   ,1,MPI_INTEGER,0,MYCOMM);

  MPI_Gather(&local_disk_wspeed ,1,MPI_DOUBLE, all_disk_wspeed ,1,MPI_DOUBLE ,0,MYCOMM);
  MPI_Gather(&local_disk_rspeed ,1,MPI_DOUBLE, all_disk_rspeed ,1,MPI_DOUBLE ,0,MYCOMM);

  if(master)
    {
      bool disk_overflow_triggered = false;

      for(int i=1;i<mpi_nprocs;i++)
	if(all_disk_written[i] > 0)
	  {
	    disk_overflow_triggered = true;
	    break;
	  }

      if(disk_overflow_triggered)
	{

	  int sum_disk_writes = 0;
	  int sum_disk_reads  = 0;

	  grvy_printf(info,"%s: Disk-based transactions for MPI Ocore Overflow:\n\n",prefix);
	  for(int i=1;i<mpi_nprocs;i++)
	    {
	      sum_disk_writes += all_disk_written[i];
	      sum_disk_reads  += all_disk_read[i];

	      grvy_printf(info,"%s:   --> Proc %5i - Written to Disk = %12.5e GBs; Avg = %12.5e MB/sec\n",prefix,i,
			  1.0*all_disk_written[i]*blocksize*word_size/(1024*1024*1024),all_disk_wspeed[i]/(1024*1024));
	    }

	  for(int i=1;i<mpi_nprocs;i++)
	    grvy_printf(info,"%s:   --> Proc %5i - Read from Disk  = %12.5e GBs; Avg = %12.5e MB/sec\n",prefix,i,
			1.0*all_disk_read[i]*blocksize*word_size/(1024*1024*1024),all_disk_rspeed[i]/(1024*1024));

	  grvy_printf(info,"\n");
	  grvy_printf(info,"%s:   --> %9i of %9i total write transactions were disk based (%5.2f %)\n",prefix,
		      sum_disk_writes,total_written,100.0*sum_disk_writes/total_written);
	  grvy_printf(info,"%s:   --> %9i of %9i total read  transactions were disk based (%5.2f %)\n",prefix,
		      sum_disk_reads,total_read,100.0*sum_disk_reads/total_read);
			
	}
      else
	grvy_printf(info,"%s: No disk-based overflow transactions required\n",prefix);
    }

  // -------------
  // Empty reads
  // -------------

  if(allow_empty_records && master && (num_empty_reads > 0) )
    {
      grvy_printf(info,"\n%s: Empty Record Access:\n",prefix);
      grvy_printf(info,"%s:   --> Number of empty reads     = %12i\n",prefix,num_empty_reads);
    }

  // -------------
  // Memory Usage
  // -------------

  int *num_active_per_task;

  if(master)
    {
      num_active_per_task = new int[mpi_nprocs];
      MPI_Gather(&num_active_records,1,MPI_INTEGER,num_active_per_task,1,MPI_INTEGER,0,MYCOMM);

      grvy_printf(info,"\n%s: MPI Memory Consumption:\n",prefix);

      double usage;

      for(int i=1;i<mpi_nprocs;i++)
	{
	  if(all_disk_written[i] > 0)       // overflow enabled, report peak memory used
	    usage = max_poolsize_MBs;
	  else			            // overflow avoided, report current usage
	    usage = 1.0*num_active_per_task[i]*blocksize*word_size/(1024*1024);
	    
	  grvy_printf(info,"%s:   --> Proc %5i - Memory Used = %12.5e MBs (Max Req. = %i MBs)\n",prefix,i,
		      usage,max_poolsize_MBs);
	}

      grvy_printf(info,"\n---------------------------------------------------------------------------------------------\n");

      delete [] all_disk_written;
      delete [] all_disk_read;
      delete [] all_disk_wspeed;
      delete [] num_active_per_task;
    }
  else
    MPI_Gather(&num_active_records,1,MPI_INTEGER,num_active_per_task,1,MPI_INTEGER,0,MYCOMM);
    

  return;
}

// ---------------------------------------------------------------------------------------
// Write_to_Pool(): Internal write method - use MPI to xfer desired data
// ---------------------------------------------------------------------------------------

template <typename T> int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: Write_to_Pool(int dest_task, bool new_data, 
											size_t offset, T *data)
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

    grvy_printf(debug,"%s (%5i): Performing mpi_bcast to prep rank %i\n",prefix,mpi_rank,dest_task);

    MPI_Bcast(hbuf,4,MPI_INTEGER,0,MYCOMM);

    // Perform the write (sync for now)

    grvy_printf(debug,"%s (%5i): Performing mpi_send to rank %i\n",prefix,mpi_rank,dest_task);

    //MPI_Send(data,blocksize,MPI_DOUBLE,dest_task,sendtag,MYCOMM);
    MPI_Send(data,blocksize,GRVY_Internal::Get_MPI_Type(data[0]),dest_task,sendtag,MYCOMM);

    ptimer.EndTimer("write_to_pool");

    return 0;
  }

  // ---------------------------------------------------------------------------------------
  // Read_from_Pool(): Internal read method - use MPI to xfer desired data
  // ---------------------------------------------------------------------------------------

template <typename T> int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: Read_from_Pool(int recv_task, size_t offset,
											 T *data)
  {
    MPI_Status status;
    ptimer.BeginTimer("read_from_pool");

    // create msg header

    int hbuf[4] = {OCORE_READ,recv_task,offset,blocksize};

    // Notify children of upcoming read request

    grvy_printf(debug,"%s (%5i): Performing mpi_bcast to prep rank %i\n",prefix,mpi_rank,recv_task);

    MPI_Bcast(hbuf,4,MPI_INTEGER,0,MYCOMM);

    // Perform the read (sync for now)

    grvy_printf(debug,"%s (%5i): Performing mpi_recv from rank %i\n",prefix,mpi_rank,recv_task);

    MPI_Recv(data,blocksize,GRVY_Internal::Get_MPI_Type(data[0]),recv_task,recvtag,MYCOMM,&status);

    ptimer.EndTimer("read_from_pool");

    return 0;
  }

  // ---------------------------------------------------------------------------------------
  // Initialize(): Initialize data structures for MPI Ocore
  // ---------------------------------------------------------------------------------------

  int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp::Initialize(string input_file)
  {

    if(!use_mpi_ocore) return 1;

    // Initialize timer
    
    ptimer.Init("GRVY MPI_Ocore");

    if(master)
      grvy_printf(info,"\n%s: Initializing on %i processors...\n",prefix,mpi_nprocs);

    // ------------------
    // Read user options
    // ------------------

    int default_priority;
    GRVY_Input_Class iparse;      // Input parsing object
    int flag=1;
    int tmp_use_ocore;

    if(master)
      {
	grvy_printf(info,"%s: --> Parsing runtime options from file %s\n",prefix,input_file.c_str());
	
	default_priority = grvy_log_getlevel();
	grvy_log_setlevel(GRVY_NOLOG);    
	
	if(! iparse.Open(input_file.c_str()) )
	  {
	    grvy_log_setlevel(default_priority);
	    grvy_printf(info,"%s: --> Unable to open provided input file (%s)\n",prefix,input_file.c_str());
	    grvy_printf(info,"%s: --> Disabling MPI ocore\n",prefix);
	    tmp_use_ocore = 0;
	  }
	else
	  {
	    iparse.Register_Var ("grvy/mpi_ocore/enable_ocore",0);
	    iparse.Read_Var     ("grvy/mpi_ocore/enable_ocore",&tmp_use_ocore);
	  }

	grvy_log_setlevel(default_priority);
	fflush(NULL);
      }

    MPI_Bcast(&tmp_use_ocore,1,MPI_LOGICAL, 0,MYCOMM);

    use_mpi_ocore = (tmp_use_ocore == 1) ? true : false;
    
    if(!use_mpi_ocore)	// don't use ocore at user's request
      {
	if(master)
	  grvy_log_setlevel(default_priority);

	return 1;
      }
    
    if(master)
      {
	// Register default values (0.32.0)

	iparse.Register_Var    ("grvy/mpi_ocore/statsfile",           "grvy_ocore_stats.dat");

	// Register default values (0.31.0)

	iparse.Register_Var    ("grvy/mpi_ocore/use_disk_overflow",    1                    );
	iparse.Register_Var    ("grvy/mpi_ocore/allow_empty_records",  0                    );
	iparse.Register_Var    ("grvy/mpi_ocore/watermark_ratio",      dump_watermark_ratio );
	iparse.Register_Var    ("grvy/mpi_ocore/max_pool_size_in_mbs", max_poolsize_MBs     );
	iparse.Register_Var    ("grvy/mpi_ocore/max_map_size_in_mbs",  max_mapsize_MBs      );
	iparse.Register_Var    ("grvy/mpi_ocore/blocksize",            blocksize            );
	
	// Read any user-provided inputs
	
	int tmp_use_overflow;
	int tmp_allow_empties;

	flag *= iparse.Read_Var("grvy/mpi_ocore/use_disk_overflow",    &tmp_use_overflow    );
	flag *= iparse.Read_Var("grvy/mpi_ocore/allow_empty_records",  &tmp_allow_empties   );
	flag *= iparse.Read_Var("grvy/mpi_ocore/watermark_ratio",      &dump_watermark_ratio);
	flag *= iparse.Read_Var("grvy/mpi_ocore/max_pool_size_in_mbs", &max_poolsize_MBs    );
	flag *= iparse.Read_Var("grvy/mpi_ocore/max_map_size_in_mbs",  &max_mapsize_MBs     );
	flag *= iparse.Read_Var("grvy/mpi_ocore/blocksize",            &blocksize           );
	flag *= iparse.Read_Var("grvy/mpi_ocore/dump_raw_statistics",  &dump_stats          ,true);

	if(dump_stats)
	  flag *= iparse.Read_Var("grvy/mpi_ocore/statsfile",          &statsfile           );	  

	use_disk_overflow   = (tmp_use_overflow  == 1) ? true : false ;
	allow_empty_records = (tmp_allow_empties == 1) ? true : false ;
	
	if(flag == 0)
	  {
	    grvy_printf(error,"%s: --> Error parsing grvy mpi_ocore inputs...aborting\n",prefix);
	    Abort();
	  }

	iparse.Close();
	
	// Input sanity check
	
	assert(dump_watermark_ratio > 0.0 && dump_watermark_ratio < 1.0);
	assert(max_poolsize_MBs > 0);
	assert(max_mapsize_MBs  > 0);
	assert(blocksize        > 0);

	// restore original logging priority

	grvy_log_setlevel(default_priority);
      }
    
    // Bcast inputs to all ocore children

    MPI_Bcast(&dump_watermark_ratio,1,MPI_DOUBLE, 0,MYCOMM);
    MPI_Bcast(&max_poolsize_MBs,    1,MPI_INTEGER,0,MYCOMM);
    MPI_Bcast(&max_mapsize_MBs,     1,MPI_INTEGER,0,MYCOMM);
    MPI_Bcast(&blocksize,           1,MPI_INTEGER,0,MYCOMM);

    int tmp_dump_stats = (dump_stats == true) ? 1 : 0 ;

    MPI_Bcast(&tmp_dump_stats,      1,MPI_INTEGER,0,MYCOMM);
    dump_stats = (tmp_dump_stats == 1) ? true : false ;   

    int   tmp_string_size = statsfile.size()+1;
    char *tmp_string      = NULL;

    MPI_Bcast(&tmp_string_size,     1,MPI_INTEGER,0,MYCOMM);

    tmp_string = (char *)calloc(tmp_string_size,sizeof(char));
    
    strcpy(tmp_string,statsfile.c_str());
    MPI_Bcast(tmp_string,tmp_string_size,MPI_CHAR,0,MYCOMM);
    
    if(!master)
      statsfile = tmp_string;

    free(tmp_string);

    num_smap_records = max_mapsize_MBs*1024*1024/sizeof(int);
    max_num_records  = max_poolsize_MBs*1024*1024/(blocksize*word_size);

    if(master)
      {
	grvy_printf(info,"\n---------------------------------------------------------------------------------------------\n");
	grvy_printf(info,"%s: Final Overall MPI Ocore Read/Write Stats:\n",prefix);
	grvy_printf(info,"\n%s: Runtime Options:\n",prefix);
	grvy_printf(info,"%s: --> Individual record word size         = %15i (Bytes )\n",prefix,word_size);
	grvy_printf(info,"%s: --> Individual record blocksize         = %15i (Words )\n",prefix,blocksize);
	grvy_printf(info,"\n");
	grvy_printf(info,"%s: --> Max sparse map memory size          = %15i (MBytes)\n",prefix,max_mapsize_MBs );
	grvy_printf(info,"%s: --> MPI ramdisk memory size (per proc)  = %15i (MBytes)\n",prefix,max_poolsize_MBs);
	grvy_printf(info,"\n");
	grvy_printf(info,"%s: --> Max number of mappable records      = %15i\n",         prefix,num_smap_records);
	grvy_printf(info,"%s: --> Max number of ocore ramdisk records = %15i\n",         prefix,max_num_records );

	grvy_printf(info,"\n%s: --> Enable disk-based overflow          = %15i\n",       prefix,use_disk_overflow);
	if(use_disk_overflow)
	  grvy_printf(info,"%s: --> Overflow watermark ratio            = %15.3f\n",     prefix,dump_watermark_ratio);

	grvy_printf(info,"%s: --> Allow empty read records            = %15i\n",         prefix,allow_empty_records);
	grvy_printf(info,"\n");

	MPI_Barrier(MYCOMM);
      }
    else 			// children
      {

	MPI_Barrier(MYCOMM);

	// -------------------------------------------
	// Setup scratch space for file-based overflow
	// -------------------------------------------
	
	if(use_disk_overflow)
	  {
	    num_active_disk_records = 0;

	    char dir_template[] = "/grvy_ocore_XXXXXX";
	    
	    tmp_unique_path = (char *)malloc(tmpdir.size()+strlen(dir_template)+1);
	    assert(tmp_unique_path != NULL);
	    
	    sprintf(tmp_unique_path,"%s%s",tmpdir.c_str(),dir_template);
	    
	    if(grvy_create_unique_dir(tmp_unique_path) != 0)
	      {
		grvy_printf(error,"%s (%5i): Unable to create local scratch directory\n",prefix,mpi_rank);
		Abort();
	      }
	    else
	      {
		string tmpfile(tmp_unique_path);
		tmpfile += "/ocore.tmp";
		fp_tmp = fopen(tmpfile.c_str(),"wb+");
		if(fp_tmp == NULL)
		  {
		    grvy_printf(info,"%s (%5i): Unable to open file %s for ramdisk overflow\n",
				prefix,mpi_rank,tmpfile.c_str());
		    Abort();
		  }

		grvy_printf(info,"\n%s (%5i): File-based scratch space created for ocore overflow\n",prefix,mpi_rank);
		grvy_printf(info,"%s (%5i): --> local scratch file = %s\n\n",prefix,mpi_rank,tmpfile.c_str());
	      }
	  }
	else
	  grvy_printf(info,"\n%s(%5i): File-based scratch space disabled for ocore overflow\n",prefix,mpi_rank);

	// -------------------------------
	// allocation for raw storage pool
	// -------------------------------
	
	// \todo: allow for non-fixed size pool

	grvy_printf(debug,"%s (%5i): Attempting allocation of %i records\n",prefix,mpi_rank,blocksize);
#if 1
	try 
	  {
	    vector<double> data(blocksize);
	    
	    for(int i=0;i<max_num_records;i++)
	      pool.push_back(data);

	    if(use_disk_overflow)
	      data_tmp = new double[blocksize];
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

    fflush(NULL);
    MPI_Barrier(MYCOMM);

      
    if(master)
      grvy_printf(info,"\n---------------------------------------------------------------------------------------------\n");
    
    fflush(NULL);

    overflow_triggered = false;
    initialized        = true;    
    
    // Put children tasks to work in polling mode
    
    if(!master)
      PollForWork();
    
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
    //    owner.write_count = 1;
    //    owner.read_count  = 0;
    
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

    grvy_printf(debug,"\n%s: Assigned task ownership for new record %i to task %i\n",prefix,sparse_index,distrib_rank);

    return(distrib_rank);
  }

// ---------------------------------------------------------------------------------------
// StoreData(): Recv and store ocore data from rank 0
// ---------------------------------------------------------------------------------------

int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp::StoreData(size_t sparse_index, bool new_data)
{

  MPI_Status status;

  if(!initialized)
    {
      grvy_printf(info,"%s (%5i): Ocore must be initialized prior to storing data\n",prefix,mpi_rank);
      Abort();
    }

  assert(mpi_rank > 0);

  map<size_t,MPI_Ocore_datagram>:: iterator it = smap.find(sparse_index);
  size_t offset;
  bool   in_mem;

  if(it == smap.end())	// new record being written (always written to ramdisk)
    {

      MPI_Ocore_datagram datagram;	
      datagram.in_mem      = true;
      datagram.index       = GetFreeRecord(sparse_index);
      datagram.write_count = 1;
      datagram.read_count  = 0;

      offset               = datagram.index;
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

      smap[sparse_index].write_count++;
    }

  grvy_printf(debug,"%s (%5i): Prepping to recv data (user index %i -> ocore index %i)\n",prefix,mpi_rank,
	      sparse_index,offset);

  if(in_mem)			// save record in ramdisk
    {
      MPI_Recv(&pool[offset][0],blocksize,MPI_DOUBLE,0,sendtag,MYCOMM,&status);
      grvy_printf(debug,"%s (%5i): Data received into memory pool\n",prefix,mpi_rank);
    }
  else			// save record to disk
    {
      MPI_Recv(data_tmp,blocksize,MPI_DOUBLE,0,sendtag,MYCOMM,&status);

      ptimer.BeginTimer("write_to_disk");

      size_t disk_offset = offset*blocksize*word_size;

      fseek(fp_tmp,disk_offset,SEEK_SET);
      fwrite(data_tmp,word_size,blocksize,fp_tmp);

      ptimer.EndTimer("write_to_disk");

      grvy_printf(debug,"%s (%5i): Data received into disk pool\n",prefix,mpi_rank);
    }
    
  return(0);
}

// ---------------------------------------------------------------------------------------
// PullData(): Read and send ocore data record to rank 0
// ---------------------------------------------------------------------------------------

int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: PullData(size_t sparse_index)
{

  MPI_Status status;

  grvy_printf(debug,"%s (%5i): Entering PullData()\n",prefix,mpi_rank);

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

  smap[sparse_index].read_count++;

  grvy_printf(debug,"%s (%5i): Prepping to send data (user index %i -> ocore index %i)\n",prefix,mpi_rank,
	      sparse_index,offset);

  if(in_mem)		// record in ramdisk
    {
      MPI_Send(&pool[offset][0],blocksize,MPI_DOUBLE,0,recvtag,MYCOMM);
    }
  else			// record on disk, read first prior to sending
    {
      ptimer.BeginTimer("read_from_disk");

      size_t disk_offset = offset*blocksize*word_size;
      fseek(fp_tmp,disk_offset,SEEK_SET);

      assert( fread(data_tmp,word_size,blocksize,fp_tmp) == blocksize );

      ptimer.EndTimer("read_from_disk");

      MPI_Send(data_tmp,blocksize,MPI_DOUBLE,0,recvtag,MYCOMM);

    }
    
  return(0);
}

// --------------------------------------------------------------------------------
// DumptoDisk(): saves least frequently used data to disk
// --------------------------------------------------------------------------------

int GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: DumptoDisk()
{
  ptimer.BeginTimer("dump_priority");

  if(!overflow_triggered)
    overflow_triggered = true;

  // Build priority queue based on how frequently Ocore records have been read

  priority_queue< NodePriority,vector<NodePriority>, ComparePriority> q;

  map<size_t,MPI_Ocore_datagram> :: iterator it;   

  for(it = smap.begin(); it != smap.end(); it++)
    {
      if(!it->second.in_mem)	// only records in memory cache are eliglbe for dump-to-disk
	continue;

      NodePriority node;

      node.priority     = it->second.read_count;
      node.global_index = it->first;
      node.ocore_index  = it->second.index;

      q.push(node);
    }

  // Dump least-frequently used data to disk
    
  size_t num_to_dump = (size_t) 1.0*dump_watermark_ratio*q.size();

  grvy_printf(debug,"\n%s (%5i): Ocore memory cache full -> moving %li records to disk)\n",
	      prefix,mpi_rank,num_to_dump);

  for(size_t i=0;i<num_to_dump;i++)
    {
      ptimer.BeginTimer("write_to_disk");

      size_t disk_index = num_active_disk_records;
      size_t offset     = disk_index*blocksize*word_size;

      fseek(fp_tmp,offset,SEEK_SET);
      fwrite(&pool[q.top().ocore_index][0],word_size,blocksize,fp_tmp);

      ptimer.EndTimer("write_to_disk");

      // mark record as residing on disk and save local disk index

      smap[q.top().global_index].in_mem  = false;
      smap[q.top().global_index].index   = disk_index;

      // flag the now-unoccupied record

      free_records.push(q.top().ocore_index);

      // pop the priority stack and increment index counters

      num_active_disk_records++;
      num_active_records--;

      q.pop();
    }

  ptimer.EndTimer("dump_priority");

  return 0;
}

// --------------------------------------------------------------------------------
// GetFreeRecord(): return index to next free record location in
// memory cache. Make room for additional records by dumping
// less-frequently accessed records to disk.
// --------------------------------------------------------------------------------

size_t GRVY_MPI_Ocore_Class::GRVY_MPI_Ocore_ClassImp:: GetFreeRecord(size_t sparse_index)
{

  // increment assigned records until memory cache is exhausted;
  // after that, use unoccupied records freed up via DumptoDisk

  if(num_active_records < max_num_records )
    if(free_records.empty())
      return(num_active_records);
    else
      {
	size_t free_index = free_records.top();
	free_records.pop();
	return(free_index);
      }
  else
    {
      if(use_disk_overflow)
	DumptoDisk();
      else
	{
	  grvy_printf(error,"%s (%5i): Error: MPI ocore ramdisk is full and disk-based overflow is disabled\n",
		      prefix,mpi_rank);
	  grvy_printf(error,"%s (%5i):       Please increase amount of memory available for MPI Ocore storage\n",
		      prefix,mpi_rank);
	  Abort();
	}

      assert( !free_records.empty() );

      size_t free_index = free_records.top();
      free_records.pop();
      num_active_records++;
      return(free_index);
    }
    
  grvy_printf(error,"%s (%5i): Logic error in Ocore GetFreeRecord()\n",prefix,mpi_rank);
  Abort();
}

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

} // end namespace grvy

int    GRVY_MPI_Ocore_Class::Initialize(string input_file) { return 0; }

size_t GRVY_MPI_Ocore_Class::NumActive () { return 0;     }
void   GRVY_MPI_Ocore_Class::Finalize  () { return;       }
bool   GRVY_MPI_Ocore_Class::isMaster  () { return true;  }
bool   GRVY_MPI_Ocore_Class::isEnabled () { return false; }

template <typename T>    int GRVY_MPI_Ocore_Class::Write     (size_t offset, T *data) { return 0; }
template <typename T>    int GRVY_MPI_Ocore_Class::Read      (size_t offset, T *data) { return 0; }
template <typename T> size_t GRVY_MPI_Ocore_Class::PopRecord (T *data) { return 0; }

#endif


