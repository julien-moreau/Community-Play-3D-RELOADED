/*****************************************************************************

        Array.hpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71329

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



#if defined (ohm_flip_Array_CURRENT_CODEHEADER)
   #error Recursive inclusion of Array code header.
#endif
#define  ohm_flip_Array_CURRENT_CODEHEADER

#if ! defined (ohm_flip_Array_CODEHEADER_INCLUDED)
#define  ohm_flip_Array_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/flip/private/MachineCode.h"
#include "ohm/flip/private/ObjectTravelAgency.h"
#include "ohm/flip/private/TravelerObjectBackup.h"
#include "ohm/flip/private/TravelerSerialKiller.h"
#include "ohm/flip/private/TravelerObjectInit.h"
#include "ohm/flip/private/TravelerObjectRestore.h"
#include "ohm/flip/ClassDescription.h"
#include "ohm/flip/ObjectMold.h"

#include "ohm/util/trace.h"

#include <stdexcept>

#include <climits>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

template <class T>
Array <T>::Array (DocumentBase & document)
:  ArrayMaster (document)
,  _key_ptr_map ()
,  _old_cur_equal_flag (true)
,  _child_order_changed_flag (false)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
Array <T>::~Array ()
{
   // must not be cleared on destruction
   
   // check that we have only keys to be removed
   
   typename KeyPtrMap::const_iterator it = _key_ptr_map.begin ();
   const typename KeyPtrMap::const_iterator it_end = _key_ptr_map.end ();
         
   for (; it != it_end ; ++it)
   {
      bool ok_flag
         = (it->second._state == ObjectPtrState::State_REMOVED)
         || (it->second._state == ObjectPtrState::State_MOVED)
      ;
      
      // there is still an element which is not to be removed
      assert (ok_flag);
   }
}



/*
==============================================================================
Name : begin
==============================================================================
*/

template <class T>
typename Array <T>::iterator  Array <T>::begin (FindMask find_mask)
{
   typename KeyPtrMap::iterator it = _key_ptr_map.begin ();
   
   bool in_post_flag = use_document ().is_in_post ();
   
   if (in_post_flag && ((find_mask & FindMask_POST) != 0))
   {
      // nothing
   }
   else
   {
      bool added_flag = (find_mask & FindMask_ADDED) != 0;
      bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
      bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
      
      while (true)
      {
         if (it == _key_ptr_map.end ())
         {
            break;
         }
         
         bool break_flag
            = (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
            || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
            || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
            || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
         ;
         
         if (break_flag)
         {
            break;
         }
         
         ++it;
      }
   }
   
   return iterator (it, _key_ptr_map.end (), in_post_flag, find_mask);
}



/*
==============================================================================
Name : begin
==============================================================================
*/

template <class T>
typename Array <T>::const_iterator  Array <T>::begin (FindMask find_mask) const
{
   typename KeyPtrMap::const_iterator it = _key_ptr_map.begin ();
   
   bool in_post_flag = use_document ().is_in_post ();
   
   if (in_post_flag && ((find_mask & FindMask_POST) != 0))
   {
      // nothing
   }
   else
   {
      bool added_flag = (find_mask & FindMask_ADDED) != 0;
      bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
      bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
      
      while (true)
      {
         if (it == _key_ptr_map.end ())
         {
            break;
         }
         
         bool break_flag
            = (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
            || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
            || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
            || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
         ;
         
         if (break_flag)
         {
            break;
         }
         
         ++it;
      }
   }
   
   return const_iterator (it, _key_ptr_map.end (), in_post_flag, find_mask);
}



/*
==============================================================================
Name : end
==============================================================================
*/

template <class T>
typename Array <T>::iterator  Array <T>::end (FindMask find_mask)
{
   return iterator (
      _key_ptr_map.end (), _key_ptr_map.end (),
      use_document ().is_in_post (),
      find_mask
   );
}



/*
==============================================================================
Name : end
==============================================================================
*/

template <class T>
typename Array <T>::const_iterator  Array <T>::end (FindMask find_mask) const
{
   return const_iterator (
      _key_ptr_map.end (), _key_ptr_map.end (),
      use_document ().is_in_post (),
      find_mask
   );
}



/*
==============================================================================
Name : rbegin
==============================================================================
*/

template <class T>
typename Array <T>::reverse_iterator   Array <T>::rbegin (FindMask find_mask)
{
   typename KeyPtrMap::reverse_iterator it = _key_ptr_map.rbegin ();
   
   bool in_post_flag = use_document ().is_in_post ();
   
   if (in_post_flag && ((find_mask & FindMask_POST) != 0))
   {
      // nothing
   }
   else
   {
      bool added_flag = (find_mask & FindMask_ADDED) != 0;
      bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
      bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
      
      while (true)
      {
         if (it == _key_ptr_map.rend ())
         {
            break;
         }
         
         bool break_flag
            = (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
            || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
            || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
            || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
         ;
         
         if (break_flag)
         {
            break;
         }
         
         ++it;
      }
   }
   
   return reverse_iterator (it, _key_ptr_map.rend (), in_post_flag, find_mask);
}



/*
==============================================================================
Name : rbegin
==============================================================================
*/

template <class T>
typename Array <T>::const_reverse_iterator   Array <T>::rbegin (FindMask find_mask) const
{
   typename KeyPtrMap::const_reverse_iterator it = _key_ptr_map.rbegin ();
   
   bool in_post_flag = use_document ().is_in_post ();
   
   if (in_post_flag && ((find_mask & FindMask_POST) != 0))
   {
      // nothing
   }
   else
   {
      bool added_flag = (find_mask & FindMask_ADDED) != 0;
      bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
      bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
      
      while (true)
      {
         if (it == _key_ptr_map.rend ())
         {
            break;
         }
         
         bool break_flag
            = (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
            || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
            || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
            || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
         ;
         
         if (break_flag)
         {
            break;
         }
         
         ++it;
      }
   }
   
   return const_reverse_iterator (it, _key_ptr_map.rend (), in_post_flag, find_mask);
}



/*
==============================================================================
Name : rend
==============================================================================
*/

template <class T>
typename Array <T>::reverse_iterator   Array <T>::rend (FindMask find_mask)
{
   return reverse_iterator (
      _key_ptr_map.rend (), _key_ptr_map.rend (),
      use_document ().is_in_post (),
      find_mask
   );
}



/*
==============================================================================
Name : rend
==============================================================================
*/

template <class T>
typename Array <T>::const_reverse_iterator   Array <T>::rend (FindMask find_mask) const
{
   return const_reverse_iterator (
      _key_ptr_map.rend (), _key_ptr_map.rend (),
      use_document ().is_in_post (),
      find_mask
   );
}



/*
==============================================================================
Name : find
==============================================================================
*/

template <class T>
typename Array <T>::iterator  Array <T>::find (const T & elem, FindMask find_mask)
{
   const Object * obj_ptr = &elem;
   
   KeyPtrMap_iterator it = _key_ptr_map.begin ();
   const KeyPtrMap_iterator it_end = _key_ptr_map.end ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
         || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
         || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
         || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
      ;
      
      if (parse_flag)
      {
         bool equal_flag = (obj_ptr == it->second._object_ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }

   return iterator (it, _key_ptr_map.end (), use_document ().is_in_post (), find_mask);
}



/*
==============================================================================
Name : find
==============================================================================
*/

template <class T>
typename Array <T>::const_iterator  Array <T>::find (const T & elem, FindMask find_mask) const
{
   const Object * obj_ptr = &elem;
   
   KeyPtrMap_const_iterator it = _key_ptr_map.begin ();
   const KeyPtrMap_const_iterator it_end = _key_ptr_map.end ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
         || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
         || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
         || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
      ;
      
      if (parse_flag)
      {
         bool equal_flag = (obj_ptr == it->second._object_ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return const_iterator (it, _key_ptr_map.end (), use_document ().is_in_post (), find_mask);
}



/*
==============================================================================
Name : find
Note :
   'Predicate' must conform to :
   bool  equal (const T & object) const;

Example :
   class MyPredicate
   {
   public:
            MyPredicate (int some_value) : _some_value (some_value) {}
      bool  equal (const SomeFlipClass & object) const
      {
         return (object._some_class_member == _some_value);
      }
   private:
      int   _some_value;
   };
   
   MyPredicate my_predicate (1);
   
   Array <SomeFlipClass> & array = get_some_array ();
   Array <SomeFlipClass>::iterator it = array.find (my_predicate);
   
==============================================================================
*/

template <class T>
template <class Predicate>
typename Array <T>::iterator  Array <T>::find (const Predicate & predicate, FindMask find_mask)
{
   KeyPtrMap_iterator it = _key_ptr_map.begin ();
   const KeyPtrMap_iterator it_end = _key_ptr_map.end ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
         || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
         || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
         || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
      ;
      
      if (parse_flag)
      {
         const T * ptr = dynamic_cast <const T *> (it->second._object_ptr);
         assert (ptr != 0);
         
         bool equal_flag = predicate.equal (*ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return iterator (it, _key_ptr_map.end (), use_document ().is_in_post (), find_mask);
}


   
/*
==============================================================================
Name : find
==============================================================================
*/

template <class T>
template <class Predicate>
typename Array <T>::const_iterator  Array <T>::find (const Predicate & predicate, FindMask find_mask) const
{
   KeyPtrMap_const_iterator it = _key_ptr_map.begin ();
   const KeyPtrMap_const_iterator it_end = _key_ptr_map.end ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
         || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
         || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
         || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
      ;
      
      if (parse_flag)
      {
         const T * ptr = dynamic_cast <const T *> (it->second._object_ptr);
         assert (ptr != 0);
         
         bool equal_flag = predicate.equal (*ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return const_iterator (it, _key_ptr_map.end (), use_document ().is_in_post (), find_mask);
}



/*
==============================================================================
Name : rfind
==============================================================================
*/

template <class T>
typename Array <T>::reverse_iterator   Array <T>::rfind (const T & elem, FindMask find_mask)
{
   const Object * obj_ptr = &elem;
   
   KeyPtrMap_reverse_iterator it = _key_ptr_map.rbegin ();
   const KeyPtrMap_reverse_iterator it_end = _key_ptr_map.rend ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
         || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
         || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
         || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
      ;
      
      if (parse_flag)
      {
         bool equal_flag = (obj_ptr == it->second._object_ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }

   return reverse_iterator (it, _key_ptr_map.rend (), use_document ().is_in_post (), find_mask);
}



/*
==============================================================================
Name : rfind
==============================================================================
*/

template <class T>
typename Array <T>::const_reverse_iterator   Array <T>::rfind (const T & elem, FindMask find_mask) const
{
   const Object * obj_ptr = &elem;
   
   KeyPtrMap_const_reverse_iterator it = _key_ptr_map.rbegin ();
   const KeyPtrMap_const_reverse_iterator it_end = _key_ptr_map.rend ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
         || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
         || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
         || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
      ;
      
      if (parse_flag)
      {
         bool equal_flag = (obj_ptr == it->second._object_ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return const_reverse_iterator (it, _key_ptr_map.rend (), use_document ().is_in_post (), find_mask);
}



/*
==============================================================================
Name : rfind
Note :
   'Predicate' must conform to :
   bool  equal (const T & object) const;

Example :
   class MyPredicate
   {
   public:
            MyPredicate (int some_value) : _some_value (some_value) {}
      bool  equal (const SomeFlipClass & object) const
      {
         return (object._some_class_member == _some_value);
      }
   private:
      int   _some_value;
   };
   
   MyPredicate my_predicate (1);
   
   Array <SomeFlipClass> & array = get_some_array ();
   Array <SomeFlipClass>::reverse_iterator it = array.rfind (my_predicate);
   
==============================================================================
*/

template <class T>
template <class Predicate>
typename Array <T>::reverse_iterator   Array <T>::rfind (const Predicate & predicate, FindMask find_mask)
{
   KeyPtrMap_reverse_iterator it = _key_ptr_map.rbegin ();
   const KeyPtrMap_reverse_iterator it_end = _key_ptr_map.rend ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
         || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
         || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
         || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
      ;
      
      if (parse_flag)
      {
         const T * ptr = dynamic_cast <const T *> (it->second._object_ptr);
         assert (ptr != 0);
         
         bool equal_flag = predicate.equal (*ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return reverse_iterator (it, _key_ptr_map.rend (), use_document ().is_in_post (), find_mask);
}


   
/*
==============================================================================
Name : rfind
==============================================================================
*/

template <class T>
template <class Predicate>
typename Array <T>::const_reverse_iterator   Array <T>::rfind (const Predicate & predicate, FindMask find_mask) const
{
   KeyPtrMap_const_reverse_iterator it = _key_ptr_map.rbegin ();
   const KeyPtrMap_const_reverse_iterator it_end = _key_ptr_map.rend ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
         || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
         || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
         || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
      ;
      
      if (parse_flag)
      {
         const T * ptr = dynamic_cast <const T *> (it->second._object_ptr);
         assert (ptr != 0);
         
         bool equal_flag = predicate.equal (*ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return const_reverse_iterator (it, _key_ptr_map.rend (), use_document ().is_in_post (), find_mask);
}



/*
==============================================================================
Name : size
==============================================================================
*/

template <class T>
size_t   Array <T>::size (FindMask find_mask) const
{
   size_t ret_val = 0;
   
   KeyPtrMap_const_iterator it = _key_ptr_map.begin ();
   const KeyPtrMap_const_iterator it_end = _key_ptr_map.end ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->second._state == ObjectPtrState::State_ADDED))
         || (resident_flag && (it->second._state == ObjectPtrState::State_RESIDENT))
         || (removed_flag && (it->second._state == ObjectPtrState::State_REMOVED))
         || (removed_flag && (it->second._state == ObjectPtrState::State_MOVED))
      ;
      
      if (parse_flag)
      {
         ++ret_val;
      }
   }
   
   return ret_val;
}



/*
==============================================================================
Name : operator []
==============================================================================
*/

template <class T>
T &   Array <T>::operator [] (size_t index)
{
   assert (index < size ());
   
   iterator it = begin ();
   it += index;
   
   return *it;
}



/*
==============================================================================
Name : operator []
==============================================================================
*/

template <class T>
const T &   Array <T>::operator [] (size_t index) const
{
   assert (index < size ());
   
   const_iterator it = begin ();
   it += index;
   
   return *it;
}



/*
==============================================================================
Name : at
==============================================================================
*/

template <class T>
T &   Array <T>::at (size_t index)
{
   if (index >= size ())
   {
      throw std::out_of_range ("Array::at out of range");
   }
   
   iterator it = begin ();
   it += index;
   
   return *it;
}



/*
==============================================================================
Name : at
==============================================================================
*/

template <class T>
const T &   Array <T>::at (size_t index) const
{
   if (index >= size ())
   {
      throw std::out_of_range ("Array::at out of range");
   }
   
   const_iterator it = begin ();
   it += index;
   
   return *it;
}



/*
==============================================================================
Name : empty
==============================================================================
*/

template <class T>
bool  Array <T>::empty (FindMask find_mask) const
{
   return (size (find_mask) == 0);
}



/*
==============================================================================
Name : front
==============================================================================
*/

template <class T>
typename Array <T>::reference Array <T>::front (FindMask find_mask)
{
   assert (!empty ());
   
   return *begin (find_mask);
}



/*
==============================================================================
Name : front
==============================================================================
*/

template <class T>
typename Array <T>::const_reference Array <T>::front (FindMask find_mask) const
{
   assert (!empty ());
   
   return *begin (find_mask);
}



/*
==============================================================================
Name : back
==============================================================================
*/

template <class T>
typename Array <T>::reference Array <T>::back (FindMask find_mask)
{
   assert (!empty ());

   return *rbegin (find_mask);
}



/*
==============================================================================
Name : back
==============================================================================
*/

template <class T>
typename Array <T>::const_reference Array <T>::back (FindMask find_mask) const
{
   assert (!empty ());

   return *rbegin (find_mask);
}



/*
==============================================================================
Name : insert
Description :
   insert an element into the array, the element being the base class with
   which the array is declared.
==============================================================================
*/

template <class T>
typename Array <T>::iterator  Array <T>::insert (iterator it, Args & args)
{
   return insert <T> (it, args);
}



/*
==============================================================================
Name : insert
Description :
   insert an element into the array, the element being possibly not the
   base class with which the array was declared. However, the class template
   parameter must inherit from the base class directly or indirectly.
==============================================================================
*/

template <class T>
template <class U>
typename Array <T>::iterator  Array <T>::insert (iterator it, Args & args)
{
   Ref this_ref = Object::get_ref ();
   assert (this_ref != 0);
   
   BinaryKey key;
   compute_key (key, it);
   
   DocumentBase & document = use_document ();
   ObjectManager & object_manager = document.use_object_manager ();
   
   Ref ref = object_manager.get_planned_ref ();
   
   ClassDescriptionMaster & cdm = ClassDescription <U>::use ();
   
   Data & transaction = document.use_transaction ();
   
   MachineCode::build_array_insert (transaction, this_ref, key, ref, cdm.get_class_id ());
   
   ObjectPtrState ops;
   ops._object_ptr = object_manager.create (cdm, this, ref, args);
   ops._state = ObjectPtrState::State_ADDED;
   
   std::pair <typename KeyPtrMap::iterator, bool> rep
      = _key_ptr_map.insert (std::make_pair (key, ops));

   // user inputs, key cannot be already here
   assert (rep.second);
   
   // User Inputs, _key_ptr_map is necesserally different from _old_key_ptr_map
   array_changed_invalidate ();

   typename KeyPtrMap::iterator new_it = rep.first;
   
   TravelerObjectInit traveler (document);
   
   ObjectTravelAgency travel_agency (document);
   travel_agency.travel (
      ref,
      ObjectTravelAgency::Flag_DEPTH_FIRST,
      traveler
   );
   
   //broadcast_order_change ();
   
   assert (!use_document ().is_in_post ());
   
   document.notify_order_changed (*this);
   
   return iterator (new_it, _key_ptr_map.end (), false, FindMask_NATURAL);
}



/*
==============================================================================
Name : erase
==============================================================================
*/

template <class T>
void  Array <T>::erase (iterator it)
{
   Object * object_ptr = it._it->second._object_ptr;
   assert (object_ptr != 0);
   
   assert (it._it->second._state != ObjectPtrState::State_REMOVED);
   assert (it._it->second._state != ObjectPtrState::State_MOVED);

   DocumentBase & document = use_document ();

   {
      TravelerSerialKiller traveler (document);
   
      ObjectTravelAgency travel_agency (document);
      travel_agency.travel (
         object_ptr->get_ref (),
         ObjectTravelAgency::Flag_REVERSE
            | ObjectTravelAgency::Flag_FOLLOW_CONTAINERS,
         traveler
      );
   }
   
   Data & transaction = document.use_transaction ();
   
   ClassDescriptionMaster & cdm = object_ptr->get_cdm ();
   
   Ref this_ref = get_ref ();
   const BinaryKey & key = it._it->first;
   Ref obj_ref = object_ptr->get_ref ();
   
   {
      TravelerObjectRestore traveler (document);
      
      ObjectTravelAgency travel_agency (document);
      travel_agency.travel (
         obj_ref,
         ObjectTravelAgency::Flag_REVERSE
            | ObjectTravelAgency::Flag_DEPTH_FIRST,
         traveler
      );
   }
   
   {
      TravelerObjectBackup traveler (document);
   
      ObjectTravelAgency travel_agency (document);
      travel_agency.travel (
         object_ptr->get_ref (),
         ObjectTravelAgency::Flag_FOLLOW_CONTAINERS,
         traveler
      );
   }
   
   use_object_manager ().recycle (object_ptr);
   
   MachineCode::build_array_erase (
      transaction, this_ref, key, obj_ref, cdm.get_class_id ()
   );
   
   if (it._it->second._state == ObjectPtrState::State_ADDED)
   {
      _key_ptr_map.erase (it._it);
   }
   else
   {
      assert (it._it->second._state == ObjectPtrState::State_RESIDENT);
      it._it->second._state = ObjectPtrState::State_REMOVED;
   }
   
   // User Inputs, _key_ptr_map can be the same as _old_key_ptr_map
   array_changed ();

   object_ptr->change_order (-1);
   //broadcast_order_change ();

   document.notify_order_changed (*this);
}



/*
==============================================================================
Name : erase
==============================================================================
*/

template <class T>
void  Array <T>::erase (iterator begin, iterator end)
{
   iterator it = begin;
   for (; it != end ;)
   {
      iterator tmp = it;
      ++tmp;
      
      erase (it);
      
      it = tmp;
   }
}



/*
==============================================================================
Name : erase
==============================================================================
*/

template <class T>
void  Array <T>::erase (Ref ref)
{
   typename KeyPtrMap::iterator it = _key_ptr_map.begin ();
   typename KeyPtrMap::iterator it_end = _key_ptr_map.end ();
   
   for (; it != it_end ; ++it)
   {
      Object * ptr = it->second._object_ptr;
      
      if (ptr->get_ref () == ref)
      {
         break;
      }
   }
   
   assert (it != it_end);
   
   assert (!use_document ().is_in_post ());
   
   erase (iterator (it, _key_ptr_map.end (), false, FindMask_NATURAL));
}



/*
==============================================================================
Name : clear
==============================================================================
*/

template <class T>
void  Array <T>::clear ()
{
   erase (begin (), end ());
}



/*
==============================================================================
Name : move
==============================================================================
*/

template <class T>
typename Array <T>::iterator  Array <T>::move (iterator src_it, iterator dst_it)
{
   assert (src_it._it->second._state != ObjectPtrState::State_REMOVED);
   assert (src_it._it->second._state != ObjectPtrState::State_MOVED);
   
   if (src_it == dst_it)
   {
      return src_it;
   }
   
   iterator next_src_it = src_it;
   ++next_src_it;
   
   if (next_src_it == dst_it)
   {
      return src_it;
   }
   
   Ref this_ref = Object::get_ref ();
   assert (this_ref != 0);
   
   BinaryKey dst_key;
   compute_key (dst_key, dst_it);
   
   const BinaryKey & src_key = src_it._it->first;
   
   Object * object_ptr = src_it._it->second._object_ptr;
   assert (object_ptr != 0);

   DocumentBase & document = use_document ();
   
   Data & transaction = document.use_transaction ();
   
   MachineCode::build_array_move (transaction, this_ref, src_key, this_ref, dst_key);
   
   if (src_it._it->second._state == ObjectPtrState::State_ADDED)
   {
      _key_ptr_map.erase (src_it._it);
   }
   else
   {
      assert (src_it._it->second._state == ObjectPtrState::State_RESIDENT);
      src_it._it->second._state = ObjectPtrState::State_MOVED;
   }

   ObjectPtrState ops;
   ops._object_ptr = object_ptr;
   ops._state = ObjectPtrState::State_ADDED;

   std::pair <typename KeyPtrMap::iterator, bool> rep
      = _key_ptr_map.insert (std::make_pair (dst_key, ops));
   
   // user inputs
   assert (rep.second);
   
   array_changed_invalidate ();

   typename KeyPtrMap::iterator new_it = rep.first;
   
   //broadcast_order_change ();
   
   assert (!use_document ().is_in_post ());
   
   document.notify_order_changed (*this);
   
   return iterator (new_it, _key_ptr_map.end (), false, FindMask_NATURAL);
}



/*
==============================================================================
Name : move
==============================================================================
*/

template <class T>
typename Array <T>::iterator  Array <T>::move (Array <T> & src_array, iterator src_it, iterator dst_it)
{
   assert (src_it._it->second._state != ObjectPtrState::State_REMOVED);
   assert (src_it._it->second._state != ObjectPtrState::State_MOVED);
   
   if (&src_array == this)
   {
      return move (src_it, dst_it);
   }
   else
   {
      Ref src_ref = src_array.Object::get_ref ();
      assert (src_ref != 0);

      Ref dst_ref = Object::get_ref ();
      assert (dst_ref != 0);
      
      BinaryKey dst_key;
      compute_key (dst_key, dst_it);
      
      const BinaryKey & src_key = src_it._it->first;
      
      Object * object_ptr = src_it._it->second._object_ptr;
      assert (object_ptr != 0);

      DocumentBase & document = use_document ();
      
      Data & transaction = document.use_transaction ();
      
      MachineCode::build_array_move (transaction, src_ref, src_key, dst_ref, dst_key);
      
      if (src_it._it->second._state == ObjectPtrState::State_ADDED)
      {
         src_array._key_ptr_map.erase (src_it._it);
      }
      else
      {
         assert (src_it._it->second._state == ObjectPtrState::State_RESIDENT);
         src_it._it->second._state = ObjectPtrState::State_MOVED;
      }

      // User Inputs, _key_ptr_map can be the same as _old_key_ptr_map
      src_array.array_changed ();
      
      object_ptr->change_order (-1);
      //src_array.broadcast_order_change ();

      ObjectPtrState ops;
      ops._object_ptr = object_ptr;
      ops._state = ObjectPtrState::State_ADDED;

      std::pair <typename KeyPtrMap::iterator, bool> rep
         = _key_ptr_map.insert (std::make_pair (dst_key, ops));
      
      // user inputs
      assert (rep.second);
      
      array_changed_invalidate ();

      typename KeyPtrMap::iterator new_it = rep.first;
      
      object_ptr->change_parent (this);
      
      //broadcast_order_change ();
      
      assert (!use_document ().is_in_post ());
      
      document.notify_order_changed (src_array);
      document.notify_order_changed (*this);

      return iterator (new_it, _key_ptr_map.end (), false, FindMask_NATURAL);
   }
}



/*
==============================================================================
Name : did_value_change
==============================================================================
*/

template <class T>
bool  Array <T>::did_value_change () const
{
   return !_old_cur_equal_flag;
}



/*
==============================================================================
Name : did_child_order_change
==============================================================================
*/

template <class T>
bool  Array <T>::did_child_order_change () const
{
   return _child_order_changed_flag;
}



/*
==============================================================================
Name : synchronize
==============================================================================
*/

template <class T>
void  Array <T>::synchronize ()
{
   assert (use_document ().is_in_post ());
   
   // synchronize children that needs it
   
   typename KeyPtrMap::iterator it = _key_ptr_map.begin ();
   const typename KeyPtrMap::iterator it_end = _key_ptr_map.end ();
   
   for (; it != it_end ;)
   {
      typename KeyPtrMap::iterator next_it = it;
      ++next_it;
      
      bool erase_flag
         = (it->second._state == ObjectPtrState::State_REMOVED)
         || (it->second._state == ObjectPtrState::State_MOVED)
      ;
      
      if (erase_flag)
      {
         _key_ptr_map.erase (it);
      }
      else
      {
         Object * sub_obj_ptr = it->second._object_ptr;
         
         if (sub_obj_ptr->is_invalid ())
         {
            sub_obj_ptr->synchronize ();
         }

         it->second._state = ObjectPtrState::State_RESIDENT;
      }
      
      it = next_it;
   }
   
   // reset
   
   _old_cur_equal_flag = true;
   _child_order_changed_flag = false;
   
   Object::synchronize ();
}



/*
==============================================================================
Name : check_synchronized
==============================================================================
*/

template <class T>
void  Array <T>::check_synchronized () const
{
   typename KeyPtrMap::const_iterator it = _key_ptr_map.begin ();
   const typename KeyPtrMap::const_iterator it_end = _key_ptr_map.end ();
   
   for (; it != it_end ; ++it)
   {
      assert (it->second._state == ObjectPtrState::State_RESIDENT);
      
      Object * sub_obj_ptr = it->second._object_ptr;
      
      sub_obj_ptr->check_synchronized ();
   }
   
   assert (_old_cur_equal_flag);
   // this is just a hint
   //assert (!_child_order_changed_flag);
   
   Object::check_synchronized ();
}



/*
==============================================================================
Name : check_inner_invalidation_cnt
==============================================================================
*/

template <class T>
void  Array <T>::check_inner_invalidation_cnt (size_t invalid_cnt) const
{
   if (!_old_cur_equal_flag)
   {
      ohm_util_TRACE_DEBUG ("  ARR");
      assert (invalid_cnt > 0);
      --invalid_cnt;
   }
   
   Object::check_inner_invalidation_cnt (invalid_cnt);
}



/*
==============================================================================
Name : paste
==============================================================================
*/

template <class T>
typename Array <T>::iterator  Array <T>::paste (iterator it, ObjectMold & mold)
{
   ClassDescriptionMaster & cdm = mold.get_cdm ();
   
   Ref ref = insert (it, cdm);
   
   mold.execute (use_document (), ref);
   
   typename KeyPtrMap::iterator sit = _key_ptr_map.begin ();
   const typename KeyPtrMap::iterator sit_end = _key_ptr_map.end ();
   
   for (; sit != sit_end ; ++sit)
   {
      T * obj_ptr = dynamic_cast <T *> (sit->second._object_ptr);
      assert (obj_ptr != 0);
      
      if (obj_ptr->get_ref () == ref)
      {
         assert (!use_document ().is_in_post ());
         
         return iterator (sit, _key_ptr_map.end (), false, FindMask_NATURAL);
      }
   }
   
   // not found
   assert (false);
   
   return end ();
}



/*
==============================================================================
Name : paste
==============================================================================
*/

template <class T>
typename Array <T>::iterator  Array <T>::paste (iterator it, ObjectMold & mold, RefRefArr & ref_ref_arr)
{
   ClassDescriptionMaster & cdm = mold.get_cdm ();
   
   Ref ref = insert (it, cdm);
   
   mold.execute (use_document (), ref, ref_ref_arr);
   
   typename KeyPtrMap::iterator sit = _key_ptr_map.begin ();
   const typename KeyPtrMap::iterator sit_end = _key_ptr_map.end ();
   
   for (; sit != sit_end ; ++sit)
   {
      T * obj_ptr = dynamic_cast <T *> (sit->second._object_ptr);
      assert (obj_ptr != 0);
      
      if (obj_ptr->get_ref () == ref)
      {
         assert (!use_document ().is_in_post ());
         
         return iterator (sit, _key_ptr_map.end (), false, FindMask_NATURAL);
      }
   }
   
   // not found
   assert (false);
   
   return end ();
}



/*
==============================================================================
Name : insert
Description :
   insert an element into the array, the element being possibly not the
   base class with which the array was declared. However, the class template
   parameter must inherit from the base class directly or indirectly.
   This function should be only accessed by the PasteBoard system.
==============================================================================
*/

template <class T>
Ref   Array <T>::insert (iterator it, ClassDescriptionMaster & cdm)
{
   Ref this_ref = Object::get_ref ();
   assert (this_ref != 0);
   
   BinaryKey key;
   compute_key (key, it);
   
   DocumentBase & document = use_document ();
   ObjectManager & object_manager = document.use_object_manager ();
   
   Ref ref = object_manager.get_planned_ref ();
   
   Data & transaction = document.use_transaction ();
   
   MachineCode::build_array_insert (transaction, this_ref, key, ref, cdm.get_class_id ());
   
   ObjectPtrState ops;
   ops._object_ptr = object_manager.create (cdm, this, ref);
   ops._state = ObjectPtrState::State_ADDED;
   
   std::pair <typename KeyPtrMap::iterator, bool> rep
      = _key_ptr_map.insert (std::make_pair (key, ops));
   
   // user inputs, key cannot be already here
   assert (rep.second);
   
   // User Inputs, _key_ptr_map is necesserally different from _old_key_ptr_map
   array_changed_invalidate ();
   
   typename KeyPtrMap::iterator new_it = rep.first;
   
   TravelerObjectInit traveler (document);
   
   ObjectTravelAgency travel_agency (document);
   travel_agency.travel (
      ref,
      ObjectTravelAgency::Flag_DEPTH_FIRST,
      traveler
   );
   
   //broadcast_order_change ();
   
   assert (!use_document ().is_in_post ());
   
   document.notify_order_changed (*this);
   
   return ref;
}



/*
==============================================================================
Name : push_back
Description :
   This function should be only accessed by the PasteBoard system.
==============================================================================
*/

template <class T>
Ref   Array <T>::push_back (ClassDescriptionMaster & cdm)
{
   return insert (end (), cdm);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : compute_key
==============================================================================
*/

template <class T>
void  Array <T>::compute_key (BinaryKey & key, iterator it)
{
   if ((it == begin ()) && (it == end ()))
   {
      BinaryKey zero;
      BinaryKey one = BinaryKey::create_value_one ();
      
      key.generate_random_between (zero, one);
   }
   else if (it == begin ())
   {
      BinaryKey zero;
      
      key.generate_random_between (zero, it._it->first);
   }
   else if (it == end ())
   {
      iterator it_prev = it;
      --it_prev;
      
      BinaryKey one = BinaryKey::create_value_one ();
      
      key.generate_random_between (it_prev._it->first, one);
   }
   else
   {
      iterator it_prev = it;
      --it_prev;
      
      key.generate_random_between (it_prev._it->first, it._it->first);
   }
}



/*
==============================================================================
Name : array_changed
==============================================================================
*/

template <class T>
void  Array <T>::array_changed ()
{
   if (_old_cur_equal_flag)
   {
      _old_cur_equal_flag = false;
      
      invalidate ();
   }
   else
   {
      // look if there are only residents
      
      _old_cur_equal_flag = true;
      
      typename KeyPtrMap::iterator it = _key_ptr_map.begin ();
      const typename KeyPtrMap::iterator it_end = _key_ptr_map.end ();
      
      for (; it != it_end ; ++it)
      {
         if (it->second._state != ObjectPtrState::State_RESIDENT)
         {
            _old_cur_equal_flag = false;
            break;
         }
      }
      
      if (_old_cur_equal_flag)
      {
         validate ();
      }
   }
}



/*
==============================================================================
Name : array_changed_invalidate
Description :
   Used in the case where a validate cannot occur
==============================================================================
*/

template <class T>
void  Array <T>::array_changed_invalidate ()
{
   if (_old_cur_equal_flag)
   {
      _old_cur_equal_flag = false;
      
      invalidate ();
   }
}



/*
==============================================================================
Name : insert_no_notification
==============================================================================
*/

template <class T>
int   Array <T>::insert_no_notification (const BinaryKey & key, Ref ref, int class_id, bool tolerant_flag)
{
   typename KeyPtrMap::iterator it = _key_ptr_map.find (key);
   
   if (it != _key_ptr_map.end ())
   {
      bool exist_flag
         = (it->second._state == ObjectPtrState::State_RESIDENT)
         || (it->second._state == ObjectPtrState::State_ADDED)
      ;
      
      if (exist_flag)
      {
         assert (tolerant_flag);
         return Err_VM_ARRAY_KEY_EXISTS;
      }
   }
   else
   {
      // we don't need to check that the ref is already in the array or
      // not
      // in object_manager.check_creation we are going to watch if
      // the ref does not already exists
   }

   ClassDescriptionMaster & cdm = ClassDescManager::use_instance ().get (class_id);
   
   DocumentBase & document = use_document ();
   ObjectManager & object_manager = document.use_object_manager ();

   int err = object_manager.check_creation (
      cdm, ref, tolerant_flag
   );
   
   if (err != Err_OK)
   {
      assert (tolerant_flag);
      return err;
   }
   
   Object * object_ptr = object_manager.create (
      cdm, this, ref
   );
   assert (object_ptr != 0);
   
   ObjectPtrState ops;
   ops._object_ptr = object_ptr;
   ops._state = ObjectPtrState::State_ADDED;
   
   if (it == _key_ptr_map.end ())
   {
      std::pair <typename KeyPtrMap::iterator, bool> rep
         = _key_ptr_map.insert (std::make_pair (key, ops));
      assert (rep.second);
   }
   else
   {
      bool ok_flag
         = (it->second._state == ObjectPtrState::State_REMOVED)
         || (it->second._state == ObjectPtrState::State_MOVED)
      ;
      
      assert (ok_flag);
      it->second._state = ObjectPtrState::State_RESIDENT;
   }
   
   array_changed ();
   
   TravelerObjectInit traveler (document);
   
   ObjectTravelAgency travel_agency (document);
   travel_agency.travel (
      ref,
      ObjectTravelAgency::Flag_DEPTH_FIRST,
      traveler
   );
   
   document.notify_order_changed (*this);
   
   return Err_OK;
}



/*
==============================================================================
Name : erase_no_notification
==============================================================================
*/

template <class T>
int   Array <T>::erase_no_notification (const BinaryKey & key, Ref ref, bool tolerant_flag)
{
   typename KeyPtrMap::iterator it = _key_ptr_map.find (key);
   
   if (it == _key_ptr_map.end ())
   {
      assert (tolerant_flag);
      return Err_VM_ARRAY_KEY_DOES_NOT_EXIST;
   }
   else
   {
      bool does_not_exist_flag
         = (it->second._state == ObjectPtrState::State_REMOVED)
         || (it->second._state == ObjectPtrState::State_MOVED)
      ;
      
      if (does_not_exist_flag)
      {
         assert (tolerant_flag);
         return Err_VM_ARRAY_KEY_DOES_NOT_EXIST;
      }
   }
   
   Object * object_ptr = it->second._object_ptr;
   assert (object_ptr != 0);
   
   DocumentBase & document = use_document ();
   ObjectManager & object_manager = document.use_object_manager ();

   bool can_release_flag = object_manager.can_recycle (object_ptr);
   
   if (!can_release_flag)
   {
      assert (tolerant_flag);
      return Err_VM_ARRAY_OBJECT_CANNOT_BE_RELEASED;
   }
   
   {
      TravelerObjectRestore traveler (document);
         
      ObjectTravelAgency travel_agency (document);
      travel_agency.travel (
         ref,
         ObjectTravelAgency::Flag_REVERSE
            | ObjectTravelAgency::Flag_DEPTH_FIRST,
         traveler
      );
   }
   
   object_manager.recycle (object_ptr);
   
   if (it->second._state == ObjectPtrState::State_ADDED)
   {
      _key_ptr_map.erase (it);
   }
   else
   {
      assert (it->second._state == ObjectPtrState::State_RESIDENT);
      it->second._state = ObjectPtrState::State_REMOVED;
   }
   
   array_changed ();
   
   object_ptr->change_order (-1);
   document.notify_order_changed (*this);
   
   return Err_OK;
}



/*
==============================================================================
Name : move_no_notification
==============================================================================
*/

template <class T>
int   Array <T>::move_no_notification (ArrayMaster * dst_ptr, const BinaryKey & dst_key, const BinaryKey & src_key, bool tolerant_flag)
{
   typename KeyPtrMap::iterator src_it = _key_ptr_map.find (src_key);
   
   if (src_it == _key_ptr_map.end ())
   {
      assert (tolerant_flag);
      return Err_VM_ARRAY_KEY_DOES_NOT_EXIST;
   }
   else
   {
      bool does_not_exist_flag
         = (src_it->second._state == ObjectPtrState::State_REMOVED)
         || (src_it->second._state == ObjectPtrState::State_MOVED)
      ;
      
      if (does_not_exist_flag)
      {
         assert (tolerant_flag);
         return Err_VM_ARRAY_KEY_DOES_NOT_EXIST;
      }
   }
   
   Array <T> * dst_concrete_ptr = dynamic_cast <Array <T> *> (dst_ptr);
   
   if (dst_concrete_ptr == 0)
   {
      assert (tolerant_flag);
      return Err_VM_ARRAY_BAD_ELEMENT_TYPE;
   }
   
   typename KeyPtrMap::iterator dst_it = dst_concrete_ptr->_key_ptr_map.find (dst_key);
   
   if (dst_it != dst_concrete_ptr->_key_ptr_map.end ())
   {
      bool exist_flag
         = (dst_it->second._state == ObjectPtrState::State_RESIDENT)
         || (dst_it->second._state == ObjectPtrState::State_ADDED)
      ;
      
      if (exist_flag)
      {
         assert (tolerant_flag);
         return Err_VM_ARRAY_KEY_EXISTS;
      }
   }
   
   Object * object_ptr = src_it->second._object_ptr;
   assert (object_ptr != 0);
   
   if (src_it->second._state == ObjectPtrState::State_ADDED)
   {
      _key_ptr_map.erase (src_it);
   }
   else
   {
      assert (src_it->second._state == ObjectPtrState::State_RESIDENT);
      src_it->second._state = ObjectPtrState::State_MOVED;
   }
   
   DocumentBase & document = use_document ();
   
   if (this != dst_ptr)
   {
      array_changed ();
      
      object_ptr->change_order (-1);
      document.notify_order_changed (*this);
   
      object_ptr->change_parent (dst_ptr);
   }
   
   ObjectPtrState ops;
   ops._object_ptr = object_ptr;
   ops._state = ObjectPtrState::State_ADDED;
   
   std::pair <typename KeyPtrMap::iterator, bool> rep
      = dst_concrete_ptr->_key_ptr_map.insert (std::make_pair (dst_key, ops));

   if (!rep.second)
   {
      bool ok_flag
         = (rep.first->second._state == ObjectPtrState::State_REMOVED)
         || (rep.first->second._state == ObjectPtrState::State_MOVED)
      ;
      
      assert (ok_flag);
      rep.first->second._state = ObjectPtrState::State_RESIDENT;
   }
   
   dst_concrete_ptr->array_changed ();
   
   document.notify_order_changed (*dst_concrete_ptr);
   
   return Err_OK;
}



/*
==============================================================================
Name : broadcast_order_change
Description :
   Notify every child of its new order in the array.
==============================================================================
*/

template <class T>
int   Array <T>::broadcast_order_change ()
{
   KeyPtrMap_iterator it = _key_ptr_map.begin ();
   const KeyPtrMap_iterator it_end = _key_ptr_map.end ();
      
   size_t nbr = 0;
   for (; it != it_end ; ++it)
   {
      if (it->second._state != ObjectPtrState::State_MOVED)
      {
         size_t sub_nbr = nbr;
         
         if (it->second._state == ObjectPtrState::State_REMOVED)
         {
            sub_nbr = size_t (-1);
         }
         else
         {
            ++nbr;
         }
         
         T * object_ptr = dynamic_cast <T *> (it->second._object_ptr);
         assert (object_ptr != 0);
         
         _child_order_changed_flag |= object_ptr->get_order () != sub_nbr;
         
         object_ptr->change_order (sub_nbr);
      }
   }
   
   return Err_OK;
}



/*
==============================================================================
Name : get_ref
==============================================================================
*/

template <class T>
Ref   Array <T>::get_element_ref (size_t i) const
{
   const_iterator it = begin ();
   it += i;
   
   assert (it != end ());
   
   return it._it->second._object_ptr->get_ref ();
}



/*
==============================================================================
Name : get_key
==============================================================================
*/

template <class T>
const BinaryKey & Array <T>::get_element_key (size_t i) const
{
   const_iterator it = begin ();
   it += i;
   
   return it._it->first;
}



/*
==============================================================================
Name : get_element_class_id
==============================================================================
*/

template <class T>
int   Array <T>::get_element_class_id () const
{
   int ret_val = ClassDescription <T>::use ().get_class_id ();
   assert (ret_val > 0);
   
   return ret_val;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_Array_CODEHEADER_INCLUDED

#undef ohm_flip_Array_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
