/*****************************************************************************

        WidgetList.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53326

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

#include "ohm/opa/WidgetList.h"
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

WidgetList::~WidgetList ()
{
}



/*
==============================================================================
Name : push_item
==============================================================================
*/

WidgetListItemSPtr   WidgetList::push_item ()
{
   WidgetListItemSPtr widget_sptr = push <WidgetListItem> ();
   
   return widget_sptr;
}
   


/*
==============================================================================
Name: invalidate_layout
==============================================================================
*/

void WidgetList::invalidate_layout ()
{
   _metrics_need_update_flag = true;

   WidgetBase::invalidate_layout ();
}



/*
==============================================================================
Name : layout
==============================================================================
*/

bool  WidgetList::layout (const Size & bounds)
{
   bool retrigger_layout_flag = false;
   
   iterator it = begin ();
   const iterator it_end = end ();
   
   float offset_x = -_min_bbox._origin._x;
   
   float y = 0.f;
   float previous_margin = 0.f;
   
   float max_width = 0.f;
   
   for (; it != it_end ; ++it)
   {
      WidgetBase & widget = *it;
      
      Rect sub_min_bbox;
      widget.get_min_bbox (sub_min_bbox);
      
      lang::max_in_place (max_width, sub_min_bbox._size._width);
   }
   
   it = begin ();
   
   for (; it != it_end ; ++it)
   {
      WidgetBase & widget = *it;
      
      Point origin (offset_x, y);
      
      Rect sub_min_bbox;
      widget.get_min_bbox (sub_min_bbox);
      
      Size sub_bounds = sub_min_bbox._size;
      
      Size sub_available_bounds = sub_bounds;
      sub_available_bounds._width = max_width;
      
      retrigger_layout_flag |= widget.layout (sub_bounds);
      
      widget.set_origin (origin);
      
      Margin sub_margin;
      widget.get_margin (sub_margin);
      
      y += sub_bounds._height;
      y += lang::max (previous_margin, sub_margin._top);
      previous_margin = sub_margin._bottom;
   }
   
   return retrigger_layout_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetList::WidgetList ()
:  _metrics_need_update_flag (true)
{
   _frame_aptr = new opal::Frame ("WidgetList::frame");
   _frame_aptr->set_bounds (opa::Size::_zero);
   
   _layer_aptr = new LayerWidget;
   _layer_aptr->set_name ("WidgetList::layer");
   
   _prim_aptr = new opak::RectFilled;
   _prim_aptr->_color = opal::Color::_white;
   _layer_aptr->push_back (*_prim_aptr);
   
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

void  WidgetList::do_get_min_bbox (Rect & min_bbox)
{
   update_metrics ();
   
   min_bbox = _min_bbox;
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetList::do_get_margin (Margin & margin)
{
   margin = opa::Margin::_zero;
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetList::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);

   _layer_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetList::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);

   _prim_aptr->_size.stop ().push (bounds, tt, tf);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetList::do_enable (bool enabled_flag)
{
   _layer_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetList::do_show (bool show_flag)
{
   _layer_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetList::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_metrics
==============================================================================
*/

void  WidgetList::update_metrics ()
{
   if (_metrics_need_update_flag)
   {
      _min_bbox = opa::Rect::_zero;
      
      iterator it = begin ();
      const iterator it_end = end ();
      
      for (; it != it_end ; ++it)
      {
         WidgetBase & widget = *it;
         
         Rect sub_min_bbox;
         widget.get_min_bbox (sub_min_bbox);
         
         _min_bbox._size._height += sub_min_bbox._size._height;
         lang::max_in_place (_min_bbox._size._width, sub_min_bbox._size._width);
      }
      
      _metrics_need_update_flag = false;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
