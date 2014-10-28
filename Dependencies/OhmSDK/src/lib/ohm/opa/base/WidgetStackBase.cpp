/*****************************************************************************

        WidgetStackBase.cpp
        Copyright (c) 2011 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 59722

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

#include "ohm/opa/BundleView.h"
#include "ohm/opa/base/WidgetBox.h"
#include "ohm/opa/base/WidgetStackBase.h"
#include "ohm/opak/base/PrimitiveContainer.h"
#include "ohm/opak/base/Sizable.h"
#include "ohm/opal/Frame.h"
#include "ohm/opa/private/FrameDialogBack.h"

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

WidgetStackBase::~WidgetStackBase ()
{
   try
   {
      clear ();
      
      unset_all ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : set_width
Description :
   Fix the width of 'this', independent of actual content width.
==============================================================================
*/

void  WidgetStackBase::set_width (float width)
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
==============================================================================
*/

void  WidgetStackBase::set_width_auto_content ()
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

void  WidgetStackBase::set_width_auto_parent (float weight)
{
   _width.set_ext ();
   set_horizontal_weight (weight);
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

float WidgetStackBase::get_width ()
{
   float ret_val = 0.f;
      
   if (_width.is_ext ())
   {
      WidgetBase * widget_ptr = get_ancestor_ptr <WidgetBase> ();
      assert (widget_ptr != 0);
      
      if (widget_ptr != 0)
      {
         ret_val = widget_ptr->get_bounds ()._width;
      }
   }
   else
   {
      if (_width.need_update ())
      {
         update_metrics ();
      }
      
      ret_val = _width.get ();
   }
   
   return ret_val;
}



/*
==============================================================================
Name : set_height
Description :
   Fix the height of 'this', independent of actual content height.
==============================================================================
*/

void  WidgetStackBase::set_height (float height)
{
   _height.set (height);
   set_vertical_weight (0.f);
}



/*
==============================================================================
Name : set_height_auto_content
Description :
   Tell the layout system that the height of 'this' is dependent on the
   actual content height.
==============================================================================
*/

void  WidgetStackBase::set_height_auto_content ()
{
   _height.set_auto ();
   set_vertical_weight (0.f);
}



/*
==============================================================================
Name : set_height_auto_parent
Description :
   Tell the layout system that the height of 'this' is dependent on the
   actual parent height.
==============================================================================
*/

void  WidgetStackBase::set_height_auto_parent (float weight)
{
   _height.set_ext ();
   set_vertical_weight (weight);
}



/*
==============================================================================
Name : get_height
==============================================================================
*/

float WidgetStackBase::get_height ()
{
   float ret_val = 0.f;
      
   if (_height.is_ext ())
   {
      WidgetBase * widget_ptr = get_ancestor_ptr <WidgetBase> ();
      assert (widget_ptr != 0);
      
      if (widget_ptr != 0)
      {
         ret_val = widget_ptr->get_bounds ()._height;
      }
   }
   else
   {
      if (_height.need_update ())
      {
         update_metrics ();
      }
      
      ret_val = _height.get ();
   }
   
   return ret_val;
}



/*
==============================================================================
Name: invalidate_layout
==============================================================================
*/

void WidgetStackBase::invalidate_layout ()
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

bool  WidgetStackBase::layout (const Size & bounds)
{
   /*assert (!_width.need_update ());
   assert (!_height.need_update ());*/
   
   bool retrigger_layout_flag = WidgetBase::layout (bounds);
   
   synchronize ();
   
   iterator it = begin ();
   const iterator it_end = end ();
   
   Rect min_bbox;
   get_min_bbox (min_bbox);
   
   if (get_orientation () == Orientation_HORIZONTAL)
   {
      float dsize = bounds._width - min_bbox._size._width;
      
      float x = 0.f;
      float previous_margin = 0.f;
      
      bool first_flag = true;
   
      for (; it != it_end ; ++it)
      {
         WidgetBase & widget = *it;
         
         if (!widget.is_visible ())
         {
            // skip
            
            continue;
         }
         
         Rect sub_min_bbox;
         widget.get_min_bbox (sub_min_bbox);
         
         Margin sub_margin;
         widget.get_margin (sub_margin);
         
         if (!first_flag)
         {
            x += lang::max (previous_margin, sub_margin._left);
         }

         Point origin (x, sub_min_bbox._origin._y - min_bbox._origin._y);
         
         Size sub_bounds = sub_min_bbox._size;
         
         if (_total_sub_weight > 0.f)
         {
            float sub_weight = widget.get_horizontal_weight ();
            
            sub_bounds._width += floor (dsize * sub_weight / _total_sub_weight);
         }
         
         if (widget.get_vertical_weight () > 0)
         {
            sub_bounds._height = bounds._height;
         }
         
         retrigger_layout_flag |= widget.layout (sub_bounds);
         
         widget.set_origin (origin, get_transition_time (), get_transition_function ());
         
         x += sub_bounds._width;
         previous_margin = sub_margin._right;
         first_flag = false;
      }
   }
   else if (get_orientation () == Orientation_VERTICAL)
   {
      float dsize = bounds._height - min_bbox._size._height;
      
      float y = 0.f;
      float previous_margin = 0.f;
      
      bool first_flag = true;
      
      for (; it != it_end ; ++it)
      {
         WidgetBase & widget = *it;
         
         if (!widget.is_visible ())
         {
            // skip
            
            continue;
         }
         
         Rect sub_min_bbox;
         widget.get_min_bbox (sub_min_bbox);

         Margin sub_margin;
         widget.get_margin (sub_margin);
         
         if (!first_flag)
         {
            y += lang::max (previous_margin, sub_margin._top);
         }

         Point origin (sub_min_bbox._origin._x - min_bbox._origin._x, y);
         
         Size sub_bounds = sub_min_bbox._size;
         
         if (_total_sub_weight > 0.f)
         {
            float sub_weight = widget.get_vertical_weight ();
            
            sub_bounds._height += floor (dsize * sub_weight / _total_sub_weight);
         }
         
         if (widget.get_horizontal_weight () > 0)
         {
            sub_bounds._width = bounds._width;
         }
         
         retrigger_layout_flag |= widget.layout (sub_bounds);
         
         widget.set_origin (origin, get_transition_time (), get_transition_function ());
         
         y += sub_bounds._height;
         previous_margin = sub_margin._bottom;
         first_flag = false;
      }
   }
   else
   {
      assert (false);
   }
   
   if (retrigger_layout_flag)
   {
      _width.invalidate ();
      _height.invalidate ();
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

void  WidgetStackBase::spread_view (const Rect & rect)
{
   iterator it = begin ();
   const iterator it_end = end ();
   
   for (; it != it_end ; ++it)
   {
      Rect sub_rect = rect;
      
      WidgetBase & sub_widget = *it;
      
      sub_rect -= sub_widget.get_origin ();
      
      sub_widget.spread_view (sub_rect);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetStackBase::WidgetStackBase ()
:  _frame_aptr ()
,  _layer_aptr ()
,  _width (*this)
,  _height (*this)
,  _min_bbox_origin ()
,  _margin ()
,  _total_sub_weight (0.f)
{
   _frame_aptr = new opal::Frame ("WidgetStackBase::frame");
   
   _layer_aptr = new LayerWidget;
   _layer_aptr->set_name ("WidgetStackBase::layer");
   
   set_embeddable (*_frame_aptr);
   set_embeddable (*_layer_aptr);
   
   set_nbr_slot (1);

   set_slot (0, *_frame_aptr);
   set_slot (0, *_layer_aptr);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetStackBase::do_get_min_bbox (Rect & min_bbox)
{
   synchronize ();
   
   if (_width.need_update () || _height.need_update ())
   {
      update_metrics ();
   }
   
   min_bbox = Rect (
      _min_bbox_origin,
      Size (_width.get (), _height.get ())
   );
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetStackBase::do_get_margin (Margin & margin)
{
   synchronize ();
   
   if (_width.need_update () || _height.need_update ())
   {
      update_metrics ();
   }
   
   margin = _margin;
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetStackBase::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _layer_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetStackBase::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetStackBase::do_enable (bool enabled_flag)
{
   _layer_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetStackBase::do_show (bool show_flag)
{
   _layer_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetStackBase::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_metrics
==============================================================================
*/

void  WidgetStackBase::update_metrics ()
{
   if (_width.need_update () || _height.need_update ())
   {
      WidgetBox box (get_orientation ());
      
      box.begin ();
      
      _min_bbox_origin = Point::_zero;
      _margin = Margin::_zero;
      _total_sub_weight = 0.f;
      
      iterator it = begin ();
      const iterator it_end = end ();
      
      for (; it != it_end ; ++it)
      {
         WidgetBase & widget = *it;
         
         if (!widget.is_visible ())
         {
            // skip
            
            continue;
         }
         
         Rect sub_min_bbox;
         widget.get_min_bbox (sub_min_bbox);

         Margin sub_margin;
         widget.get_margin (sub_margin);
         
         float sub_weight = 0.f;
         
         if (get_orientation () == Orientation_HORIZONTAL)
         {
            sub_weight = widget.get_horizontal_weight ();
         }
         else if (get_orientation () == Orientation_VERTICAL)
         {
            sub_weight = widget.get_vertical_weight ();
         }
         
         _total_sub_weight += sub_weight;
         
         box.push (sub_min_bbox, sub_margin);
      }
      
      box.end ();
      
      const Rect & bbox = box.get_rect ();
      
      _min_bbox_origin = bbox._origin;
      
      if (_width.need_update ())
      {
         _width.update (bbox._size._width);
      }

      if (_height.need_update ())
      {
         _height.update (bbox._size._height);
      }
      
      _margin = box.get_margin ();
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
