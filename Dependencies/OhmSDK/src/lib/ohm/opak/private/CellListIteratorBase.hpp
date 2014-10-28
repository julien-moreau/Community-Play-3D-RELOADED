/*****************************************************************************

        CellListIteratorBase.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38760

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



#if defined (ohm_opak_CellListIteratorBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of CellListIteratorBase code header.
#endif
#define  ohm_opak_CellListIteratorBase_CURRENT_CODEHEADER

#if ! defined (ohm_opak_CellListIteratorBase_CODEHEADER_INCLUDED)
#define  ohm_opak_CellListIteratorBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/def.h"
#include "ohm/opak/private/CellPoolWrapper.h"



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T, class U>
CellListIteratorBase <T, U>::CellListIteratorBase ()
:  _cell_ptr (0)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T, class U>
CellListIteratorBase <T, U>::CellListIteratorBase (const CellListIteratorBase & other)
:  _cell_ptr (other._cell_ptr)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T, class U>
CellListIteratorBase <T, U>::~CellListIteratorBase ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T, class U>
CellListIteratorBase <T, U> & CellListIteratorBase <T, U>::operator = (const CellListIteratorBase & other)
{
   _cell_ptr = other._cell_ptr;
   
   return *this;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class T, class U>
T &   CellListIteratorBase <T, U>::operator * ()
{
   return *operator -> ();
}



/*
==============================================================================
Name : operator ->
==============================================================================
*/

template <class T, class U>
T *   CellListIteratorBase <T, U>::operator -> ()
{
   assert (_cell_ptr != 0);
   
   T * ptr = reinterpret_cast <T *> (&_cell_ptr->_data [0]);
   
   return ptr;
}



/*
==============================================================================
Name : operator ++
==============================================================================
*/

template <class T, class U>
CellListIteratorBase <T, U> & CellListIteratorBase <T, U>::operator ++ ()
{
   _cell_ptr = _cell_ptr->_next_ptr;
   
   return *this;
}



/*
==============================================================================
Name : operator ++
==============================================================================
*/

template <class T, class U>
CellListIteratorBase <T, U>   CellListIteratorBase <T, U>::operator ++ (int)
{
   CellListIteratorBase tmp = *this;
   ++*this;
   
   return tmp;
}



/*
==============================================================================
Name : operator --
==============================================================================
*/

template <class T, class U>
CellListIteratorBase <T, U> & CellListIteratorBase <T, U>::operator -- ()
{
   _cell_ptr = _cell_ptr->_previous_ptr;
   
   return *this;
}



/*
==============================================================================
Name : operator --
==============================================================================
*/

template <class T, class U>
CellListIteratorBase <T, U>   CellListIteratorBase <T, U>::operator -- (int)
{
   CellListIteratorBase tmp = *this;
   --*this;
   
   return tmp;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

template <class T, class U>
bool  CellListIteratorBase <T, U>::operator == (const CellListIteratorBase & other) const
{
   return _cell_ptr == other._cell_ptr;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

template <class T, class U>
bool  CellListIteratorBase <T, U>::operator != (const CellListIteratorBase & other) const
{
   return _cell_ptr != other._cell_ptr;
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T, class U>
CellListIteratorBase <T, U>::CellListIteratorBase  (U * cell_ptr)
:  _cell_ptr (cell_ptr)
{
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_CellListIteratorBase_CODEHEADER_INCLUDED

#undef ohm_opak_CellListIteratorBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
