
module hpct
  interface

     subroutine hpct_input_fopen(filename,flag)
       character :: filename
       integer   :: flag
     end subroutine hpct_input_fopen

     subroutine hpct_input_fclose()
     end subroutine hpct_input_fclose

     subroutine hpct_input_fdump(flag)
       integer   :: flag
     end subroutine hpct_input_fdump

     subroutine hpct_input_fdump_delim(prefix,flag)
       character :: prefix
       integer   :: flag
     end subroutine hpct_input_fdump_delim

     subroutine hpct_input_fdump_file(prefix,filename,flag)
       character :: prefix
       character :: filename
       integer   :: flag
     end subroutine hpct_input_fdump_file

  end interface
end module hpct
