/*****************************************************************************

        WidgetText.cpp
        Copyright (c) 2012 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 69169

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

#include "ohm/opa/WidgetText.h"

#include "ohm/opal/FontManager.h"
#include "ohm/opal/private/Typesetter.h"
#include "ohm/opal/Scripture.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~WidgetText
==============================================================================
*/

WidgetText::~WidgetText ()
{
}



/*
==============================================================================
Name: set_width
==============================================================================
*/

void WidgetText::set_width (float width)
{
   _width.set (width);
   set_horizontal_weight (0.f);
}



/*
==============================================================================
Name: set_width_auto_parent
==============================================================================
*/

void WidgetText::set_width_auto_parent (float weight)
{
   _width.set_ext ();
   set_horizontal_weight (weight);
}



/*
==============================================================================
Name: get_width
==============================================================================
*/

float WidgetText::get_width ()
{
   if (_width.need_update ())
   {
      _width.update (0.f);
   }
   
   return _width.get ();
}



/*
==============================================================================
Name: set_text
==============================================================================
*/

void WidgetText::set_text (const std::string & text)
{
   _text = text;
   _prim_aptr->_text = text;
   
   invalidate_layout ();
}



/*
==============================================================================
Name: invalidate_layout
==============================================================================
*/

void WidgetText::invalidate_layout ()
{
   _invalid_flag = true;
   
   opa::WidgetBase::invalidate_layout ();
}



/*
==============================================================================
Name: layout
==============================================================================
*/

bool WidgetText::layout (const opa::Size & bounds)
{
   bool retrigger_layout_flag
      = (get_bounds ()._width != bounds._width)
      || _invalid_flag;
   
   retrigger_layout_flag |= opa::WidgetBase::layout (bounds);
   
   if (retrigger_layout_flag)
   {
      // for the given width, calculate the needed height
      
      update_metrics (bounds._width);
      
      _invalid_flag = false;
   }
   
   return retrigger_layout_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: WidgetText
==============================================================================
*/

WidgetText::WidgetText ()
:  _width (*this)
,  _height (0.f)
,  _invalid_flag (false)
{
   _prim_aptr = new PrimText;
   
   set_embeddable (*_prim_aptr);
}



/*
==============================================================================
Name: do_get_min_bbox
==============================================================================
*/

void WidgetText::do_get_min_bbox (opa::Rect & min_bbox)
{
   if (_width.need_update ())
   {
      _width.update (0.f);
   }
   
   min_bbox = opa::Rect::_zero;
   min_bbox._size._width = _width.get ();
   min_bbox._size._height = _height;
}



/*
==============================================================================
Name: do_get_margin
==============================================================================
*/

void WidgetText::do_get_margin (opa::Margin & margin)
{
   margin = opa::Margin (0, 0, 0, 0);
}



/*
==============================================================================
Name: do_set_origin
==============================================================================
*/

void WidgetText::do_set_origin (const opa::Point & origin, double tt, opak::TransitionFunction tf)
{
   _prim_aptr->_origin.stop ().push (origin, tt, tf);
}



/*
==============================================================================
Name: do_set_bounds
==============================================================================
*/

void WidgetText::do_set_bounds (const opa::Size & bounds, double tt, opak::TransitionFunction tf)
{
   if (bounds != _last_size)
   {
      _last_size = bounds;

      _prim_aptr->_size.stop ().push (bounds, tt, tf);
   }
}



/*
==============================================================================
Name: do_enable
==============================================================================
*/

void WidgetText::do_enable (bool enabled_flag)
{
   _prim_aptr->_enabled_flag = enabled_flag;
}



/*
==============================================================================
Name: do_show
==============================================================================
*/

void WidgetText::do_show (bool show_flag)
{
   _prim_aptr->_visibility = show_flag;
}



/*
==============================================================================
Name: do_activate
==============================================================================
*/

void WidgetText::do_activate (bool activate_flag)
{
   // nothing
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: update_metrics
==============================================================================
*/

void WidgetText::update_metrics (float width)
{
   const float str_x_margin = 6.f;
   const float msg_width = lang::max (1.f, width - 2.f * str_x_margin);
   
   opal::Scripture scripture;

   const opal::Font & font
      = opal::FontManager::use_instance ().get_font (opa::Constants::_default_font_0, 12.f);
   
   scripture.push_state_font (font);
   
   scripture.begin_edit ();
   scripture.insert (0, _text);
   scripture.end_edit ();
   
   opal::Typesetter typesetter (scripture);
   
   _height = 0.f;
   
   for (size_t pos = 0 ; pos < scripture.size () ; )
   {
      size_t next_pos = 0;
      float height = 0;
      typesetter.suggest_line_break (next_pos, height, pos, msg_width);
      
      _height += height;

      pos = next_pos;
   }
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

