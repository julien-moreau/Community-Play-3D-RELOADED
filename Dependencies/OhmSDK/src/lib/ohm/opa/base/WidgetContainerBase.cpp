/*****************************************************************************

        WidgetContainerBase.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58576

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

#include "ohm/opa/base/WidgetContainerBase.h"

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

WidgetContainerBase::~WidgetContainerBase ()
{
}



/*
==============================================================================
Name : unset
==============================================================================
*/

void  WidgetContainerBase::unset ()
{
   _widget_sptr.destroy ();
}



/*
==============================================================================
Name: get_horizontal_weight
==============================================================================
*/

float WidgetContainerBase::get_horizontal_weight () const
{
   float weight = 0.f;
   
   if (_widget_sptr.get () != 0)
   {
      weight = _widget_sptr->get_horizontal_weight ();
   }
   
   return weight;
}



/*
==============================================================================
Name: get_vertical_weight
==============================================================================
*/

float WidgetContainerBase::get_vertical_weight () const
{
   float weight = 0.f;
   
   if (_widget_sptr.get () != 0)
   {
      weight = _widget_sptr->get_vertical_weight ();
   }
   
   return weight;
}



/*
==============================================================================
Name : layout
==============================================================================
*/

bool  WidgetContainerBase::layout (const Size & bounds)
{
   bool retrigger_layout_flag = WidgetBase::layout (bounds);
   
   if (_widget_sptr.get () != 0)
   {
      retrigger_layout_flag |= _widget_sptr->layout (bounds);
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

void  WidgetContainerBase::spread_view (const Rect & rect)
{
   if (_widget_sptr.get () != 0)
   {
      _widget_sptr->spread_view (rect);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetContainerBase::WidgetContainerBase ()
{
   _frame_aptr = new opal::Frame ("WidgetContainerBase::frame");
   _frame_aptr->set_bounds (opa::Size (100000.f, 100000.f));
   _frame_aptr->set_no_clip ();
   set_embeddable (*_frame_aptr);
   
   _layer_aptr = new LayerWidget;
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

void  WidgetContainerBase::do_get_min_bbox (Rect & min_bbox)
{
   if (_widget_sptr.get () != 0)
   {
      _widget_sptr->get_min_bbox (min_bbox);
   }
   else
   {
      min_bbox = opa::Rect::_zero;
   }
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetContainerBase::do_get_margin (Margin & margin)
{
   if (_widget_sptr.get () != 0)
   {
      _widget_sptr->get_margin (margin);
   }
   else
   {
      margin = opa::Margin::_zero;
   }
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void  WidgetContainerBase::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _layer_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void  WidgetContainerBase::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   if (_widget_sptr.get () != 0)
   {
      _widget_sptr->set_bounds (bounds, tt, tf);
   }
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetContainerBase::do_enable (bool enabled_flag)
{
   _layer_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetContainerBase::do_show (bool show_flag)
{
   _layer_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetContainerBase::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
