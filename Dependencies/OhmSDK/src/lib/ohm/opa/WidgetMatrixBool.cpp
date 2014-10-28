/*****************************************************************************

        WidgetMatrixBool.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54714

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

#include "ohm/opa/WidgetMatrixBool.h"
#include "ohm/opa/base/WidgetMatrixBoolCell.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/opal/Frame.h"
#include "ohm/lang/fnc.h"

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

WidgetMatrixBool::~WidgetMatrixBool ()
{
}



/*
==============================================================================
Name : handle_event_default
==============================================================================
*/

void  WidgetMatrixBool::handle_event_default (Event & event)
{
   set_val (event._x, event._y, event._suggested_flag);
}



/*
==============================================================================
Name : resize
Note :
   width * height is the number of checkboxes
   (does not account for label row/column headers)
==============================================================================
*/

void  WidgetMatrixBool::resize (size_t width, size_t height)
{
   WidgetSPtrArr old_widget_sptr_arr = _widget_sptr_arr;
   size_t old_width = _width;
   size_t old_height = _height;
   
   _width = width;
   _height = height;
   
   _widget_sptr_arr.clear ();
   _widget_sptr_arr.resize ((_width + 1) * (_height + 1));
   
   for (size_t y = 0 ; y < _height + 1 ; ++y)
   {
      for (size_t x = 0 ; x < _width + 1 ; ++x)
      {
         size_t old_pos = y * (old_width + 1) + x;
         
         bool ok_flag = true;
         ok_flag &= y < old_height + 1;
         ok_flag &= x < old_width + 1;
         ok_flag &= old_pos < old_widget_sptr_arr.size ();
         
         size_t new_pos = y * (_width + 1) + x;
         
         WidgetSPtr & widget_sptr = _widget_sptr_arr [new_pos];
         
         if (ok_flag)
         {
            widget_sptr = old_widget_sptr_arr [old_pos];
         }
         else if ((y > 0) && (x > 0))
         {
            WidgetMatrixBoolCellSPtr widget_mbc_sptr = WidgetMatrixBoolCell::create ();
            
            widget_sptr.assign_dynamic (widget_mbc_sptr);
            
            widget_mbc_sptr->bind <
               WidgetMatrixBool,
               &WidgetMatrixBool::handle_cell_event
            > (*this);
            
            embed (*widget_mbc_sptr);
         }
         
         if ((y > 0) && (x > 0))
         {
            CellPoint pt;
            pt._x = x - 1;
            pt._y = y - 1;
            
            widget_sptr->set_data (pt);
         }
      }
   }
}



/*
==============================================================================
Name : width
==============================================================================
*/

size_t   WidgetMatrixBool::width () const
{
   return _width;
}



/*
==============================================================================
Name : height
==============================================================================
*/

size_t   WidgetMatrixBool::height () const
{
   return _height;
}



/*
==============================================================================
Name : push_back_row
==============================================================================
*/

void  WidgetMatrixBool::push_back_row (const std::string & label)
{
   resize (width (), height () + 1);
   
   set_row_label (height () - 1, label);
}



/*
==============================================================================
Name : push_back_column
==============================================================================
*/

void  WidgetMatrixBool::push_back_column (const std::string & label)
{
   resize (width () + 1, height ());
   
   set_column_label (width () - 1, label);
}



/*
==============================================================================
Name : set_row_label
==============================================================================
*/

void  WidgetMatrixBool::set_row_label (size_t row_index, const std::string & label)
{
   assert (row_index < _height);
   
   WidgetLabelSPtr widget_sptr = WidgetLabel::create ();
   widget_sptr->set_label (label);
   
   size_t pos = (row_index + 1) * (_width + 1);
   
   _widget_sptr_arr [pos].assign_dynamic (widget_sptr);
   
   embed (*widget_sptr);
}



/*
==============================================================================
Name : set_row_label
==============================================================================
*/

void  WidgetMatrixBool::set_column_label (size_t column_index, const std::string & label)
{
   assert (column_index < _width);
   
   WidgetLabelSPtr widget_sptr = WidgetLabel::create ();
   widget_sptr->set_label (label);
   
   size_t pos = column_index + 1;
   
   _widget_sptr_arr [pos].assign_dynamic (widget_sptr);
   
   embed (*widget_sptr, Slot_ROTATED);
}



/*
==============================================================================
Name : set_val
==============================================================================
*/

void  WidgetMatrixBool::set_val (size_t x, size_t y, bool val)
{
   assert (x < _width);
   assert (y < _height);
   
   size_t pos = (y + 1) * (_width + 1) + x + 1;
   
   WidgetMatrixBoolCellSPtr widget_sptr;
   widget_sptr.assign_dynamic (_widget_sptr_arr [pos]);
   assert (widget_sptr.get () != 0);
   
   widget_sptr->set_state (val);
}



/*
==============================================================================
Name : get_val
==============================================================================
*/

bool  WidgetMatrixBool::get_val (size_t x, size_t y) const
{
   assert (x < _width);
   assert (y < _height);
   
   size_t pos = (y + 1) * (_width + 1) + x + 1;
   
   WidgetMatrixBoolCellSPtr widget_sptr;
   widget_sptr.assign_dynamic (_widget_sptr_arr [pos]);
   assert (widget_sptr.get () != 0);
   
   bool state_flag = widget_sptr->get_state ();
   
   return state_flag;
}



/*
==============================================================================
Name: invalidate_layout
==============================================================================
*/

void  WidgetMatrixBool::invalidate_layout ()
{
   _metrics_need_update_flag = true;

   WidgetBase::invalidate_layout ();
}



/*
==============================================================================
Name : layout
==============================================================================
*/

bool  WidgetMatrixBool::layout (const Size & bounds)
{
   _layer_col_label_aptr->_origin._y = _max_col_label_height;
   
   float yp = 0.f;
   float xp = 0.f;
   
   for (size_t y = 0 ; y < _height + 1 ; ++y)
   {
      xp = 0.f;
      
      for (size_t x = 0 ; x < _width + 1 ; ++x)
      {
         if ((x != 0) || (y != 0))
         {
            size_t pos = y * (_width + 1) + x;
            
            WidgetSPtr widget_sptr = _widget_sptr_arr [pos];
            
            opa::Point origin (xp, yp);
            
            if ((x > 0) && (y > 0))
            {
               widget_sptr->set_bounds (opa::Size (16, 16));
            }
            
            if (y == 0)
            {
               origin._x = 0.f;
               origin._y
                  = _max_row_label_width
                  + 6.f
                  + float (x - 1) * 30.f;
               
               widget_sptr->set_bounds (opa::Size (_max_col_label_height, 16));
            }
            
            if (x == 0)
            {
               widget_sptr->set_bounds (opa::Size (_max_row_label_width, 16));
            }
            
            widget_sptr->set_origin (origin);
         }
         
         if (x == 0)
         {
            xp += _max_row_label_width + 6.f;
         }
         else
         {
            xp += 30.f;
         }
      }
      
      if (y == 0)
      {
         yp += _max_col_label_height + 6.f;
      }
      else
      {
         yp += 18.f;
      }
   }
   
   bool retrigger_layout_flag = false;
   
   return retrigger_layout_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetMatrixBool::WidgetMatrixBool ()
:  _width (0)
,  _height (0)
,  _metrics_need_update_flag (true)
{
   _frame_aptr = new opal::Frame ("WidgetMatrixBool::frame");
   
   _layer_aptr = new LayerWidget;
   _layer_aptr->set_name ("WidgetMatrixBool::layer");
   
   _layer_col_label_aptr = new opak::LayerTransform;
   _layer_col_label_aptr->set_name ("WidgetMatrixBool::transform_layer");
   _layer_col_label_aptr->_rotation = -.25f;
   //_layer_col_label_aptr->_origin._y = 50.f;
   _layer_aptr->push_back (*_layer_col_label_aptr);
   
   set_embeddable (*_frame_aptr);
   set_embeddable (*_layer_aptr);
   
   set_nbr_slot (Slot_NBR_ELT);
   set_slot (Slot_NORMAL, *_frame_aptr);
   set_slot (Slot_NORMAL, *_layer_aptr);

   set_slot (Slot_ROTATED, *_layer_col_label_aptr);

   resize (0, 0);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetMatrixBool::do_get_min_bbox (Rect & min_bbox)
{
   update ();
   
   min_bbox = Rect (0, 0, _total_width, _total_height);
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetMatrixBool::do_get_margin (Margin & margin)
{
   margin = Margin::_zero;
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetMatrixBool::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _layer_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetMatrixBool::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetMatrixBool::do_enable (bool enabled_flag)
{
   _layer_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetMatrixBool::do_show (bool show_flag)
{
   _layer_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetMatrixBool::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*
==============================================================================
Name : handle_cell_event
==============================================================================
*/

void  WidgetMatrixBool::handle_cell_event (WidgetMatrixBoolCell::Event & event)
{
   WidgetMatrixBoolCell & widget = event._widget;
   
   CellPoint pt = widget.get_data <CellPoint> ();
   
   Event widget_event (*this);
   widget_event._x = pt._x;
   widget_event._y = pt._y;
   widget_event._cur_flag = event._cur_state_flag;
   widget_event._suggested_flag = event._suggested_state_flag;
   
   _delegate.param_ref_signal (widget_event);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update
==============================================================================
*/

void  WidgetMatrixBool::update ()
{
   if (_metrics_need_update_flag)
   {
      _max_row_label_width = 0.f;
   
      for (size_t y = 0 ; y < _height ; ++y)
      {
         size_t pos = (y + 1) * (_width + 1);
         
         WidgetSPtr widget_sptr = _widget_sptr_arr [pos];
         
         Rect sub_min_bbox;
         widget_sptr->get_min_bbox (sub_min_bbox);
         
         float sub_width = sub_min_bbox._size._width;
         
         lang::max_in_place (_max_row_label_width, sub_width);
      }
      
      _total_width
         = _max_row_label_width
         + 6.f
         + 30.f * float (_width);
      
      _max_col_label_height = 0.f;
      
      for (size_t x = 0 ; x < _width ; ++x)
      {
         size_t pos = x + 1;
         
         WidgetSPtr widget_sptr = _widget_sptr_arr [pos];
         
         Rect sub_min_bbox;
         widget_sptr->get_min_bbox (sub_min_bbox);
         
         // take width (not height)
         float sub_width = sub_min_bbox._size._width;
         
         lang::max_in_place (_max_col_label_height, sub_width);
      }
      
      _total_height
         = _max_col_label_height
         + 6.f
         + 18.f * float (_height);
      
      _metrics_need_update_flag = true;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
