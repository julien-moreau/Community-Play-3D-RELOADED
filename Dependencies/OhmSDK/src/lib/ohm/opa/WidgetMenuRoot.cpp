/*****************************************************************************

        WidgetMenuRoot.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69935

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

#include "ohm/opa/WidgetMenuRoot.h"
#include "ohm/opa/BundleView.h"
#include "ohm/opa/prim/PrimMenuRoot.h"
#include "ohm/opa/private/WidgetLayoutManager.h"
#include "ohm/opal/Screens.h"

#include "ohm/math/fnc.h"
#include "ohm/sys/TimerReliable.h"

#include <algorithm>

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : SafeLink::ctor
Description :
   see link/unlink
==============================================================================
*/

WidgetMenuRoot::SafeLink::SafeLink ()
:  _widget_ptr (0)
{
}



/*
==============================================================================
Name : SafeLink::ctor
Description :
   see link/unlink
==============================================================================
*/

WidgetMenuRoot::SafeLink::~SafeLink ()
{
   if (_widget_ptr != 0)
   {
      _widget_ptr->unlink (*this);
   }
}



/*
==============================================================================
Name : SafeLink::bind
Description :
   see link/unlink
==============================================================================
*/

void  WidgetMenuRoot::SafeLink::bind (WidgetMenuRoot & widget)
{
   _widget_ptr = &widget;
   
   _widget_ptr->link (*this);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

WidgetMenuRoot::~WidgetMenuRoot ()
{
   _prim_back_aptr->keep_alive (.25f);
   
   if (_safe_link_ptr != 0)
   {
      _safe_link_ptr->_widget_ptr = 0;
   }
}



/*
==============================================================================
Name: fade_out
==============================================================================
*/

void WidgetMenuRoot::fade_out ()
{
   _prim_back_aptr->_opacity
      .stop ()
      .push (0.f, .25f, opak::ease_out);
}



/*
==============================================================================
Name : req_close
==============================================================================
*/

void  WidgetMenuRoot::req_close (const opal::Event & event)
{
   Event widget_event (*this, event);
   widget_event._type = Event::Type_CLOSE;
   
   _delegate.param_ref_signal (widget_event);
}



/*
==============================================================================
Name : can_select
Description :
   Returns true if the initial delay (0.5 s) is over, false otherwise.
==============================================================================
*/

bool  WidgetMenuRoot::can_select ()
{
   double now = sys::TimerReliable::use_instance ().get ();
   
   return now > _creation_time + 0.5;
}



/*
==============================================================================
Name : handle_event_default
==============================================================================
*/

void  WidgetMenuRoot::handle_event_default (Event & event)
{
   if (event._type == Event::Type_CLOSE)
   {
      hide ();
   }
}



/*
==============================================================================
Name : invalidate_layout
==============================================================================
*/

void  WidgetMenuRoot::invalidate_layout ()
{
   WidgetLayoutManager::use_instance ().invalidate (*this);
}



/*
==============================================================================
Name : update_layout
==============================================================================
*/

void  WidgetMenuRoot::update_layout ()
{
   assert (_safe_link_ptr != 0);
   
   opa::Rect min_bbox;
   get_min_bbox (min_bbox);
   
   // if we don't have enough height, we switch to multi col
   
   BundleView & bundle_view = get_ancestor <BundleView> ();
   
   const opa::Size & view_size = bundle_view.get_bounds ();
   
   _nbr_total_col = 1;
   _nbr_visible_col = 1;
   _col_width = min_bbox._size._width;
   
   if (min_bbox._size._height + 20.f > view_size._height)
   {
      // switch to multi col
      
      _nbr_total_col = math::ceil_int (
         (min_bbox._size._height + 20.f) / view_size._height
      );
      assert (_nbr_total_col > 1);
      
      _nbr_visible_col = _nbr_total_col;
      
      min_bbox._size._height /= float (_nbr_total_col);
      min_bbox._size._width *= _nbr_total_col;
      
      min_bbox._size.upper_integral ();
   }
   
   // now we can layout our items
   
   layout (min_bbox._size);
   
   _layer_clip_aptr->_clip_size = min_bbox._size;
   
   // then if we don't have enough width, we activate scrolling
   
   opa::Size menu_size = min_bbox._size;
   
   if (min_bbox._size._width + 20.f > view_size._width)
   {
      _nbr_visible_col = math::floor_int (
         (view_size._width - 20.f) / _col_width
      );
      
      menu_size._width = _col_width * _nbr_visible_col;
      
      _layer_clip_aptr->_clip_size = menu_size;
      
      float margin = 20.f;
      
      menu_size._width += margin * 2.f;
      
      _layer_clip_aptr->_origin._x = margin;
      
      _frame_scroll_left_aptr->set_bounds (opa::Size (margin, menu_size._height));
      
      _frame_scroll_right_aptr->set_origin (opa::Point (menu_size._width - margin, 0.f));
      _frame_scroll_right_aptr->set_bounds (opa::Size (margin, menu_size._height));
      
      _frame_scroll_left_aptr->activate ();
      _frame_scroll_right_aptr->activate ();
      
      set_col (0);
   }
   
   set_bounds (menu_size);
   
   // now that we have the final size, we reposition the menu
   // if needed
   
   opa::Size avail_size = view_size;
   
#if 0
   std::vector <opa::Rect> rect_arr = opal::Screens::use_instance ().get_layout ();
   assert (!rect_arr.empty ());

   opa::Size main_size = rect_arr [0]._size;
   
   lang::min_in_place (avail_size._width, main_size._width);
   lang::min_in_place (avail_size._height, main_size._height);
#endif
   
   opa::Rect avail_rect (opa::Point::_zero, avail_size);
   
   opa::Point origin = get_origin ();
   opa::Rect rect (
      origin,
      menu_size
      + opa::Size (0.f, 10.f) // part of bottom from PrimMenuRoot
   );
   
   if (!avail_rect.contains (rect))
   {
      if (rect._origin._x + rect._size._width > avail_rect._size._width)
      {
         origin._x = avail_rect._size._width - rect._size._width;
      }
      
      if (rect._origin._y + rect._size._height > avail_rect._size._height)
      {
         origin._y = avail_rect._size._height - rect._size._height;
      }
      
      origin.lower_integral ();
      
      set_origin (origin);
   }
}



/*
==============================================================================
Name : link
Description :
   Link to an object having a potential different life cycle than the
   menu.
==============================================================================
*/

void  WidgetMenuRoot::link (SafeLink & safe_link)
{
   _safe_link_ptr = &safe_link;
}



/*
==============================================================================
Name : unlink
Description :
   The safe link object is going to be destroyed : abort.
==============================================================================
*/

void  WidgetMenuRoot::unlink (SafeLink & safe_link)
{
   if (_safe_link_ptr == &safe_link)
   {
      BundleView & bundle_view = get_ancestor <BundleView> ();
      
      bundle_view.req_release_popup_menu ();
      
      _safe_link_ptr = 0;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetMenuRoot::WidgetMenuRoot ()
:  _creation_time (sys::TimerReliable::use_instance ().get ())
,  _safe_link_ptr (0)
,  _nbr_total_col (1)
,  _nbr_visible_col (1)
,  _col_width (0.f)
{
   bind <WidgetMenuRoot, &WidgetMenuRoot::handle_event_default> (*this);

   _frame_aptr = new opal::Frame ("WidgetMenuRoot::frame");
   _frame_aptr->set_bounds (opa::Size (100000.f, 100000.f));
   set_embeddable (*_frame_aptr);
   
   _frame_content_aptr = new opal::Frame ("WidgetMenuRoot::frame_content");
   _frame_content_aptr->set_bounds (opa::Size (100000.f, 100000.f));
   _frame_aptr->insert_top (*_frame_content_aptr);
   
   _frame_scroll_left_aptr = new opal::FrameSignal;
   _frame_scroll_left_aptr->deactivate ();
   _frame_aptr->insert_top (*_frame_scroll_left_aptr);
   
   _frame_scroll_left_aptr->bind_gesture_signal <
      WidgetMenuRoot,
      &WidgetMenuRoot::process_signal_left
   > (*this);

   _frame_scroll_right_aptr = new opal::FrameSignal;
   _frame_scroll_right_aptr->deactivate ();
   _frame_aptr->insert_top (*_frame_scroll_right_aptr);
   
   _frame_scroll_right_aptr->bind_gesture_signal <
      WidgetMenuRoot,
      &WidgetMenuRoot::process_signal_right
   > (*this);
   
   _prim_back_aptr = new PrimMenuRoot;
   set_embeddable (*_prim_back_aptr);
   
   _layer_clip_aptr = new opak::LayerClip;
   _prim_back_aptr->push_back (*_layer_clip_aptr);
   
   _layer_content_aptr = new opak::Layer;
   _layer_clip_aptr->push_back (*_layer_content_aptr);
   
   set_nbr_slot (1);
   set_slot (0, *_frame_content_aptr);
   set_slot (0, *_layer_content_aptr);
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetMenuRoot::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _prim_back_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
Note :
   called from 'layout' first, then from 'set_bounds'
==============================================================================
*/

void  WidgetMenuRoot::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
   
   _prim_back_aptr->_size.stop ().push (bounds, tt, tf);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetMenuRoot::do_enable (bool enabled_flag)
{
   _prim_back_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetMenuRoot::do_show (bool show_flag)
{
   _prim_back_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetMenuRoot::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process_signal_left
==============================================================================
*/

void  WidgetMenuRoot::process_signal_left (opal::FrameSignal::Event & event)
{
   set_col (_col_x - _nbr_visible_col);
}



/*
==============================================================================
Name : process_signal_right
==============================================================================
*/

void  WidgetMenuRoot::process_signal_right (opal::FrameSignal::Event & event)
{
   set_col (_col_x + _nbr_visible_col);
}



/*
==============================================================================
Name : set_col
==============================================================================
*/

void  WidgetMenuRoot::set_col (int col_x)
{
   lang::limit_in_place (col_x, 0, _nbr_total_col - _nbr_visible_col + 1);
   
   _col_x = col_x;
   
   opa::Point origin (- _col_x * _col_width, 0.f);
   
   _frame_content_aptr->set_origin (origin);
   _layer_content_aptr->_origin.stop ().push (origin, 0.25, opak::ease_in_ease_out);
   
   {
      float opacity = (_col_x == 0) ? .25f : 1.f;
      
      _prim_back_aptr->_arrow_left_opacity
         .stop ()
         .push (opacity, 0.25, opak::ease_in_ease_out);
   }

   {
      float opacity = (_col_x == _nbr_total_col - _nbr_visible_col + 1) ? .25f : 1.f;
      
      _prim_back_aptr->_arrow_right_opacity
         .stop ()
         .push (opacity, 0.25, opak::ease_in_ease_out);
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
