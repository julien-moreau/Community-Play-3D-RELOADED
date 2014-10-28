/*****************************************************************************

        Selection.hpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70472

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



#if defined (model_Selection_CURRENT_CODEHEADER)
   #error Recursive inclusion of Selection code header.
#endif
#define  model_Selection_CURRENT_CODEHEADER

#if ! defined (model_Selection_CODEHEADER_INCLUDED)
#define  model_Selection_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "model/Pedestrian.h"

#include <algorithm>



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : tx_select
==============================================================================
*/

template <class InputPtrIterator>
void  Selection::tx_select (InputPtrIterator it, InputPtrIterator it_end, bool toggle_flag)
{
   if (!_tx_session_guard.start ())
   {
      return;
   }

   ext_select (it, it_end, toggle_flag);
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name : ext_select
Note :
   No elements from it it_end are supposed to be identical.
==============================================================================
*/

template <class InputPtrIterator>
void  Selection::ext_select (InputPtrIterator it, InputPtrIterator it_end, bool toggle_flag)
{
   using namespace ohm;
   
   const InputPtrIterator it_start = it;
   
   bool force_clear_flag = false;
   
   {
      InputPtrIterator it_temp = it_start;
      
      for (; it_temp != it_end && !force_clear_flag ; ++it_temp)
      {
         flip::Object * object_ptr = *it_temp;
         
         if (object_ptr != 0)
         {
            force_clear_flag = !accepts (*object_ptr);
         }
      }
   }
   
   
   flip::Object * base_object_ptr = 0;
   
   if (!toggle_flag || force_clear_flag)
   {
      _coll.clear ();

      InputPtrIterator it_temp = it_start;
      
      for (; it_temp != it_end ; ++it_temp)
      {
         flip::Object * object_ptr = *it_temp;
         
         if (object_ptr != 0)
         {
            bool insert_flag = true;
            
            if (base_object_ptr != 0)
            {
               insert_flag = match (*base_object_ptr, *object_ptr);
            }
            
            if (insert_flag)
            {
               base_object_ptr = object_ptr;
               
               _coll.insert (
                  flip::Args ().push (object_ptr->get_ref ())
               );
            }
         }
      }

      // special case end
      return;
   }
   
   {
      ObjectRefColl::iterator it = _coll.begin ();
      const ObjectRefColl::iterator it_end = _coll.end ();
      
      for (; it != it_end ; ++it)
      {
         ObjectRef & object_ref = *it;
         
         base_object_ptr = object_ref.get_object_ptr ();
         
         if (base_object_ptr != 0)
         {
            break;
         }
      }
   }

   if (toggle_flag)
   {
      std::list <flip::Object *> input_ptr_list (it_start, it_end);
      input_ptr_list.sort ();
      
      std::list <ObjPtrIt> cur_ptr_it_list;
      {
         ObjectRefColl::iterator it = _coll.begin ();
         const ObjectRefColl::iterator it_end = _coll.end ();
         
         for ( ; it != it_end ; ++it)
         {
            flip::Object * obj_ptr = it->get_object_ptr ();
            ObjPtrIt obj_ptr_it;
            obj_ptr_it._obj_ptr = obj_ptr;
            obj_ptr_it._it = it;
            
            cur_ptr_it_list.push_back (obj_ptr_it);
         }
         cur_ptr_it_list.sort ();
      }
         
      std::list <flip::Object *> cur_ptr_list;
      {
         ObjectRefColl::iterator it = _coll.begin ();
         const ObjectRefColl::iterator it_end = _coll.end ();
         
         for ( ; it != it_end ; ++it)
         {
            flip::Object * obj_ptr = it->get_object_ptr ();
            cur_ptr_list.push_back (obj_ptr);
         }
         cur_ptr_list.sort ();
      }
         
      // erase elem in (it_start, it_end) & (_coll.begin (), _coll.end ())
      
      {
         static std::vector <ObjPtrIt> output_ptr_arr;
         output_ptr_arr.resize (lang::max (
            std::distance (it_start, it_end),
            std::distance (cur_ptr_it_list.begin (), cur_ptr_it_list.end ())
         ));
         
         std::vector <ObjPtrIt>::iterator it = output_ptr_arr.begin ();
         std::vector <ObjPtrIt>::iterator it_end = std::set_intersection (
            cur_ptr_it_list.begin (), cur_ptr_it_list.end (),
            input_ptr_list.begin (), input_ptr_list.end (),
            it
         );
         
         for (; it != it_end ; ++it)
         {
            _coll.erase (it->_it);
         }
      }
      
      // add elem in (it_start, it_end) - (_coll.begin (), _coll.end ())
      
      {
         static std::vector <flip::Object *> output_ptr_arr;
         output_ptr_arr.resize (std::distance (it_start, it_end));
         
         std::vector <flip::Object *>::iterator it = output_ptr_arr.begin ();
         std::vector <flip::Object *>::iterator it_end = std::set_difference (
            input_ptr_list.begin (), input_ptr_list.end (),
            cur_ptr_list.begin (), cur_ptr_list.end (),
            it
         );
         
         for (; it != it_end ; ++it)
         {
            flip::Object * object_ptr = *it;
            
            bool insert_flag = true;
                     
            if (base_object_ptr != 0)
            {
               insert_flag = match (*base_object_ptr, *object_ptr);
            }
            
            if (insert_flag)
            {
               base_object_ptr = object_ptr;
               
               _coll.insert (
                  flip::Args ().push (object_ptr->get_ref ())
               );
            }
         }
      }
   }
   else
   {
      std::list <flip::Object *> input_ptr_list (it_start, it_end);
      input_ptr_list.sort ();
      
      std::list <flip::Object *> cur_ptr_list;
      {
         ObjectRefColl::iterator it = _coll.begin ();
         const ObjectRefColl::iterator it_end = _coll.end ();
         
         for ( ; it != it_end ; ++it)
         {
            flip::Object * obj_ptr = it->get_object_ptr ();
            cur_ptr_list.push_back (obj_ptr);
         }
         cur_ptr_list.sort ();
      }
      
      // add elem in (it_start, it_end) - (_coll.begin (), _coll.end ())
      
      static std::vector <flip::Object *> output_ptr_arr;
      output_ptr_arr.resize (std::distance (it_start, it_end));
      
      std::vector <flip::Object *>::iterator it = output_ptr_arr.begin ();
      std::vector <flip::Object *>::iterator it_end = std::set_difference (
         input_ptr_list.begin (), input_ptr_list.end (),
         cur_ptr_list.begin (), cur_ptr_list.end (),
         it
      );
      
      for (; it != it_end ; ++it)
      {
         flip::Object * object_ptr = *it;
         
         bool insert_flag = true;
                  
         if (base_object_ptr != 0)
         {
            insert_flag = match (*base_object_ptr, *object_ptr);
         }
         
         if (insert_flag)
         {
            base_object_ptr = object_ptr;
            
            _coll.insert (
               flip::Args ().push (object_ptr->get_ref ())
            );
         }
      }
   }
}



/*
==============================================================================
Name : tx_replace
==============================================================================
*/

template <class InputPtrIterator>
void  Selection::tx_replace (InputPtrIterator it, InputPtrIterator it_end)
{
   if (!_tx_session_guard.start ())
   {
      return;
   }

   ext_replace (it, it_end);
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name : ext_replace
==============================================================================
*/

template <class InputPtrIterator>
void  Selection::ext_replace (InputPtrIterator it, InputPtrIterator it_end)
{
   using namespace ohm;
   
   _coll.clear ();
   
   for (; it != it_end ; ++it)
   {
      flip::Object * object_ptr = *it;
      
      ObjectRefColl::iterator sit = find (*object_ptr);
      
      if (sit == _coll.end ())
      {
         _coll.insert (
            flip::Args ().push (object_ptr->get_ref ())
         );
      }
   }
}



/*
==============================================================================
Name : ext_replace
==============================================================================
*/

template <class UnaryPredicate>
void  Selection::ext_remove_if (UnaryPredicate predicate)
{
   using namespace ohm;
   
   ObjectRefColl::iterator it = _coll.begin ();
   const ObjectRefColl::iterator it_end = _coll.end ();
   
   for ( ; it != it_end ;)
   {
      ObjectRefColl::iterator it_next = it;
      ++it_next;
      
      flip::Object * obj_ptr = it->get_object_ptr ();
      
      if (obj_ptr != 0)
      {
         bool erase_flag = predicate (*obj_ptr);
         
         if (erase_flag)
         {
            _coll.erase (it);
         }
      }
      
      it = it_next;
   }
}



/*
==============================================================================
Name: find_first_of
==============================================================================
*/

template <class T> 
ObjectRefColl::iterator Selection::find_first_of (ohm::flip::FindMask find_mask)
{
   PredicateType <T> predicate;
   
   return _coll.find (predicate, find_mask);
}



/*
==============================================================================
Name: find_first_of
==============================================================================
*/

template <class T> 
ObjectRefColl::const_iterator Selection::find_first_of (ohm::flip::FindMask find_mask) const
{
   PredicateType <T> predicate;
   
   return _coll.find (predicate, find_mask);
}



/*
==============================================================================
Name: contains
==============================================================================
*/

template <class T> 
bool Selection::contains () const
{
   assert (false);
   return false;
}

template <> bool Selection::contains <Taxi> () const;
template <> bool Selection::contains <Pedestrian> () const;



/*
==============================================================================
Name: get_object_ptr_arr
==============================================================================
*/

template <class T> 
void Selection::get_object_ptr_arr (std::vector <T *> & ptr_coll)
{
   ptr_coll.clear ();
   
   ObjectRefColl::iterator it = _coll.begin ();
   const ObjectRefColl::iterator it_end = _coll.end ();
   
   for ( ; it != it_end ; ++it)
   {
      T * obj_ptr = it->get_object_ptr <T> ();
      
      if (obj_ptr != 0)
      {
         ptr_coll.push_back (obj_ptr);
      }
   }
}



/*
==============================================================================
Name: accepts
==============================================================================
*/

template <class T> 
bool Selection::accepts () const
{
   assert (false);
   return false;
}

template <> bool Selection::accepts <Taxi> () const;
template <> bool Selection::accepts <Pedestrian> () const;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : Predicate::ctor
==============================================================================
*/

template <class T>
Selection::PredicateType <T>::PredicateType ()
{
}



/*
==============================================================================
Name : Predicate::equal
==============================================================================
*/

template <class T>
bool  Selection::PredicateType <T>::equal (const ObjectRef & object_ref) const
{
   T * t_ptr = object_ref.get_object_ptr <T> ();
   
   return (t_ptr != 0);
}



/*
==============================================================================
Name: count
==============================================================================
*/

template <class T>
size_t Selection::count (ohm::flip::FindMask find_mask) const
{
   size_t count = 0;
   
   ObjectRefColl::const_iterator it = _coll.begin (find_mask);
   const ObjectRefColl::const_iterator it_end = _coll.end (find_mask);
   
   for ( ; it != it_end ; ++it)
   {
      const T * cast_object_ptr = it->get_object_ptr <T> ();
      
      if (cast_object_ptr != 0)
      {
         count += 1;
      }
   }
   
   return count;
}



}  // namespace model



#endif   // model_Selection_CODEHEADER_INCLUDED

#undef model_Selection_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

