
#include<sys/utsname.h>
#include<grvy.h>
#include<stdio.h>
//#include<grvy_int.h>
#include<grvy_env.h>

GRVY_Hostenv_Class::GRVY_Hostenv_Class()
{
  struct utsname uts;

  uname (&uts);
  
  grvy_printf(GRVY_INFO,"nodename = %s\n",uts.nodename);
  grvy_printf(GRVY_INFO,"sysname  = %s\n",uts.sysname);
  grvy_printf(GRVY_INFO,"release  = %s\n",uts.release);
  grvy_printf(GRVY_INFO,"version  = %s\n",uts.version);
  
  return;
}



