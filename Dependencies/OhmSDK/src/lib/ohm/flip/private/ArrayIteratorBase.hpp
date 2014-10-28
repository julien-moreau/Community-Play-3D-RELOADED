/*****************************************************************************

        ArrayIteratorBase.hpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53348

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



#if defined (ohm_flip_ArrayIteratorBase_CURRENT_CODEHEADER)
   #error Recursive inclusion of Array code header.
#endif
#define  ohm_flip_ArrayIteratorBase_CURRENT_CODEHEADER

#if ! defined (ohm_flip_ArrayIteratorBase_CODEHEADER_INCLUDED)
#define  ohm_flip_ArrayIteratorBase_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/Object.h"
#include "ohm/flip/DynamicCast.h"



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

template <class T, class iterator_type>
ArrayIteratorBase <T, iterator_type>::ArrayIteratorBase ()
:  _it ()
,  _it_end ()
,  _all_flag (false)
,  _added_flag (false)
,  _resident_flag (false)
,  _removed_flag (false)
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

template <class T, class iterator_type>
ArrayIteratorBase <T, iterator_type>::ArrayIteratorBase (const ArrayIteratorBase & other)
:  _it (other._it)
,  _it_end (other._it_end)
,  _all_flag (other._all_flag)
,  _added_flag (other._added_flag)
,  _resident_flag (other._resident_flag)
,  _removed_flag (other._removed_flag)
{
}



/*
==============================================================================
Name : operator =
==============================================================================
*/

template <class T, class iterator_type>
ArrayIteratorBase <T, iterator_type> & ArrayIteratorBase <T, iterator_type>::operator = (const ArrayIteratorBase & other)
{
   _it = other._it;
   _it_end = other._it_end;
   _all_flag = other._all_flag;
   _added_flag = other._added_flag;
   _resident_flag = other._resident_flag;
   _removed_flag = other._removed_flag;
   
   return *this;
}



/*
==============================================================================
Name : operator ++ (pre)
==============================================================================
*/

template <class T, class iterator_type>
ArrayIteratorBase <T, iterator_type> & ArrayIteratorBase <T, iterator_type>::operator ++ ()
{
   assert (_it != _it_end);
   assert (is_matching_find_mask ());
   
   while (true)
   {
      ++_it;
      
      if (_it == _it_end)
      {
         break;
      }
      
      if (is_matching_find_mask ())
      {
         break;
      }
   }
   
   return *this;
}



/*
==============================================================================
Name : operator -- (pre)
==============================================================================
*/

template <class T, class iterator_type>
ArrayIteratorBase <T, iterator_type> & ArrayIteratorBase <T, iterator_type>::operator -- ()
{
   // no check : caller is supposed to have test with begin before to know
   // if he can decrement
   
   if (_it != _it_end)
   {
      assert (is_matching_find_mask ());
   }
   
   while (true)
   {
      --_it;
      
      if (is_matching_find_mask ())
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

template <class T, class iterator_type>
ArrayIteratorBase <T, iterator_type> ArrayIteratorBase <T, iterator_type>::operator ++ (int)
{
   ArrayIteratorBase tmp = *this;

   ++*this;
   
   return tmp;
}



/*
==============================================================================
Name : operator -- (post)
==============================================================================
*/

template <class T, class iterator_type>
ArrayIteratorBase <T, iterator_type> ArrayIteratorBase <T, iterator_type>::operator -- (int)
{
   ArrayIteratorBase tmp = *this;

   --*this;
   
   return tmp;
}



/*
==============================================================================
Name : operator +=
==============================================================================
*/

template <class T, class iterator_type>
ArrayIteratorBase <T, iterator_type> & ArrayIteratorBase <T, iterator_type>::operator += (int n)
{
   if (n > 0)
   {
      for (int i = 0 ; i < n ; ++i)
      {
         ++*this;
      }
   }
   else if (n < 0)
   {
      operator -= (- n);
   }
   
   return *this;
}



/*
==============================================================================
Name : operator -=
==============================================================================
*/

template <class T, class iterator_type>
ArrayIteratorBase <T, iterator_type> & ArrayIteratorBase <T, iterator_type>::operator -= (int n)
{
   if (n > 0)
   {
      for (int i = 0 ; i < n ; ++i)
      {
         --*this;
      }
   }
   else if (n < 0)
   {
      operator += (- n);
   }
   
   return *this;
}



/*
==============================================================================
Name : operator *
==============================================================================
*/

template <class T, class iterator_type>
T & ArrayIteratorBase <T, iterator_type>::operator * ()
{
   return *operator -> ();
}



/*
==============================================================================
Name : operator ->
==============================================================================
*/

template <class T, class iterator_type>
T * ArrayIteratorBase <T, iterator_type>::operator -> ()
{
   Object * obj_ptr = _it->second._object_ptr;
   
   T * cast_ptr = dynamic_cast <T *> (obj_ptr);
   
   assert (cast_ptr != 0);
   
   return cast_ptr;
}



/*
==============================================================================
Name : operator ==
==============================================================================
*/

template <class T, class iterator_type>
bool ArrayIteratorBase <T, iterator_type>::operator == (const ArrayIteratorBase & other) const
{
   return _it == other._it;
}



/*
==============================================================================
Name : operator !=
==============================================================================
*/

template <class T, class iterator_type>
bool ArrayIteratorBase <T, iterator_type>::operator != (const ArrayIteratorBase & other) const
{
   return _it != other._it;
}



/*
==============================================================================
Name : was_added
==============================================================================
*/

template <class T, class iterator_type>
bool ArrayIteratorBase <T, iterator_type>::was_added () const
{
   return _it->second._state == Array <T>::ObjectPtrState::State_ADDED;
}



/*
==============================================================================
Name : was_removed
==============================================================================
*/

template <class T, class iterator_type>
bool ArrayIteratorBase <T, iterator_type>::was_removed () const
{
   bool removed_flag
      = (_it->second._state == Array <T>::ObjectPtrState::State_REMOVED)
      || (_it->second._state == Array <T>::ObjectPtrState::State_MOVED)
   ;
   
   return removed_flag;
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Note :
   Everything except it_end version
==============================================================================
*/

template <class T, class iterator_type>
ArrayIteratorBase <T, iterator_type>::ArrayIteratorBase (iterator_type it, iterator_type it_end, bool in_post_flag, FindMask find_mask)
:  _it (it)
,  _it_end (it_end)
,  _all_flag (in_post_flag && ((find_mask & FindMask_POST) != 0))
,  _added_flag ((find_mask & FindMask_ADDED) != 0)
,  _resident_flag ((find_mask & FindMask_RESIDENT) != 0)
,  _removed_flag ((find_mask & FindMask_REMOVED) != 0)
{
   if (_it != _it_end)
   {
      assert (is_matching_find_mask ());
   }
}



/*
==============================================================================
Name : is_matching_find_mask
==============================================================================
*/

template <class T, class iterator_type>
bool  ArrayIteratorBase <T, iterator_type>::is_matching_find_mask () const
{
   assert (_it != _it_end);
   
   if (_all_flag)
   {
      return true;
   }
   
   bool match_flag
      = (_added_flag && (_it->second._state == Array <T>::ObjectPtrState::State_ADDED))
      || (_resident_flag && (_it->second._state == Array <T>::ObjectPtrState::State_RESIDENT))
      || (_removed_flag && (_it->second._state == Array <T>::ObjectPtrState::State_REMOVED))
      || (_removed_flag && (_it->second._state == Array <T>::ObjectPtrState::State_MOVED))
   ;
   
   return match_flag;
}



}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_ArrayIteratorBase_CODEHEADER_INCLUDED

#undef ohm_flip_ArrayIteratorBase_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
