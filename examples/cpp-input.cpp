//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008-2013,2018-2021 The PECOS Development Team
// Additional Copyright (C) 2018 individual authors
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
// Simple usage example illustrating input file parsing in C++.
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<grvy.h>

using namespace GRVY;
using namespace std;

int main(int argc, char **argv)
{

  GRVY_Input_Class iparse;	// Input parsing object

  double reyn, mach, temp;
  float aoa,A[3];
  int   iter_max,turbulent, ikoomie;
  bool restart;
  std::string GridFile, RestartFile;
  std::string Key1, Key2;

  //----------------------------
  // Example input file parsing
  //----------------------------

  // Initialize/read the file 

  if(! iparse.Open("./pecos-input.txt"))
    exit(1);
  
  // Read specific variables (with no default values provided)

  if( iparse.Read_Var("reyn",&reyn,0.) )
    printf("--> %-11s = %f\n","reyn",reyn);

  if( iparse.Read_Var("mach",&mach) )
    printf("--> %-11s = %f\n","mach",mach);

  // Read variables with default values provided (note that the
  // default is only used if the keyword does not exist within the
  // input file; this provides a means to have backwards-compatibile
  // input file support).

  if ( iparse.Read_Var("temp",&temp,1200.) )
    printf("--> %-11s = %f\n","temp",temp);

  if( iparse.Read_Var("aoa",&aoa,0.0f) )
    printf("--> %-11s = %f\n","aoa",aoa);

  if( iparse.Read_Var("iter_max",&iter_max,100) )
    printf("--> %-11s = %i\n","iter_max",iter_max);

  if( iparse.Read_Var("gridfile",&GridFile) )
    cout << "--> gridfile    = " << GridFile << endl;

  std::string restart_flag("restart");

  if( iparse.Read_Var(restart_flag,&restart,false) )
    cout << "--> restart     = " << restart << endl;

  if(restart)
    {
      cout << endl << "Restart enabled - parsing desired restart file" << endl;
      if( iparse.Read_Var("restartfile",&RestartFile,"sol.h5") )
	cout << "--> restartfile = " << RestartFile << endl;
    }

  // Read from the solver subsection of input file

  if( iparse.Read_Var("solver/turbulence",&turbulent))
    printf("--> %-10s = %i\n","turbulent",turbulent);

  if( iparse.Read_Var_iVec("solver/keywords",&Key1,0))
    cout << "--> solver/keyword (#1) = " << Key1 << endl;

  if( iparse.Read_Var_iVec("solver/keywords",&Key2,1))
    cout << "--> solver/keyword (#2) = " << Key2 << endl;

  if( iparse.Read_Var_Vec("turbulence/A",A,3))
    printf("--> %-10s = %f %f %f\n","turbulence/A",A[0],A[1],A[2]);

  // Attempt to read unknown variable

  if( iparse.Read_Var("koomie",&ikoomie) )
    printf("--> %-11s = %i\n","koomie",ikoomie);

  // Disable error messages if you want to control them yourself. If
  // you now query the unknown variable again, stdout should be quiet

  grvy_printf(GRVY_INFO,"Switching to silent message mode...\n");
  grvy_log_setlevel(GRVY_NOLOG);

  if( iparse.Read_Var("koomie",&ikoomie))
    printf("--> %-11s = %i\n","koomie",ikoomie);

  grvy_log_setlevel(GRVY_INFO);
  grvy_printf(GRVY_INFO,"Switching back to standard INFO mode...\n");

  // Dump the whole file to stdout

  printf("\n ------ Full Dump ------\n\n");
  iparse.Fdump();
  printf(" ---- End Full Dump ----\n\n");

  // Dump the whole file to stdout with a delimiter

  printf("\n ------ Full Dump (delimited) ------\n\n");
  iparse.Fdump("# ");
  printf(" ---- End Full Dump ----\n\n");

  // Dump the whole file to a file (appends to file if it already exists)

  printf("\n ------ Full Dump to test.out ------\n\n");
  iparse.Fdump("% ","test.out");
  printf(" -------    End Full Dump    -------\n\n");

  // Close the file

  iparse.Close();

  return 0;
}
