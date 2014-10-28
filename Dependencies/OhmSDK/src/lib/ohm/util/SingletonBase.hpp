/*****************************************************************************

        SingletonBase.hpp
        Copyright (c) 2005 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 49577

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



#if defined (ohm_util_SingletonBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of SingletonBase code header.
#endif
#define  ohm_util_SingletonBase_CURRENT_CODEHEADER

#if ! defined (ohm_util_SingletonBase_CODEHEADER_INCLUDED)
#define  ohm_util_SingletonBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/sys/CritSec.h"
#include "ohm/sys/Mutex.h"

#include <cassert>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
typename SingletonBase <T>::ObjectType &  SingletonBase <T>::use_instance ()
{
   // First check
   if (! _singleton_init_flag)
   {
      // Ensure serialization (guard constructor acquires mutex_new).
      static sys::Mutex    mutex_new;
      sys::CritSec   guard (mutex_new);

      // Double check.
      if (! _singleton_init_flag)
      {
         assert (! _singleton_init_flag && _singleton_aptr.get () == 0);
         _singleton_aptr = ObjectAPtr (new ObjectType);
         _singleton_init_flag = true;
      }

      // guard destructor releases mutex_new.
   }

   return (*_singleton_aptr);
}



// Not thread safe. Client has to make sure that no external access to the
// singleton is done during the call.
template <class T>
void  SingletonBase <T>::force_destroy ()
{
   _singleton_init_flag = false;
   _singleton_aptr.reset (0);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
SingletonBase <T>::SingletonBase ()
{
   // Nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
typename SingletonBase <T>::ObjectAPtr SingletonBase <T>::_singleton_aptr;

template <class T>
bool  SingletonBase <T>::_singleton_init_flag = false;



}  // namespace util
}  // namespace ohm



#endif   // ohm_util_SingletonBase_CODEHEADER_INCLUDED

#undef ohm_util_SingletonBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
