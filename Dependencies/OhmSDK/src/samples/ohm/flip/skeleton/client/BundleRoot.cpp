/*****************************************************************************

        BundleRoot.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70483

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

#include "client/BundleRoot.h"

#include "client/BundleGui.h"

#include "model/Root.h"

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

BundleRoot::BundleRoot (model::Root & root, BundleGui & parent)
:  ohm::opa::Bundle (parent, "BundleRoot")
,  _root (root)
,  _parent (parent)
{
   using namespace ohm;
   
   _frame_aptr = new opal::Frame ("BundleRoot::_frame_aptr");
   parent.get_slot_frame (BundleGui::Slot_CONTENT).insert_top (*_frame_aptr);

   _frame_conduct_aptr = new ohm::opal::FrameConduct <Conduct> ("BundleRoot::_frame_conduct_aptr");
   _frame_conduct_aptr->set_no_clip ();
   _frame_aptr->insert_top (*_frame_conduct_aptr);

   _frame_marquee_aptr = new ohm::opal::FrameConduct <ConductMarquee> ("BundleRoot::_frame_marquee_aptr");
   _frame_marquee_aptr->set_no_clip ();
   _frame_conduct_aptr->insert_top (*_frame_marquee_aptr);

   _frame_root_aptr = new FrameRoot (*this);
   _frame_root_aptr->set_no_clip ();
   _frame_marquee_aptr->insert_top (*_frame_root_aptr);
   
   _prim_aptr = new PrimRoot;
   parent.get_slot_layer (BundleGui::Slot_CONTENT).push_back (*_prim_aptr);

   _layer_root_aptr = new opak::Layer;
   _prim_aptr->push_back (*_layer_root_aptr);

   _prim_marquee_aptr = new PrimMarquee;
   _prim_aptr->push_back (*_prim_marquee_aptr);
   
   set_nbr_slot (1);
   set_slot (0, *_frame_root_aptr);
   set_slot (0, *_layer_root_aptr);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BundleRoot::~BundleRoot ()
{
}



/*
==============================================================================
Name : req_start
==============================================================================
*/

bool  BundleRoot::req_start ()
{
   model::ClientData & client_data = model::ClientData::use_self (_root);
   model::ViewInfo & view_info = client_data.use_view_info ();
   
   return view_info.start_session ();
}



/*
==============================================================================
Name : req_commit
==============================================================================
*/

void  BundleRoot::req_commit ()
{
   model::ClientData & client_data = model::ClientData::use_self (_root);
   model::ViewInfo & view_info = client_data.use_view_info ();
   
   view_info.commit_session ();
}



/*
==============================================================================
Name : req_revert
==============================================================================
*/

void  BundleRoot::req_revert ()
{
   model::ClientData & client_data = model::ClientData::use_self (_root);
   model::ViewInfo & view_info = client_data.use_view_info ();
   
   view_info.revert_session ();
}



/*
==============================================================================
Name : req_focus_delta
==============================================================================
*/

void  BundleRoot::req_focus_delta (const ohm::opa::Point & delta)
{
   model::ClientData & client_data = model::ClientData::use_self (_root);
   model::ViewInfo & view_info = client_data.use_view_info ();
   
   ohm::archi::Float64 focus_x = view_info.get_focus_x ();
   ohm::archi::Float64 focus_y = view_info.get_focus_y ();
   
   focus_x += delta._x;
   focus_y += delta._y;
   
   view_info.tx_set_focus (focus_x, focus_y);
}



/*
==============================================================================
Name : req_start_marquee
==============================================================================
*/

void  BundleRoot::req_start_marquee (const ohm::opa::Point & origin)
{
   _prim_marquee_aptr->_origin = origin;
}



/*
==============================================================================
Name : req_change_marquee_size
==============================================================================
*/

void  BundleRoot::req_change_marquee_size (const ohm::opa::Size & bounds)
{
   // we might be in dtor
   
   if (_prim_marquee_aptr.get () != 0)
   {
      _prim_marquee_aptr->_size = bounds;
   }
}



/*
==============================================================================
Name : req_end_marquee
==============================================================================
*/

void  BundleRoot::req_end_marquee (bool toggle_flag)
{
   using namespace ohm;
   
   opa::Rect marquee_rect (_prim_marquee_aptr->_origin, _prim_marquee_aptr->_size);

   marquee_rect = marquee_rect.standardize ();
   
   _frame_marquee_aptr->use ().begin_collecting (marquee_rect);
   _frame_marquee_aptr->use ()._toggle_flag = toggle_flag;
   
   _frame_marquee_aptr->post_conduct_changed_event (_frame_marquee_aptr->use ());
   
   _prim_marquee_aptr->_size = opa::Size::_zero;
}



/*
==============================================================================
Name : req_clear_selection
==============================================================================
*/

void  BundleRoot::req_clear_selection ()
{
   model::ClientData & client_data = model::ClientData::use_self (_root);
   model::Selection & selection = client_data.use_selection ();
   
   selection.tx_clear ();
}



/*
==============================================================================
Name : req_delete_selection
==============================================================================
*/

void  BundleRoot::req_delete_selection ()
{
   model::ClientData & client_data = model::ClientData::use_self (_root);
   model::Selection & selection = client_data.use_selection ();
   
   selection.tx_delete_content ();
}



/*
==============================================================================
Name : req_move_selection
==============================================================================
*/

void  BundleRoot::req_move_selection (const ohm::opa::Point & delta, bool big_flag, bool copy_flag)
{
   using namespace ohm;
   
   model::ClientData & client_data = model::ClientData::use_self (_root);
   model::Selection & selection = client_data.use_selection ();
   
   if (selection.start_session ())
   {
      archi::Float64 position_x_delta = delta._x;
      archi::Float64 position_y_delta = delta._y;
      
      if (big_flag)
      {
         position_x_delta *= 10.0;
         position_y_delta *= 10.0;
      }
      
      selection.session_pedestrian_move (position_x_delta, position_y_delta);
      
      if (copy_flag)
      {
         selection.session_copy ();
      }
      
      selection.commit_session ();
   }
}



/*
==============================================================================
Name : req_select_marquee
==============================================================================
*/

void  BundleRoot::req_select_marquee ()
{
   model::ClientData & client_data = model::ClientData::use_self (_root);
   model::Selection & selection = client_data.use_selection ();
   
   ConductMarquee & conduct = _frame_marquee_aptr->use ();
   
   selection.tx_select (
      conduct.use_object_ptr_set ().begin (),
      conduct.use_object_ptr_set ().end (),
      conduct._toggle_flag
   );
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
