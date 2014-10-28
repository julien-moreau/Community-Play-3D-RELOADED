/*****************************************************************************

        WidgetPushButtonHelp.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70101

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

#include "ohm/opa/WidgetPushButtonHelp.h"
#include "ohm/opal/base/FrameSignal.h"
#include "ohm/opal/base/FrameKey.h"

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

WidgetPushButtonHelp::~WidgetPushButtonHelp ()
{
   try
   {
      // explicitely call frame reset :
      // if an edit is ungoing, this is going to release
      // the mouse capture which in turn will send an
      // event to this widget - therefore we want primitives
      // and delegate to still exist.
      
      _frame_aptr.reset ();
   }
   catch (...)
   {
   }
}



/*
==============================================================================
Name : add_key_bind
==============================================================================
*/

void  WidgetPushButtonHelp::add_key_bind (const opal::Key & key)
{
   _frame_key_aptr->add_key (key);
}



/*
==============================================================================
Name : clear_key_binds
==============================================================================
*/

void  WidgetPushButtonHelp::clear_key_binds ()
{
   _frame_key_aptr->clear_keys ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetPushButtonHelp::WidgetPushButtonHelp ()
:  _frame_aptr ()
,  _prim_aptr ()
,  _width (0.f)
,  _delegate ()
{
   _frame_aptr = new opal::FrameSignal;
   
   _frame_aptr->bind_rollover <
      WidgetPushButtonHelp,
      &WidgetPushButtonHelp::req_rollover_begin,
      &WidgetPushButtonHelp::req_rollover_end
   > (*this);

   _frame_aptr->bind_gesture_trip <
      WidgetPushButtonHelp,
      &WidgetPushButtonHelp::req_gesture_entered,
      &WidgetPushButtonHelp::req_gesture_exited
   > (*this);

   _frame_aptr->bind_gesture_signal <
      WidgetPushButtonHelp, &WidgetPushButtonHelp::req_gesture_signal
   > (*this);
   
   set_embeddable (*_frame_aptr);
   
   _prim_aptr = new opak::Image;
   _prim_aptr->set_name ("PrimPushButtonHelp");
   _prim_aptr->_image = "ohm.widget.pushbutton.help.off.png";

   set_embeddable (*_prim_aptr);
   
   _frame_key_aptr = new opal::FrameKey;
   _frame_key_aptr->bind <
      WidgetPushButtonHelp,
      &WidgetPushButtonHelp::req_key
   > (*this);

   _frame_aptr->insert_top (*_frame_key_aptr);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetPushButtonHelp::do_get_min_bbox (Rect & min_bbox)
{
   min_bbox = opa::Rect (0, -13, 18, 18);
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetPushButtonHelp::do_get_margin (Margin & margin)
{
   margin = opa::Margin (6, 6, 6, 6);
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void  WidgetPushButtonHelp::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void  WidgetPushButtonHelp::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetPushButtonHelp::do_enable (bool enabled_flag)
{
   _prim_aptr->_opacity = enabled_flag ? 1.f : .30f;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetPushButtonHelp::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetPushButtonHelp::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: req_rollover_begin
==============================================================================
*/

void WidgetPushButtonHelp::req_rollover_begin (opal::FrameSignal::Event & event)
{
   _prim_aptr->_image = "ohm.widget.pushbutton.help.over.png";
}



/*
==============================================================================
Name: req_rollover_end
==============================================================================
*/

void WidgetPushButtonHelp::req_rollover_end (opal::FrameSignal::Event & event)
{
   _prim_aptr->_image = "ohm.widget.pushbutton.help.off.png";
}



/*
==============================================================================
Name : req_gesture_entered
==============================================================================
*/

void  WidgetPushButtonHelp::req_gesture_entered (opal::FrameSignal::Event & event)
{
   _prim_aptr->_image = "ohm.widget.pushbutton.help.on.png";
}



/*
==============================================================================
Name : req_gesture_exited
==============================================================================
*/

void  WidgetPushButtonHelp::req_gesture_exited (opal::FrameSignal::Event & event)
{
   _prim_aptr->_image = "ohm.widget.pushbutton.help.off.png";
}



/*
==============================================================================
Name : req_gesture_signal
==============================================================================
*/

void  WidgetPushButtonHelp::req_gesture_signal (opal::FrameSignal::Event & event)
{
   assert (event._event_ptr != 0);
   
   Event widget_event (*this, *event._event_ptr);
   
   _delegate.param_ref_signal (widget_event);
}



/*
==============================================================================
Name : req_gesture_signal
==============================================================================
*/

void  WidgetPushButtonHelp::req_key (opal::FrameKey::Event & event)
{
   assert (event._event_ptr != 0);
   
   Event widget_event (*this, *event._event_ptr);
   
   _delegate.param_ref_signal (widget_event);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
