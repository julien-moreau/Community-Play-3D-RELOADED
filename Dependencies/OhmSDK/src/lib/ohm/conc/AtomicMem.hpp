/*****************************************************************************

        AtomicMem.hpp
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



#if ! defined (ohm_conc_AtomicMem_CODEHEADER_INCLUDED)
#define  ohm_conc_AtomicMem_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/def.h"
#include "ohm/sys/Interlocked.h"



namespace ohm
{
namespace conc
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <int SL2>
typename AtomicMem <SL2>::DataType  AtomicMem <SL2>::swap (volatile DataType &dest, DataType excg)
{
   ohm_lang_CHECK_CST (Size, (SL2 >= 0 && SL2 <= 2));

   return (sys::Interlocked::exchange_int32 (dest, excg));
}



template <int SL2>
typename AtomicMem <SL2>::DataType  AtomicMem <SL2>::cas (volatile DataType &dest, DataType excg, DataType comp)
{
   return (sys::Interlocked::compare_exchange_int32 (dest, excg, comp));
}



#if defined (ohm_archi_HAS_CAS_64)



AtomicMem <3>::DataType AtomicMem <3>::swap (volatile DataType &dest, DataType excg)
{
   return (sys::Interlocked::exchange_int64 (dest, excg));
}



AtomicMem <3>::DataType AtomicMem <3>::cas (volatile DataType &dest, DataType excg, DataType comp)
{
   return (sys::Interlocked::compare_exchange_int64 (dest, excg, comp));
}



#endif   // ohm_archi_HAS_CAS_64



#if defined (ohm_archi_HAS_CAS_128)



AtomicMem <4>::DataType AtomicMem <4>::swap (volatile DataType &dest, DataType excg)
{
   sys::Interlocked::Data128  old;

   sys::Interlocked::exchange_int128 (
      old,
      *reinterpret_cast <volatile sys::Interlocked::Data128 *> (&dest),
      *reinterpret_cast <const sys::Interlocked::Data128 *> (&excg)
   );

   const DataType result = *reinterpret_cast <DataType *> (&old);

   return (result);
}



AtomicMem <4>::DataType AtomicMem <4>::cas (volatile DataType &dest, DataType excg, DataType comp)
{
   sys::Interlocked::Data128  old;

   sys::Interlocked::compare_exchange_int128 (
      old,
      *reinterpret_cast <volatile sys::Interlocked::Data128 *> (&dest),
      *reinterpret_cast <const sys::Interlocked::Data128 *> (&excg),
      *reinterpret_cast <const sys::Interlocked::Data128 *> (&comp)
   );

   const DataType result = *reinterpret_cast <DataType *> (&old);

   return (result);
}



#endif   // ohm_archi_HAS_CAS_128



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace conc
}  // namespace ohm



#endif   // ohm_conc_AtomicMem_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
