#include<grvy.h>

using namespace std;
using namespace GRVY;

int main()
{
  GRVY_MPI_Ocore_Class ocore;

  string inputfile("myfile");

  ocore.Initialize(inputfile,8192);
  ocore.Finalize();

}
