/*****************************************************************************

        WidgetListItem.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54611

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

#include "ohm/opa/base/WidgetListItem.h"
#include "ohm/opal/Frame.h"
#include "ohm/opal/private/TypeSetter.h"
#include "ohm/opa/prim/PrimListItem.h"
#include "ohm/opal/FontManager.h"

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

WidgetListItem::~WidgetListItem ()
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
Name : set_label
==============================================================================
*/

void  WidgetListItem::set_label (const std::string & label)
{
   if (_label  != label)
   {
      _prim_aptr->_label = label;
      
      _scripture.begin_edit ();
      _scripture.clear ();
      _scripture.insert (0, label);
      _scripture.end_edit ();
      
      opal::Typesetter typesetter (_scripture);
      
      size_t end_pos = 0;
      float height = 0.f;
      typesetter.suggest_line_break (end_pos, height, 0, 100000.f);
      
      opal::Scripture::Range range;
      range._start = 0;
      range._end = end_pos;

      const opal::Typesetter::LineInfo * line_info_ptr
         = typesetter.layout_line (range, 100000.f, false, 0);
      
      _width = line_info_ptr->_width;
   }
}



/*
==============================================================================
Name : get_label
==============================================================================
*/

const std::string &  WidgetListItem::get_label () const
{
   return _label;
}



/*
==============================================================================
Name : select
==============================================================================
*/

void  WidgetListItem::select (bool select_flag)
{
   _prim_aptr->_selected_flag = select_flag;
}
   


/*
==============================================================================
Name : is_selected
==============================================================================
*/

bool  WidgetListItem::is_selected () const
{
   return _prim_aptr->_selected_flag;
}



/*
==============================================================================
Name : handle_event_default
==============================================================================
*/

void  WidgetListItem::handle_event_default (Event & event)
{
   if (event._toggle_flag)
   {
      select (!is_selected ());
   }
   else
   {
      select ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetListItem::WidgetListItem ()
{
   _scripture.push_state_font (
      opal::FontManager::use_instance ().get_font (Constants::_default_font_0, 11.f)
   );

   _frame_aptr = new opal::FrameSignal;
   _frame_aptr->bind_gesture_signal <
      WidgetListItem,
      &WidgetListItem::req_gesture_signal
   > (*this);
   
   _prim_aptr = new PrimListItem;
   _prim_aptr->set_name ("PrimListItem");
   
   set_embeddable (*_frame_aptr);
   set_embeddable (*_prim_aptr);
   
   bind <
      WidgetListItem,
      &WidgetListItem::handle_event_default
   > (*this);
}



/*
==============================================================================
Name : req_gesture_signal
==============================================================================
*/

void  WidgetListItem::req_gesture_signal (opal::FrameSignal::Event & event)
{
   assert (event._event_ptr != 0);
   opal::Event & opal_event = *event._event_ptr;

   Event widget_event (*this);
   
   widget_event._toggle_flag = opal_event._key.has_shift_modifier ();
   
   _delegate.param_ref_signal (widget_event);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetListItem::do_get_min_bbox (Rect & min_bbox)
{
   min_bbox = Rect (0, 0, 8.f + _width + 8.f, 16.f);
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetListItem::do_get_margin (Margin & margin)
{
   margin = Margin::_zero;
}



/*
==============================================================================
Name : do_set_origin
==============================================================================
*/

void  WidgetListItem::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name : do_set_bounds
==============================================================================
*/

void  WidgetListItem::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
   
   _prim_aptr->_size.stop ().push (bounds, tt, tf);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetListItem::do_enable (bool enabled_flag)
{
   _prim_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetListItem::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetListItem::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
