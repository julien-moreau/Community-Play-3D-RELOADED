/*****************************************************************************

        AtomicMem.h
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



#if ! defined (ohm_conc_AtomicMem_HEADER_INCLUDED)
#define  ohm_conc_AtomicMem_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/types.h"



namespace ohm
{
namespace conc
{



template <int SL2>
class AtomicMem
{
public:

   typedef  archi::Int32   DataType;
   ohm_archi_TYPEDEF_ALIGN (4, DataType, DataTypeAlign);

   ohm_archi_FORCEINLINE static DataType
                  swap (volatile DataType &dest, DataType excg);
   ohm_archi_FORCEINLINE static DataType
                  cas (volatile DataType &dest, DataType excg, DataType comp);

}; // class AtomicMem



#if defined (ohm_archi_HAS_CAS_64)

template <>
class AtomicMem <3>
{
public:

   typedef  archi::Int64   DataType;
   ohm_archi_TYPEDEF_ALIGN (8, DataType, DataTypeAlign);

   ohm_archi_FORCEINLINE static DataType
                  swap (volatile DataType &dest, DataType excg);
   ohm_archi_FORCEINLINE static DataType
                  cas (volatile DataType &dest, DataType excg, DataType comp);
}; // class AtomicMem <3>

#endif   // ohm_archi_HAS_CAS_64



#if defined (ohm_archi_HAS_CAS_128)

template <>
class AtomicMem <4>
{
public:

   typedef  archi::Vect128 DataType;
   ohm_archi_TYPEDEF_ALIGN (16, DataType, DataTypeAlign);

   ohm_archi_FORCEINLINE static DataType
                  swap (volatile DataType &dest, DataType excg);
   ohm_archi_FORCEINLINE static DataType
                  cas (volatile DataType &dest, DataType excg, DataType comp);
}; // class AtomicMem <4>

#endif   // ohm_archi_HAS_CAS_128



template <>
class AtomicMem <-1>
{
   // Nothing
}; // class AtomicMem <-1>



}  // namespace conc
}  // namespace ohm



#include "ohm/conc/AtomicMem.hpp"



#endif   // ohm_conc_AtomicMem_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
