/*****************************************************************************

        WidgetPushButton.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54712

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

#include "ohm/opa/WidgetPushButton.h"
#include "ohm/opa/WidgetLabel.h"
#include "ohm/opa/prim/PrimPushButton.h"
#include "ohm/opal/base/FrameSignal.h"

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

WidgetPushButton::~WidgetPushButton ()
{
}



/*
==============================================================================
Name : set_width
==============================================================================
*/

void  WidgetPushButton::set_width (float width)
{
   // not enough room to display any label !
   assert (width > _inner_hori_margin * 2.f);
   
   if (
      (_widget_button_sptr->get_width () != width)
      || (_widget_label_sptr->is_width_auto () != false)
      )
   {
      _widget_button_sptr->set_width (width);
      _widget_label_sptr->set_width (width - _inner_hori_margin * 2.f);
      set_horizontal_weight (0.f);
   }
}



/*
==============================================================================
Name : set_width_auto_content
==============================================================================
*/

void  WidgetPushButton::set_width_auto_content ()
{
   _widget_label_sptr->set_width_auto_content ();
   set_horizontal_weight (0.f);
}



/*
==============================================================================
Name : set_width_auto_parent
==============================================================================
*/

void  WidgetPushButton::set_width_auto_parent (float weight)
{
   _widget_label_sptr->set_width_auto_parent (1.f);
   set_horizontal_weight (weight);
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

float WidgetPushButton::get_width ()
{
   return _widget_label_sptr->get_width () + _inner_hori_margin * 2.f;
}



/*
==============================================================================
Name : set_label
==============================================================================
*/

void  WidgetPushButton::set_label (const std::string & label)
{
   if (_widget_label_sptr->get_label () != label)
   {
      _widget_label_sptr->set_label (label);
   }
}



/*
==============================================================================
Name : get_label
==============================================================================
*/

const std::string &  WidgetPushButton::get_label () const
{
   return _widget_label_sptr->get_label ();
}



/*
==============================================================================
Name : add_key_bind
==============================================================================
*/

void  WidgetPushButton::add_key_bind (const opal::Key & key)
{
   _widget_button_sptr->add_key_bind (key);
}



/*
==============================================================================
Name : clear_key_binds
==============================================================================
*/

void  WidgetPushButton::clear_key_binds ()
{
   _widget_button_sptr->clear_key_binds ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetPushButton::WidgetPushButton ()
:  _frame_aptr ()
,  _layer_aptr ()
,  _delegate ()
{
   _frame_aptr = new opal::Frame ("WidgetPushButton::frame");
   _frame_aptr->set_bounds (opa::Size (100000.f, 100000.f));
   set_embeddable (*_frame_aptr);
   
   _layer_aptr = new LayerWidget;
   set_embeddable (*_layer_aptr);
   
   _layer_aptr->_label_style = AspectWidgetLabel::Style_BUTTON;
   
   set_nbr_slot (1);
   set_slot (0, *_frame_aptr);
   set_slot (0, *_layer_aptr);

   _widget_button_sptr = WidgetPushButtonBase::create ();
   embed (*_widget_button_sptr);

   _widget_label_sptr = WidgetLabel::create ();
   embed (*_widget_label_sptr);
   
   _widget_button_sptr->bind <
      WidgetPushButton,
      &WidgetPushButton::handle_base_event
   > (*this);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetPushButton::do_get_min_bbox (Rect & min_bbox)
{
   if (_widget_label_sptr->is_width_auto ())
   {
      opa::Rect label_min_bbox;
      _widget_label_sptr->get_min_bbox (label_min_bbox);
         
      opa::Rect button_min_bbox;
      _widget_button_sptr->get_min_bbox (button_min_bbox);
      
      min_bbox = button_min_bbox;
      min_bbox._size._width = label_min_bbox._size._width + _inner_hori_margin * 2.f;
   }
   else
   {
      _widget_button_sptr->get_min_bbox (min_bbox);
   }
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetPushButton::do_get_margin (Margin & margin)
{
   _widget_button_sptr->get_margin (margin);
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void  WidgetPushButton::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _layer_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void  WidgetPushButton::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
   
   _widget_button_sptr->set_bounds (bounds, tt, tf);
   
   opa::Rect label_min_bbox;
   _widget_label_sptr->get_min_bbox (label_min_bbox);
   
   opa::Rect button_min_bbox;
   _widget_button_sptr->get_min_bbox (button_min_bbox);

   _label_origin._y = floor (
      (button_min_bbox._size._height - label_min_bbox._size._height) * .5f
   );
   
   if (_widget_label_sptr->is_width_auto ())
   {
      _label_origin._x = _inner_hori_margin;
   }
   else
   {
      _label_origin._x = floor (
         (button_min_bbox._size._width - label_min_bbox._size._width) * .5f
      );
   }
   
   _widget_label_sptr->set_origin (_label_origin, tt, tf);
   _widget_label_sptr->set_bounds (bounds - opa::Size (_inner_hori_margin * 2.f, 0.f));
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetPushButton::do_enable (bool enabled_flag)
{
   _layer_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetPushButton::do_show (bool show_flag)
{
   _layer_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetPushButton::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*
==============================================================================
Name : handle_base_event
==============================================================================
*/

void  WidgetPushButton::handle_base_event (WidgetPushButtonBase::Event & event)
{
   switch (event._type)
   {
   case WidgetPushButtonBase::Event::Type_SIGNAL:
      {
         Event widget_event (*this, event._event);
         
         _delegate.param_ref_signal (widget_event);
      }
      break;

   case WidgetPushButtonBase::Event::Type_OFF:
   case WidgetPushButtonBase::Event::Type_OVER:
      _widget_label_sptr->set_origin (_label_origin);
      break;

   case WidgetPushButtonBase::Event::Type_ON:
      _widget_label_sptr->set_origin (_label_origin + opa::Point (0, 1));
      break;
   }
}



float WidgetPushButton::_inner_hori_margin = 14.f;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
