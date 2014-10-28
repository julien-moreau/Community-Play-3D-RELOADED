/*****************************************************************************

        WidgetMenuItem.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69920

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

#include "ohm/opa/WidgetMenuItem.h"
#include "ohm/opa/BundleView.h"
#include "ohm/opa/WidgetMenuRoot.h"
#include "ohm/opa/frame/FrameMenuItem.h"
#include "ohm/opa/prim/PrimMenuItem.h"
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

WidgetMenuItem::~WidgetMenuItem ()
{
   try
   {
      // explicitely call frame reset :
      // if an edit is ungoing, this is going to release
      // the mouse capture which in turn will send an
      // event to this widget - therefore we want primitives
      // and delegate to still exist.
      
      _frame_aptr.reset ();
      
      // keep the primitive alive to delay primitive removal
      _prim_aptr->keep_alive (.25f);
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

void  WidgetMenuItem::set_label (const std::string & label)
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

const std::string &  WidgetMenuItem::get_label () const
{
   return _prim_aptr->_label;
}



/*
==============================================================================
Name : set_section
==============================================================================
*/

void  WidgetMenuItem::set_section ()
{
   _prim_aptr->_section_flag = true;
   
   enable (false);
}



/*
==============================================================================
Name : mark
==============================================================================
*/

void  WidgetMenuItem::mark (bool mark_flag, bool multi_flag)
{
   if (mark_flag)
   {
      if (multi_flag)
      {
         _prim_aptr->_mark = PrimMenuItem::MarkType_CHECK_MULTI;
      }
      else
      {
         _prim_aptr->_mark = PrimMenuItem::MarkType_CHECK;
      }
   }
   else
   {
      _prim_aptr->_mark = PrimMenuItem::MarkType_NONE;
   }
}
   


/*
==============================================================================
Name : is_marked
==============================================================================
*/

bool  WidgetMenuItem::is_marked () const
{
   return _prim_aptr->_mark != PrimMenuItem::MarkType_NONE;
}
   


/*
==============================================================================
Name : req_entered
==============================================================================
*/

void  WidgetMenuItem::req_entered ()
{
   _prim_aptr->_rollover_flag = true;
}



/*
==============================================================================
Name : req_exited
==============================================================================
*/

void  WidgetMenuItem::req_exited ()
{
   _prim_aptr->_rollover_flag = false;
}



/*
==============================================================================
Name : req_select
==============================================================================
*/

void  WidgetMenuItem::req_select (const opal::Event & event)
{
   float dt = .05f;
   
   _prim_aptr->_rollover_flag
      .push (false, dt)
      .push (true, dt)
      .push (false, dt)
      .push (true, dt)
      .push (false, dt)
   ;
   
   Event widget_event (*this, event);
   widget_event._type = Event::Type_CLOSE;
   
   _delegate.param_ref_signal (widget_event);
   
   BundleView & view = get_ancestor <BundleView> ();
      
   view.release_popup_menu ();
}



/*
==============================================================================
Name : can_select
Description :
   Returns true if the initial delay (0.5 s) is over, false otherwise.
==============================================================================
*/

bool  WidgetMenuItem::can_select ()
{
   WidgetMenuRoot & menu_root = get_ancestor <WidgetMenuRoot> ();
   
   return menu_root.can_select ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetMenuItem::WidgetMenuItem ()
:  _frame_aptr ()
,  _prim_aptr ()
,  _width (0.f)
{
   _scripture.push_state_font (
      opal::FontManager::use_instance ().get_font (Constants::_default_font_0, 11.f)
   );

   _frame_aptr = new FrameMenuItem (*this);
   
   _frame_aptr->set_bounds (opa::Size (16, 16));
   
   _prim_aptr = new PrimMenuItem;
   _prim_aptr->set_name ("PrimMenuItem");

   set_embeddable (*_frame_aptr);
   set_embeddable (*_prim_aptr);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetMenuItem::do_get_min_bbox (Rect & min_bbox)
{
   min_bbox = Rect (0, 0, 23.f + _width + 25.f, 16.f);
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetMenuItem::do_get_margin (Margin & margin)
{
   margin = Margin::_zero;
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void  WidgetMenuItem::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_origin (origin);
   
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void  WidgetMenuItem::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _frame_aptr->set_bounds (bounds);
   
   _prim_aptr->_size.stop ().push (bounds, tt, tf);
}



/*
==============================================================================
Name: do_enable
==============================================================================
*/

void WidgetMenuItem::do_enable (bool enabled_flag)
{
   _prim_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetMenuItem::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetMenuItem::do_activate (bool activate_flag)
{
   _frame_aptr->activate (activate_flag);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
