/*****************************************************************************

        TimerReliable.cpp
        Copyright (c) 2010 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 43363

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

#include "ohm/sys/win32/TimerReliableImpl.h"
#include "ohm/sys/TimerReliable.h"

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#include <windows.h>
#include <mmsystem.h>

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



TimerReliable::TimerReliable ()
:  _impl_ptr (new TimerReliableImpl)
{
   _impl_ptr->_tgt_base_time = ::timeGetTime ();
}



TimerReliable::~TimerReliable ()
{
   delete _impl_ptr;
   _impl_ptr = 0;
}



double   TimerReliable::get () const
{
   const ::DWORD  now = ::timeGetTime ();
   const archi::UInt32  diff = now - _impl_ptr->_tgt_base_time;
   const double   time_s = static_cast <double> (diff) / 1000.0;

   return (time_s);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
