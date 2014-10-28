/*****************************************************************************

        ReadWriteLockImpl.cpp
        Copyright (c) 2010 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 39647

        This file is part of Ohm Force Libraries.

        This program is free software; you can redistribute it and/or
        modify it under the terms of the GNU General Public License
        version 2 as published by the Free Software Foundation;

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program;
        if not, write to the Free Software Foundation, Inc.,
        59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*Tab=3***********************************************************************/



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/sys/win32/CritSectUtil.h"
#include "ohm/sys/win32/ReadWriteLockImpl.h"

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



const char *   ReadWriteLockImpl::init_mtx (int &count)
{
   const char *      err_msg_0 = 0;
   count = 0;

   while (err_msg_0 == 0 && count < Mtx_NBR_ELT)
   {
      err_msg_0 = CritSectUtil::init (_mtx_arr [count]);
      if (err_msg_0 == 0)
      {
         ++ count;
      }
   }

   return (err_msg_0);
}



void  ReadWriteLockImpl::restore_mtx (int count)
{
   assert (count >= 0);
   assert (count <= Mtx_NBR_ELT);

   for (int index = count - 1; index >= 0; -- index)
   {
      ::DeleteCriticalSection (&_mtx_arr [index]);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
