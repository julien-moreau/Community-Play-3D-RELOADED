/*****************************************************************************

        WidgetFader.cpp
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 62138

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

#include "ohm/math/fnc.h"
#include "ohm/opa/WidgetFader.h"

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

WidgetFader::~WidgetFader ()
{
}



/*
==============================================================================
Name: set_type
==============================================================================
*/

void  WidgetFader::set_type (WidgetFader::Type type)
{
   if (_type != type)
   {
      bool horizontal_flag = (type == Type_HORIZONTAL);
      
      _type = type;
      _prim_aptr->_type = (
         horizontal_flag ? 
         PrimFader::Type_HORIZONTAL : 
         PrimFader::Type_VERTICAL
      );
      _frame_aptr->set_horizontal (horizontal_flag);
      
      invalidate_layout ();
   }
}



/*
==============================================================================
Name: set_length
   Fix the length of 'this', independent of actual content length.
==============================================================================
*/

void  WidgetFader::set_length (float length)
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

void  WidgetFader::set_length_auto_parent (float weight)
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

float WidgetFader::get_length ()
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

bool  WidgetFader::is_length_auto () const
{
   return _length.is_auto ();
}



/*
==============================================================================
Name: set_range
==============================================================================
*/

void  WidgetFader::set_range (float val_min, float val_max)
{
   assert (val_min < val_max);
   
   _prim_aptr->_value_min = val_min;
   _prim_aptr->_value_max = val_max;
   
   _range.first = val_min;
   _range.second = val_max;
}



/*
==============================================================================
Name: get_range
==============================================================================
*/

const std::pair <float, float> & WidgetFader::get_range () const
{
   return _range;
}



/*
==============================================================================
Name: set_discrete
==============================================================================
*/

void  WidgetFader::set_discrete (bool discrete_flag)
{
   _discrete_flag = discrete_flag;

   _prim_aptr->_discrete_flag = discrete_flag;
}



/*
==============================================================================
Name: is_discrete
==============================================================================
*/

bool  WidgetFader::is_discrete () const
{
   return _discrete_flag;
}



/*
==============================================================================
Name: set_value
==============================================================================
*/

void  WidgetFader::set_value (float value)
{
   // accept small imprecision of 'value' outside range
   
   lang::limit_in_place (value, _range.first, _range.second);
   
   _prim_aptr->_value = value;
}



/*
==============================================================================
Name: get_value
==============================================================================
*/

float WidgetFader::get_value () const
{
   return _prim_aptr->_value;
}


   
/*
==============================================================================
Name: set_norm_value
==============================================================================
*/

void  WidgetFader::set_norm_value (float norm_value)
{
   // accept small imprecision of 'norm_value' outside range
   
   set_value (_range.first + norm_value * (_range.second - _range.first));
}



/*
==============================================================================
Name: get_norm_value
==============================================================================
*/

float WidgetFader::get_norm_value () const
{
   assert (_range.second > _range.first);
   
   float value = get_value ();
   
   return (value - _range.first) / (_range.second - _range.first);
}



/*
==============================================================================
Name: is_in_gesture
Description:
   Returns true iff a gesture (edition) is ongoing.
==============================================================================
*/

bool  WidgetFader::is_in_gesture ()
{
   return _editing_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: WidgetFader
==============================================================================
*/

WidgetFader::WidgetFader ()
:  _type (Type_HORIZONTAL)
,  _length (*this)
,  _range (std::make_pair (0.f, 1.f))
,  _discrete_flag (false)
,  _editing_flag (false)
{
   _frame_aptr = new FrameFader (*this);
   
   _prim_aptr = new PrimFader;
   
   set_embeddable (*_frame_aptr);
   set_embeddable (*_prim_aptr);
}



/*
==============================================================================
Name: do_get_min_bbox
==============================================================================
*/

void  WidgetFader::do_get_min_bbox (Rect & min_bbox)
{
   if (_length.need_update ())
   {
      update_metrics ();
   }

   if (_type == Type_HORIZONTAL)
   {
      min_bbox._origin = opa::Point (0, -10);
      min_bbox._size = opa::Size (_length.get () + 10, 12);
   }
   else if (_type == Type_VERTICAL)
   {
      min_bbox._origin = opa::Point (-6, 0);
      min_bbox._size = opa::Size (12, _length.get () + 10);
   }
   else
   {
      assert (false);
   }
}



/*
==============================================================================
Name: do_get_margin
==============================================================================
*/

void  WidgetFader::do_get_margin (Margin & margin)
{
   margin = opa::Margin (10, 5, 10, 5);
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetFader::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetFader::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);

   const float course_len =
      _type == Type_HORIZONTAL ?
      lang::max (0.f, bounds._width) :
      lang::max (0.f, bounds._height)
   ;
   
   _prim_aptr->_course_len.stop ().push (course_len, tt, tf);

}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetFader::do_enable (bool enabled_flag)
{
   _prim_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetFader::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetFader::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*
==============================================================================
Name: req_set_value_delta
==============================================================================
*/

void WidgetFader::req_set_value (float value, Event::Type type)
{
   assert (value >= 0.f);
   assert (value <= 1.f);
   
   value = _range.first + value * (_range.second - _range.first);
   
   if (_discrete_flag)
   {
      value = float (math::round (value));
   }
   
   if (type == Event::Type_BEGIN)
   {
      _editing_flag = true;
      _begin_value = value;
   }
   else if (type == Event::Type_COMMIT)
   {
      _editing_flag = false;
   }
   else if (type == Event::Type_CANCEL)
   {
      _editing_flag = false;
      value = _begin_value;
   }
   else if (type == Event::Type_RESET)
   {
      value = _begin_value;
   }
   
   Event widget_event (*this, type);
   
   widget_event._begin_value = _begin_value;
   widget_event._previous_value = _prim_aptr->_value;
   widget_event._suggested_value = value;
   
   _delegate.param_ref_signal (widget_event);
   
   if (_discrete_flag)
   {
      if (_prim_aptr->_value != value)
      {
         _prim_aptr->_value
            .stop ()
            .push (widget_event._suggested_value, 0.125, opak::ease_in_ease_out);
      }
   }
   else
   {
      _prim_aptr->_value = widget_event._suggested_value;
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_metrics
==============================================================================
*/

void  WidgetFader::update_metrics ()
{
   if (_length.need_update ())
   {
      _length.update (60.f);  // default value
   }
}




}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

