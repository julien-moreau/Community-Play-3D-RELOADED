/*****************************************************************************

        WidgetScrollBar.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69631

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

#include "ohm/opa/WidgetScrollBar.h"
#include "ohm/opa/WidgetScroll.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: dtor
==============================================================================
*/

WidgetScrollBar::~WidgetScrollBar ()
{
}



/*
==============================================================================
Name: bind
==============================================================================
*/

void  WidgetScrollBar::bind (WidgetScroll & widget)
{
   assert (&widget != 0);
   
   _listener_ptr = &widget;
}



/*
==============================================================================
Name: set_type
==============================================================================
*/

void  WidgetScrollBar::set_type (WidgetScrollBar::Type type)
{
   if (_type != type)
   {
      bool horizontal_flag = (type == Type_HORIZONTAL);
      
      _type = type;
      _prim_aptr->_type = (
         horizontal_flag ? 
         PrimScrollBar::Type_HORIZONTAL : 
         PrimScrollBar::Type_VERTICAL
      );
      
      invalidate_layout ();
   }
}



/*
==============================================================================
Name: set_type
==============================================================================
*/

bool  WidgetScrollBar::is_horizontal () const
{
   return _type == Type_HORIZONTAL;
}



/*
==============================================================================
Name: set_length
   Fix the length of 'this', independent of actual content length.
==============================================================================
*/

void  WidgetScrollBar::set_length (float length)
{
   _length.set (length);
   
   if (_type == Type_HORIZONTAL)
   {
      set_horizontal_weight (0.f);
   }
   else if (_type == Type_VERTICAL)
   {
      set_vertical_weight (0.f);
   }
   else
   {
      assert (false);
   }
}



/*
==============================================================================
Name: set_length_auto_parent
   Tell the layout system that the length of 'this' is dependent on the
   actual parent length.
==============================================================================
*/

void  WidgetScrollBar::set_length_auto_parent (float weight)
{
   _length.set_ext ();

   if (_type == Type_HORIZONTAL)
   {
      set_horizontal_weight (weight);
   }
   else if (_type == Type_VERTICAL)
   {
      set_vertical_weight (weight);
   }
   else
   {
      assert (false);
   }
}



/*
==============================================================================
Name: get_length
==============================================================================
*/

float WidgetScrollBar::get_length ()
{
   if (_length.need_update ())
   {
      update_metrics ();
   }
   
   return _length.get ();
}



/*
==============================================================================
Name: is_length_auto
==============================================================================
*/

bool  WidgetScrollBar::is_length_auto () const
{
   return _length.is_auto ();
}



/*
==============================================================================
Name: set_content_length
==============================================================================
*/

void  WidgetScrollBar::set_content_length (float length)
{
   bool activate_flag = length > _prim_aptr->_visible_length;
   // quick fix
   activate_flag = true;
   
   if (activate_flag != _activated_flag)
   {
      _activated_flag = activate_flag;
      
      invalidate_layout ();
   }
   
   _prim_aptr->_content_length = length;
   _frame_aptr->set_content_length (length);
}



/*
==============================================================================
Name: set_visible_position
==============================================================================
*/

void  WidgetScrollBar::set_visible_position (float position)
{
   _prim_aptr->_visible_position = position;
   _frame_aptr->set_visible_position (position);
}



/*
==============================================================================
Name: set_visible_length
==============================================================================
*/

void  WidgetScrollBar::set_visible_length (float length)
{
   bool activate_flag = length < _prim_aptr->_content_length;
   // quick fix
   activate_flag = true;
   
   if (activate_flag != _activated_flag)
   {
      _activated_flag = activate_flag;
      
      invalidate_layout ();
   }
   
   _prim_aptr->_visible_length = length;
   _frame_aptr->set_visible_length (length);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: WidgetScrollBar
==============================================================================
*/

WidgetScrollBar::WidgetScrollBar ()
:  _type (Type_VERTICAL)
,  _length (*this)
,  _listener_ptr (0)
,  _activated_flag (true) // quick fix
{
   _frame_aptr = new FrameScrollBar (*this);
   
   _prim_aptr = new PrimScrollBar;
   
   set_embeddable (*_frame_aptr);
   set_embeddable (*_prim_aptr);
}



/*
==============================================================================
Name: do_get_min_bbox
==============================================================================
*/

void  WidgetScrollBar::do_get_min_bbox (Rect & min_bbox)
{
   if (_activated_flag)
   {
      if (_length.need_update ())
      {
         update_metrics ();
      }

      if (_type == Type_HORIZONTAL)
      {
         min_bbox._origin = Point (0, 0);
         min_bbox._size = Size (_length.get (), 7);
      }
      else if (_type == Type_VERTICAL)
      {
         min_bbox._origin = Point (0, 0);
         min_bbox._size = Size (7, _length.get ());
      }
      else
      {
         assert (false);
      }
   }
   else
   {
      min_bbox = Rect::_zero;
   }
}



/*
==============================================================================
Name: do_get_margin
==============================================================================
*/

void  WidgetScrollBar::do_get_margin (Margin & margin)
{
   if (_activated_flag)
   {
      margin = Margin (6, 6, 6, 6);
   }
   else
   {
      margin = Margin::_zero;
   }
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetScrollBar::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetScrollBar::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
   _frame_aptr->frame_bounds_changed ();
   
   const float length =
      _type == Type_HORIZONTAL ?
      lang::max (0.f, bounds._width) :
      lang::max (0.f, bounds._height)
   ;
   
   _prim_aptr->_length.stop ().push (length, tt, tf);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetScrollBar::do_enable (bool enabled_flag)
{
   _prim_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetScrollBar::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetScrollBar::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*
==============================================================================
Name: req_set_position
==============================================================================
*/

void WidgetScrollBar::req_set_position (float position)
{
   if (_listener_ptr != 0)
   {
      opa::Point point = _listener_ptr->get_content_position ();
      
      if (is_horizontal ())
      {
         point._x = position;
      }
      else
      {
         point._y = position;
      }
      
      _listener_ptr->req_set_content (point);
   }
   else
   {
      _prim_aptr->_visible_position = position;
   }
   
   

   //_prim_aptr->_value = lang::limit (widget_event._suggested_value, 0.f, 1.f);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_metrics
==============================================================================
*/

void  WidgetScrollBar::update_metrics ()
{
   if (_length.need_update ())
   {
      _length.update (60.f);  // default value
   }
}




}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

