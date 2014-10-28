/*****************************************************************************

        ThreadSync.cpp
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 64466

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

#include "ohm/sys/win32/ThreadSyncImpl.h"
#include "ohm/sys/ThreadSync.h"

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



ThreadSync::ThreadSync ()
:  _impl_ptr (new ThreadSyncImpl)
{
   // Nothing
}



ThreadSync::~ThreadSync ()
{
   delete _impl_ptr;
   _impl_ptr = 0;
}



void  ThreadSync::signal (int cnt)
{
   assert (_impl_ptr != 0);
   assert (cnt >= 0);

   if (cnt > 0)
   {
      _impl_ptr->signal (cnt);
   }
}



bool  ThreadSync::wait (double timeout)
{
   assert (_impl_ptr != 0);

   return (_impl_ptr->wait (timeout));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
