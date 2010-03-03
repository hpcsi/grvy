!--------------------------------------------------------------------------
! -------------------------------------------------------------------------
!
! Copyright (C) 2008,2009,2010 The PECOS Development Team
!
! Please see http://pecos.ices.utexas.edu for more information.
!
! This file is part of the PECOS HPC Toolkit (HPCT)
!
! HPCT is free software: you can redistribute it and/or modify
! it under the terms of the GNU General Public License as published by
! the Free Software Foundation, either version 3 of the License, or
! (at your option) any later version.
!
! HPCT is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU General Public License for more details.
!
! You should have received a copy of the GNU General Public License
! along with HPCT.  If not, see <http:!www.gnu.org/licenses/>.
!
! -------------------------------------------------------------------------
!
! HPCT Test Suite
!
! $Id$
! -------------------------------------------------------------------------
! -------------------------------------------------------------------------

program F_log
  use hpct
  implicit none

  write(*,*)
  write(*,'(a)')'Priority = ALL'
!  call flush(6)
  call hpct_log_setlevel(HPCT_ALL);
  call post_log_messages();

  write(*,*)
  write(*,'(a)')'Priority = DEBUG'
  call hpct_log_setlevel(HPCT_DEBUG);
  call post_log_messages();

  write(*,*)
  write(*,'(a)')'Priority = INFO'
  call hpct_log_setlevel(HPCT_INFO);
  call post_log_messages();

  write(*,*)
  write(*,'(a)')'Priority = WARN'
  call hpct_log_setlevel(HPCT_WARN);
  call post_log_messages();

  write(*,*)
  write(*,'(a)')'Priority = ERROR'
  call hpct_log_setlevel(HPCT_ERROR);
  call post_log_messages();

  write(*,*)
  write(*,'(a)')'Priority = FATAL'
  call hpct_log_setlevel(HPCT_FATAL);
  call post_log_messages();

  write(*,*)
  write(*,'(a)')'Priority = NOLOG'
  call hpct_log_setlevel(HPCT_NOLOG);
  call post_log_messages();

  write(*,*)
  write(*,'(a)')'[End of Test]'

  return
end program F_log

subroutine post_log_messages()
  use hpct
  implicit none

  call hpct_log(HPCT_FATAL,"a fine fatal message"    )
  call hpct_log(HPCT_ERROR,"an astute error message" )
  call hpct_log(HPCT_WARN, "a fabulous warn message")
  call hpct_log(HPCT_INFO, "a useful info message"  )
  call hpct_log(HPCT_DEBUG,"a clever debug message"  )

  return
end subroutine post_log_messages


