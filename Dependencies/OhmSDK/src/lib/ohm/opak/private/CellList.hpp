/*****************************************************************************

        CellList.hpp
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



#if defined (ohm_opak_CellList_CURRENT_CODEHEADER)
   #error Recursive inclusion of CellList code header.
#endif
#define  ohm_opak_CellList_CURRENT_CODEHEADER

#if ! defined (ohm_opak_CellList_CODEHEADER_INCLUDED)
#define  ohm_opak_CellList_CODEHEADER_INCLUDED



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

template <class T>
CellList <T>::CellList ()
:  _node ()
{
   ohm_lang_CHECK_CST (type_is_small_enough, sizeof (T) <= CellGeneric::DATA_SIZE_UPPER_BOUND);
   
   _node._next_ptr = &_node;
   _node._previous_ptr = &_node;
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T>
CellList <T>::~CellList ()
{
   clear ();
}



/*
==============================================================================
Name : size
==============================================================================
*/

template <class T>
size_t   CellList <T>::size () const
{
   size_t size = 0;
   
   CellGeneric * cell_ptr = _node._next_ptr;
   
   while (cell_ptr != &_node)
   {
      ++size;
      
      cell_ptr = cell_ptr->_next_ptr;
   }
   
   return size;
}



/*
==============================================================================
Name : empty
==============================================================================
*/

template <class T>
bool  CellList <T>::empty () const
{
   return (_node._next_ptr == &_node);
}



/*
==============================================================================
Name : front
==============================================================================
*/

template <class T>
T &   CellList <T>::front ()
{
   return *begin ();
}



/*
==============================================================================
Name : back
==============================================================================
*/

template <class T>
T &   CellList <T>::back ()
{
   iterator tmp = end ();
   --tmp;
   return *tmp;
}



/*
==============================================================================
Name : push_back
==============================================================================
*/

template <class T>
void  CellList <T>::push_back (const T & elem)
{
   insert (end (), elem);
}



/*
==============================================================================
Name : begin
==============================================================================
*/

template <class T>
typename CellList <T>::iterator  CellList <T>::begin ()
{
   return iterator (_node._next_ptr);
}



/*
==============================================================================
Name : end
==============================================================================
*/

template <class T>
typename CellList <T>::iterator  CellList <T>::end ()
{
   return iterator (&_node);
}



/*
==============================================================================
Name : begin
==============================================================================
*/

template <class T>
typename CellList <T>::const_iterator  CellList <T>::begin () const
{
   return const_iterator (_node._next_ptr);
}



/*
==============================================================================
Name : end
==============================================================================
*/

template <class T>
typename CellList <T>::const_iterator  CellList <T>::end () const
{
   return const_iterator (&_node);
}



/*
==============================================================================
Name : insert
==============================================================================
*/

template <class T>
void  CellList <T>::insert (iterator it, const T & elem)
{
   using namespace std;
   
   CellGeneric * cell_ptr = 0;
   
   mem::mmgr::CellPool <CellGeneric> & cell_pool = CellPoolWrapper::use_instance ().use ();
   
   if (cell_pool.has_free_cell ())
   {
      cell_ptr = &cell_pool.use_free_cell ();
   }
   else
   {
      cell_ptr = &cell_pool.buy_new_cell ();
   }
   
   cell_ptr->_next_ptr = it._cell_ptr;
   cell_ptr->_previous_ptr = it._cell_ptr->_previous_ptr;
   
   it._cell_ptr->_previous_ptr->_next_ptr = cell_ptr;
   it._cell_ptr->_previous_ptr = cell_ptr;
   
   new (&cell_ptr->_data [0]) T (elem);
}



/*
==============================================================================
Name : clear
==============================================================================
*/

template <class T>
void  CellList <T>::clear ()
{
   erase (begin (), end ());
}



/*
==============================================================================
Name : erase
==============================================================================
*/

template <class T>
typename CellList <T>::iterator  CellList <T>::erase (iterator it)
{
   it._cell_ptr->_previous_ptr->_next_ptr = it._cell_ptr->_next_ptr;
   it._cell_ptr->_next_ptr->_previous_ptr = it._cell_ptr->_previous_ptr;
   
   T * ptr = reinterpret_cast <T *> (&it._cell_ptr->_data [0]);
   ptr->~T ();
   
   CellGeneric * next_cell_ptr = it._cell_ptr->_next_ptr;
   
   mem::mmgr::CellPool <CellGeneric> & cell_pool = CellPoolWrapper::use_instance ().use ();
   cell_pool.free_cell (*it._cell_ptr);
   
   return iterator (next_cell_ptr);
}



/*
==============================================================================
Name : erase
==============================================================================
*/

template <class T>
typename CellList <T>::iterator  CellList <T>::erase (iterator it_first, iterator it_last)
{
   while (it_first != it_last)
   {
      it_first = erase (it_first);
   }
   
   return it_last;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_CellList_CODEHEADER_INCLUDED

#undef ohm_opak_CellList_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
