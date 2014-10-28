/*****************************************************************************

        ItemizerDyn.hpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63861

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



#if defined (ohm_flip_ItemizerDyn_CURRENT_CODEHEADER)
   #error Recursive inclusion of ItemizerDyn code header.
#endif
#define  ohm_flip_ItemizerDyn_CURRENT_CODEHEADER

#if ! defined (ohm_flip_ItemizerDyn_CODEHEADER_INCLUDED)
#define  ohm_flip_ItemizerDyn_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Container, class T>
ItemizerDyn <Container, T>::ItemizerDyn (Container & container, FindMask find_mask)
:  _it (container.begin (find_mask))
,  _it_next ()
,  _it_end (container.end (find_mask))
,  _item_ptr (0)
,  _next_item_ptr (0)
,  _first_flag (true)
{
   // make _it point to the first T or _it_end
   
   _item_ptr = advance (_it);
   
   // make _it_next point to the second T or _it_end
   
   if (_it != _it_end)
   {
      _it_next = _it;
      ++_it_next;
      
      _next_item_ptr = advance (_it_next);
   }
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Container, class T>
ItemizerDyn <Container, T>::~ItemizerDyn ()
{
}



/*
==============================================================================
Name : loop
==============================================================================
*/

template <class Container, class T>
bool  ItemizerDyn <Container, T>::loop ()
{
   if (_it == _it_end)
   {
      return false;
   }
   
   if (_first_flag)
   {
      _first_flag = false;
      
      return true;
   }
   else
   {
      _it = _it_next;
      _item_ptr = _next_item_ptr;

      bool ret_val = _it != _it_end;
      
      if (ret_val && (_it_next != _it_end))
      {
         ++_it_next;
         _next_item_ptr = advance (_it_next);
      }
      
      return ret_val;
   }
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class Container, class T>
T &   ItemizerDyn <Container, T>::operator * ()
{
   assert (_it != _it_end);
   assert (_item_ptr != 0);
   
   return *_item_ptr;
}



/*
==============================================================================
Name : operator ->
==============================================================================
*/

template <class Container, class T>
T *   ItemizerDyn <Container, T>::operator -> ()
{
   assert (_it != _it_end);
   assert (_item_ptr != 0);
   
   return _item_ptr;
}



/*
==============================================================================
Name : operator typename Container::iterator
==============================================================================
*/

template <class Container, class T>
ItemizerDyn <Container, T>::operator typename Container::iterator ()
{
   return _it;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : advance
==============================================================================
*/

template <class Container, class T>
T *   ItemizerDyn <Container, T>::advance (iterator & it)
{
   for (; it != _it_end ; ++it)
   {
      typename Container::reference elem = *it;
      
      T * item_ptr = dynamic_cast <T *> (&elem);
      
      if (item_ptr != 0)
      {
         return item_ptr;
      }
   }
   
   return 0;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_ItemizerDyn_CODEHEADER_INCLUDED

#undef ohm_flip_ItemizerDyn_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
