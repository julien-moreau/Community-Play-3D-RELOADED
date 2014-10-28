/*****************************************************************************

        BundleGui.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71440

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
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/BundleGui.h"

#include "client/BundleTaxi.h"
#include "client/BundleTaxiGhost.h"
#include "client/BundlePedestrian.h"
#include "client/BundlePedestrianGhost.h"

#include "model/SelectionChangeInfo.h"

#include "ohm/opa/BundleView.h"
#include "ohm/opa/EventBundleView.h"
#include "ohm/util/BinaryStreamInput.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

BundleGui::BundleGui (ohm::opa::BundleView & parent)
:  ohm::opa::Bundle ("BundleGui")
,  _parent (parent)
,  _bnd_root_ptr (0)
{
   using namespace ohm;
   
   _frame_base_aptr = new opal::Frame ("BundleGui::_frame_base_aptr");
   set_embeddable (*_frame_base_aptr);
   
   _layer_base_aptr = new opak::Layer;
   set_embeddable (*_layer_base_aptr);
   
   parent.embed (*this, opa::BundleView::Slot_WORKSPACE);
   
   _frame_content_aptr = new opal::Frame ("BundleGui::_frame_content_aptr");
   _frame_content_aptr->set_no_clip ();
   _frame_base_aptr->insert_top (*_frame_content_aptr);
   
   _layer_content_aptr = new ohm::opak::Layer;
   _layer_base_aptr->push_back (*_layer_content_aptr);
   
   set_nbr_slot (Slot_NBR_ELT);
   set_slot (Slot_BASE, *_frame_base_aptr);
   set_slot (Slot_BASE, *_layer_base_aptr);
   set_slot (Slot_CONTENT, *_frame_content_aptr);
   set_slot (Slot_CONTENT, *_layer_content_aptr);
   
   bind <
      BundleGui, 
      opa::EventBundleView, 
      &BundleGui::handle_event
   > ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BundleGui::~BundleGui ()
{
}



/*
==============================================================================
Name: update
==============================================================================
*/

void  BundleGui::update ()
{
   using namespace ohm;
   
   const opa::Size & bounds = _parent.get_bounds ();
   
   set (opa::Rect (opa::Point::_zero, bounds), 0.0, opak::step);
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  BundleGui::set (const ohm::opa::Rect & rect, double tt, ohm::opak::TransitionFunction tf)
{
   using namespace ohm;
   
   _frame_base_aptr->set_origin (rect._origin);
   _frame_base_aptr->set_bounds (rect._size);
   
   _layer_base_aptr->_origin.stop ().push (rect._origin, tt, tf);
}


/*
==============================================================================
Name : document_changed
==============================================================================
*/

void  BundleGui::document_changed (model::Root & root)
{
   assert (&root != 0);
   
   if (root.was_inited ())
   {
      _bnd_root_ptr = new BundleRoot (root, *this);
      
      root.reset_data (_bnd_root_ptr);
   }
   
   model::ClientDataColl & client_data_coll = root.use_client_data_coll ();
   
   {
      model::ClientDataColl::iterator it = client_data_coll.begin ();
      const model::ClientDataColl::iterator it_end = client_data_coll.end ();
      
      for (; it != it_end ; ++it)
      {
         model::ClientData & client_data = *it;
         
         if (client_data.is_invalid ())
         {
            document_changed_selection (client_data);
         }
      }
   }
   
   {
      model::ClientData * client_data_ptr = root.use_self_client_data_ptr ();
      
      if (client_data_ptr != 0)
      {
         if (client_data_ptr->is_invalid ())
         {
            document_changed_view_info (*client_data_ptr);
         }
      }
   }
   
   model::SpacialObjectColl & spacial_object_coll = root.use_spacial_object_coll ();
   
   if (spacial_object_coll.is_invalid ())
   {
      model::SpacialObjectColl::iterator it = spacial_object_coll.begin ();
      const model::SpacialObjectColl::iterator it_end = spacial_object_coll.end ();
      
      for (; it != it_end ; ++it)
      {
         model::SpacialObject & spacial_object = *it;
         
         if (spacial_object.is_invalid ())
         {
            document_changed (spacial_object);
         }
      }
   }
   
   if (root.was_restored ())
   {
      BundleRoot * bnd_ptr = root.get_data <BundleRoot> ();
      assert (bnd_ptr != 0);
      
      delete bnd_ptr;
      bnd_ptr = 0;
      _bnd_root_ptr = 0;
      
      root.reset_data <BundleRoot> ();
   }
}



/*
==============================================================================
Name: signal
==============================================================================
*/

void BundleGui::signal (ohm::flip::Object * obj_ptr, ohm::archi::UInt32 type, const ohm::flip::DataMaster & signal_data)
{
   bool continue_flag = true;
   
   if (continue_flag)
   {
      model::Selection * cast_ptr = dynamic_cast <model::Selection *> (obj_ptr);
      
      if (cast_ptr != 0)
      {
         signal (*cast_ptr, type, signal_data);
         
         continue_flag = false;
      }
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : document_changed_selection
==============================================================================
*/

void  BundleGui::document_changed_selection (model::ClientData & client_data)
{
   using namespace ohm;
   
   typedef flip::ClassDescriptionMaster Cdm;
   
   model::Selection & selection = client_data.use_selection ();
   
   if (!selection.is_invalid ())
   {
      return;
   }
   
   static const Cdm * cdm_taxi_ptr = &flip::ClassDescription <model::Taxi>::use ();
   static const Cdm * cdm_pedestrian_ptr = &flip::ClassDescription <model::Pedestrian>::use ();
   
   const archi::Int32 user_id = client_data.get_user_id ();
   
   model::ObjectRefColl::iterator it = selection.use ().begin ();
   const model::ObjectRefColl::iterator it_end = selection.use ().end ();
   
   for (; it != it_end ; ++it)
   {
      model::ObjectRef & object_ref = *it;
      
      if (object_ref.was_restored () || object_ref.was_inited ())
      {
         bool select_flag = object_ref.was_inited ();
         
         flip::Object * object_ptr = object_ref.get_object_ptr ();
         
         if (object_ptr != 0)
         {
            const Cdm * cdm_ptr = &object_ptr->get_cdm ();
            
            if (cdm_ptr == cdm_taxi_ptr)
            {
               BundleTaxi * bnd_ptr = object_ptr->get_data <BundleTaxi> ();
               
               if (bnd_ptr != 0)
               {
                  bnd_ptr->update_selection_state (select_flag, user_id);
               }
            }
            else if (cdm_ptr == cdm_pedestrian_ptr)
            {
               BundlePedestrian * bnd_ptr = object_ptr->get_data <BundlePedestrian> ();
               
               if (bnd_ptr != 0)
               {
                  bnd_ptr->update_selection_state (select_flag, user_id);
               }
            }
         }
      }
   }
}



/*
==============================================================================
Name : document_changed_view_info
==============================================================================
*/

void  BundleGui::document_changed_view_info (model::ClientData & client_data)
{
   using namespace ohm;
   
   model::ViewInfo & view_info = client_data.use_view_info ();
   
   if (!view_info.is_invalid ())
   {
      return;
   }
   
   if (view_info.focus_changed ())
   {
      archi::Float64 focus_x = view_info.get_focus_x ();
      archi::Float64 focus_y = view_info.get_focus_y ();
      
      opa::Point origin (focus_x, focus_y);
      
      _frame_content_aptr->set_origin (origin);
      _layer_content_aptr->_origin = origin;
   }
}



/*
==============================================================================
Name : document_changed
==============================================================================
*/

void  BundleGui::document_changed (model::SpacialObject & spacial_object)
{
   model::Taxi * taxi_ptr = dynamic_cast <model::Taxi *> (&spacial_object);
   model::Pedestrian * pedestrian_ptr = dynamic_cast <model::Pedestrian *> (&spacial_object);
   
   if (taxi_ptr != 0)
   {
      document_changed (*taxi_ptr);
   }
   else if (pedestrian_ptr != 0)
   {
      document_changed (*pedestrian_ptr);
   }
}



/*
==============================================================================
Name : document_changed
==============================================================================
*/

void  BundleGui::document_changed (model::Taxi & taxi)
{
   BundleTaxi * bnd_ptr = 0;
   
   if (taxi.was_inited ())
   {
      model::Root & root = taxi.get_ancestor <model::Root> ();
      
      BundleRoot * bnd_parent_ptr = root.get_data <BundleRoot> ();
      assert (bnd_parent_ptr != 0);
      
      bnd_ptr = new BundleTaxi (taxi, *bnd_parent_ptr);
      
      taxi.reset_data (bnd_ptr);
   }
   else if (!taxi.was_restored ())
   {
      bnd_ptr = taxi.get_data <BundleTaxi> ();
      assert (bnd_ptr != 0);
   }
   
   if (bnd_ptr != 0)
   {
      if (taxi.position_changed ())
      {
         bnd_ptr->update_position ();
      }
      
      if (taxi.width_changed ())
      {
         bnd_ptr->update_width ();
      }
      
      if (taxi.color_changed ())
      {
         bnd_ptr->update_color ();
      }
   }
   
   if (taxi.was_restored ())
   {
      bnd_ptr = taxi.get_data <BundleTaxi> ();
      assert (bnd_ptr != 0);
      
      delete bnd_ptr;
      bnd_ptr = 0;
      
      taxi.reset_data <BundleTaxi> ();
   }
}



/*
==============================================================================
Name : document_changed
==============================================================================
*/

void  BundleGui::document_changed (model::Pedestrian & pedestrian)
{
   BundlePedestrian * bnd_ptr = 0;
   
   if (pedestrian.was_inited ())
   {
      model::Root & root = pedestrian.get_ancestor <model::Root> ();
      
      BundleRoot * bnd_parent_ptr = root.get_data <BundleRoot> ();
      assert (bnd_parent_ptr != 0);
      
      bnd_ptr = new BundlePedestrian (pedestrian, *bnd_parent_ptr);
      
      pedestrian.reset_data (bnd_ptr);
   }
   else if (!pedestrian.was_restored ())
   {
      bnd_ptr = pedestrian.get_data <BundlePedestrian> ();
      assert (bnd_ptr != 0);
   }
   
   if (bnd_ptr != 0)
   {
      if (pedestrian.position_changed ())
      {
         bnd_ptr->update_position ();
      }
   }
   
   if (pedestrian.was_restored ())
   {
      bnd_ptr = pedestrian.get_data <BundlePedestrian> ();
      assert (bnd_ptr != 0);
      
      delete bnd_ptr;
      bnd_ptr = 0;
      
      pedestrian.reset_data <BundlePedestrian> ();
   }
}



/*
==============================================================================
Name : signal
==============================================================================
*/

void  BundleGui::signal (model::Selection & selection, ohm::archi::UInt32 type, const ohm::flip::DataMaster & signal_data)
{
   switch (type)
   {
   case model::Selection::Signal_START:
      signal_start (selection, signal_data);
      break;

   case model::Selection::Signal_END:
      signal_end (selection, signal_data);
      break;

   case model::Selection::Signal_CHANGE:
      signal_change (selection, signal_data);
      break;
   }
}



/*
==============================================================================
Name : signal_start
==============================================================================
*/

void  BundleGui::signal_start (model::Selection & selection, const ohm::flip::DataMaster & signal_data)
{
   if (_bnd_root_ptr == 0) return;  // abort
   
   model::ObjectRefColl::iterator it = selection.use ().begin ();
   const model::ObjectRefColl::iterator it_end = selection.use ().end ();
   
   for (; it != it_end ; ++it)
   {
      model::ObjectRef & object_ref = *it;
      
      model::Taxi * taxi_ptr = object_ref.get_object_ptr <model::Taxi> ();
      
      if (taxi_ptr != 0)
      {
         BundleTaxiGhost * ghost_ptr = new BundleTaxiGhost (*taxi_ptr, *_bnd_root_ptr);
         
         taxi_ptr->reset_data (ghost_ptr);
      }

      model::Pedestrian * pedestrian_ptr = object_ref.get_object_ptr <model::Pedestrian> ();
      
      if (pedestrian_ptr != 0)
      {
         BundlePedestrianGhost * ghost_ptr = new BundlePedestrianGhost (*pedestrian_ptr, *_bnd_root_ptr);
         
         pedestrian_ptr->reset_data (ghost_ptr);
      }
   }
}



/*
==============================================================================
Name : signal_end
==============================================================================
*/

void  BundleGui::signal_end (model::Selection & selection, const ohm::flip::DataMaster & signal_data)
{
   model::Root & root = selection.get_ancestor <model::Root> ();
   model::SpacialObjectColl & spacial_object_coll = root.use_spacial_object_coll ();
   
   model::SpacialObjectColl::iterator it = spacial_object_coll.begin ();
   const model::SpacialObjectColl::iterator it_end = spacial_object_coll.end ();
   
   for (; it != it_end ; ++it)
   {
      model::SpacialObject & object = *it;
      
      model::Taxi * taxi_ptr = dynamic_cast <model::Taxi *> (&object);
      
      if (taxi_ptr != 0)
      {
         BundleTaxiGhost * ghost_ptr = taxi_ptr->get_data <BundleTaxiGhost> ();
         
         delete ghost_ptr;
         ghost_ptr = 0;
         
         taxi_ptr->reset_data <BundleTaxiGhost> ();
      }

      model::Pedestrian * pedestrian_ptr = dynamic_cast <model::Pedestrian *> (&object);
      
      if (pedestrian_ptr != 0)
      {
         BundlePedestrianGhost * ghost_ptr = pedestrian_ptr->get_data <BundlePedestrianGhost> ();
         
         delete ghost_ptr;
         ghost_ptr = 0;
         
         pedestrian_ptr->reset_data <BundlePedestrianGhost> ();
      }
   }
}



/*
==============================================================================
Name : signal_change
==============================================================================
*/

void  BundleGui::signal_change (model::Selection & selection, const ohm::flip::DataMaster & signal_data)
{
   using namespace ohm;
   
   util::BinaryStreamInput bsi (signal_data.data (), signal_data.size ());
   
   while (!bsi.empty ())
   {
      model::SelectionChangeInfo info;
      info.read (bsi);
      
      signal_change (selection, info);
   }
}



/*
==============================================================================
Name : signal_change
==============================================================================
*/

void  BundleGui::signal_change (model::Selection & selection, const model::SelectionChangeInfo & info)
{
   model::ObjectRefColl::iterator it = selection.use ().begin ();
   const model::ObjectRefColl::iterator it_end = selection.use ().end ();
   
   for (; it != it_end ; ++it)
   {
      model::ObjectRef & object_ref = *it;
      
      model::Taxi * taxi_ptr = object_ref.get_object_ptr <model::Taxi> ();
      
      if (taxi_ptr != 0)
      {
         BundleTaxiGhost * ghost_ptr = taxi_ptr->get_data <BundleTaxiGhost> ();
         
         if (ghost_ptr != 0)
         {
            ghost_ptr->update (info);
         }
      }

      model::Pedestrian * pedestrian_ptr = object_ref.get_object_ptr <model::Pedestrian> ();
      
      if (pedestrian_ptr != 0)
      {
         BundlePedestrianGhost * ghost_ptr = pedestrian_ptr->get_data <BundlePedestrianGhost> ();
         
         if (ghost_ptr != 0)
         {
            ghost_ptr->update (info);
         }
      }
   }
}



/*
==============================================================================
Name: handle_event
==============================================================================
*/

void  BundleGui::handle_event (ohm::opa::EventBundleView & event)
{
   using namespace ohm;
   
   assert (event._type == opa::EventBundleView::Type_LAYOUT_CHANGED);

   update ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
