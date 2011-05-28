#include<grvy.h>

using namespace std;
using namespace GRVY;

int main()
{
  grvy_printf(GRVY_INFO,"hallo koomie\n");
  GRVY_MPI_Ocore_Class ocore;

  string inputfile("myfile");

  ocore.Initialize(inputfile,8192);

}
