/*****************************************************************************

        ReadWriteLock.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 64968

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

#include "ohm/sys/ReadWriteLock.h"
#include "ohm/lang/def.h"

#include <stdexcept>

#include <pthread.h>
#include <cassert>



namespace ohm
{
namespace sys
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ReadWriteLock::ReadWriteLock ()
{
   ohm_lang_CHECK_CST (
      MAX_IMPL_DATA_SIZE_is_too_small,
      sizeof (pthread_rwlock_t) <= MAX_IMPL_DATA_SIZE
   );
   
   ::pthread_rwlock_t & rwlock =
      reinterpret_cast < ::pthread_rwlock_t &> (_impl);
   
   int err = pthread_rwlock_init (&rwlock, 0);

   assert (err != EINVAL); // invalid 'mutex'
   assert (err != ENOMEM); // not enough memory
   assert (err != EAGAIN); // lack of resources
   assert (err != EPERM);  // no sufficient privilege
   assert (err != EBUSY);  // illegal reuse of referenced rwlock
   assert (err == 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ReadWriteLock::~ReadWriteLock ()
{
   ::pthread_rwlock_t & rwlock =
      reinterpret_cast < ::pthread_rwlock_t &> (_impl);
   
   int err = pthread_rwlock_destroy (&rwlock);
   
   assert (err != EINVAL); // invalid 'mutex'
   assert (err != EBUSY);  // 'mutex' is locked in another thread
   assert (err != EPERM);  // no sufficient privilege
   assert (err == 0);
}



/*
==============================================================================
Name : lock_read
==============================================================================
*/

void  ReadWriteLock::lock_read () const
{
   ::pthread_rwlock_t & rwlock =
      reinterpret_cast < ::pthread_rwlock_t &> (_impl);
   
   int err = pthread_rwlock_rdlock (&rwlock);
   
   assert (err != EAGAIN); // maximum number of reader reached
   assert (err != EINVAL); // invalid 'mutex'
   assert (err != EDEADLK);// a deadlock would occur
   assert (err != ENOMEM); // not enough memory
   assert (err == 0);
   
}



/*
==============================================================================
Name : unlock_read
==============================================================================
*/

void  ReadWriteLock::unlock_read () const
{
   ::pthread_rwlock_t & rwlock =
      reinterpret_cast < ::pthread_rwlock_t &> (_impl);
   
   int err = pthread_rwlock_unlock (&rwlock);
   
   assert (err != EINVAL); // invalid 'rwlock'
   assert (err != EPERM);  // current thread does not hold a lock on 'rwlock'
   assert (err == 0);
}



/*
==============================================================================
Name : lock_write
==============================================================================
*/

void  ReadWriteLock::lock_write () const
{
   ::pthread_rwlock_t & rwlock =
      reinterpret_cast < ::pthread_rwlock_t &> (_impl);
   
   int err = pthread_rwlock_wrlock (&rwlock);
   
   assert (err != EINVAL); // invalid 'mutex'
   assert (err != EDEADLK);// a deadlock would occur
   assert (err != ENOMEM); // not enough memory
   assert (err == 0);
   
}



/*
==============================================================================
Name : unlock_write
==============================================================================
*/

void  ReadWriteLock::unlock_write () const
{
   unlock_read ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
