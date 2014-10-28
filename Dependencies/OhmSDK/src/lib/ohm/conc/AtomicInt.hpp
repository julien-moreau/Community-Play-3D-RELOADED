/*****************************************************************************

        AtomicInt.hpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 28075

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



#if defined (ohm_conc_AtomicInt_CURRENT_CODEHEADER)
   #error Recursive inclusion of AtomicInt code header.
#endif
#define  ohm_conc_AtomicInt_CURRENT_CODEHEADER

#if ! defined (ohm_conc_AtomicInt_CODEHEADER_INCLUDED)
#define  ohm_conc_AtomicInt_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/fnc.h"
#include "ohm/sys/Interlocked.h"

#include <cassert>



namespace ohm
{
namespace conc
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
AtomicInt <T>::AtomicInt ()
:  _val ()
{
   assert (archi::is_ptr_aligned_nonzero ((const void *) (&_val), sizeof (_val)));
}



template <class T>
AtomicInt <T>::AtomicInt (T val)
:  _val ()
{
   assert (archi::is_ptr_aligned_nonzero ((const void *) (&_val), sizeof (_val)));
   StoredTypeWrapper::swap (_val, val);
}



template <class T>
AtomicInt <T>::AtomicInt (const AtomicInt <T> &other)
:  _val (other._val)
{
   assert (archi::is_ptr_aligned_nonzero ((const void *) (&_val), sizeof (_val)));
   assert (&other != 0);
}



template <class T>
AtomicInt <T> &   AtomicInt <T>::operator = (T other)
{
   StoredTypeWrapper::swap (_val, other);

   return (*this);
}



template <class T>
AtomicInt <T>::operator T () const
{
   return (T (_val));
}



template <class T>
T  AtomicInt <T>::swap (T other)
{
   return (T (StoredTypeWrapper::swap (_val, other)));
}



template <class T>
T  AtomicInt <T>::cas (T other, T comp)
{
   return (T (StoredTypeWrapper::cas (_val, other, comp)));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace conc
}  // namespace ohm



#endif   // ohm_conc_AtomicInt_CODEHEADER_INCLUDED

#undef ohm_conc_AtomicInt_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
