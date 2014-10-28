/*****************************************************************************

        Mutex.cpp
        Copyright (c) 2002 Ohm Force

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
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/def.h"
#include "ohm/sys/win32/CritSectUtil.h"
#include "ohm/sys/win32/MutexImpl.h"
#include "ohm/sys/Mutex.h"
#include "ohm/util/trace.h"

#include <stdexcept>

#include <cassert>
#include <cstdio>
#include <cstring>



namespace std {}

namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// Throws std::runtime_error
Mutex::Mutex ()
:  _impl ()
{
   ohm_lang_CHECK_CST (ImplSize, sizeof (MutexImpl) <= sizeof (_impl));

   MutexImpl *    impl_ptr = reinterpret_cast <MutexImpl *> (&_impl [0]);
   const char *   err_string_0 = CritSectUtil::init (impl_ptr->_mutex);
   if (err_string_0 != 0)
   {
      throw std::runtime_error (err_string_0);
   }
   impl_ptr->_lock_flag = false;
}



// Create a new critical section for the new object, not owned by any thread.
// Throws std::runtime_error
Mutex::Mutex (const Mutex &other)
:  _impl ()
{
   MutexImpl *    impl_ptr = reinterpret_cast <MutexImpl *> (&_impl [0]);
   const char *   err_string_0 = CritSectUtil::init (impl_ptr->_mutex);
   if (err_string_0 != 0)
   {
      throw std::runtime_error (err_string_0);
   }
   impl_ptr->_lock_flag = false;
}



// Keep critical section, ownership and reference counting.
Mutex &  Mutex::operator = (const Mutex &other)
{
   // Nothing to do...

   return (*this);
}



Mutex::~Mutex ()
{
   MutexImpl *    impl_ptr = reinterpret_cast <MutexImpl *> (&_impl [0]);
   ::DeleteCriticalSection (&impl_ptr->_mutex);
}



void  Mutex::lock () const
{
   MutexImpl *    impl_ptr = reinterpret_cast <MutexImpl *> (&_impl [0]);
   ::EnterCriticalSection (&impl_ptr->_mutex);
   if (impl_ptr->_lock_flag)
   {
      ohm_util_TRACE_ERROR ("lock(): Mutex already locked by this thread.");
      throw std::runtime_error (
         "ohm::sys::Mutex::lock(): Mutex already locked by this thread."
      );
   }
   impl_ptr->_lock_flag = true;
}



void  Mutex::unlock () const
{
   MutexImpl *    impl_ptr = reinterpret_cast <MutexImpl *> (&_impl [0]);
   if (! impl_ptr->_lock_flag)
   {
      ohm_util_TRACE_ERROR ("unlock(): Mutex not locked.");
      throw std::runtime_error (
         "ohm::sys::Mutex::unlock(): Mutex not locked."
      );
   }
   impl_ptr->_lock_flag = false;
   ::LeaveCriticalSection (&impl_ptr->_mutex);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
