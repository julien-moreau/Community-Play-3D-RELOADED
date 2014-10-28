/*****************************************************************************

        BundleView.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71210

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

#include "ohm/opal/def.h"

#include "ohm/opa/BundleView.h"
#include "ohm/opa/EventBundleView.h"
#include "ohm/opa/ToolTipSafeLink.h"
#include "ohm/opak/View.h"
#include "ohm/opal/Application.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opal/Frame.h"
#include "ohm/opal/VideoThread.h"
#include "ohm/opal/View.h"
#include "ohm/task/RunLoopGui.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

BundleView::BundleView (opal::Window & window)
:  opa::Bundle ("view")
,  _window (window)
,  _opal_view_allocated_flag (false)
,  _opal_view_ptr (0)
,  _opak_view_ptr (0)

,  _intercalated_layer_map ()
,  _menu_root_sptr ()

,  _tool_tip_represented_obj_ptr (0)
{
   assert (&window != 0);
   
   _opal_view_ptr = new opal::View (window);
   _opal_view_allocated_flag = true;
   
   init ();
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

BundleView::BundleView (opal::View & view)
:  opa::Bundle ("view")
,  _window (view.use_window ())
,  _opal_view_allocated_flag (false)
,  _opal_view_ptr (&view)
,  _opak_view_ptr (0)

,  _intercalated_layer_map ()
,  _menu_root_sptr ()

,  _tool_tip_represented_obj_ptr (0)
{
   assert (&view != 0);
   
   _opal_view_allocated_flag = false;
   
   init ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BundleView::~BundleView ()
{
   try
   {
      _menu_root_sptr.destroy ();
      
      for (size_t i = 0 ; i < NBR_Slot_TOTAL ; ++i)
      {
         _frame_aptr_arr [i].reset ();
         
         _layer_aptr_arr [i].reset ();
      }
      
      _intercalated_layer_map.clear ();
      
      // stop video thread from calling opal/opak view

      if (_opal_view_ptr != 0)
      {
         opal::VideoThread::use_instance ().deactivate (*_opal_view_ptr);
      }
      
      // releasing opak View will unbind it from the opal View
      
      if (_opak_view_ptr != 0)
      {
         opak::View::release (*_opak_view_ptr);

         _opak_view_ptr = 0;
      }

      if (_opal_view_ptr != 0)
      {
         if (_opal_view_allocated_flag)
         {
            delete _opal_view_ptr;
            _opal_view_ptr = 0;
         }
      }
      
   }
   catch (...)
   {
   }
}



/*
==============================================================================
Name : use_window
==============================================================================
*/

opal::Window & BundleView::use_window ()
{
   return _window;
}



/*
==============================================================================
Name : run
==============================================================================
*/

void  BundleView::run ()
{
   assert (_opak_view_ptr != 0);
   _opak_view_ptr->run ();
}



/*
==============================================================================
Name : stop
==============================================================================
*/

void  BundleView::stop ()
{
   assert (_opak_view_ptr != 0);
   _opak_view_ptr->clear ();
}



/*
==============================================================================
Name : add
==============================================================================
*/

void  BundleView::add (opal::TimeListener & listener)
{
   assert (_opal_view_ptr != 0);
   
   _opal_view_ptr->add (listener);
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  BundleView::remove (opal::TimeListener & listener)
{
   assert (_opal_view_ptr != 0);
   
   _opal_view_ptr->remove (listener);
}



/*
==============================================================================
Name : add
==============================================================================
*/

void  BundleView::add (opal::PostActionListener & listener)
{
   assert (_opal_view_ptr != 0);
   
   _opal_view_ptr->add (listener);
}



/*
==============================================================================
Name : remove
==============================================================================
*/

void  BundleView::remove (opal::PostActionListener & listener)
{
   assert (_opal_view_ptr != 0);
   
   _opal_view_ptr->remove (listener);
}



/*
==============================================================================
Name : deintercalate
==============================================================================
*/

void  BundleView::deintercalate (const std::string & name)
{
   NamePrimMap::iterator it = _intercalated_layer_map.find (name);
   
   if (it != _intercalated_layer_map.end ())
   {
      AutoContainerPtrSPtr & prim_aptr_sptr = it->second;
      
      opak::Layer & root_layer = *_opak_view_ptr->_layer_aptr;
      
      root_layer.deintercalate (**prim_aptr_sptr);
   }
}



/*
==============================================================================
Name : deintercalate
==============================================================================
*/

void  BundleView::deintercalate (Slot slot, const std::string & name)
{
   NamePrimMap::iterator it = _intercalated_layer_map.find (name);
   
   if (it != _intercalated_layer_map.end ())
   {
      AutoContainerPtrSPtr & prim_aptr_sptr = it->second;
      
      opak::Layer & slot_layer = *_layer_aptr_arr [slot];
      
      slot_layer.deintercalate (**prim_aptr_sptr);
   }
}



/*
==============================================================================
Name : get_bounds
==============================================================================
*/

const opa::Size & BundleView::get_bounds () const
{
   assert (_opal_view_ptr != 0);
   
   opal::Frame & root_frame = _opal_view_ptr->use_root_frame ();
   
   return root_frame.get_bounds ();
}



/*
==============================================================================
Name: create_popup_menu
==============================================================================
*/

WidgetMenuRoot & BundleView::create_popup_menu (WidgetMenuRoot::SafeLink & safe_link)
{
   assert (_menu_root_sptr.get () == 0);
   assert (_opal_view_ptr != 0);

   _menu_root_sptr = WidgetMenuRoot::create ();
   safe_link.bind (*_menu_root_sptr);

   _frame_menu_close_aptr = new FrameWidgetMenuClose (*this, *_menu_root_sptr);
   _frame_menu_close_aptr->set_no_clip ();
   _frame_aptr_arr [Slot_POPUP_MENU]->insert_top (*_frame_menu_close_aptr);
   
   embed (*_menu_root_sptr, Slot_POPUP_MENU);
   
   const opal::Event & event = _opal_view_ptr->get_event ();
   
   opa::Point menu_root_origin = event._mouse._view_relative_location;
   
   _menu_root_sptr->set_origin (menu_root_origin);

#if defined (ohm_opal_NEW_CURSOR_API_FLAG)   
   opal::Application::use_instance ().set_cursor_arrow ();
#endif
   
   return *_menu_root_sptr;
}



/*
==============================================================================
Name: stop_popup_menu
Description :
   Close popup menu instantaneously.
   Used when a popup menu is empty.
==============================================================================
*/

void  BundleView::stop_popup_menu ()
{
   release_popup_menu ();
}



/*
==============================================================================
Name: create_tool_tip
Description :
   associate a tool tip with object pointer 'id_ptr', display 'str' around
   'pt' and try not to obscurate 'rect'
==============================================================================
*/

void  BundleView::create_tool_tip (ToolTipSafeLink & safe_link, const opa::Point & pt, const opa::Rect & rect, const std::string & label)
{
   safe_link.bind (*this);
   _tool_tip_represented_obj_ptr = safe_link.get_obj_ptr ();
   
   opa::Size tip_size;
   tip_size._height = 16;
   
   {
      const opal::Font & font
         = opal::FontManager::use_instance ().get_font (Constants::_default_font_0, 10.f);

      opal::Scripture scripture;
      scripture.push_state_font (font);
      
      scripture.begin_edit ();
      scripture.insert (0, label);
      scripture.end_edit ();
      
      opal::Typesetter typesetter (scripture);
      
      size_t next_pos = 0;
      float height = 0;
      float width = typesetter.suggest_char_break (next_pos, height, 0, 10000.f);
      
      tip_size._width = width + 7.f * 2.f;
   }
   
   const float space = 2.f;
   
   opa::Point tip_origin;
   tip_origin._y = rect._origin._y + rect._size._height + space;
   
   assert (_opal_view_ptr != 0);
   opa::Size view_size = _opal_view_ptr->get_bounds ();
   
   if (tip_origin._y + tip_size._height + space > view_size._height)
   {
      // put it above
      
      tip_origin._y = rect._origin._y - tip_size._height - space;
   }
   
   tip_origin._x = pt._x;

   if (tip_origin._x + tip_size._width + space > view_size._width)
   {
      tip_origin._x = view_size._width - tip_size._width - space;
   }
   
   tip_origin.lower_integral ();
   
   _prim_tool_tip_aptr->_origin = tip_origin;
   _prim_tool_tip_aptr->_label = label;
   _prim_tool_tip_aptr->_opacity
      .stop ()
      .push (0.f, 0.0, opak::step)
      .push (0.f, 0.5, opak::step)
      .push (1.f, 0.125, opak::linear);
}



/*
==============================================================================
Name: release_tool_tip
==============================================================================
*/

void  BundleView::release_tool_tip (ToolTipSafeLink & safe_link)
{
   if (_tool_tip_represented_obj_ptr == safe_link.get_obj_ptr ())
   {
      _prim_tool_tip_aptr->_opacity
         .stop ()
         .push (0.f, 0.25, opak::linear);
      
      _tool_tip_represented_obj_ptr = 0;
   }
}



/*
==============================================================================
Name : debug_toggle_show_invalid_rect
==============================================================================
*/

void  BundleView::debug_toggle_show_invalid_rect ()
{
   assert (_opal_view_ptr != 0);
   
   if (_opal_view_ptr != 0)
   {
      _opal_view_ptr->debug_toggle_show_invalid_rect ();
   }
}



/*
==============================================================================
Name : set_mouse_wheel_inversion
==============================================================================
*/

void  BundleView::set_mouse_wheel_inversion (bool invert_flag)
{
   assert (_opal_view_ptr != 0);
   
   if (_opal_view_ptr != 0)
   {
      _opal_view_ptr->set_mouse_wheel_inversion (invert_flag);
   }
}



/*
==============================================================================
Name : post
==============================================================================
*/

void  BundleView::post (const std::vector <opal::HIDEvent> & hid_event_arr)
{
   if (_opal_view_ptr != 0)
   {
      _opal_view_ptr->post (hid_event_arr);
   }
}



/*
==============================================================================
Name : post_update_cursor
==============================================================================
*/

void  BundleView::post_update_cursor ()
{
   if (_opal_view_ptr != 0)
   {
      _opal_view_ptr->post_update_cursor ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: init
==============================================================================
*/

void  BundleView::init ()
{
   _opak_view_ptr = &opak::View::create (*_opal_view_ptr);
   
   opal::Frame & root_frame = _opal_view_ptr->use_root_frame ();
   opak::Layer & root_layer = *_opak_view_ptr->_layer_aptr;
   
   set_embeddable (root_frame);
   set_embeddable (root_layer);
   
   set_nbr_slot (NBR_Slot_TOTAL);
   
   for (int i = 0 ; i < NBR_Slot_TOTAL ; ++i)
   {
      static const char * plane_name_0_arr [] = {
         "View::plane_workspace",
         "View::plane_dialog",
         "View::plane_popup",
         "View::plane_overlay",
      };
      
      opal::AutoFramePtr <opal::Frame> & frame_aptr = _frame_aptr_arr [i];
      
      frame_aptr
         = new opal::Frame (plane_name_0_arr [i]);
      root_frame.insert_top (*frame_aptr);
      
      frame_aptr->set_bounds (root_frame.get_bounds ());
      
      set_slot (static_cast <Slot> (i), *frame_aptr);
      
      _layer_aptr_arr [i] = new opak::Layer;
      _layer_aptr_arr [i]->set_name (plane_name_0_arr [i]);
      root_layer.push_back (*_layer_aptr_arr [i]);
      
      set_slot (static_cast <Slot> (i), *_layer_aptr_arr [i]);
   }
   
   _prim_tool_tip_aptr = new PrimToolTip;
   _layer_aptr_arr [Slot_OVERLAY]->push_back (*_prim_tool_tip_aptr);
   
   root_frame.mark (MarkId_ROOT);
   
   _opal_view_ptr->bind_layout_notification <
      BundleView, 
      &BundleView::view_layout_changed
   > (*this);
}



/*
==============================================================================
Name: release_popup_menu
==============================================================================
*/

void  BundleView::release_popup_menu ()
{
   assert (_menu_root_sptr.get () != 0);
   
   _frame_menu_close_aptr.reset ();
   _menu_root_sptr.destroy ();
   
   post_update_cursor ();
}



/*
==============================================================================
Name: req_release_popup_menu
==============================================================================
*/

void BundleView::req_release_popup_menu ()
{
   _menu_root_sptr->fade_out ();
   
   release_popup_menu ();
}



/*
==============================================================================
Name: view_layout_changed
==============================================================================
*/

void  BundleView::view_layout_changed ()
{
   opal::Frame & root_frame = _opal_view_ptr->use_root_frame ();
   const opa::Size & frame_size = root_frame.get_bounds ();

   for (int i = 0 ; i < NBR_Slot_TOTAL ; ++i)
   {
      opal::AutoFramePtr <opal::Frame> & frame_aptr = _frame_aptr_arr [i];
      frame_aptr->set_bounds (frame_size);
   }

   EventBundleView bundle_event (EventBundleView::Type_LAYOUT_CHANGED);

   process_event (bundle_event);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
