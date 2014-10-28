/*****************************************************************************

        AtomicPtr.hpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 26453

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



#if defined (ohm_conc_AtomicPtr_CURRENT_CODEHEADER)
   #error Recursive inclusion of AtomicPtr code header.
#endif
#define  ohm_conc_AtomicPtr_CURRENT_CODEHEADER

#if ! defined (ohm_conc_AtomicPtr_CODEHEADER_INCLUDED)
#define  ohm_conc_AtomicPtr_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/fnc.h"
#include "ohm/sys/Interlocked.h"



namespace ohm
{
namespace conc
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
AtomicPtr <T>::AtomicPtr ()
:  _ptr ()
{
   assert (archi::is_ptr_aligned_nonzero ((const void *) (&_ptr), sizeof (_ptr)));
}



template <class T>
AtomicPtr <T>::AtomicPtr (T *ptr)
:  _ptr ()
{
   assert (archi::is_ptr_aligned_nonzero ((const void *) (&_ptr), sizeof (_ptr)));
   sys::Interlocked::exchange_ptr (reinterpret_cast <void * volatile &> (_ptr), ptr);
}



template <class T>
AtomicPtr <T> &   AtomicPtr <T>::operator = (T *other_ptr)
{
   sys::Interlocked::exchange_ptr (reinterpret_cast <void * volatile &> (_ptr), other_ptr);

   return (*this);
}



template <class T>
AtomicPtr <T>::operator T * () const
{
   return (read_ptr ());
}



template <class T>
bool  AtomicPtr <T>::operator == (T *other_ptr) const
{
   const T *      ptr = read_ptr ();

   return (ptr == other_ptr);
}



template <class T>
bool  AtomicPtr <T>::operator != (T *other_ptr) const
{
   return (! ((*this) == other_ptr));
}



template <class T>
T *   AtomicPtr <T>::swap (T *other_ptr)
{
   return (static_cast <T *> (sys::Interlocked::exchange_ptr (
      reinterpret_cast <void * volatile &> (_ptr),
      other_ptr
   )));
}



template <class T>
T *   AtomicPtr <T>::cas (T *other_ptr, T *comp_ptr)
{
   return (static_cast <T *> (sys::Interlocked::compare_exchange_ptr (
      reinterpret_cast <void * volatile &> (_ptr),
      other_ptr,
      comp_ptr
   )));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
T *   AtomicPtr <T>::read_ptr () const
{
   return (static_cast <T *> (_ptr));
}



}  // namespace conc
}  // namespace ohm



#endif   // ohm_conc_AtomicPtr_CODEHEADER_INCLUDED

#undef ohm_conc_AtomicPtr_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
