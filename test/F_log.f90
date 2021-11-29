!!-----------------------------------------------------------------------bl-
!!--------------------------------------------------------------------------
!! 
!! libGRVY - a utility library for scientific computing.
!!
!! Copyright (C) 2008-2013,2018-2021 The PECOS Development Team
!! Additional Copyright (C) 2018 individual authors
!!
!! This library is free software; you can redistribute it and/or
!! modify it under the terms of the Version 2.1 GNU Lesser General
!! Public License as published by the Free Software Foundation.
!!
!! This library is distributed in the hope that it will be useful,
!! but WITHOUT ANY WARRANTY; without even the implied warranty of
!! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
!! Lesser General Public License for more details.
!!
!! You should have received a copy of the GNU Lesser General Public
!! License along with this library; if not, write to the Free Software
!! Foundation, Inc. 51 Franklin Street, Fifth Floor, 
!! Boston, MA  02110-1301  USA
!!
!!-----------------------------------------------------------------------el-
!!
!! Regression/Test Suite for libGRVY.
!!
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

program F_log
  use grvy
  implicit none

  write(*,'(a)')''
  write(*,'(a)')'Priority = ALL'
  call grvy_log_setlevel(GRVY_ALL);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'Priority = DEBUG'
  call grvy_log_setlevel(GRVY_DEBUG);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'Priority = INFO'
  call grvy_log_setlevel(GRVY_INFO);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'Priority = WARN'
  call grvy_log_setlevel(GRVY_WARN);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'Priority = ERROR'
  call grvy_log_setlevel(GRVY_ERROR);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'Priority = FATAL'
  call grvy_log_setlevel(GRVY_FATAL);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'Priority = NOLOG'
  call grvy_log_setlevel(GRVY_NOLOG);
  call post_log_messages();

  ! Repeat with non-default logmask

  call grvy_log_setmask(GRVY_FATAL,"[*] Fatal: ");
  call grvy_log_setmask(GRVY_ERROR,"[*] Error: ");
  call grvy_log_setmask(GRVY_WARN, "[*]  Warn: ");
  call grvy_log_setmask(GRVY_INFO, "[*]  Info: ");
  call grvy_log_setmask(GRVY_DEBUG,"[*] Debug: ");

  write(*,'(a)')''
  write(*,'(a)')'Priority = ALL'
  call grvy_log_setlevel(GRVY_ALL);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'Priority = DEBUG'
  call grvy_log_setlevel(GRVY_DEBUG);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'Priority = INFO'
  call grvy_log_setlevel(GRVY_INFO);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'Priority = WARN'
  call grvy_log_setlevel(GRVY_WARN);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'Priority = ERROR'
  call grvy_log_setlevel(GRVY_ERROR);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'Priority = FATAL'
  call grvy_log_setlevel(GRVY_FATAL);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'Priority = NOLOG'
  call grvy_log_setlevel(GRVY_NOLOG);
  call post_log_messages();

  write(*,'(a)')''
  write(*,'(a)')'[End of Test]'

  stop
end program F_log

subroutine post_log_messages()
  use grvy
  implicit none

  call grvy_log(GRVY_FATAL,"a fine fatal message"    )
  call grvy_log(GRVY_ERROR,"an astute error message" )
  call grvy_log(GRVY_WARN, "a fabulous warn message")
  call grvy_log(GRVY_INFO, "a useful info message"  )
  call grvy_log(GRVY_DEBUG,"a clever debug message"  )

  return
end subroutine post_log_messages


