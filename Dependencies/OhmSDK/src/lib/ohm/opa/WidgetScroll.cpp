/*****************************************************************************

        WidgetScroll.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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

#include "ohm/opa/WidgetScroll.h"
#include "ohm/opa/WidgetScrollBar.h"
#include "ohm/opa/private/FrameScroll.h"
#include "ohm/opal/Frame.h"
#include "ohm/opak/base/PrimitiveContainer.h"
#include "ohm/opak/base/Sizable.h"

#include <algorithm>

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

WidgetScroll::~WidgetScroll ()
{
   try
   {
      unset ();
      
      // explicitely call frame reset :
      // if an edit is ungoing, this is going to release
      // the mouse capture which in turn will send an
      // event to this widget - therefore we want primitives
      // and delegate to still exist.
      
      _frame_container_aptr.reset ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  WidgetScroll::bind (WidgetScrollBar & widget)
{
   assert (&widget != 0);

   _listener_ptr = &widget;
   
   notify_listener ();
}



/*
==============================================================================
Name : set_width
Description :
   Fix the width of 'this', independent of actual content width.
   In particular this means that the widget may scroll horizontally.
==============================================================================
*/

void  WidgetScroll::set_width (float width)
{
   _width.set (width);
   set_horizontal_weight (0.f);
}



/*
==============================================================================
Name : set_width_auto_content
Description :
   Tell the layout system that the width of 'this' is dependent on the
   actual content width.
   In particular this means that the widget won't scroll horizontally.
==============================================================================
*/

void  WidgetScroll::set_width_auto_content ()
{
   _width.set_auto ();
   set_horizontal_weight (0.f);
}



/*
==============================================================================
Name : set_width_auto_parent
Description :
   Tell the layout system that the width of 'this' is dependent on the
   actual parent width.
==============================================================================
*/

void  WidgetScroll::set_width_auto_parent (float weight)
{
   _width.set_ext ();
   set_horizontal_weight (weight);
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

float WidgetScroll::get_width ()
{
   if (_width.need_update ())
   {
      update_metrics ();
   }
   
   return _width.get ();
}



/*
==============================================================================
Name : set_height
Description :
   Fix the height of 'this', independent of actual content height.
   In particular this means that the widget may scroll vertically.
==============================================================================
*/

void  WidgetScroll::set_height (float height)
{
   _height.set (height);
}



/*
==============================================================================
Name : set_height_auto_content
Description :
   Tell the layout system that the height of 'this' is dependent on the
   actual content height.
   In particular this means that the widget won't scroll vertically.
==============================================================================
*/

void  WidgetScroll::set_height_auto_content ()
{
   _height.set_auto ();
}



/*
==============================================================================
Name : set_height_auto_parent
Description :
   Tell the layout system that the height of 'this' is dependent on the
   actual parent width.
==============================================================================
*/

void  WidgetScroll::set_height_auto_parent (float weight)
{
   _height.set_ext ();
   set_vertical_weight (weight);
}



/*
==============================================================================
Name : get_height
==============================================================================
*/

float WidgetScroll::get_height ()
{
   if (_height.need_update ())
   {
      update_metrics ();
   }
   
   return _height.get ();
}



/*
==============================================================================
Name : set_flush
==============================================================================
*/

void  WidgetScroll::set_flush (Border border)
{
   switch (border)
   {
   case Border_LEFT:
      _flush_right_flag = false;
      break;

   case Border_TOP:
      _flush_bottom_flag = false;
      break;

   case Border_RIGHT:
      _flush_right_flag = true;
      break;

   case Border_BOTTOM:
      _flush_bottom_flag = true;
      break;
   
   default:
      assert (false);
      break;
   }
   
   invalidate_layout ();
}



/*
==============================================================================
Name : enable_clip
==============================================================================
*/

void  WidgetScroll::enable_clip (Border border, bool enable_flag)
{
   _enable_clip_flag_arr [border] = enable_flag;
   
   update_clip (0.0, opak::step);
   update_content ();
}



/*
==============================================================================
Name : unset
==============================================================================
*/

void  WidgetScroll::unset ()
{
   assert (_widget_sptr.get () != 0);
   
   if (_widget_sptr.get () != 0)
   {
      unembed (*_widget_sptr);
      
      _widget_sptr.destroy ();
   }
}



/*
==============================================================================
Name : empty
==============================================================================
*/

bool  WidgetScroll::empty () const
{
   return (_widget_sptr.get () == 0);
}



/*
==============================================================================
Name : req_move_content
==============================================================================
*/

void  WidgetScroll::req_move_content (const opa::Point & delta)
{
   if (_widget_sptr.get () != 0)
   {
      opa::Point origin = _widget_sptr->get_origin ();
      
      if (!_width.is_auto ())
      {
         origin._x += delta._x * 28.f;
      }
      
      if (!_height.is_auto ())
      {
         origin._y += delta._y * 28.f;
      }
      
      _widget_sptr->set_origin (origin);
      
      update_content ();

      // directly propaget the visible rect
      spread_view (_view_rect);
   }
}



/*
==============================================================================
Name : req_move_content
==============================================================================
*/

void  WidgetScroll::req_set_content (const opa::Point & point)
{
   if (_widget_sptr.get () != 0)
   {
      opa::Point origin = _widget_sptr->get_origin ();
      
      if (!_width.is_auto ())
      {
         origin._x = - point._x;
      }
      
      if (!_height.is_auto ())
      {
         origin._y = - point._y;
      }
      
      _widget_sptr->set_origin (origin);
      
      update_content ();
      
      // directly propaget the visible rect
      spread_view (_view_rect);
   }
}



/*
==============================================================================
Name : req_move_content
==============================================================================
*/

opa::Point  WidgetScroll::get_content_position ()
{
   if (_widget_sptr.get () != 0)
   {
      return - _widget_sptr->get_origin ();
   }
   else
   {
      return opa::Point::_zero;
   }
}



/*
==============================================================================
Name: invalidate_layout
==============================================================================
*/

void WidgetScroll::invalidate_layout ()
{
   _width.invalidate ();
   _height.invalidate ();

   WidgetBase::invalidate_layout ();
}



/*
==============================================================================
Name : layout
==============================================================================
*/

bool  WidgetScroll::layout (const Size & bounds)
{
   bool retrigger_layout_flag = WidgetBase::layout (bounds);
   
   if (!empty ())
   {
      for (int i = 0 ; i < 2 ; ++i)
      {
         Size sub_bounds (1e8f, 1e8f);
         
         {
            float weight = _widget_sptr->get_horizontal_weight ();
            
            if (weight > 0.f)
            {
               sub_bounds._width = bounds._width;
            }
         }
         
         {
            float weight = _widget_sptr->get_vertical_weight ();
            
            if (weight > 0.f)
            {
               sub_bounds._height = bounds._height;
            }
         }
         
         retrigger_layout_flag |= _widget_sptr->layout (sub_bounds);
         
         if (!retrigger_layout_flag)
         {
            break;
         }
      }
      
      update_content ();
   }
   
   return retrigger_layout_flag;
}



/*
==============================================================================
Name : spread_view
Description :
   Propagate the visible portion in a widget tree. 'rect' is expressed
   relative to 'this' origin (not accounting relative origin).
==============================================================================
*/

void  WidgetScroll::spread_view (const Rect & rect)
{
   _view_rect = rect;
   
   if (!empty ())
   {
      Rect sub_rect = rect;
      
      Rect clip_rect (
         _layer_container_aptr->_clip_origin,
         _layer_container_aptr->_clip_size
      );
      
      sub_rect &= clip_rect;
      
      // propagate even if 'sub_rect' is empty
      // this will let us notify our sub widget that they became
      // invisible
      
      sub_rect -= _widget_sptr->get_origin ();
      
      _widget_sptr->spread_view (sub_rect);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetScroll::WidgetScroll ()
:  _listener_ptr (0)
,  _widget_sptr ()
,  _width (*this)
,  _height (*this)
,  _flush_right_flag (false)
,  _flush_bottom_flag (false)
{
   _frame_container_aptr = new FrameScroll (*this);
   
   _layer_aptr = new LayerWidget;
   _layer_aptr->set_name ("WidgetScroll::base");

   _layer_container_aptr = new opak::LayerClip;
   _layer_container_aptr->set_name ("WidgetScroll::container");
   _layer_aptr->push_back (*_layer_container_aptr);
   
   set_embeddable (*_frame_container_aptr);
   set_embeddable (*_layer_aptr);
   
   _frame_content_aptr = new opal::Frame ("WidgetScroll::content");
   _frame_container_aptr->insert_top (*_frame_content_aptr);
   
   _layer_content_aptr = new opak::Layer;
   _layer_content_aptr->set_name ("WidgetScroll::content");
   _layer_container_aptr->push_back (*_layer_content_aptr);
   
   set_nbr_slot (1);
   set_slot (0, *_frame_content_aptr);
   set_slot (0, *_layer_content_aptr);
   
   for (int i = 0 ; i < Border_NBR_ELT ; ++i)
   {
      _enable_clip_flag_arr [i] = true;
      
      opak::AutoPrimPtr <opak::SprungImage> & prim_shadow_aptr
         = _prim_shadow_aptr_arr [i];
      
      prim_shadow_aptr = new opak::SprungImage;
      
#if 1
      #define ohm_opa_WidgetScroll_ctor_shadow ""
      float width = 1.f;
#else
      #define ohm_opa_WidgetScroll_ctor_shadow "radial."
      float width = 64.f;
#endif
      
      switch (i)
      {
      case Border_LEFT:
         prim_shadow_aptr->_image =
            "ohm.widget.scroll.shadow." ohm_opa_WidgetScroll_ctor_shadow "left.png";
         prim_shadow_aptr->_rect = opa::Rect (0, 0, 12, width);
         break;

      case Border_TOP:
         prim_shadow_aptr->_image =
            "ohm.widget.scroll.shadow." ohm_opa_WidgetScroll_ctor_shadow "top.png";
         prim_shadow_aptr->_rect = opa::Rect (0, 0, width, 12);
         break;

      case Border_RIGHT:
         prim_shadow_aptr->_image =
            "ohm.widget.scroll.shadow." ohm_opa_WidgetScroll_ctor_shadow "right.png";
         prim_shadow_aptr->_rect = opa::Rect (0, 0, 12, width);
         break;

      case Border_BOTTOM:
         prim_shadow_aptr->_image =
            "ohm.widget.scroll.shadow." ohm_opa_WidgetScroll_ctor_shadow "bottom.png";
         prim_shadow_aptr->_rect = opa::Rect (0, 0, width, 12);
         break;
      }

      #undef ohm_opa_WidgetScroll_ctor_shadow
      
      prim_shadow_aptr->_blend_mode = opak::BlendMode_MULTIPLY;
      
      _layer_container_aptr->push_back (*prim_shadow_aptr);
   }
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetScroll::do_get_min_bbox (Rect & min_bbox)
{
   if (_width.need_update () || _height.need_update ())
   {
      update_metrics ();
   }
   
   min_bbox = Rect::_zero;
   min_bbox._size._width = _width.get ();
   min_bbox._size._height = _height.get ();
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetScroll::do_get_margin (Margin & margin)
{
   margin = Margin::_zero;
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetScroll::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_container_aptr->set_origin (origin);

   _layer_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetScroll::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_container_aptr->set_bounds (bounds);

   update_clip (tt, tf);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetScroll::do_enable (bool enabled_flag)
{
   _layer_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetScroll::do_show (bool show_flag)
{
   _layer_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetScroll::do_activate (bool activate_flag)
{
   _frame_container_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_metrics
==============================================================================
*/

void  WidgetScroll::update_metrics ()
{
   if (_width.need_update () || _height.need_update ())
   {
      if (_widget_sptr.get () != 0)
      {
         Rect bbox;
         _widget_sptr->get_min_bbox (bbox);
         
         // ignore margin
         
         //_min_bbox_origin = bbox._origin;
         
         if (_width.need_update ())
         {
            _width.update (bbox._size._width);
         }

         if (_height.need_update ())
         {
            _height.update (bbox._size._height);
         }
      }
   }
}



/*
==============================================================================
Name : update_content
Description :
   Ensure that the content is properly displayed
==============================================================================
*/

void  WidgetScroll::update_content ()
{
   if (!empty ())
   {
      const opa::Size & container_size = get_bounds ();
      
      Rect bbox;
      _widget_sptr->get_min_bbox (bbox);
      
      Margin margin;
      _widget_sptr->get_margin (margin);
      
      opa::Size old_content_bounds = _content_bounds;
      _content_bounds = bbox._size;
      
      opa::Rect old_content_rect;
      old_content_rect._origin = _widget_sptr->get_origin ();
      old_content_rect._size = opa::Size (
         - bbox._origin._x + bbox._size._width + margin._left + margin._right,
         - bbox._origin._y + bbox._size._height + margin._top + margin._bottom
      );
      
      opa::Rect content_rect = old_content_rect;

      if (_flush_right_flag)
      {
         content_rect._origin._x -= _content_bounds._width - old_content_bounds._width;
      }
      
      if (_flush_bottom_flag)
      {
         content_rect._origin._y -= _content_bounds._height - old_content_bounds._height;
      }
      
      // make sure it is in bounds
      
      if (!_width.is_auto ())
      {
         float content_right
            = content_rect._origin._x
            + content_rect._size._width;
         
         if (content_right < container_size._width)
         {
            content_rect._origin._x
               = container_size._width
               - content_rect._size._width;
         }
         
         if (content_rect._origin._x > 0.f)
         {
            content_rect._origin._x = 0.f;
         }
      }
      
      if (!_height.is_auto ())
      {
         float content_bottom
            = content_rect._origin._y
            + content_rect._size._height;
         
         if (content_bottom < container_size._height)
         {
            content_rect._origin._y
               = container_size._height
               - content_rect._size._height;
         }
         
         if (content_rect._origin._y > 0.f)
         {
            content_rect._origin._y = 0.f;
         }
      }
      
      // update to keep in bounds
      
      if (content_rect != old_content_rect)
      {
         if (content_rect._origin == old_content_rect._origin)
         {
            _widget_sptr->set_bounds (content_rect._size);
         }
         else if (content_rect._size == old_content_rect._size)
         {
            _widget_sptr->set_origin (content_rect._origin);
         }
         else
         {
            _widget_sptr->set_rect (content_rect);
         }
      }
      
      {
         const opa::Size & cont_bounds = _frame_container_aptr->get_bounds ();
         
         opa::Point origin = _widget_sptr->get_origin ();
         opa::Size bounds = old_content_rect._size;
            
         _prim_shadow_aptr_arr [Border_LEFT]->_size
            = opa::Size (12, cont_bounds._height);

         _prim_shadow_aptr_arr [Border_TOP]->_size
            = opa::Size (cont_bounds._width, 12);
         
         _prim_shadow_aptr_arr [Border_RIGHT]->_origin
            = opa::Point (cont_bounds._width - 12, 0);

         _prim_shadow_aptr_arr [Border_RIGHT]->_size
            = opa::Size (12, cont_bounds._height);

         _prim_shadow_aptr_arr [Border_BOTTOM]->_origin
            = opa::Point (0, cont_bounds._height - 12);

         _prim_shadow_aptr_arr [Border_BOTTOM]->_size
            = opa::Size (cont_bounds._width, 12);
         
         bool cut_left_flag = origin._x < 0.f;
         cut_left_flag &= _enable_clip_flag_arr [Border_LEFT];
         
         bool cut_top_flag = origin._y < 0.f;
         cut_top_flag &= _enable_clip_flag_arr [Border_TOP];

         bool cut_right_flag = origin._x + bounds._width > cont_bounds._width;
         cut_right_flag &= _enable_clip_flag_arr [Border_RIGHT];

         bool cut_bottom_flag = origin._y + bounds._height > cont_bounds._height;
         cut_bottom_flag &= _enable_clip_flag_arr [Border_BOTTOM];
         
         float opacity = .5f;
         
         _prim_shadow_aptr_arr [Border_LEFT]->_opacity
            .stop ()
            .push (cut_left_flag ? opacity : 0.f, 0.0, opak::step);

         _prim_shadow_aptr_arr [Border_TOP]->_opacity
            .stop ()
            .push (cut_top_flag ? opacity : 0.f, 0.0, opak::step);

         _prim_shadow_aptr_arr [Border_RIGHT]->_opacity
            .stop ()
            .push (cut_right_flag ? opacity : 0.f, 0.0, opak::step);

         _prim_shadow_aptr_arr [Border_BOTTOM]->_opacity
            .stop ()
            .push (cut_bottom_flag ? opacity : 0.f, 0.0, opak::step);
      }
      
      // update scroll bar if any
      
      notify_listener ();
   }
}



/*
==============================================================================
Name : update_clip
Note :
   Based on frame bounds
==============================================================================
*/

void  WidgetScroll::update_clip (double tt, opak::TransitionFunction tf)
{
   const opa::Size & bounds = _frame_container_aptr->get_bounds ();
   
   float big_num = 1e5f;
   
   float left
      = _enable_clip_flag_arr [Border_LEFT]
      ? 0 : - big_num;

   float top
      = _enable_clip_flag_arr [Border_TOP]
      ? 0 : - big_num;
   
   float right
      = _enable_clip_flag_arr [Border_RIGHT]
      ? bounds._width : bounds._width + big_num;

   float bottom
      = _enable_clip_flag_arr [Border_BOTTOM]
      ? bounds._height : bounds._height + big_num;
   
   
   _layer_container_aptr->_clip_origin
      .stop ()
      .push (opa::Point (left, top), tt, tf);
      
   _layer_container_aptr->_clip_size
      .stop ()
      .push (opa::Size (right - left, bottom - top), tt, tf);
   
   // update scroll bar if any
   
   notify_listener ();
}



/*
==============================================================================
Name : notify_listener
==============================================================================
*/

void  WidgetScroll::notify_listener ()
{
   if ((_listener_ptr != 0) && !empty ())
   {
      const opa::Size & bounds = _frame_container_aptr->get_bounds ();
      
      if (_listener_ptr->is_horizontal ())
      {
         _listener_ptr->set_visible_position (
            - _widget_sptr->get_origin ()._x
         );
         
         _listener_ptr->set_visible_length (
            bounds._width
         );
         
         _listener_ptr->set_content_length (
            _content_bounds._width
         );
      }
      else
      {
         _listener_ptr->set_visible_position (
            - _widget_sptr->get_origin ()._y
         );
         
         _listener_ptr->set_visible_length (
            bounds._height
         );

         _listener_ptr->set_content_length (
            _content_bounds._height
         );
      }
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
