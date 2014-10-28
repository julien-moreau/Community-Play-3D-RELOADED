/*****************************************************************************

        BundlePedestrian.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71391

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

#include "client/BundlePedestrian.h"

#include "client/Constants.h"
#include "client/SkeletonGui.h"

#include "model/Pedestrian.h"

#include "ohm/opa/BundleView.h"
#include "ohm/opa/EventBundleView.h"
#include "ohm/opal/Frame.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

BundlePedestrian::BundlePedestrian (model::Pedestrian & pedestrian, ohm::opa::Bundle & parent)
:  ohm::opa::Bundle (parent, "BundlePedestrian")
,  _pedestrian (pedestrian)
,  _parent (parent)
,  _sel_state_mgr (pedestrian)
{
   using namespace ohm;
   
   _frame_aptr = new FramePedestrian (*this);
   parent.get_slot_frame ().insert_top (*_frame_aptr);
   
   _frame_aptr->set_bounds (opa::Size (
      Constants::_metric_pedestrian_width, Constants::_metric_pedestrian_height
   ));
   
   _prim_aptr = new PrimPedestrian;
   parent.get_slot_layer ().push_back (*_prim_aptr);
   
   // post
   
   _sel_state_mgr.apply (*_prim_aptr);
   update_position ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BundlePedestrian::~BundlePedestrian ()
{
   try
   {
      _sel_state_mgr.stop (*_prim_aptr);
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : use_pedestrian
==============================================================================
*/

model::Pedestrian &  BundlePedestrian::use_pedestrian ()
{
   return _pedestrian;
}



/*
==============================================================================
Name : update_selection_state
==============================================================================
*/

void  BundlePedestrian::update_selection_state (bool selected_flag, ohm::archi::Int32 user_id)
{
   assert (user_id >= 0);
   
   _sel_state_mgr.update (selected_flag, user_id);
   _sel_state_mgr.apply (*_prim_aptr);
}



/*
==============================================================================
Name : update_conflict
==============================================================================
*/

void  BundlePedestrian::update_conflict ()
{
   _sel_state_mgr.conflict (*_prim_aptr);
}



/*
==============================================================================
Name : update_position
==============================================================================
*/

void  BundlePedestrian::update_position ()
{
   using namespace ohm;
   
   double tt;
   opak::TransitionFunction tf;
   
   SkeletonGui::get_transition (tt, tf, _pedestrian);
   
   const opa::Point position (_pedestrian.get_position_x (), _pedestrian.get_position_y ());
   
   _frame_aptr->set_origin (position);
   _prim_aptr->_origin.stop ().push (position, tt, tf);
}



/*
==============================================================================
Name : req_start
==============================================================================
*/

bool  BundlePedestrian::req_start ()
{
   model::ClientData & client_data = model::ClientData::use_self (_pedestrian);
   model::Selection & selection = client_data.use_selection ();
   
   bool ok_flag = selection.start_session ();
   
   if (ok_flag)
   {
      selection.signal_start ();
      
      _position_x = _pedestrian.get_position_x ();
      _position_y = _pedestrian.get_position_y ();
   }
   
   return ok_flag;
}



/*
==============================================================================
Name : req_commit
==============================================================================
*/

void  BundlePedestrian::req_commit (bool copy_flag)
{
   using namespace ohm;
   
   model::ClientData & client_data = model::ClientData::use_self (_pedestrian);
   model::Selection & selection = client_data.use_selection ();
   
   archi::Float64 position_x_delta = _position_x - _pedestrian.get_position_x ();
   archi::Float64 position_y_delta = _position_y - _pedestrian.get_position_y ();
   
   if (copy_flag)
   {
      selection.session_copy ();
   }
   
   if ((position_x_delta != 0) || (position_y_delta != 0))
   {
      selection.session_pedestrian_move (position_x_delta, position_y_delta);
   }

   selection.commit_session ();
   selection.signal_end ();
}



/*
==============================================================================
Name : req_revert
==============================================================================
*/

void  BundlePedestrian::req_revert ()
{
   model::ClientData & client_data = model::ClientData::use_self (_pedestrian);
   model::Selection & selection = client_data.use_selection ();
   
   selection.revert_session ();
   selection.signal_end ();
}



/*
==============================================================================
Name : req_change_copy_mode
==============================================================================
*/

void  BundlePedestrian::req_change_copy_mode (bool copy_flag)
{
   model::ClientData & client_data = model::ClientData::use_self (_pedestrian);
   model::Selection & selection = client_data.use_selection ();
   
   selection.signal_copy (copy_flag);
}



/*
==============================================================================
Name : req_change_position
==============================================================================
*/

void  BundlePedestrian::req_change_position (const ohm::opa::Point & position)
{
   using namespace ohm;
   
   model::ClientData & client_data = model::ClientData::use_self (_pedestrian);
   model::Selection & selection = client_data.use_selection ();
   
   archi::Float64 position_x_delta = position._x - _pedestrian.get_position_x ();
   archi::Float64 position_y_delta = position._y - _pedestrian.get_position_y ();
   
   selection.signal_pedestrian_move (position_x_delta, position_y_delta);
   
   _position_x = _pedestrian.get_position_x () + position_x_delta;
   _position_y = _pedestrian.get_position_y () + position_y_delta;
}



/*
==============================================================================
Name : req_highlight
==============================================================================
*/

void  BundlePedestrian::req_highlight (bool highlight_flag)
{
   using namespace ohm;
   
   _prim_aptr->_highlight_flag
      .stop ()
      .push (highlight_flag ? 1.f : 0.f, 0.25, opak::ease_in_ease_out);
}



/*
==============================================================================
Name : req_select
==============================================================================
*/

void  BundlePedestrian::req_select (bool toggle_flag)
{
   model::ClientData & client_data = model::ClientData::use_self (_pedestrian);
   model::Selection & selection = client_data.use_selection ();
   
   selection.tx_select (_pedestrian, toggle_flag);
}



/*
==============================================================================
Name : req_delete
==============================================================================
*/

void  BundlePedestrian::req_delete ()
{
   model::ClientData & client_data = model::ClientData::use_self (_pedestrian);
   model::Selection & selection = client_data.use_selection ();
   
   selection.tx_delete_content ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
