/*****************************************************************************

        WidgetBox.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54597

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

#include "ohm/opa/base/WidgetBox.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

WidgetBox::WidgetBox (Orientation orientation)
:  _orientation (orientation)
,  _rect (Rect::_union_stable)
,  _margin (Margin::_zero)
,  _editing_flag (false)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

WidgetBox::~WidgetBox ()
{
}



/*
==============================================================================
Name : begin
==============================================================================
*/

void  WidgetBox::begin ()
{
   assert (!_editing_flag);
   
   _editing_flag = true;
}



/*
==============================================================================
Name : end
==============================================================================
*/

void  WidgetBox::end ()
{
   assert (_editing_flag);
   
   _editing_flag = false;
   
   if (_rect == Rect::_union_stable)
   {
      _rect = Rect::_zero;
      _margin = Margin::_zero;
   }
}



/*
==============================================================================
Name : push
==============================================================================
*/

void  WidgetBox::push (const Rect & rect, const Margin & margin)
{
   assert (_editing_flag);
   assert (!rect.is_null ());
   
   if (_rect == Rect::_union_stable)
   {
      assert (_margin.is_none ());
      
      _rect = rect;
      _margin = margin;
   }
   else
   {
      assert (_orientation != Orientation_NONE);
      
      if (_orientation == Orientation_HORIZONTAL)
      {
         float inter_margin = lang::max (_margin._right, margin._left);
         
         _margin._right = margin._right;
         
         float box_top = lang::min (_rect.get_top (), rect.get_top ());
         float box_bottom = lang::max (_rect.get_bottom (), rect.get_bottom ());
         
         float margined_top = lang::min (
            _rect.get_top () - _margin._top,
            rect.get_top () - margin._top
         );

         float margined_bottom = lang::max (
            _rect.get_bottom () + _margin._bottom,
            rect.get_bottom () + margin._bottom
         );
         
         _margin._top = box_top - margined_top;
         _margin._bottom = margined_bottom - box_bottom;
         
         _rect._size._width += inter_margin;

         _rect._size._width += rect._size._width;
         _rect.set_top (box_top);
         _rect.set_bottom (box_bottom);
      }
      else if (_orientation == Orientation_VERTICAL)
      {
         _margin._bottom = lang::max (_margin._bottom, margin._top);

         float inter_margin = lang::max (_margin._bottom, margin._top);
         
         _margin._bottom = margin._bottom;
         
         float box_left = lang::min (_rect.get_left (), rect.get_left ());
         float box_right = lang::max (_rect.get_right (), rect.get_right ());
         
         float margined_left = lang::min (
            _rect.get_left () - _margin._left,
            rect.get_left () - margin._left
         );

         float margined_right = lang::max (
            _rect.get_right () + _margin._right,
            rect.get_right () + margin._right
         );
         
         _margin._left = box_left - margined_left;
         _margin._right = margined_right - box_right;
         
         _rect._size._height += inter_margin;

         _rect._size._height += rect._size._height;
         _rect.set_left (box_left);
         _rect.set_right (box_right);
      }
      else
      {
         assert (false);
      }
   }
}



/*
==============================================================================
Name : push
==============================================================================
*/

void  WidgetBox::push (WidgetBox & box)
{
   assert (_editing_flag);

   push (box._rect, box._margin);
}



/*
==============================================================================
Name : get_orientation
==============================================================================
*/

Orientation WidgetBox::get_orientation () const
{
   return _orientation;
}



/*
==============================================================================
Name : get_rect
==============================================================================
*/

const Rect &   WidgetBox::get_rect () const
{
   assert (!_editing_flag);

   return _rect;
}



/*
==============================================================================
Name : get_margin
==============================================================================
*/

const Margin & WidgetBox::get_margin () const
{
   assert (!_editing_flag);

   return _margin;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
