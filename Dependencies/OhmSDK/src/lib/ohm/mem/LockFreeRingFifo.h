/*****************************************************************************

        LockFreeRingFifo.h
        Copyright (c) 2002 Ohm Force

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



#if defined (ohm_mem_LockFreeRingFifo_CURRENT_HEADER)
   #error Recursive inclusion of ohm/mem/LockFreeRingFifo header.
#endif
#define  ohm_mem_LockFreeRingFifo_CURRENT_HEADER

#if ! defined (ohm_mem_LockFreeRingFifo_HEADER_INCLUDED)
#define  ohm_mem_LockFreeRingFifo_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/AtomicInt.h"



namespace ohm
{
namespace mem
{



// T requirement :
// T::T ();
// T::operator = ();

template <class T>
class LockFreeRingFifo
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  T  ElementType;

   explicit       LockFreeRingFifo (long size);
   virtual        ~LockFreeRingFifo ();

   // Non-concurent access
   void           resize (long size);
   void           clear ();

   // Writing
   bool           is_full () const;
   long           get_nbr_free_elt () const;
   bool           push (const T &data);
   bool           push_array (const T data_arr [], long nbr_elt);

   // Direct writing + post-validation
   T *            use_data_block_write (long &max_nbr_elt);
   bool           validate (long nbr_elt);

   // Reading
   bool           is_empty () const;
   long           get_nbr_stored_elt () const;
   bool           pop (T &data);
   bool           pop_array (T data_arr [], long nbr_elt);

   // Direct reading + post-erasing
   const T *      use_data_block_read (long &nbr_elt) const;
   bool           discard (long nbr_elt);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef  long  PosType;

   T *            _buf_ptr;
   conc::AtomicInt <PosType>
                  _write_pos;
   conc::AtomicInt <PosType>
                  _read_pos;
   long           _size;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  LockFreeRingFifo ();
                  LockFreeRingFifo (const LockFreeRingFifo &other);
   LockFreeRingFifo &
                  operator = (const LockFreeRingFifo &other);
   bool           operator == (const LockFreeRingFifo &other);
   bool           operator != (const LockFreeRingFifo &other);

}; // class LockFreeRingFifo



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace mem
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/LockFreeRingFifo.hpp"



#endif   // ohm_mem_LockFreeRingFifo_HEADER_INCLUDED

#undef ohm_mem_LockFreeRingFifo_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
