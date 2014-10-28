/*****************************************************************************

        ClientData.cpp
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

#include "model/ClientData.h"

#include "model/Root.h"

#include "ohm/flip/ClassDescription.h"
#include "ohm/flip/EnumDescription.h"
#include "ohm/flip/PasteBoard.h"

#include "ohm/util/BinaryStreamInput.h"

#include "ohm/util/trace.h"

#include <cassert>



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : declare
Note :
   Rather than putting a _display_name into the client, we will fetch it
   when we would need it, as it might actually change
   User name is guaranteed to be unique and non changing, it is the one
   used for authentification
==============================================================================
*/

void  ClientData::declare ()
{
   using namespace ohm::flip;
   
   ClassDescription <ClientData>::use ().set_name ("skeleton.ClientData");
   ClassDescription <ClientData>::use ().push_var_desc (&ClientData::_user_name, "_user_name");
   ClassDescription <ClientData>::use ().push_var_desc (&ClientData::_primary_tool_cue, "_primary_tool_cue");
   ClassDescription <ClientData>::use ().push_var_desc (&ClientData::_momentary_tool_cue, "_momentary_tool_cue");
   ClassDescription <ClientData>::use ().push_var_desc (&ClientData::_tool_state_cue, "_tool_state_cue");
   ClassDescription <ClientData>::use ().push_var_desc (&ClientData::_selection, "_selection");
   ClassDescription <ClientData>::use ().push_var_desc (&ClientData::_view_info, "_view_info");
   
   ClassDescManager::declare (ClassDescription <ClientData>::use ());
}



/*
==============================================================================
Name: use_self
==============================================================================
*/

ClientData &   ClientData::use_self (ohm::flip::Object & obj)
{
   Root & root = obj.get_ancestor <model::Root> ();
   ClientData * client_data_ptr = root.use_self_client_data_ptr ();
   
   if (client_data_ptr == 0)
   {
      ohm_util_TRACE_ERROR ("ClientData::use_self () : bad cast");
      throw std::bad_cast ();
   }
   
   return *client_data_ptr;
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

ClientData::ClientData (ohm::flip::DocumentBase & document)
:  ohm::flip::Object (document)
,  _user_name (document)
,  _primary_tool_cue (document)
,  _primary_tool (Tool_ARROW)
,  _old_primary_tool (Tool_ARROW)
,  _momentary_tool_cue (document)
,  _momentary_tool (Tool_ARROW)
,  _old_momentary_tool (Tool_ARROW)
,  _tool_state_cue (document)
,  _tool_state (ToolState_PRIMARY)
,  _old_tool_state (ToolState_PRIMARY)
,  _selection (document)
,  _view_info (document)

,  _tx_session_guard (document)
{
   document.require_sync_operations (*this);

   _user_name.push_encoder_escaped_ascii ();
   _user_name.set_inline ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ClientData::~ClientData ()
{
}



/*
==============================================================================
Name : operator <
==============================================================================
*/

bool  ClientData::operator < (const ClientData & other)
{
   return get_ref () < other.get_ref ();
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

void  ClientData::ctor (ohm::flip::Args & args)
{
   args.pop (_user_name);
}



/*
==============================================================================
Name: pre_sync
   Called before observer are notified.
==============================================================================
*/

void ClientData::pre_sync ()
{
   update ();
}



/*
==============================================================================
Name: post_sync
==============================================================================
*/

void ClientData::post_sync ()
{
   _old_primary_tool = _primary_tool;
   _old_momentary_tool = _momentary_tool;
   _old_tool_state = _tool_state;
}



/*
==============================================================================
Name : get_user_name
==============================================================================
*/

std::string ClientData::get_user_name () const
{
   return _user_name.to_string ();
}



/*
==============================================================================
Name : tx_set_primary_tool
==============================================================================
*/

void  ClientData::tx_set_primary_tool (Tool tool)
{
   if (!_tx_session_guard.start ())
   {
      return;
   }

   ext_set_primary_tool (tool);
   ext_set_tool_state (ToolState_PRIMARY);
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name : tx_set_momentary_tool
==============================================================================
*/

void  ClientData::tx_set_momentary_tool (Tool tool)
{
   if (!_tx_session_guard.start ())
   {
      return;
   }

   ext_set_momentary_tool (tool);
   ext_set_tool_state (ToolState_MOMENTARY);
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name : join_set_momentary_tool
==============================================================================
*/

void  ClientData::join_set_momentary_tool (Tool tool)
{
   if (_tx_session_guard.can_join ())
   {
      ext_set_momentary_tool (tool);
      ext_set_tool_state (ToolState_MOMENTARY);
   }
   else
   {
      tx_set_momentary_tool (tool);
   }
}



/*
==============================================================================
Name : tx_set_primary_tool
==============================================================================
*/

void  ClientData::tx_set_primary_tool ()
{
   if (!_tx_session_guard.start ())
   {
      return;
   }

   ext_set_primary_tool ();
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name : join_set_primary_tool
==============================================================================
*/

void  ClientData::join_set_primary_tool ()
{
   if (_tx_session_guard.can_join ())
   {
      ext_set_primary_tool ();
   }
   else
   {
      tx_set_primary_tool ();
   }
}



/*
==============================================================================
Name : ext_set_primary_tool
==============================================================================
*/

void  ClientData::ext_set_primary_tool ()
{
   ext_set_tool_state (ToolState_PRIMARY);
}



/*
==============================================================================
Name : tool_changed
==============================================================================
*/

bool  ClientData::tool_changed () const
{
   update ();
   
   return
         (_primary_tool != _old_primary_tool)
      || (_momentary_tool != _old_momentary_tool)
      || (_tool_state != _old_tool_state)
   ;
}



/*
==============================================================================
Name : get_tool
==============================================================================
*/

Tool  ClientData::get_tool ()
{
   update ();
   
   Tool tool = Tool_ARROW;
   
   if (_tool_state == ToolState_PRIMARY)
   {
      tool = _primary_tool;
   }
   else if (_tool_state == ToolState_MOMENTARY)
   {
      tool = _momentary_tool;
   }
   
   return tool;
}



/*
==============================================================================
Name: use_selection
==============================================================================
*/

Selection & ClientData::use_selection ()
{
   return _selection;
}



/*
==============================================================================
Name: use_selection
==============================================================================
*/

const Selection & ClientData::use_selection () const
{
   return _selection;
}



/*
==============================================================================
Name: use_view_info
==============================================================================
*/

ViewInfo &  ClientData::use_view_info ()
{
   return _view_info;
}



/*
==============================================================================
Name: use_view_info
==============================================================================
*/

const ViewInfo &  ClientData::use_view_info () const
{
   return _view_info;
}



/*
==============================================================================
Name : tx_delete_selection
==============================================================================
*/

void  ClientData::tx_delete_selection ()
{
   use_selection ().tx_delete_content ();
}



/*
==============================================================================
Name : copy_selection
==============================================================================
*/

void  ClientData::copy_selection ()
{
   using namespace ohm;
   
   flip::PasteBoard::use_instance ().clear ();
   
   use_selection ().copy_content ();
}



/*
==============================================================================
Name : tx_paste
==============================================================================
*/

void  ClientData::tx_paste ()
{
   using namespace ohm;
   
   if (!flip::PasteBoard::use_instance ().empty ())
   {
      flip::PasteBoard::iterator it = flip::PasteBoard::use_instance ().begin ();
      
      flip::ObjectMold & mold = **it;
      
      if (mold.is_of_type <Taxi> () || mold.is_of_type <Pedestrian> ())
      {
         tx_paste_spacial_object ();
      }
      else
      {
         assert (false);
      }
   }
}



/*
==============================================================================
Name : undo
==============================================================================
*/

void  ClientData::undo (std::string & name)
{
#if ! defined (ohm_flip_SERVER)
   using namespace ohm;
   
   use_selection ().tx_clear ();
   
   Root & root = get_ancestor <Root> ();
   
   int err = root.undo (name);
   
   if (err != flip::Err_OK)
   {
      name = "Cannot Undo";
   }
#endif
}



/*
==============================================================================
Name : redo
==============================================================================
*/

void  ClientData::redo (std::string & name)
{
#if ! defined (ohm_flip_SERVER)
   using namespace ohm;
   
   use_selection ().tx_clear ();
   
   Root & root = get_ancestor <Root> ();
   
   int err = root.redo (name);
   
   if (err != flip::Err_OK)
   {
      name = "Cannot Redo";
   }
#endif
}



/*
==============================================================================
Name : tx_select_all
==============================================================================
*/

void  ClientData::tx_select_all ()
{
   std::list <model::SpacialObject *> object_ptr_list;
   
   Root & root = get_ancestor <Root> ();
   SpacialObjectColl & spacial_object_coll = root.use_spacial_object_coll ();
   
   SpacialObjectColl::iterator it = spacial_object_coll.begin ();
   const SpacialObjectColl::iterator it_end = spacial_object_coll.end ();
   
   for (; it != it_end ; ++it)
   {
      model::SpacialObject & object = *it;
      
      object_ptr_list.push_back (&object);
   }
   
   use_selection ().tx_replace (object_ptr_list.begin (), object_ptr_list.end ());
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ext_set_primary_tool
==============================================================================
*/

void ClientData::ext_set_primary_tool (Tool tool)
{
   using namespace ohm;
   
   archi::UByte val = tool;
   
   _primary_tool_cue.fire (&val, sizeof (val));
   _primary_tool = tool;
}



/*
==============================================================================
Name: ext_set_momentary_tool
==============================================================================
*/

void ClientData::ext_set_momentary_tool (Tool tool)
{
   using namespace ohm;
   
   archi::UByte val = tool;
   
   _momentary_tool_cue.fire (&val, sizeof (val));
   _momentary_tool = tool;
}



/*
==============================================================================
Name: ext_set_tool_state
==============================================================================
*/

void ClientData::ext_set_tool_state (ToolState state)
{
   using namespace ohm;
   
   archi::UByte val = state;
   
   _tool_state_cue.fire (&val, sizeof (val));
   _tool_state = state;
}



/*
==============================================================================
Name : tx_paste_spacial_object
==============================================================================
*/

void  ClientData::tx_paste_spacial_object ()
{
   Root & root = get_ancestor <Root> ();
   
   _tx_session_guard.prepare_record (root.use_scribe ());
   
   if (!_tx_session_guard.start ())
   {
      return;
   }
   
   size_t taxi_nbr = 0;
   size_t pedestrian_nbr = 0;
   
   ext_paste_spacial_object (taxi_nbr, pedestrian_nbr);
   
   std::string name;
   
   if ((taxi_nbr > 0) && (pedestrian_nbr > 0))
   {
      name = "Paste Objects";
   }
   else if (taxi_nbr > 1)
   {
      name = "Paste Taxis";
   }
   else if (taxi_nbr == 1)
   {
      name = "Paste Taxi";
   }
   else if (pedestrian_nbr > 1)
   {
      name = "Paste Pedestrians";
   }
   else if (pedestrian_nbr == 1)
   {
      name = "Paste Pedestrian";
   }

   root.set_scribe_metadata (name);

   _tx_session_guard.commit ();
}



/*
==============================================================================
Name : ext_paste_spacial_object
==============================================================================
*/

void  ClientData::ext_paste_spacial_object (size_t & taxi_nbr, size_t & pedestrian_nbr)
{
   using namespace ohm;
   
   Root & root = get_ancestor <Root> ();
   SpacialObjectColl & spacial_object_coll = root.use_spacial_object_coll ();
   
   std::list <SpacialObject *> object_ptr_list;
   
   flip::PasteBoard & paste_board = flip::PasteBoard::use_instance ();
   
   flip::PasteBoard::iterator it = paste_board.begin ();
   const flip::PasteBoard::iterator it_end = paste_board.end ();

   for (; it != it_end ; ++it)
   {
      flip::ObjectMold & mold = **it;
      
      assert (mold.is_of_type <SpacialObject> ());
      
      if (mold.is_of_type <Taxi> ())
      {
         ++taxi_nbr;
      }
      else if (mold.is_of_type <Pedestrian> ())
      {
         ++pedestrian_nbr;
      }
      
      {
         SpacialObjectColl::iterator it = spacial_object_coll.paste (mold);
         
         object_ptr_list.push_back (&*it);
      }
   }
   
   use_selection ().ext_replace (
      object_ptr_list.begin (),
      object_ptr_list.end ()
   );
}



/*
==============================================================================
Name: update
==============================================================================
*/

void ClientData::update () const
{
   using namespace ohm;
   
   if (_primary_tool_cue.was_fired ())
   {
      for (size_t i = 0 ; i < _primary_tool_cue.get_elem_cnt () ; ++i)
      {
         const archi::UByte * data_ptr = 0;
         size_t data_size = 0;
         int dir = 0;
         
         _primary_tool_cue.get_elem (data_ptr, data_size, dir, i);
         assert (data_size == 1);
         assert (data_ptr != 0);
         
         util::BinaryStreamInput bsi (data_ptr, data_size);
         
         archi::UByte primary_tool_val = data_ptr [0];
         assert (primary_tool_val < Tool_NBR_ELT);
         
         _primary_tool = static_cast <Tool> (primary_tool_val);
      }
   }
   
   if (_momentary_tool_cue.was_fired ())
   {
      for (size_t i = 0 ; i < _momentary_tool_cue.get_elem_cnt () ; ++i)
      {
         const archi::UByte * data_ptr = 0;
         size_t data_size = 0;
         int dir = 0;
         
         _momentary_tool_cue.get_elem (data_ptr, data_size, dir, i);
         assert (data_size == 1);
         assert (data_ptr != 0);
         
         util::BinaryStreamInput bsi (data_ptr, data_size);
         
         archi::UByte momentary_tool_val = data_ptr [0];
         assert (momentary_tool_val < Tool_NBR_ELT);
         
         _momentary_tool = static_cast <Tool> (momentary_tool_val);
      }
   }
   
   if (_tool_state_cue.was_fired ())
   {
      for (size_t i = 0 ; i < _tool_state_cue.get_elem_cnt () ; ++i)
      {
         const archi::UByte * data_ptr = 0;
         size_t data_size = 0;
         int dir = 0;
         
         _tool_state_cue.get_elem (data_ptr, data_size, dir, i);
         assert (data_size == 1);
         assert (data_ptr != 0);
         
         util::BinaryStreamInput bsi (data_ptr, data_size);
         
         archi::UByte tool_state_val = data_ptr [0];
         assert (tool_state_val < NBR_ToolState_TOTAL);
         
         _tool_state = static_cast <ToolState> (tool_state_val);
      }
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
