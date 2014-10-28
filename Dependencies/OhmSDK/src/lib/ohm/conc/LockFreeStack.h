/*****************************************************************************

        LockFreeStack.h
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



#if ! defined (ohm_conc_LockFreeStack_HEADER_INCLUDED)
#define  ohm_conc_LockFreeStack_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/conc/AtomicPtrIntPair.h"
#include "ohm/conc/LockFreeCell.h"



namespace ohm
{
namespace conc
{



template <class T>
class LockFreeStack
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   typedef  T  ValueType;
   typedef  LockFreeCell <T>  CellType;

                  LockFreeStack ();
   virtual        ~LockFreeStack () {}

   void           push (CellType &cell);
   CellType *     pop ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   AtomicPtrIntPair <CellType>
                  _head_ptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  LockFreeStack (const LockFreeStack &other);
   LockFreeStack& operator = (const LockFreeStack &other);
   bool           operator == (const LockFreeStack &other);
   bool           operator != (const LockFreeStack &other);

}; // class LockFreeStack



}  // namespace conc
}  // namespace ohm



#include "ohm/conc/LockFreeStack.hpp"



#endif   // ohm_conc_LockFreeStack_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
