/*****************************************************************************

        AtomicPtrIntPair.h
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



#if ! defined (ohm_conc_AtomicPtrIntPair_HEADER_INCLUDED)
#define  ohm_conc_AtomicPtrIntPair_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/sys/Interlocked.h"

#include <cstddef>



namespace ohm
{
namespace conc
{



template <class T>
class AtomicPtrIntPair
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  AtomicPtrIntPair ();
   virtual        ~AtomicPtrIntPair () {}

   void           set (T * ptr, ptrdiff_t val);
   void           get (T * &ptr, ptrdiff_t &val) const;
   T *            get_ptr () const;
   ptrdiff_t      get_val () const;
   bool           cas (T *new_ptr, T *comp_ptr);
   bool           cas2 (T *new_ptr, ptrdiff_t new_val, T *comp_ptr, ptrdiff_t comp_val);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

#if ohm_archi_WORD_SIZE == 64

   typedef  sys::Interlocked::Data128  DataType;
   ohm_archi_TYPEDEF_ALIGN (16, DataType, DataTypeAlign);

#else    // ohm_archi_WORD_SIZE

   typedef  archi::Int64   DataType;
   ohm_archi_TYPEDEF_ALIGN (8, DataType, DataTypeAlign);

#endif   // ohm_archi_WORD_SIZE

   class RealContent
   {
   public:
      T * volatile   _ptr;
      volatile ptrdiff_t
                     _val;
   };

   union Combi
   {
      DataTypeAlign  _storage;
      RealContent    _content;
   };

   static void    cas_combi (Combi &old, Combi &dest, const Combi &excg, const Combi &comp);

   Combi          _data;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  AtomicPtrIntPair (const AtomicPtrIntPair &other);
   AtomicPtrIntPair &
                  operator = (const AtomicPtrIntPair &other);
   bool           operator == (const AtomicPtrIntPair &other);
   bool           operator != (const AtomicPtrIntPair &other);

}; // class AtomicPtrIntPair



}  // namespace conc
}  // namespace ohm



#include "ohm/conc/AtomicPtrIntPair.hpp"



#endif   // ohm_conc_AtomicPtrIntPair_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
