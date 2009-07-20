//--------------------------------------------------------------------------
// -------------------------------------------------------------------------
//
// Copyright (C) 2008,2009 The PECOS Development Team
//
// Please see http://pecos.ices.utexas.edu for more information.
//
// This file is part of the PECOS HPC Toolkit (HPCT)
//
// HPCT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// HPCT is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HPCT.  If not, see <http://www.gnu.org/licenses/>.
//
// -------------------------------------------------------------------------
// hpct_messages: Routines for controlling output messages
//
// $Id$
//
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

using namespace std;

namespace HPCT {

static int _HPCT_Message_Mode = 0;
static int _HPCT_Message_Initialized = 0;

extern "C" void HPCT_initialize();
extern "C" void HPCT_message_mode(int mode);
extern "C" void HPCT_message(int mode, char *mask,char *message);

extern "C" void HPCT_message_mode(int mode)
{
  if(! _HPCT_Message_Initialized) HPCT_initialize();
  _HPCT_Message_Mode = mode;
  return;
}

extern "C" void HPCT_message(int mode, char *mask,char *message)
{
  if(! _HPCT_Message_Initialized) HPCT_initialize();
  if(mode > _HPCT_Message_Mode) return;

  printf("(%s): %s\n",mask,message);
  return;
}

extern "C" void HPCT_initialize()
{
  char *env_ctrl;
  env_ctrl = getenv("HPCT_MESSAGE_MODE");
  if(env_ctrl != NULL)
    _HPCT_Message_Mode = atoi(env_ctrl);

  _HPCT_Message_Initialized = 1;
}

}
