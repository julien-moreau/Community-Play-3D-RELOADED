/*****************************************************************************

        WidgetProgressBar.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67009

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

#include "ohm/opa/WidgetProgressBar.h"

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

WidgetProgressBar::~WidgetProgressBar ()
{
   try
   {
      _prim_aptr->stop ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name: set_length
   Fix the length of 'this', independent of actual content length.
==============================================================================
*/

void  WidgetProgressBar::set_length (float length)
{
   _length.set (length);
   set_horizontal_weight (0.f);
}



/*
==============================================================================
Name: set_length_auto_parent
   Tell the layout system that the length of 'this' is dependent on the
   actual parent length.
==============================================================================
*/

void  WidgetProgressBar::set_length_auto_parent (float weight)
{
   _length.set_ext ();
   set_horizontal_weight (weight);
}



/*
==============================================================================
Name: get_length
==============================================================================
*/

float WidgetProgressBar::get_length ()
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

bool  WidgetProgressBar::is_length_auto () const
{
   return _length.is_auto ();
}



/*
==============================================================================
Name: set_indeterminate
==============================================================================
*/

void  WidgetProgressBar::set_indeterminate ()
{
   _prim_aptr->_norm_value = -1;
   //_prim_aptr->start ();
}



/*
==============================================================================
Name: set_norm_value
==============================================================================
*/

void  WidgetProgressBar::set_norm_value (float norm_value)
{
   assert (norm_value >= 0.f);
   assert (norm_value <= 1.f);
   
   lang::limit_in_place (norm_value, 0.f, 1.f);
   
   if (get_norm_value () != norm_value)
   {
      //_prim_aptr->start ();
      _prim_aptr->_norm_value.stop ().push (norm_value, 0.125, opak::ease_in_ease_out);
   }
}



/*
==============================================================================
Name: get_norm_value
==============================================================================
*/

float WidgetProgressBar::get_norm_value () const
{
   return _prim_aptr->_norm_value;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: WidgetProgressBar
==============================================================================
*/

WidgetProgressBar::WidgetProgressBar ()
:  _length (*this)
{
   _prim_aptr = new PrimProgressBar;
   
   set_embeddable (*_prim_aptr);
}



/*
==============================================================================
Name: do_get_min_bbox
==============================================================================
*/

void  WidgetProgressBar::do_get_min_bbox (Rect & min_bbox)
{
   if (_length.need_update ())
   {
      update_metrics ();
   }
   
   min_bbox._origin = opa::Point (0, -11);
   min_bbox._size = opa::Size (_length.get (), 12);
}



/*
==============================================================================
Name: do_get_margin
==============================================================================
*/

void  WidgetProgressBar::do_get_margin (Margin & margin)
{
   margin = opa::Margin (5, 5, 5, 5);
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetProgressBar::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetProgressBar::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   const float width = lang::max (0.f, bounds._width);
   
   _prim_aptr->_width.stop ().push (width, tt, tf);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetProgressBar::do_enable (bool enabled_flag)
{
   _prim_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetProgressBar::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
   //_prim_aptr->start ();
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetProgressBar::do_activate (bool activate_flag)
{
   // nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_metrics
==============================================================================
*/

void  WidgetProgressBar::update_metrics ()
{
   if (_length.need_update ())
   {
      _length.update (60.f);  // default value
   }
}




}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

