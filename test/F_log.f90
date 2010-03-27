!--------------------------------------------------------------------------
! -------------------------------------------------------------------------
!
! Copyright (C) 2008,2009,2010 The PECOS Development Team
!
! Please see http://pecos.ices.utexas.edu for more information.
!
! This file is part of the PECOS HPC Toolkit (GRVY)
!
! GRVY is free software: you can redistribute it and/or modify
! it under the terms of the GNU General Public License as published by
! the Free Software Foundation, either version 3 of the License, or
! (at your option) any later version.
!
! GRVY is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU General Public License for more details.
!
! You should have received a copy of the GNU General Public License
! along with GRVY.  If not, see <http:!www.gnu.org/licenses/>.
!
! -------------------------------------------------------------------------
!
! GRVY Test Suite
!
! $Id$
! -------------------------------------------------------------------------
! -------------------------------------------------------------------------

program F_log
  use grvy
  implicit none

  write(*,'(a)')''
  write(*,'(a)')'Priority = ALL'
!  call flush(6)
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


