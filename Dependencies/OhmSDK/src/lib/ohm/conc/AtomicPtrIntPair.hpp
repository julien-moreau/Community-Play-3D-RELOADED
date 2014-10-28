/*****************************************************************************

        AtomicPtrIntPair.hpp
        Copyright (c) 2009 Ohm Force

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



#if ! defined (ohm_conc_AtomicPtrIntPair_CODEHEADER_INCLUDED)
#define  ohm_conc_AtomicPtrIntPair_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/def.h"
#include "ohm/sys/Interlocked.h"



namespace ohm
{
namespace conc
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
AtomicPtrIntPair <T>::AtomicPtrIntPair ()
:  _data ()
{
   ohm_lang_CHECK_CST (DataSize, sizeof (RealContent) == sizeof (DataType));

   set (0, 0);
}


template <class T>
void  AtomicPtrIntPair <T>::set (T * ptr, ptrdiff_t val)
{
   _data._content._ptr = ptr;
   _data._content._val = val;
}



template <class T>
void  AtomicPtrIntPair <T>::get (T * &ptr, ptrdiff_t &val) const
{
   assert (&ptr != 0);
   assert (&val != 0);

   Combi          res;
   Combi          old;
   do
   {
      res = _data;
      cas_combi (old, const_cast <Combi &> (_data), _data, _data);
   }
   while (old._storage != res._storage);

   ptr = res._content._ptr;
   val = res._content._val;
}



template <class T>
T *   AtomicPtrIntPair <T>::get_ptr () const
{
   return (_data._content._ptr);
}



template <class T>
ptrdiff_t   AtomicPtrIntPair <T>::get_val () const
{
   return (_data._content._val);
}



template <class T>
bool  AtomicPtrIntPair <T>::cas (T *new_ptr, T *comp_ptr)
{
   T *            old_ptr = sys::Interlocked::compare_exchange_ptr (
      _data._content._ptr,
      new_ptr,
      comp_ptr
   );

   return (old_ptr == comp_ptr);
}



template <class T>
bool  AtomicPtrIntPair <T>::cas2 (T *new_ptr, ptrdiff_t new_val, T *comp_ptr, ptrdiff_t comp_val)
{
   Combi          newx;
   newx._content._ptr = new_ptr;
   newx._content._val = new_val;

   Combi          comp;
   comp._content._ptr = comp_ptr;
   comp._content._val = comp_val;

   Combi          old;
   cas_combi (old, _data, newx, comp);

   return (old._storage == comp._storage);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
void  AtomicPtrIntPair <T>::cas_combi (Combi &old, Combi &dest, const Combi &excg, const Combi &comp)
{
#if (ohm_archi_WORD_SIZE == 64)

   sys::Interlocked::compare_exchange_int128 (
      old._storage,
      dest._storage,
      excg._storage,
      comp._storage
   );

#else    // ohm_archi_WORD_SIZE

   old._storage = sys::Interlocked::compare_exchange_int64 (dest._storage, excg._storage, comp._storage);

#endif   // ohm_archi_WORD_SIZE
}



}  // namespace conc
}  // namespace ohm



#endif   // ohm_conc_AtomicPtrIntPair_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
