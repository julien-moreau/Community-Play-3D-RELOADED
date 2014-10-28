/*****************************************************************************

        Mutex.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45149

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

#include "ohm/sys/Mutex.h"
#include "ohm/lang/def.h"

#include <stdexcept>

#include <pthread.h>
#include <errno.h>
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

Mutex::Mutex ()
{
   ohm_lang_CHECK_CST (
      impl_is_too_small,
      sizeof (pthread_mutex_t) <= sizeof (_impl)
   );
   
   ::pthread_mutex_t & mutex =
      reinterpret_cast < ::pthread_mutex_t &> (_impl);
   
   int err = pthread_mutex_init (&mutex, 0);

   assert (err != EINVAL); // invalid 'mutex'
   assert (err != ENOMEM); // not enough memory
   assert (err != EAGAIN); // lack of resources
   assert (err == 0);
   
   if (err != 0)
   {
      throw std::runtime_error (
         "ohm::sys::Mutex::Mutex(): Cannot create mutex."
      );
   }
}



/*
==============================================================================
Name : ctor
Description :
   Create a new critical section for the new object, not owned by any thread.
==============================================================================
*/

Mutex::Mutex (const Mutex & /*other*/)
{
   ::pthread_mutex_t & mutex =
      reinterpret_cast < ::pthread_mutex_t &> (_impl);
   
   int err = pthread_mutex_init (&mutex, 0);

   assert (err != EINVAL); // invalid 'mutex'
   assert (err != ENOMEM); // not enough memory
   assert (err != EAGAIN); // lack of resources
   assert (err == 0);
   
   if (err != 0)
   {
      throw std::runtime_error (
         "ohm::sys::Mutex::Mutex(const Mutex &): Cannot create mutex."
      );
   }
}



/*
==============================================================================
Name : operator =
Description :
   Keep critical section, ownership and reference counting.
==============================================================================
*/

Mutex &  Mutex::operator = (const Mutex & /*other*/)
{
   // Nothing to do...

   return (*this);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Mutex::~Mutex ()
{
   ::pthread_mutex_t & mutex =
      reinterpret_cast < ::pthread_mutex_t &> (_impl);
   
   int err = pthread_mutex_destroy (&mutex);
   
   assert (err != EINVAL); // invalid 'mutex'
   assert (err != EBUSY);  // 'mutex' is locked in another thread
   assert (err == 0);
}



/*
==============================================================================
Name : lock
==============================================================================
*/

void  Mutex::lock () const
{
   ::pthread_mutex_t & mutex =
      reinterpret_cast < ::pthread_mutex_t &> (_impl);
   
   int err = pthread_mutex_lock (&mutex);
   
   assert (err != EINVAL); // invalid 'mutex'
   assert (err != EDEADLK);// a deadlock would occur
   assert (err == 0);
   
   if (err != 0)
   {
      throw std::runtime_error (
         "ohm::sys::Mutex::lock(): Cannot lock mutex."
      );
   }
}



/*
==============================================================================
Name : unlock
==============================================================================
*/

void  Mutex::unlock () const
{
   ::pthread_mutex_t & mutex =
      reinterpret_cast < ::pthread_mutex_t &> (_impl);
   
   int err = pthread_mutex_unlock (&mutex);
   
   assert (err != EINVAL); // invalid 'mutex'
   assert (err != EPERM);  // current thread does not hold a lock on 'mutex'
   assert (err == 0);
   
   if (err != 0)
   {
      throw std::runtime_error (
         "ohm::sys::Mutex::unlock(): Cannot unlock mutex."
      );
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace sys
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
