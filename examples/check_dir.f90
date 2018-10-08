!!-----------------------------------------------------------------------bl-
!!--------------------------------------------------------------------------
!! 
!! libGRVY - a utility library for scientific computing.
!!
!! Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
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
!! check_dir.f90: Example illustrating creation of parent directories for 
!!                output files via libGRVY via Fortran.
!!
!! $Id$
!!--------------------------------------------------------------------------
!!--------------------------------------------------------------------------

program main
  use grvy

  character :: filepath1*100 = './afile'
  character :: filepath2*100 = 'bfile'
  character :: filepath3*100 = 'adir/bdir/cfile'
  character :: filepath4*100 = '/tmp/moontower'
  character :: filepath5*100 = '../sneaky/adir/dfile'


  print*,'Fortran Rules'
  print*,' '
  print*,'Possibly creating parent directories for the following files'

  print*,'--> ',filepath1
  print*,'--> ',filepath2
  print*,'--> ',filepath3
  print*,'--> ',filepath4
  print*,'--> ',filepath5

  ! Create parent directories if necessary

  call grvy_check_file_path(filepath1);
  call grvy_check_file_path(filepath2);
  call grvy_check_file_path(filepath3);
  call grvy_check_file_path(filepath4);
  call grvy_check_file_path(filepath5);

  ! Write to the files

  open(unit=13,file=filepath1,status='unknown')
  write(13,*) '42'
  close(13)

  open(unit=13,file=filepath2,status='unknown')
  write(13,*) 'is'
  close(13)

  open(unit=13,file=filepath3,status='unknown')
  write(13,*) 'all'
  close(13)

  open(unit=13,file=filepath4,status='unknown')
  write(13,*) 'you'
  close(13)

  open(unit=13,file=filepath5,status='unknown')
  write(13,*) 'need'
  close(13)
  
  print*,' '
  print*,'SUCCESS'

stop
end program main
