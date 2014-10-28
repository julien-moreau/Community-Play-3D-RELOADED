/*****************************************************************************

        Frame_iterator.hpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38610

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



#if defined (ohm_opal_Frame_iterator_CURRENT_CODEHEADER)
   #error Recursive inclusion of Frame_iterator code header.
#endif
#define  ohm_opal_Frame_iterator_CURRENT_CODEHEADER

#if ! defined (ohm_opal_Frame_iterator_CODEHEADER_INCLUDED)
#define  ohm_opal_Frame_iterator_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Type, class IteratorType>
Frame_iterator <Type, IteratorType>::Frame_iterator ()
:  _it ()
,  _it_end ()
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Type, class IteratorType>
Frame_iterator <Type, IteratorType>::Frame_iterator (const Frame_iterator &other)
:  _it (other._it)
,  _it_end (other._it_end)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class Type, class IteratorType>
Frame_iterator <Type, IteratorType>::~Frame_iterator ()
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class Type, class IteratorType>
Frame_iterator <Type, IteratorType> &  Frame_iterator <Type, IteratorType>::operator = (const Frame_iterator &other)
{
   _it = other._it;
   _it_end = other._it_end;
   
   return *this;
}



/*
==============================================================================
Name : operator ++ (pre)
==============================================================================
*/

template <class Type, class IteratorType>
Frame_iterator <Type, IteratorType> &  Frame_iterator <Type, IteratorType>::operator ++ ()
{
   assert (_it != _it_end);
   ++_it;
   
   for (; _it != _it_end ; ++_it)
   {
      if (!_it->was_removed ())
      {
         break;
      }
   }
   
   return *this;
}



/*
==============================================================================
Name : operator ++ (pre)
==============================================================================
*/

template <class Type, class IteratorType>
Frame_iterator <Type, IteratorType> &  Frame_iterator <Type, IteratorType>::operator -- ()
{
   // no check : caller is supposed to have test with begin before to know
   // if he can decrement
   
   while (true)
   {
      --_it;
      
      if (!_it->was_removed ())
      {
         break;
      }
   }
   
   return *this;
}



/*
==============================================================================
Name : operator ++ (post)
==============================================================================
*/

template <class Type, class IteratorType>
Frame_iterator <Type, IteratorType> Frame_iterator <Type, IteratorType>::operator ++ (int)
{
   IteratorType tmp = *this;
   
   ++*this;
   
   return tmp;
}



/*
==============================================================================
Name : operator ++ (post)
==============================================================================
*/

template <class Type, class IteratorType>
Frame_iterator <Type, IteratorType> Frame_iterator <Type, IteratorType>::operator -- (int)
{
   IteratorType tmp = *this;
   
   --*this;
   
   return tmp;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class Type, class IteratorType>
Type &   Frame_iterator <Type, IteratorType>::operator * ()
{
   return *operator -> ();
}



/*
==============================================================================
Name : operator ->
==============================================================================
*/

template <class Type, class IteratorType>
Type *   Frame_iterator <Type, IteratorType>::operator -> ()
{
   assert (!_it->was_removed ());
   
   Type * ptr = _it->get_frame_ptr ();
   assert (ptr != 0);
   
   return ptr;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

template <class Type, class IteratorType>
bool  Frame_iterator <Type, IteratorType>::operator == (const Frame_iterator &other) const
{
   assert (_it_end == other._it_end);
   
   return _it == other._it;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

template <class Type, class IteratorType>
bool  Frame_iterator <Type, IteratorType>::operator != (const Frame_iterator &other) const
{
   assert (_it_end == other._it_end);
   
   return _it != other._it;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class Type, class IteratorType>
Frame_iterator <Type, IteratorType>::Frame_iterator (IteratorType it, IteratorType it_end)
:  _it (it)
,  _it_end (it_end)
{
}



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_Frame_iterator_CODEHEADER_INCLUDED

#undef ohm_opal_Frame_iterator_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
