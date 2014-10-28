/*****************************************************************************

        Selection.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70481

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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "model/Selection.h"

#include "model/fnc.h"
#include "model/Root.h"
#include "model/SelectionChangeInfo.h"

#include "ohm/flip/PasteBoard.h"
#include "ohm/util/BinaryStreamOutput.h"

#include <limits>

#include <cassert>



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : declare
==============================================================================
*/

void  Selection::declare ()
{
   using namespace ohm::flip;
   
   ClassDescription <Selection>::use ().set_name ("skeleton.Selection");
   ClassDescription <Selection>::use ().push_var_desc (&Selection::_coll, "_coll");
   
   ClassDescManager::declare (ClassDescription <Selection>::use ());
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Selection::Selection (ohm::flip::DocumentBase & document)
:  ohm::flip::Object (document)
,  _coll (document)

,  _tx_session_guard (document)

,  _taxi_cnt (0)
,  _pedestrian_cnt (0)
{
   document.require_sync_operations (*this);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Selection::~Selection ()
{
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  Selection::ctor (ohm::flip::Args & args)
{
}



/*
==============================================================================
Name: pre_sync
Description :
   Update our cache of object type maintained in the selection.
   This is used to optimize 'accepts' and 'contains'
==============================================================================
*/

void  Selection::pre_sync ()
{
   using namespace ohm;
   
   typedef flip::ClassDescriptionMaster Cdm;
   
   static const Cdm * cdm_taxi_ptr = &flip::ClassDescription <Taxi>::use ();
   static const Cdm * cdm_pedestrian_ptr = &flip::ClassDescription <Pedestrian>::use ();

   ObjectRefColl::iterator it = _coll.begin ();
   const ObjectRefColl::iterator it_end = _coll.end ();

   for (; it != it_end ; ++it)
   {
      ObjectRef & object_ref = *it;
      
      if (object_ref.is_invalid ())
      {
         flip::Object * object_ptr = object_ref.get_object_ptr ();
         
         if (object_ptr != 0)
         {
            const Cdm * cdm_ptr = &object_ptr->get_cdm ();
         
            if (object_ref.was_inited ())
            {
               if (cdm_ptr == cdm_taxi_ptr)
               {
                  ++_taxi_cnt;
               }
               else if (cdm_ptr == cdm_pedestrian_ptr)
               {
                  ++_pedestrian_cnt;
               }
            }
            else if (object_ref.was_restored ())
            {
               if (cdm_ptr == cdm_taxi_ptr)
               {
                  assert (_taxi_cnt > 0);
                  --_taxi_cnt;
               }
               else if (cdm_ptr == cdm_pedestrian_ptr)
               {
                  assert (_pedestrian_cnt > 0);
                  --_pedestrian_cnt;
               }
            }
         }
      }
   }
}



/*
==============================================================================
Name : tx_clear
==============================================================================
*/

void  Selection::tx_clear ()
{
   if (!_tx_session_guard.start ())
   {
      return;
   }

   ext_clear ();
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name : ext_clear
==============================================================================
*/

void  Selection::ext_clear ()
{
   _coll.clear ();
}



/*
==============================================================================
Name : tx_delete_content
==============================================================================
*/

void  Selection::tx_delete_content ()
{
   Root & root = get_ancestor <Root> ();

   _tx_session_guard.prepare_record (root.use_scribe ());
   
   if (!_tx_session_guard.start ())
   {
      return;
   }

   ext_delete_content ();

   _tx_session_guard.commit ();
}


   
/*
==============================================================================
Name: ext_delete_content
==============================================================================
*/

void Selection::ext_delete_content ()
{
   using namespace ohm;
   
   Root & root = get_ancestor <Root> ();
   
   root.set_scribe_metadata ("Delete Selection");
   
   ObjectRefColl::iterator it = _coll.begin ();
   const ObjectRefColl::iterator it_end = _coll.end ();
   
   typedef flip::ClassDescriptionMaster Cdm;
   
   static const Cdm * cdm_taxi_ptr = &flip::ClassDescription <Taxi>::use ();
   static const Cdm * cdm_pedestrian_ptr = &flip::ClassDescription <Pedestrian>::use ();
   
   for (; it != it_end ; ++it)
   {
      ObjectRef & object_ref = *it;
      
      flip::Object * object_ptr = object_ref.get_object_ptr ();
      
      if (object_ptr != 0)
      {
         const Cdm * cdm_ptr = &object_ptr->get_cdm ();
         
         if (cdm_ptr == cdm_taxi_ptr)
         {
            object_ptr->erase_from_parent ();
         }
         else if (cdm_ptr == cdm_pedestrian_ptr)
         {
            object_ptr->erase_from_parent ();
         }
      }
   }
   
   _coll.clear ();
}



/*
==============================================================================
Name : copy_content
==============================================================================
*/

void  Selection::copy_content ()
{
   using namespace ohm;
   
   ObjectRefColl::iterator it = _coll.begin ();
   const ObjectRefColl::iterator it_end = _coll.end ();
   
   for (; it != it_end ; ++it)
   {
      ObjectRef & object_ref = *it;
      
      flip::Object * object_ptr = object_ref.get_object_ptr ();
      
      if (object_ptr != 0)
      {
         object_ptr->copy ();
      }
   }
}



/*
==============================================================================
Name: tx_select
==============================================================================
*/

void Selection::tx_select (ohm::flip::Object & object, bool toggle_flag)
{
   using namespace ohm;
   
   if (!_tx_session_guard.start ())
   {
      return;
   }

   ObjectRefColl::iterator it = find (object);
      
   if (toggle_flag)
   {
      if (it != _coll.end ())
      {
         _coll.erase (it);
      }
      else
      {
         if (! accepts (object))
         {
            _coll.clear ();
         }

         _coll.insert (
            flip::Args ().push (object.get_ref ())
         );
      }
   }
   else
   {
      if (it == _coll.end ())
      {
         _coll.clear ();
         _coll.insert (
            flip::Args ().push (object.get_ref ())
         );
      }
   }

   _tx_session_guard.commit ();
}



/*
==============================================================================
Name: tx_replace
==============================================================================
*/

void Selection::tx_replace (const ohm::flip::Object & object)
{
   if (!_tx_session_guard.start ())
   {
      return;
   }

   ext_replace (object);
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name: ext_replace
==============================================================================
*/

void Selection::ext_replace (const ohm::flip::Object & object)
{
   using namespace ohm;
   
   _coll.clear ();
   
   _coll.insert (
      flip::Args ().push (object.get_ref ())
   );
}



/*
==============================================================================
Name: find
==============================================================================
*/

ObjectRefColl::iterator Selection::find (const ohm::flip::Object & object, ohm::flip::FindMask find_mask)
{
   Predicate predicate (object);
   
   return _coll.find (predicate, find_mask);
}



/*
==============================================================================
Name: find
==============================================================================
*/

ObjectRefColl::const_iterator Selection::find (const ohm::flip::Object & object, ohm::flip::FindMask find_mask) const
{
   Predicate predicate (object);
   
   return _coll.find (predicate, find_mask);
}



/*
==============================================================================
Name: contains
==============================================================================
*/

bool Selection::contains (const ohm::flip::Object & object, ohm::flip::FindMask find_mask) const
{
   return (find (object, find_mask) != _coll.end (find_mask));
}



/*
==============================================================================
Name: contains
==============================================================================
*/

template <> bool Selection::contains <Taxi> () const
{
   return _taxi_cnt > 0;
}



/*
==============================================================================
Name: contains
==============================================================================
*/

template <> bool Selection::contains <Pedestrian> () const
{
   return _pedestrian_cnt > 0;
}



/*
==============================================================================
Name: match
==============================================================================
*/

bool Selection::match (ohm::flip::Object & object, ohm::flip::Object & other) const
{
   using namespace ohm;
   
   typedef flip::ClassDescriptionMaster Cdm;

   static const Cdm * cdm_taxi_ptr = &flip::ClassDescription <Taxi>::use ();
   static const Cdm * cdm_pedestrian_ptr = &flip::ClassDescription <Pedestrian>::use ();
   
   Cdm * cdm_ptr = &object.get_cdm ();
   Cdm * other_cdm_ptr = &other.get_cdm ();

   if (
      (cdm_ptr == cdm_taxi_ptr)
      || (cdm_ptr == cdm_pedestrian_ptr)
      )
   {
      return (other_cdm_ptr == cdm_taxi_ptr)
         || (other_cdm_ptr == cdm_pedestrian_ptr);
   }
   
   return false;
}



/*
==============================================================================
Name: accepts
==============================================================================
*/

bool Selection::accepts (ohm::flip::Object & object) const
{
   using namespace ohm;
   
   if (_coll.empty ())
   {
      return true;
   }

   typedef flip::ClassDescriptionMaster Cdm;

   static const Cdm * cdm_taxi_ptr = &flip::ClassDescription <Taxi>::use ();
   static const Cdm * cdm_pedestrian_ptr = &flip::ClassDescription <Pedestrian>::use ();
   
   Cdm * cdm_ptr = &object.get_cdm ();
   
   if (
      (cdm_ptr == cdm_taxi_ptr)
      || (cdm_ptr == cdm_pedestrian_ptr)
      )
   {
      return accepts <Taxi> ();
   }
   
   return false;
}



/*
==============================================================================
Name: accepts
==============================================================================
*/

template <> 
bool Selection::accepts <Taxi> () const
{
   if (_coll.empty ())
   {
      return true;
   }
   else if (contains <Taxi> ())
   {
      return true;
   }
   else if (contains <Pedestrian> ())
   {
      return true;
   }
   
   return false;
}



/*
==============================================================================
Name: accepts
==============================================================================
*/

template <> 
bool Selection::accepts <Pedestrian> () const
{
   if (_coll.empty ())
   {
      return true;
   }
   else if (contains <Taxi> ())
   {
      return true;
   }
   else if (contains <Pedestrian> ())
   {
      return true;
   }
   
   return false;
}



/*
==============================================================================
Name: use
==============================================================================
*/

ObjectRefColl & Selection::use ()
{
   return _coll;
}



/*
==============================================================================
Name: tx_taxi_move
==============================================================================
*/

void Selection::tx_taxi_move (ohm::archi::Float64 & position_x_delta)
{
   Root & root = get_ancestor <Root> ();

   _tx_session_guard.prepare_record (root.use_scribe ());
   
   if (!_tx_session_guard.start ())
   {
      return;
   }
   
   session_taxi_move (position_x_delta);
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name: tx_taxi_change_width
==============================================================================
*/

void Selection::tx_taxi_change_width (ohm::archi::Float64 & width_delta)
{
   Root & root = get_ancestor <Root> ();

   _tx_session_guard.prepare_record (root.use_scribe ());
   
   if (!_tx_session_guard.start ())
   {
      return;
   }
   
   session_taxi_change_width (width_delta);
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name: tx_taxi_set_color
==============================================================================
*/

void Selection::tx_taxi_set_color (Taxi::Color color)
{
   Root & root = get_ancestor <Root> ();

   _tx_session_guard.prepare_record (root.use_scribe ());
   
   if (!_tx_session_guard.start ())
   {
      return;
   }
   
   session_taxi_set_color (color);
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name: tx_pedestrian_move
==============================================================================
*/

void Selection::tx_pedestrian_move (ohm::archi::Float64 & position_x_delta, ohm::archi::Float64 & position_y_delta)
{
   Root & root = get_ancestor <Root> ();

   _tx_session_guard.prepare_record (root.use_scribe ());
   
   if (!_tx_session_guard.start ())
   {
      return;
   }
   
   session_pedestrian_move (position_x_delta, position_y_delta);
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name: can_start_session
==============================================================================
*/

bool Selection::can_start_session () const
{
   return _tx_session_guard.can_start ();
}



/*
==============================================================================
Name: start_session
==============================================================================
*/

bool Selection::start_session ()
{
   Root & root = get_ancestor <Root> ();
   
   _tx_session_guard.prepare_record (root.use_scribe ());
   
   if (!_tx_session_guard.start ())
   {
      return false;
   }
   
   _obj_move_flag = false;
   _taxi_width_change_flag = false;
   _taxi_color_change_flag = false;
   _obj_copy_flag = false;

   return true;
}



/*
==============================================================================
Name: commit_session
==============================================================================
*/

void Selection::commit_session ()
{
   Root & root = get_ancestor <Root> ();
   
   // undo name
   
   std::string tx_name = "Change selection";
   
   if (_obj_move_flag && _obj_copy_flag)
   {
      tx_name = "Move and Copy Object";
   }
   else if (_obj_move_flag)
   {
      tx_name = "Move Object";
   }
   else if (_taxi_width_change_flag)
   {
      tx_name = "Change Taxi Width";
   }
   else if (_taxi_color_change_flag)
   {
      tx_name = "Change Taxi Color";
   }

   root.set_scribe_metadata (tx_name);
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name: revert_session
==============================================================================
*/

void Selection::revert_session ()
{
   _tx_session_guard.revert ();
}



/*
==============================================================================
Name: session_copy
==============================================================================
*/

void Selection::session_copy ()
{
   using namespace ohm;
   
   flip::TxSessionGuard::AutoWield auto_wield (_tx_session_guard);
   
   std::list <flip::Object *> new_object_cache;
   
   Root & root = get_ancestor <Root> ();
   
   ObjectRefColl::iterator it = _coll.begin ();
   const ObjectRefColl::iterator it_end = _coll.end ();
   
   typedef flip::ClassDescriptionMaster Cdm;
   
   static const Cdm * cdm_taxi_ptr = &flip::ClassDescription <Taxi>::use ();
   static const Cdm * cdm_pedestrian_ptr = &flip::ClassDescription <Pedestrian>::use ();

   for (; it != it_end ; ++it)
   {
      ObjectRef & object_ref = *it;
      
      flip::Object * object_ptr = object_ref.get_object_ptr ();
      
      if (object_ptr != 0)
      {
         const Cdm * cdm_ptr = &object_ptr->get_cdm ();
         
         if (cdm_ptr == cdm_taxi_ptr)
         {
            SpacialObjectColl::iterator it
               = root.ext_add_taxi (dynamic_cast <Taxi &> (*object_ptr));

            new_object_cache.push_back (&*it);
         }
         else if (cdm_ptr == cdm_pedestrian_ptr)
         {
            SpacialObjectColl::iterator it
               = root.ext_add_pedestrian (dynamic_cast <Pedestrian &> (*object_ptr));

            new_object_cache.push_back (&*it);
         }
      }
   }
   
   ext_replace (new_object_cache.begin (), new_object_cache.end ());

   _obj_copy_flag = true;
}



/*
==============================================================================
Name: session_taxi_move
==============================================================================
*/

void  Selection::session_taxi_move (ohm::archi::Float64 & position_x_delta)
{
   using namespace ohm;
   
   flip::TxSessionGuard::AutoWield auto_wield (_tx_session_guard);
   
   if (position_x_delta != 0.0)
   {
      _obj_move_flag = true;
      
      ObjectRefColl::iterator it;
      const ObjectRefColl::iterator it_end = _coll.end ();
      
      for (it = _coll.begin () ; it != it_end ; ++it)
      {
         ObjectRef & object_ref = *it;
         
         SpacialObject * spacial_ptr = object_ref.get_object_ptr <SpacialObject> ();
         
         if (spacial_ptr != 0)
         {
            archi::Float64 new_position_x = spacial_ptr->get_position_x ();
            new_position_x += position_x_delta;
            
            spacial_ptr->ext_set_position_x (new_position_x);
         }
      }
   }
}



/*
==============================================================================
Name: session_taxi_change_width
==============================================================================
*/

void  Selection::session_taxi_change_width (ohm::archi::Float64 & width_delta)
{
   using namespace ohm;
   
   flip::TxSessionGuard::AutoWield auto_wield (_tx_session_guard);
   
   RangeFloat64 width_delta_range = get_taxi_width_delta_range ();
   
   if (!width_delta_range.is_null ())
   {
      width_delta_range.limit_in_place (width_delta);
      
      if (width_delta != 0.0)
      {
         _taxi_width_change_flag = true;
         
         ObjectRefColl::iterator it;
         const ObjectRefColl::iterator it_end = _coll.end ();
         
         for (it = _coll.begin () ; it != it_end ; ++it)
         {
            ObjectRef & object_ref = *it;
            
            Taxi * taxi_ptr = object_ref.get_object_ptr <Taxi> ();
            
            if (taxi_ptr != 0)
            {
               archi::Float64 new_width = taxi_ptr->get_width ();
               new_width += width_delta;
               
               taxi_ptr->ext_set_width (new_width);
            }
         }
      }
   }
}



/*
==============================================================================
Name: session_taxi_set_color
==============================================================================
*/

void  Selection::session_taxi_set_color (Taxi::Color color)
{
   using namespace ohm;
   
   flip::TxSessionGuard::AutoWield auto_wield (_tx_session_guard);
   
   _taxi_color_change_flag = true;
   
   ObjectRefColl::iterator it;
   const ObjectRefColl::iterator it_end = _coll.end ();
   
   for (it = _coll.begin () ; it != it_end ; ++it)
   {
      ObjectRef & object_ref = *it;
      
      Taxi * taxi_ptr = object_ref.get_object_ptr <Taxi> ();
      
      if (taxi_ptr != 0)
      {
         taxi_ptr->ext_set_color (color);
      }
   }
}



/*
==============================================================================
Name: session_pedestrian_move
==============================================================================
*/

void  Selection::session_pedestrian_move (ohm::archi::Float64 & position_x_delta, ohm::archi::Float64 & position_y_delta)
{
   using namespace ohm;
   
   flip::TxSessionGuard::AutoWield auto_wield (_tx_session_guard);
   
   if ((position_x_delta != 0.0) || (position_y_delta != 0))
   {
      _obj_move_flag = true;
      
      ObjectRefColl::iterator it;
      const ObjectRefColl::iterator it_end = _coll.end ();
      
      for (it = _coll.begin () ; it != it_end ; ++it)
      {
         ObjectRef & object_ref = *it;
         
         Taxi * taxi_ptr = object_ref.get_object_ptr <Taxi> ();
         Pedestrian * pedestrian_ptr = object_ref.get_object_ptr <Pedestrian> ();
         
         if (taxi_ptr != 0)
         {
            archi::Float64 new_position_x = taxi_ptr->get_position_x ();
            new_position_x += position_x_delta;

            taxi_ptr->ext_set_position_x (new_position_x);
         }
         else if (pedestrian_ptr != 0)
         {
            archi::Float64 new_position_x = pedestrian_ptr->get_position_x ();
            new_position_x += position_x_delta;

            archi::Float64 new_position_y = pedestrian_ptr->get_position_y ();
            new_position_y += position_y_delta;
            
            pedestrian_ptr->ext_set_position (new_position_x, new_position_y);
         }
      }
   }
}



/*
==============================================================================
Name: signal_start
==============================================================================
*/

void Selection::signal_start ()
{
   send_signal (Signal_START);
}



/*
==============================================================================
Name: signal_end
==============================================================================
*/

void Selection::signal_end ()
{
   send_signal (Signal_END);
}



/*
==============================================================================
Name: signal_copy
==============================================================================
*/

void Selection::signal_copy (bool copy_flag)
{
   using namespace ohm;
   
   flip::DataSPtr data_sptr (new flip::Data);
   
   util::BinaryStreamOutput bso (*data_sptr);
   
   SelectionChangeInfo info;
   info.set (Change_COPY, copy_flag);
   info.write (bso);    
   
   send_signal (Signal_CHANGE, data_sptr);
}



/*
==============================================================================
Name: signal_taxi_move
==============================================================================
*/

void Selection::signal_taxi_move (ohm::archi::Float64 & position_x_delta)
{
   using namespace ohm;
   
   flip::DataSPtr data_sptr (new flip::Data);
   
   util::BinaryStreamOutput bso (*data_sptr);
   
   SelectionChangeInfo info;
   info.set (Change_OBJ_MOVE_X, position_x_delta);
   info.write (bso);    
   
   send_signal (Signal_CHANGE, data_sptr);
}



/*
==============================================================================
Name: signal_taxi_change_width
==============================================================================
*/

void Selection::signal_taxi_change_width (ohm::archi::Float64 & width_delta)
{
   using namespace ohm;
   
   flip::DataSPtr data_sptr (new flip::Data);
   
   util::BinaryStreamOutput bso (*data_sptr);
   
   SelectionChangeInfo info;
   info.set (Change_TAXI_WIDTH, width_delta);
   info.write (bso);    
   
   send_signal (Signal_CHANGE, data_sptr);
}



/*
==============================================================================
Name: session_taxi_set_color
==============================================================================
*/

void Selection::signal_taxi_set_color (Taxi::Color color)
{
   using namespace ohm;
   
   flip::DataSPtr data_sptr (new flip::Data);
   
   util::BinaryStreamOutput bso (*data_sptr);
   
   SelectionChangeInfo info;
   info.set (Change_TAXI_COLOR, color);
   info.write (bso);    
   
   send_signal (Signal_CHANGE, data_sptr);
}



/*
==============================================================================
Name: session_pedestrian_move
==============================================================================
*/

void Selection::signal_pedestrian_move (ohm::archi::Float64 & position_x_delta, ohm::archi::Float64 & position_y_delta)
{
   using namespace ohm;
   
   flip::DataSPtr data_sptr (new flip::Data);
   
   util::BinaryStreamOutput bso (*data_sptr);
   
   SelectionChangeInfo info;

   info.set (Change_OBJ_MOVE_X, position_x_delta);
   info.write (bso);    

   info.set (Change_OBJ_MOVE_Y, position_y_delta);
   info.write (bso);    
   
   send_signal (Signal_CHANGE, data_sptr);
}



/*
==============================================================================
Name: get_taxi_width_delta_range
==============================================================================
*/

RangeFloat64 Selection::get_taxi_width_delta_range () const
{
   RangeFloat64 width_delta_range;
   width_delta_range._min = -1e+34f;
   width_delta_range._max = 2e+34f;
   
   ObjectRefColl::const_iterator it;
   const ObjectRefColl::const_iterator it_end = _coll.end ();
   
   for (it = _coll.begin () ; it != it_end ; ++it)
   {
      ObjectRef & object_ref = *it;
      
      Taxi * taxi_ptr = object_ref.get_object_ptr <Taxi> ();
      
      if (taxi_ptr != 0)
      {
         taxi_ptr->intersect_width_delta_range (width_delta_range);
      }
   }
   
   return width_delta_range;
}



/*
==============================================================================
Name: get_taxi_color
==============================================================================
*/

Taxi::Color Selection::get_taxi_color () const
{
   Taxi::Color ret_val = Taxi::Color_UNDEFINED;
   
   ObjectRefColl::const_iterator it = _coll.begin ();
   const ObjectRefColl::const_iterator it_end = _coll.end ();
   
   for (; it != it_end ; ++it)
   {
      Taxi * taxi_ptr = it->get_object_ptr <Taxi> ();
      
      if (taxi_ptr != 0)
      {
         if (ret_val == Taxi::Color_UNDEFINED)
         {
            ret_val = taxi_ptr->get_color ();
         }
         else
         {
            Taxi::Color sub_color = taxi_ptr->get_color ();
            
            if (ret_val != sub_color)
            {
               ret_val = Taxi::Color_UNDEFINED;
               
               break;
            }
         }
      }
   }
   
   return ret_val;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : Predicate::ctor
==============================================================================
*/

Selection::Predicate::Predicate (const ohm::flip::Object & object)
:  _object_ptr (&object)
{
}



/*
==============================================================================
Name : Predicate::equal
==============================================================================
*/

bool  Selection::Predicate::equal (const ObjectRef & object_ref) const
{
   return (object_ref.get_object_ptr () == _object_ptr);
}



}  // namespace model



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

