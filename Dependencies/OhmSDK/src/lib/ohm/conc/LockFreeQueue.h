/*****************************************************************************

        LockFreeQueue.h
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 67018

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



#if ! defined (ohm_conc_LockFreeQueue_HEADER_INCLUDED)
#define  ohm_conc_LockFreeQueue_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/AtomicPtrIntPair.h"
#include "ohm/conc/LockFreeCell.h"

#include <cstddef>



namespace ohm
{
namespace conc
{



template <class T>
class LockFreeQueue
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  T  ValueType;
   typedef  LockFreeCell <T>  CellType;

                  LockFreeQueue ();
   virtual        ~LockFreeQueue () {}

   void           enqueue (CellType &cell);
   void           enqueue_chain (CellType *head_ptr);
   CellType *     dequeue ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef  void *   InternalType;

   enum {         SL2   = (sizeof (InternalType) == 8) ? 4 : 3 };

   typedef  AtomicPtrIntPair <CellType>   SafePointer;

   SafePointer    _head;   // Contains ocount
   SafePointer    _tail;   // Contains icount

   CellType       _dummy;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  LockFreeQueue (const LockFreeQueue &other);
   LockFreeQueue &
                  operator = (const LockFreeQueue &other);
   bool           operator == (const LockFreeQueue &other);
   bool           operator != (const LockFreeQueue &other);

}; // class LockFreeQueue



}  // namespace conc
}  // namespace ohm



#include "ohm/conc/LockFreeQueue.hpp"



#endif   // ohm_conc_LockFreeQueue_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
