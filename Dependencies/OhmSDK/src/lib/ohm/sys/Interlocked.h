/*****************************************************************************

        Interlocked.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 67485

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



#if ! defined (ohm_sys_Interlocked_HEADER_INCLUDED)
#define  ohm_sys_Interlocked_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/types.h"



namespace ohm
{
namespace sys
{



class Interlocked
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static archi::Int32
                  exchange_int32 (archi::Int32 volatile &dest, archi::Int32 excg);
   static archi::Int32
                  compare_exchange_int32 (archi::Int32 volatile &dest, archi::Int32 excg, archi::Int32 comp);

#if defined (ohm_archi_HAS_CAS_64)

   static archi::Int64
                  exchange_int64 (archi::Int64 volatile &dest, archi::Int64 excg);
   static archi::Int64
                  compare_exchange_int64 (archi::Int64 volatile &dest, archi::Int64 excg, archi::Int64 comp);

#endif

#if defined (ohm_archi_HAS_CAS_128)
#if (ohm_archi_SYS == ohm_archi_SYS_LINUX)
   typedef __uint128_t Data128;
   
#else
   class Data128
   {
   public:
      ohm_archi_FORCEINLINE bool
                     operator == (const Data128 & other) const;
      ohm_archi_FORCEINLINE bool
                     operator != (const Data128 & other) const;
                     
      archi::UByte   _data [16];
   };
#endif

   static void    exchange_int128 (Data128 &old, volatile Data128 &dest, const Data128 &excg);
   static void    compare_exchange_int128 (Data128 &old, volatile Data128 &dest, const Data128 &excg, const Data128 &comp);

#endif

   static void *  exchange_ptr (void * volatile &dest_ptr, void *excg_ptr);
   static void *  compare_exchange_ptr (void * volatile &dest_ptr, void *excg_ptr, void *comp_ptr);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  Interlocked ();
                  Interlocked (const Interlocked &other);
   Interlocked &  operator = (const Interlocked &other);
   bool           operator == (const Interlocked &other);
   bool           operator != (const Interlocked &other);

}; // class Interlocked



}  // namespace sys
}  // namespace ohm



#include "ohm/sys/Interlocked_common.hpp"



#endif   // ohm_sys_Interlocked_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
