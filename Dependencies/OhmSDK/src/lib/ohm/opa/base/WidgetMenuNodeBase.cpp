/*****************************************************************************

        WidgetMenuNodeBase.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54293

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

#include "ohm/opa/base/WidgetMenuNodeBase.h"

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

WidgetMenuNodeBase::~WidgetMenuNodeBase ()
{
   try
   {
      clear ();
      
      unset_all ();
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name: invalidate_layout
==============================================================================
*/

void WidgetMenuNodeBase::invalidate_layout ()
{
   _metrics_need_update_flag = true;

   WidgetBase::invalidate_layout ();
}



/*
==============================================================================
Name : layout
==============================================================================
*/

bool  WidgetMenuNodeBase::layout (const Size & bounds)
{
   bool retrigger_layout_flag = WidgetBase::layout (bounds);
   
   synchronize ();
   
   iterator it = begin ();
   const iterator it_end = end ();
   
   float offset_x = -_min_bbox._origin._x;
   
   float y = 0.f;
   float previous_margin = 0.f;
   
   float max_width = 0.f;
   
   for (; it != it_end ; ++it)
   {
      WidgetBase & widget = *it;
      
      Rect sub_min_bbox;
      widget.get_min_bbox (sub_min_bbox);
      
      lang::max_in_place (max_width, sub_min_bbox._size._width);
   }
   
   it = begin ();
   
   for (; it != it_end ; ++it)
   {
      WidgetBase & widget = *it;
      
      if (!widget.is_visible ())
      {
         // skip
         
         continue;
      }
      
      Rect sub_min_bbox;
      widget.get_min_bbox (sub_min_bbox);
      
      if (y + sub_min_bbox._size._height > bounds._height)
      {
         y = 0.f;
         offset_x += max_width;
      }

      Point origin (offset_x, y);
      
      Size sub_bounds = sub_min_bbox._size;
      sub_bounds._width = max_width;
      
      retrigger_layout_flag |= widget.layout (sub_bounds);
      
      widget.set_origin (origin);
      
      Margin sub_margin;
      widget.get_margin (sub_margin);
      
      y += sub_bounds._height;
      y += lang::max (previous_margin, sub_margin._top);
      previous_margin = sub_margin._bottom;
   }
   
   return retrigger_layout_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetMenuNodeBase::WidgetMenuNodeBase ()
:  _metrics_need_update_flag (true)
{
}



/*
==============================================================================
Name : do_get_min_bbox
==============================================================================
*/

void  WidgetMenuNodeBase::do_get_min_bbox (Rect & min_bbox)
{
   update ();
   
   min_bbox = _min_bbox;
}



/*
==============================================================================
Name : do_get_margin
==============================================================================
*/

void  WidgetMenuNodeBase::do_get_margin (Margin & margin)
{
   margin = opa::Margin::_zero;
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update
==============================================================================
*/

void  WidgetMenuNodeBase::update ()
{
   if (_metrics_need_update_flag)
   {
      synchronize ();
   
      _min_bbox = opa::Rect::_zero;
      
      iterator it = begin ();
      const iterator it_end = end ();
      
      for (; it != it_end ; ++it)
      {
         WidgetBase & widget = *it;
         
         Rect sub_min_bbox;
         widget.get_min_bbox (sub_min_bbox);
         
         _min_bbox._size._height += sub_min_bbox._size._height;
         lang::max_in_place (_min_bbox._size._width, sub_min_bbox._size._width);
      }
      
      _metrics_need_update_flag = false;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
