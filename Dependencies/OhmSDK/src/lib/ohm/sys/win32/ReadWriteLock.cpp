/*****************************************************************************

        ReadWriteLock.cpp
        Copyright (c) 2010 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 39814

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

#include "ohm/lang/def.h"
#include "ohm/sys/win32/ReadWriteLockImpl.h"
#include "ohm/sys/ReadWriteLock.h"

#include <stdexcept>

#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



ReadWriteLock::ReadWriteLock ()
{
   ohm_lang_CHECK_CST (ImplSize, sizeof (ReadWriteLockImpl) <= sizeof (_impl));

   ReadWriteLockImpl *   impl_ptr =
      reinterpret_cast <ReadWriteLockImpl *> (&_impl [0]);
   int            count = 0;
   const char *   err_string_0 = impl_ptr->init_mtx (count);

   if (err_string_0 != 0)
   {
      impl_ptr->restore_mtx (count);
      throw std::runtime_error (err_string_0);
   }

   impl_ptr->_count_r = 0;
   impl_ptr->_count_w = 0;
}



ReadWriteLock::~ReadWriteLock ()
{
   ReadWriteLockImpl *   impl_ptr =
      reinterpret_cast <ReadWriteLockImpl *> (&_impl [0]);
   impl_ptr->restore_mtx (ReadWriteLockImpl::Mtx_NBR_ELT);
}



void  ReadWriteLock::lock_read () const
{
   ReadWriteLockImpl *  impl_ptr =
      reinterpret_cast <ReadWriteLockImpl *> (&_impl [0]);

   ::EnterCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_3]);
   ::EnterCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_R]);
   ::EnterCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_1]);

   ++ impl_ptr->_count_r;
   if (impl_ptr->_count_r == 1)
   {
      ::EnterCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_W]);
   }

   ::LeaveCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_1]);
   ::LeaveCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_R]);
   ::LeaveCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_3]);
}



void  ReadWriteLock::lock_write () const
{
   ReadWriteLockImpl *  impl_ptr =
      reinterpret_cast <ReadWriteLockImpl *> (&_impl [0]);

   ::EnterCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_3]);
   ::EnterCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_2]);

   ++ impl_ptr->_count_w;
   if (impl_ptr->_count_w == 1)
   {
      ::EnterCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_R]);
   }

   ::LeaveCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_2]);
   ::LeaveCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_3]);

   ::EnterCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_W]);
}



void  ReadWriteLock::unlock_read () const
{
   ReadWriteLockImpl *  impl_ptr =
      reinterpret_cast <ReadWriteLockImpl *> (&_impl [0]);

   ::EnterCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_1]);

   -- impl_ptr->_count_r;
   if (impl_ptr->_count_r == 0)
   {
      ::LeaveCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_W]);
   }

   ::LeaveCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_1]);
}



void  ReadWriteLock::unlock_write () const
{
   ReadWriteLockImpl *  impl_ptr =
      reinterpret_cast <ReadWriteLockImpl *> (&_impl [0]);

   ::LeaveCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_W]);

   ::EnterCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_2]);
   -- impl_ptr->_count_w;
   if (impl_ptr->_count_w == 0)
   {
      ::LeaveCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_R]);
   }

   ::LeaveCriticalSection (&impl_ptr->_mtx_arr [ReadWriteLockImpl::Mtx_2]);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
