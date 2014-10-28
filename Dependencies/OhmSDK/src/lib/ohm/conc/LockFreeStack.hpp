/*****************************************************************************

        LockFreeStack.hpp
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



#if ! defined (ohm_conc_LockFreeStack_CODEHEADER_INCLUDED)
#define  ohm_conc_LockFreeStack_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



namespace ohm
{
namespace conc
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class T>
LockFreeStack <T>::LockFreeStack ()
:  _head_ptr ()
{
   _head_ptr.set (0, 0);
}



template <class T>
void  LockFreeStack <T>::push (CellType &cell)
{
   assert (&cell != 0);

   CellType *     head_ptr;
   ptrdiff_t      count;
   do
   {
      head_ptr = _head_ptr.get_ptr ();
      count    = _head_ptr.get_val ();
      cell._next_ptr = head_ptr;
   }
   while (! _head_ptr.cas2 (&cell, count + 1, head_ptr, count));
}



// Returns 0 if the stack is empty.
template <class T>
typename LockFreeStack <T>::CellType * LockFreeStack <T>::pop ()
{
   CellType *     cell_ptr;
   bool           cont_flag = true;
   do
   {
      cell_ptr = _head_ptr.get_ptr ();

      if (cell_ptr == 0)
      {
         cont_flag = false;   // Empty stack.
      }

      else
      {
         const ptrdiff_t   count = _head_ptr.get_val ();
         if (cell_ptr != 0)
         {
            CellType *     next_ptr = cell_ptr->_next_ptr;
            if (_head_ptr.cas2 (next_ptr, count + 1, cell_ptr, count))
            {
               cell_ptr->_next_ptr = 0;
               cont_flag = false;
            }
         }
      }
   }
   while (cont_flag);

   return (cell_ptr);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace conc
}  // namespace ohm



#endif   // ohm_conc_LockFreeStack_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
