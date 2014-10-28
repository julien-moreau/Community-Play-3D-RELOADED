/*****************************************************************************

        WidgetLabel.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66672

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

#include "ohm/opa/WidgetLabel.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opal/private/Typesetter.h"

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

WidgetLabel::~WidgetLabel ()
{
}



/*
==============================================================================
Name : set_width
Description :
   Fix the width of 'this', independent of actual content width.
==============================================================================
*/

void  WidgetLabel::set_width (float width)
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

void  WidgetLabel::set_width_auto_content ()
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

void  WidgetLabel::set_width_auto_parent (float weight)
{
   _width.set_ext ();
   set_horizontal_weight (weight);
}



/*
==============================================================================
Name : get_width
==============================================================================
*/

float WidgetLabel::get_width ()
{
   if (_width.need_update ())
   {
      update_metrics ();
   }
   
   return _width.get ();
}



/*
==============================================================================
Name : is_width_auto
Note :
   Used by decorators (e.g. WidgetPushButton)
==============================================================================
*/

bool  WidgetLabel::is_width_auto () const
{
   return _width.is_auto ();
}



/*
==============================================================================
Name : set_label
==============================================================================
*/

void  WidgetLabel::set_label (const std::string & label)
{
   if (_label != label)
   {
      _label = label;

      _scripture.begin_edit ();
      _scripture.clear ();
      _scripture.insert (0, label);
      _scripture.end_edit ();
      
      _prim_aptr->_label = label;
      
      _width.invalidate ();
      
      invalidate_layout ();
   }
}



/*
==============================================================================
Name : get_label
==============================================================================
*/

const std::string &  WidgetLabel::get_label () const
{
   return _label;
}



/*
==============================================================================
Name : set_flush_factor
Description :
   Set the flush factor of the label with respect to its fixed width.
   0.f is equivalent to align to the left (default mode)
   .5f is equivalent to center
   1.f is equivalent to align to the right
   
Note :
   Only meaningful in non-auto mode.
==============================================================================
*/

void  WidgetLabel::set_flush_factor (float flush_factor)
{
   if (_flush_factor != flush_factor)
   {
      _flush_factor = flush_factor;
      
      _prim_aptr->_flush_factor = flush_factor;
      
      if (!_width.is_auto ())
      {
         invalidate_layout ();
      }
   }
}



/*
==============================================================================
Name: get_flush_factor
==============================================================================
*/

float WidgetLabel::get_flush_factor () const
{
   return _flush_factor;
}
   
   
   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetLabel::WidgetLabel ()
:  _prim_aptr ()
,  _ascent (0.f)
,  _descent (0.f)
,  _width (*this)
,  _label ()
,  _flush_factor (0.f)
,  _scripture ()
{
   const opal::Font & font = opal::FontManager::use_instance ().get_font (
      Constants::_default_font_0, 12.f
   );
   
   _ascent = font.get_ascent ();
   _descent = font.get_descent ();
   
   _scripture.push_state_font (font);
   
   _prim_aptr = new PrimLabel;
   _prim_aptr->set_name ("WidgetLabel::_prim_aptr");
   
   set_embeddable (*_prim_aptr);
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetLabel::do_get_min_bbox (Rect & min_bbox)
{
   if (_width.need_update ())
   {
      update_metrics ();
   }
   
   min_bbox = opa::Rect (
      0, - _ascent,
      _width.get (), _ascent + _descent
   );
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetLabel::do_get_margin (Margin & margin)
{
   margin = opa::Margin (4, 1, 4, 2);
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void  WidgetLabel::do_set_origin (const Point & origin, double tt, opak::TransitionFunction tf)
{
   opa::Point sub_origin = origin + opa::Point (0.f, _ascent);
   
   _prim_aptr->_origin.stop ().push (sub_origin, tt, tf);
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void  WidgetLabel::do_set_bounds (const Size & bounds, double tt, opak::TransitionFunction tf)
{
   _prim_aptr->_width.stop ().push (bounds._width, tt, tf);
}



/*
==============================================================================
Name : do_enable
==============================================================================
*/

void  WidgetLabel::do_enable (bool enabled_flag)
{
   _prim_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name : do_show
==============================================================================
*/

void  WidgetLabel::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name : do_activate
==============================================================================
*/

void  WidgetLabel::do_activate (bool activate_flag)
{
   // nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_metrics
==============================================================================
*/

void  WidgetLabel::update_metrics ()
{
   if (_width.need_update ())
   {
      opal::Typesetter typesetter (_scripture);
      
      size_t end_pos = 0;
      float height = 0.f;
      typesetter.suggest_line_break (end_pos, height, 0, 100000.f);
      
      if (end_pos > 0)
      {
         opal::Scripture::Range range;
         range._start = 0;
         range._end = end_pos;

         const opal::Typesetter::LineInfo * line_info_ptr
            = typesetter.layout_line (range, 100000.f, false, 0);
         
         _width.update (line_info_ptr->_width);
      }
      else
      {
         _width.update (0.f);
      }
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
