
module hpct
!  use iso_c_binding
  interface

     ! ----------------------
     ! Input Parsing Support
     ! ----------------------
     
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

     subroutine hpct_input_fread_real(var,value,flag)
       character :: var
       real      :: value
       integer   :: flag
     end subroutine hpct_input_fread_real

     subroutine hpct_input_fread_double(var,value,flag)
       character :: var
       real*8    :: value
       integer   :: flag
     end subroutine hpct_input_fread_double

     subroutine hpct_input_fread_int(var,value,flag)
       character :: var
       integer   :: value
       integer   :: flag
     end subroutine hpct_input_fread_int

     subroutine hpct_input_fread_int_vec(var,value,nelems,flag)
       character :: var
       integer   :: value(:)
       integer   :: nelems
       integer   :: flag
     end subroutine hpct_input_fread_int_vec

     subroutine hpct_input_fread_int_ivec(var,value,elem,flag)
       character :: var
       integer   :: value
       integer   :: elem
       integer   :: flag
     end subroutine hpct_input_fread_int_ivec

     subroutine hpct_input_fread_real_vec(var,value,nelems,flag)
       character :: var
       real      :: value(:)
       integer   :: nelems
       integer   :: flag
     end subroutine hpct_input_fread_real_vec

     subroutine hpct_input_fread_real_ivec(var,value,elem,flag)
       character :: var
       real      :: value
       integer   :: elem
       integer   :: flag
     end subroutine hpct_input_fread_real_ivec

     subroutine hpct_input_fread_double_vec(var,value,nelems,flag)
       character :: var
       real*8    :: value(:)
       integer   :: nelems
       integer   :: flag
     end subroutine hpct_input_fread_double_vec

     subroutine hpct_input_fread_double_ivec(var,value,elem,flag)
       character :: var
       real*8    :: value
       integer   :: elem
       integer   :: flag
     end subroutine hpct_input_fread_double_ivec

     subroutine hpct_input_fread_char(var,value,flag)
       character :: var
       character :: value
       integer   :: flag
     end subroutine hpct_input_fread_char

     subroutine hpct_input_fread_char_ivec(var,value,elem,flag)
       character :: var
       character :: value
       integer   :: elem
       integer   :: flag
     end subroutine hpct_input_fread_char_ivec

     subroutine hpct_input_toggle_messages(flag)
       integer   :: flag
     end subroutine hpct_input_toggle_messages

     ! ---------------------------------
     ! Default Input Value Registrations
     ! ---------------------------------

     subroutine hpct_input_register_int(var,value,flag)
       character :: var
       integer   :: value
       integer   :: flag
     end subroutine hpct_input_register_int

     subroutine hpct_input_register_float(var,value,flag)
       character :: var
       real      :: value
       integer   :: flag
     end subroutine hpct_input_register_float

     subroutine hpct_input_register_double(var,value,flag)
       character :: var
       real*8    :: value
       integer   :: flag
     end subroutine hpct_input_register_double

     subroutine hpct_input_register_char(var,value,flag)
       character :: var
       character :: value
       integer   :: flag
     end subroutine hpct_input_register_char

     ! ---------------
     ! Timing Routines
     ! ---------------

     subroutine hpct_asci_time(timestring)
       character :: timestring
     end subroutine hpct_asci_time

     subroutine hpct_timer_init(id)
       character :: id
     end subroutine hpct_timer_init

     subroutine hpct_timer_reset()
     end subroutine hpct_timer_reset

     subroutine hpct_timer_finalize()
     end subroutine hpct_timer_finalize

     subroutine hpct_timer_summarize()
     end subroutine hpct_timer_summarize

     subroutine hpct_timer_begin(id)
       character :: id
     end subroutine hpct_timer_begin

     subroutine hpct_timer_end(id)
       character :: id
     end subroutine hpct_timer_end

     subroutine hpct_timer(value)
       real*8 :: value
     end subroutine hpct_timer

     subroutine hpct_timer_elapsed_global(value)
       real*8 :: value
     end subroutine hpct_timer_elapsed_global

!      subroutine hpct_input_fopen(filename,flag) bind(C,name="hpct_input_fopen")
!        import c_int
!        character :: filename
!        integer(c_int),value :: flag
!      end subroutine hpct_input_fopen

  end interface
end module hpct
