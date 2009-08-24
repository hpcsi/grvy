
module hpct
  use iso_c_binding
  interface

!      subroutine hpct_input_fopen(filename,flag) bind(C,name="hpct_input_fopen")
!        import c_int
!        character :: filename
!        integer(c_int),value :: flag
!      end subroutine hpct_input_fopen

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

      subroutine hpct_input_fread_real(var,value.flag)
        character :: var
        real      :: value
        integer   :: flag
      end subroutine hpct_input_fread_real

  end interface
end module hpct
