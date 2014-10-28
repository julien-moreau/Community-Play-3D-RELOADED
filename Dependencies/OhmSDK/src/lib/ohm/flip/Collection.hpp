/*****************************************************************************

        Collection.hpp
        Copyright (c) 2009 Ohm Force

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



#if defined (ohm_flip_Collection_CURRENT_CODEHEADER)
   #error Recursive inclusion of Collection code header.
#endif
#define  ohm_flip_Collection_CURRENT_CODEHEADER

#if ! defined (ohm_flip_Collection_CODEHEADER_INCLUDED)
#define  ohm_flip_Collection_CODEHEADER_INCLUDED



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

#include <algorithm>

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
Collection <T>::Collection (DocumentBase & document)
:  CollectionMaster (document)
,  _list ()
,  _old_cur_equal_flag (true)
,  _child_order_changed_flag (false)
{
   // by default no order is applied
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

template <class T>
Collection <T>::~Collection ()
{
   // must not be cleared on destruction
   
   // check that we have only keys to be removed
   
   List_const_iterator it = _list.begin ();
   const List_const_iterator it_end = _list.end ();
         
   for (; it != it_end ; ++it)
   {
      if (it->_state != ObjectWrap::State_REMOVED)
      {
         // there is still an element which is not to be removed
         assert (false);
      }
   }
}



/*
==============================================================================
Name : add_dependency
==============================================================================
*/

template <class T>
void  Collection <T>::add_dependency (Object & object)
{
   use_document ().add_dependency (*this, object);
}



/*
==============================================================================
Name : remove_dependencies
==============================================================================
*/

template <class T>
void  Collection <T>::remove_dependencies ()
{
   use_document ().remove_dependencies (*this);
}



/*
==============================================================================
Name : set_order_fnc
Description :
   Set an order function for this collection.
   The function gets called every time a change is made to the collection itself,
   and the function is responsible to calculate the new order of elements
   in the collection. See 'order_arbitrary' and 'order_less_predicate'
   above for practical implementation examples.
   The function might be null, in which case no order change are broadcasted
   
   IMPORTANT :
   The order function must define a total order in the collection, which means
   that in the case were object are directly comparable, the less predicate
   must be strong and not weak. This is important to ensure that if a change
   is made to the collection, at least one object has a different order, as
   some other component might need to get notified even if they process
   the order differently, and even provide weak order themselves.
   
   Weak ordering = if a < b is false and b > a is false, this does not
   imply that a == b
   
   Strong ordering = if a < b is false and b > a is false, this implies
   that a == b

Note :
   If different observer component were to have different needs, they would
   have to ignore the order change reported by objects, and do their own.
   They would however needs to get notified that the order did change, which
   is why we must ensure that the comparaison fnc is strong.
==============================================================================
*/

/*template <class T>
void  Collection <T>::set_order_fnc (OrderFnc order_fnc)
{
   _order_fnc = order_fnc;
}*/



/*
==============================================================================
Name : begin
==============================================================================
*/

template <class T>
typename Collection <T>::iterator   Collection <T>::begin (FindMask find_mask)
{
   List_iterator it = _list.begin ();
   
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
         if (it == _list.end ())
         {
            break;
         }
         
         bool break_flag
            = (added_flag && (it->_state == ObjectWrap::State_ADDED))
            || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
            || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
         ;
         
         if (break_flag)
         {
            break;
         }
         
         ++it;
      }
   }
   
   return iterator (it, _list.end (), in_post_flag, find_mask);
}



/*
==============================================================================
Name : begin
==============================================================================
*/

template <class T>
typename Collection <T>::const_iterator   Collection <T>::begin (FindMask find_mask) const
{
   List_const_iterator it = _list.begin ();
   
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
         if (it == _list.end ())
         {
            break;
         }
         
         bool break_flag
            = (added_flag && (it->_state == ObjectWrap::State_ADDED))
            || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
            || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
         ;
         
         if (break_flag)
         {
            break;
         }
         
         ++it;
      }
   }
   
   return const_iterator (it, _list.end (), in_post_flag, find_mask);
}



/*
==============================================================================
Name : end
==============================================================================
*/

template <class T>
typename Collection <T>::iterator   Collection <T>::end (FindMask find_mask)
{
   return iterator (
      _list.end (), _list.end (),
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
typename Collection <T>::const_iterator   Collection <T>::end (FindMask find_mask) const
{
   return const_iterator (
      _list.end (), _list.end (),
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
typename Collection <T>::reverse_iterator Collection <T>::rbegin (FindMask find_mask)
{
   List_reverse_iterator it = _list.rbegin ();
   
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
         if (it == _list.rend ())
         {
            break;
         }
         
         bool break_flag
            = (added_flag && (it->_state == ObjectWrap::State_ADDED))
            || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
            || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
         ;
         
         if (break_flag)
         {
            break;
         }
         
         ++it;
      }
   }
   
   return reverse_iterator (it, _list.rend (), in_post_flag, find_mask);
}



/*
==============================================================================
Name : rbegin
==============================================================================
*/

template <class T>
typename Collection <T>::const_reverse_iterator Collection <T>::rbegin (FindMask find_mask) const
{
   List_const_reverse_iterator it = _list.rbegin ();
   
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
         if (it == _list.rend ())
         {
            break;
         }
         
         bool break_flag
            = (added_flag && (it->_state == ObjectWrap::State_ADDED))
            || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
            || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
         ;
         
         if (break_flag)
         {
            break;
         }
         
         ++it;
      }
   }
   
   return const_reverse_iterator (it, _list.rend (), in_post_flag, find_mask);
}



/*
==============================================================================
Name : rend
==============================================================================
*/

template <class T>
typename Collection <T>::reverse_iterator Collection <T>::rend (FindMask find_mask)
{
   return reverse_iterator (
      _list.rend (), _list.rend (),
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
typename Collection <T>::const_reverse_iterator Collection <T>::rend (FindMask find_mask) const
{
   return const_reverse_iterator (
      _list.rend (), _list.rend (),
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
typename Collection <T>::iterator   Collection <T>::find (const T & elem, FindMask find_mask)
{
   List_iterator it = _list.begin ();
   List_iterator it_end = _list.end ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->_state == ObjectWrap::State_ADDED))
         || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
         || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
      ;
      
      if (parse_flag)
      {
         const T * ptr = it->get ();
         assert (ptr != 0);
         
         bool equal_flag = (&elem == ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return iterator (it, _list.end (), use_document ().is_in_post (), find_mask);
}



/*
==============================================================================
Name : find
==============================================================================
*/

template <class T>
typename Collection <T>::const_iterator   Collection <T>::find (const T & elem, FindMask find_mask) const
{
   List_const_iterator it = _list.begin ();
   List_const_iterator it_end = _list.end ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->_state == ObjectWrap::State_ADDED))
         || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
         || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
      ;
      
      if (parse_flag)
      {
         const T * ptr = it->get ();
         assert (ptr != 0);
         
         bool equal_flag = (&elem == ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return const_iterator (it, _list.end (), use_document ().is_in_post (), find_mask);
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
   
   Collection <SomeFlipClass> & collection = get_some_collection ();
   Collection <SomeFlipClass>::iterator it = collection.find (my_predicate);
   
==============================================================================
*/

template <class T>
template <class Predicate>
typename Collection <T>::iterator   Collection <T>::find (const Predicate & predicate, FindMask find_mask)
{
   List_iterator it = _list.begin ();
   List_iterator it_end = _list.end ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->_state == ObjectWrap::State_ADDED))
         || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
         || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
      ;
      
      if (parse_flag)
      {
         const T * ptr = it->get ();
         assert (ptr != 0);
         
         bool equal_flag = predicate.equal (*ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return iterator (it, _list.end (), use_document ().is_in_post (), find_mask);
}


   
/*
==============================================================================
Name : find
==============================================================================
*/

template <class T>
template <class Predicate>
typename Collection <T>::const_iterator   Collection <T>::find (const Predicate & predicate, FindMask find_mask) const
{
   List_const_iterator it = _list.begin ();
   List_const_iterator it_end = _list.end ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->_state == ObjectWrap::State_ADDED))
         || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
         || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
      ;
      
      if (parse_flag)
      {
         const T * ptr = it->get ();
         assert (ptr != 0);
         
         bool equal_flag = predicate.equal (*ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return const_iterator (it, _list.end (), use_document ().is_in_post (), find_mask);
}



/*
==============================================================================
Name : rfind
==============================================================================
*/

template <class T>
typename Collection <T>::reverse_iterator Collection <T>::rfind (const T & elem, FindMask find_mask)
{
   List_reverse_iterator it = _list.rbegin ();
   List_reverse_iterator it_end = _list.rend ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->_state == ObjectWrap::State_ADDED))
         || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
         || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
      ;
      
      if (parse_flag)
      {
         const T * ptr = it->get ();
         assert (ptr != 0);
         
         bool equal_flag = (&elem == ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return reverse_iterator (it, _list.rend (), use_document ().is_in_post (), find_mask);
}



/*
==============================================================================
Name : rfind
==============================================================================
*/

template <class T>
typename Collection <T>::const_reverse_iterator Collection <T>::rfind (const T & elem, FindMask find_mask) const
{
   List_const_reverse_iterator it = _list.rbegin ();
   List_const_reverse_iterator it_end = _list.rend ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->_state == ObjectWrap::State_ADDED))
         || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
         || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
      ;
      
      if (parse_flag)
      {
         const T * ptr = it->get ();
         assert (ptr != 0);
         
         bool equal_flag = (&elem == ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return const_reverse_iterator (it, _list.rend (), use_document ().is_in_post (), find_mask);
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
   
   Collection <SomeFlipClass> & collection = get_some_collection ();
   Collection <SomeFlipClass>::reverse_iterator it = collection.rfind (my_predicate);
   
==============================================================================
*/

template <class T>
template <class Predicate>
typename Collection <T>::reverse_iterator Collection <T>::rfind (const Predicate & predicate, FindMask find_mask)
{
   List_reverse_iterator it = _list.rbegin ();
   List_reverse_iterator it_end = _list.rend ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->_state == ObjectWrap::State_ADDED))
         || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
         || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
      ;
      
      if (parse_flag)
      {
         const T * ptr = it->get ();
         assert (ptr != 0);
         
         bool equal_flag = predicate.equal (*ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return reverse_iterator (it, _list.rend (), use_document ().is_in_post (), find_mask);
}


   
/*
==============================================================================
Name : rfind
==============================================================================
*/

template <class T>
template <class Predicate>
typename Collection <T>::const_reverse_iterator Collection <T>::rfind (const Predicate & predicate, FindMask find_mask) const
{
   List_const_reverse_iterator it = _list.begin ();
   List_const_reverse_iterator it_end = _list.end ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->_state == ObjectWrap::State_ADDED))
         || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
         || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
      ;
      
      if (parse_flag)
      {
         const T * ptr = it->get ();
         assert (ptr != 0);
         
         bool equal_flag = predicate.equal (*ptr);
         
         if (equal_flag)
         {
            break;
         }
      }
   }
   
   return const_reverse_iterator (it, _list.rend (), use_document ().is_in_post (), find_mask);
}



/*
==============================================================================
Name : size
Description :
   only count activated or zombie objects
==============================================================================
*/

template <class T>
size_t   Collection <T>::size (FindMask find_mask) const
{
   size_t ret_val = 0;
   
   List_const_iterator it = _list.begin ();
   const List_const_iterator it_end = _list.end ();
   
   bool in_post_flag = use_document ().is_in_post ();
   bool parse_all_flag = in_post_flag && ((find_mask & FindMask_POST) != 0);
   bool added_flag = (find_mask & FindMask_ADDED) != 0;
   bool resident_flag = (find_mask & FindMask_RESIDENT) != 0;
   bool removed_flag = (find_mask & FindMask_REMOVED) != 0;
   
   for (; it != it_end ; ++it)
   {
      bool parse_flag
         = parse_all_flag
         || (added_flag && (it->_state == ObjectWrap::State_ADDED))
         || (resident_flag && (it->_state == ObjectWrap::State_RESIDENT))
         || (removed_flag && (it->_state == ObjectWrap::State_REMOVED))
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
Name : empty
==============================================================================
*/

template <class T>
bool  Collection <T>::empty (FindMask find_mask) const
{
   return (size (find_mask) == 0);
}



/*
==============================================================================
Name : front
==============================================================================
*/

template <class T>
typename Collection <T>::reference  Collection <T>::front ()
{
   assert (!empty ());
   
   return *begin ();
}



/*
==============================================================================
Name : front
==============================================================================
*/

template <class T>
typename Collection <T>::const_reference  Collection <T>::front () const
{
   assert (!empty ());
   
   return *begin ();
}



/*
==============================================================================
Name : back
==============================================================================
*/

template <class T>
typename Collection <T>::reference  Collection <T>::back ()
{
   assert (!empty ());

   return *rbegin ();
}



/*
==============================================================================
Name : back
==============================================================================
*/

template <class T>
typename Collection <T>::const_reference  Collection <T>::back () const
{
   assert (!empty ());

   return *rbegin ();
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
typename Collection <T>::iterator   Collection <T>::insert (Args & args)
{
   return insert <T> (args);
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
typename Collection <T>::iterator   Collection <T>::insert (Args & args)
{
   Ref this_ref = Object::get_ref ();
   assert (this_ref != 0);
   
   DocumentBase & document = use_document ();
   ObjectManager & object_manager = document.use_object_manager ();
   
   Ref ref = object_manager.get_planned_ref ();
   
   ClassDescriptionMaster & cdm = ClassDescription <U>::use ();
   
   Data & transaction = document.use_transaction ();
   
   MachineCode::build_collection_insert (transaction, this_ref, ref, cdm.get_class_id ());
   
   T * object_ptr = dynamic_cast <T *> (object_manager.create (cdm, this, ref, args));
   assert (object_ptr != 0);
   
   List_iterator new_it = _list.insert (_list.end (), ObjectWrap (object_ptr));
   
   // User Inputs, _list is necesserally different from _old_list
   collection_changed_invalidate ();

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
   
   return iterator (new_it, _list.end (), false, FindMask_NATURAL);
}



/*
==============================================================================
Name : erase
==============================================================================
*/

template <class T>
void  Collection <T>::erase (iterator it)
{
   Object * object_ptr = it._it->get ();
   assert (object_ptr != 0);

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
   
   MachineCode::build_collection_erase (
      transaction, this_ref, obj_ref, cdm.get_class_id ()
   );
   
   if (it._it->_state == ObjectWrap::State_ADDED)
   {
      _list.erase (it._it);
   }
   else
   {
      assert (it._it->_state == ObjectWrap::State_RESIDENT);
      it._it->_state = ObjectWrap::State_REMOVED;
   }
   
   // User Inputs, _list can be the same as _old_list
   collection_changed ();

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
void  Collection <T>::erase (iterator begin, iterator end)
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
void  Collection <T>::erase (Ref ref)
{
   List_iterator it = _list.begin ();
   List_iterator it_end = _list.end ();
   
   for (; it != it_end ; ++it)
   {
      Object * ptr = it->get ();
      
      if (ptr->get_ref () == ref)
      {
         break;
      }
   }
   
   assert (it != it_end);
   
   assert (!use_document ().is_in_post ());
   
   erase (iterator (it, _list.end (), false, FindMask_NATURAL));
}



/*
==============================================================================
Name : clear
==============================================================================
*/

template <class T>
void  Collection <T>::clear ()
{
   erase (begin (), end ());
}



/*
==============================================================================
Name : move
==============================================================================
*/

template <class T>
typename Collection <T>::iterator   Collection <T>::move (Collection <T> & src_collection, iterator src_it)
{
   assert (src_it._it->_state != ObjectWrap::State_REMOVED);
   
   if (&src_collection == this)
   {
      return src_it;
   }
   
   Ref src_ref = src_collection.Object::get_ref ();
   assert (src_ref != 0);

   Ref dst_ref = Object::get_ref ();
   assert (dst_ref != 0);
   
   Object * object_ptr = src_it._it->get ();
   assert (object_ptr != 0);
   
   Ref pointed_ref = object_ptr->Object::get_ref ();

   DocumentBase & document = use_document ();
   
   Data & transaction = document.use_transaction ();
   
   MachineCode::build_collection_move (
      transaction, src_ref, pointed_ref, dst_ref
   );
   
   if (src_it._it->_state == ObjectWrap::State_ADDED)
   {
      src_collection._list.erase (src_it._it);
   }
   else
   {
      assert (src_it._it->_state == ObjectWrap::State_RESIDENT);
      src_it._it->_state = ObjectWrap::State_REMOVED;
   }
   
   // User Inputs, _list can be the same as _old_list
   src_collection.collection_changed ();

   object_ptr->change_order (-1);
   //src_collection.broadcast_order_change ();
   
   T * obj_ptr = dynamic_cast <T *> (object_ptr);
   assert (obj_ptr != 0);
   
   List_iterator new_it = _list.insert (_list.end (), ObjectWrap (obj_ptr));
   
   // User Inputs, _list can be the same as _old_list
   collection_changed ();
   
   assert (&src_collection != this);
   object_ptr->change_parent (this);
   
   //broadcast_order_change ();
   
   assert (!use_document ().is_in_post ());
   
   document.notify_order_changed (src_collection);
   document.notify_order_changed (*this);

   return iterator (new_it, _list.end (), false, FindMask_NATURAL);
}



/*
==============================================================================
Name : did_value_change
==============================================================================
*/

template <class T>
bool  Collection <T>::did_value_change () const
{
   return !_old_cur_equal_flag;
}



/*
==============================================================================
Name : did_child_order_change
==============================================================================
*/

template <class T>
bool  Collection <T>::did_child_order_change () const
{
   return _child_order_changed_flag;
}



/*
==============================================================================
Name : synchronize
==============================================================================
*/

template <class T>
void  Collection <T>::synchronize ()
{
   // synchronize children that needs it
   
   List_iterator it = _list.begin ();
   const List_iterator it_end = _list.end ();
   
   for (; it != it_end ;)
   {
      List_iterator next_it = it;
      ++next_it;
      
      if (it->_state == ObjectWrap::State_REMOVED)
      {
         _list.erase (it);
      }
      else
      {
         Object * sub_obj_ptr = it->get ();
         
         if (sub_obj_ptr->is_invalid ())
         {
            sub_obj_ptr->synchronize ();
         }
         
         it->_state = ObjectWrap::State_RESIDENT;
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
void  Collection <T>::check_synchronized () const
{
   // synchronize children that needs it
   
   List_const_iterator it = _list.begin ();
   const List_const_iterator it_end = _list.end ();
   
   for (; it != it_end ; ++it)
   {
      assert (it->_state == ObjectWrap::State_RESIDENT);
      
      Object * sub_obj_ptr = it->get ();
      
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
void  Collection <T>::check_inner_invalidation_cnt (size_t invalid_cnt) const
{
   if (!_old_cur_equal_flag)
   {
      ohm_util_TRACE_DEBUG ("  COL");
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
typename Collection <T>::iterator   Collection <T>::paste (ObjectMold & mold)
{
   ClassDescriptionMaster & cdm = mold.get_cdm ();
   
   Ref ref = insert (cdm);
   
   mold.execute (use_document (), ref);
   
   List_iterator it = _list.begin ();
   const List_iterator it_end = _list.end ();
   
   for (; it != it_end ; ++it)
   {
      T * obj_ptr = it->get ();
      
      if (obj_ptr->get_ref () == ref)
      {
         assert (!use_document ().is_in_post ());
         
         return iterator (it, _list.end (), false, FindMask_NATURAL);
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
typename Collection <T>::iterator   Collection <T>::paste (ObjectMold & mold, RefRefArr & ref_ref_arr)
{
   ClassDescriptionMaster & cdm = mold.get_cdm ();
   
   Ref ref = insert (cdm);
   
   mold.execute (use_document (), ref, ref_ref_arr);
   
   List_iterator it = _list.begin ();
   const List_iterator it_end = _list.end ();
   
   for (; it != it_end ; ++it)
   {
      T * obj_ptr = it->get ();
      
      if (obj_ptr->get_ref () == ref)
      {
         assert (!use_document ().is_in_post ());
         
         return iterator (it, _list.end (), false, FindMask_NATURAL);
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
Ref   Collection <T>::insert (ClassDescriptionMaster & cdm)
{
   Ref this_ref = Object::get_ref ();
   assert (this_ref != 0);
   
   DocumentBase & document = use_document ();
   ObjectManager & object_manager = document.use_object_manager ();
   
   Ref ref = object_manager.get_planned_ref ();
   
   Data & transaction = document.use_transaction ();
   
   MachineCode::build_collection_insert (transaction, this_ref, ref, cdm.get_class_id ());
   
   T * object_ptr = dynamic_cast <T *> (object_manager.create (cdm, this, ref));
   
   List_iterator new_it = _list.insert (_list.end (), ObjectWrap (object_ptr));
   
   // User Inputs, _list is necesserally different from _old_list
   collection_changed_invalidate ();
   
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



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : collection_changed
==============================================================================
*/

template <class T>
void  Collection <T>::collection_changed ()
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
      
      List_iterator it = _list.begin ();
      const List_iterator it_end = _list.end ();
      
      for (; it != it_end ; ++it)
      {
         if (it->_state != ObjectWrap::State_RESIDENT)
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
Name : collection_changed_invalidate
Description :
   Used in the case where a validate cannot occur
==============================================================================
*/

template <class T>
void  Collection <T>::collection_changed_invalidate ()
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
int   Collection <T>::insert_no_notification (Ref ref, int class_id, bool tolerant_flag)
{
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
   
   T * object_ptr = dynamic_cast <T *> (object_manager.create (
      cdm, this, ref
   ));
   assert (object_ptr != 0);
   
   List_iterator it = std::find (_list.begin (), _list.end (), ObjectWrap (object_ptr));
   
   if (it != _list.end ())
   {
      if (it->_state != ObjectWrap::State_REMOVED)
      {
         assert (tolerant_flag);
         return Err_VM_COLLECTION_OBJECT_EXISTS;
      }
      
      it->_state = ObjectWrap::State_RESIDENT;
   }
   else
   {
      _list.insert (_list.end (), ObjectWrap (object_ptr));
   }
   
   collection_changed ();

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
int   Collection <T>::erase_no_notification (Ref ref, bool tolerant_flag)
{
   List_iterator it = _list.begin ();
   const List_iterator it_end = _list.end ();
   
   Object * object_ptr = 0;
   List_iterator ers_it = _list.end ();
   
   for (; it != it_end ; ++it)
   {
      Object * sub_object_ptr = it->get ();
      assert (sub_object_ptr != 0);
      
      if (sub_object_ptr->get_ref () == ref)
      {
         assert (object_ptr == 0);
         assert (ers_it == _list.end ());
         
         object_ptr = sub_object_ptr;
         ers_it = it;

#if defined (NDEBUG)
         break;
#endif
      }
   }
   
   if (ers_it == _list.end ())
   {
      assert (tolerant_flag);
      return Err_VM_COLLECTION_OBJECT_DOES_NOT_EXIST;
   }
   else
   {
      if (ers_it->_state == ObjectWrap::State_REMOVED)
      {
         assert (tolerant_flag);
         return Err_VM_COLLECTION_OBJECT_DOES_NOT_EXIST;
      }
   }
   
   DocumentBase & document = use_document ();
   ObjectManager & object_manager = document.use_object_manager ();

   bool can_release_flag = object_manager.can_recycle (object_ptr);
   
   if (!can_release_flag)
   {
      assert (tolerant_flag);
      return Err_VM_COLLECTION_OBJECT_CANNOT_BE_RELEASED;
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
   
   if (ers_it->_state == ObjectWrap::State_ADDED)
   {
      _list.erase (ers_it);
   }
   else
   {
      assert (ers_it->_state == ObjectWrap::State_RESIDENT);
      ers_it->_state = ObjectWrap::State_REMOVED;
   }
   
   collection_changed ();

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
int   Collection <T>::move_no_notification (CollectionMaster * dst_ptr, Object * obj_ptr, bool tolerant_flag)
{
   List_iterator it = std::find (
      _list.begin (), _list.end (), ObjectWrap (dynamic_cast <T *> (obj_ptr))
   );
   
   if (it == _list.end ())
   {
      assert (tolerant_flag);
      return Err_VM_COLLECTION_OBJECT_DOES_NOT_EXIST;
   }
   else
   {
      if (it->_state == ObjectWrap::State_REMOVED)
      {
         assert (tolerant_flag);
         return Err_VM_COLLECTION_OBJECT_DOES_NOT_EXIST;
      }
   }
   
   Collection <T> * dst_concrete_ptr = dynamic_cast <Collection <T> *> (dst_ptr);
   
   if (dst_concrete_ptr == 0)
   {
      assert (tolerant_flag);
      return Err_VM_COLLECTION_BAD_ELEMENT_TYPE;
   }
   
   List_iterator tit = std::find (
      dst_concrete_ptr->_list.begin (),
      dst_concrete_ptr->_list.end (),
      ObjectWrap (dynamic_cast <T *> (obj_ptr))
   );
   
   if (tit != dst_concrete_ptr->_list.end ())
   {
      if (tit->_state != ObjectWrap::State_REMOVED)
      {
         assert (tolerant_flag);
         return Err_VM_COLLECTION_OBJECT_EXISTS;
      }
   }
   
   if (it->_state == ObjectWrap::State_ADDED)
   {
      _list.erase (it);
   }
   else
   {
      assert (it->_state == ObjectWrap::State_RESIDENT);
      it->_state = ObjectWrap::State_REMOVED;
   }
   
   DocumentBase & document = use_document ();
   
   if (this != dst_ptr)
   {
      collection_changed ();

      obj_ptr->change_order (-1);
      document.notify_order_changed (*this);
   
      obj_ptr->change_parent (dst_ptr);
   }
   
   if (tit == dst_concrete_ptr->_list.end ())
   {
      dst_concrete_ptr->_list.insert (
         dst_concrete_ptr->_list.end (),
         ObjectWrap (dynamic_cast <T *> (obj_ptr))
      );
   }
   else
   {
      assert (tit->_state == ObjectWrap::State_REMOVED);
      tit->_state = ObjectWrap::State_RESIDENT;
   }
   
   dst_concrete_ptr->collection_changed ();
   
   document.notify_order_changed (*dst_concrete_ptr);
   
   return Err_OK;
}



/*
==============================================================================
Name : broadcast_order_change
Description :
   Notify every child of its new order in the collection if an order fnc
   is defined.
==============================================================================
*/

template <class T>
int   Collection <T>::broadcast_order_change ()
{
   _list.sort ();
   
   {
      List_iterator it = _list.begin ();
      const List_iterator it_end = _list.end ();
      
      size_t nbr = 0;
      for (; it != it_end ; ++it)
      {
         size_t sub_nbr = nbr;
         
         if (it->_state == ObjectWrap::State_REMOVED)
         {
            sub_nbr = size_t (-1);
         }
         else
         {
            ++nbr;
         }
         
         T * object_ptr = dynamic_cast <T *> (it->get ());
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
Ref   Collection <T>::get_element_ref (size_t i) const
{
   const_iterator it = begin ();
   it += i;
   
   assert (it != end ());
   
   return it->get_ref ();
}



/*
==============================================================================
Name : get_element_class_id
==============================================================================
*/

template <class T>
int   Collection <T>::get_element_class_id () const
{
   int ret_val = ClassDescription <T>::use ().get_class_id ();
   assert (ret_val > 0);
   
   return ret_val;
}



/*
==============================================================================
Name : ObjectWrap::ctor
==============================================================================
*/

template <class T>
Collection <T>::ObjectWrap::ObjectWrap (T * obj_ptr)
:  _obj_ptr (obj_ptr)
,  _state (State_ADDED)
{
   assert (obj_ptr != 0);
}



/*
==============================================================================
Name : ObjectWrap::operator <
==============================================================================
*/

template <class T>
bool  Collection <T>::ObjectWrap::operator < (const ObjectWrap & other) const
{
   assert (_obj_ptr != 0);
   assert (other._obj_ptr != 0);
   
   // you must provide bool T::operator < () const
   // the operator *must* give a total order, that is :
   // if a < b is false and b < a is false then this imply that a == b
   // using lexicographical order, it is always possible
   // to ensure a total order by giving get_ref () < other.get_ref ()
   // as the last predicate
   
   return (*_obj_ptr < *other._obj_ptr);
}



/*
==============================================================================
Name : ObjectWrap::operator ==
==============================================================================
*/

template <class T>
bool  Collection <T>::ObjectWrap::operator == (const ObjectWrap & other) const
{
   assert (_obj_ptr != 0);
   assert (other._obj_ptr != 0);

#if ! defined (NDEBUG)
   if (_obj_ptr != other._obj_ptr)
   {
      assert (_obj_ptr->get_ref () != other._obj_ptr->get_ref ());
   }
#endif
   
   // we don't need to compare object refs here, as the caller, by
   // calling object_manager.check_creation, ensure that the ref
   // is available
   
   return (_obj_ptr == other._obj_ptr);
}



/*
==============================================================================
Name : ObjectWrap::get
==============================================================================
*/

template <class T>
T *   Collection <T>::ObjectWrap::get () const
{
   assert (_obj_ptr != 0);
   
   return _obj_ptr;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



#endif   // ohm_flip_Collection_CODEHEADER_INCLUDED

#undef ohm_flip_Collection_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
