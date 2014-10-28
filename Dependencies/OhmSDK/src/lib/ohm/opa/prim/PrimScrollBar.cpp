/*****************************************************************************

        PrimScrollBar.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 65616

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

#include "ohm/opa/prim/PrimScrollBar.h"
#include "ohm/opa/private/AspectWidget.h"
#include "ohm/opa/private/AspectWidgetScrollBar.h"
#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: PrimScrollBar
==============================================================================
*/

PrimScrollBar::PrimScrollBar ()
:  _length (*this, &PrimScrollBar::bbox_changed)
,  _type (*this, &PrimScrollBar::bbox_changed)
,  _content_length (*this, &PrimScrollBar::something_changed)
,  _visible_position (*this, &PrimScrollBar::something_changed)
,  _visible_length (*this, &PrimScrollBar::something_changed)
{
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimScrollBar::display (opak::GraphicContext & context)
{
   PrimWidget::Auto auto_primitive (*this, context);
   
   float content_length = _content_length.get_cur_value ();
   float visible_length = _visible_length.get_cur_value ();
   
   if (content_length < visible_length)
   {
      return;
   }
   
   if ((content_length <= 0.f) || (visible_length <= 0.f))
   {
      return;
   }
   
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetScrollBar & aspect_scrollbar = aspect._scrollbar;
   
   float length = _length.get_cur_value ();
   float visible_position = _visible_position.get_cur_value ();
   
   float front_length = length * visible_length / content_length;
   
   lang::max_in_place (front_length, 16.f);
   
   float front_position
      = (length - front_length - 2)
      * visible_position / (content_length - visible_length)
      + 1.f;
   
   if (1.f >= length - front_length - 1.f)
   {
      return;
   }
   
   lang::limit_in_place (front_position, 1.f, length - front_length - 1.f);
   
   Type type = _type.get_cur_value ();
   
   if (type == Type_HORIZONTAL)
   {
      opak::use (aspect_scrollbar._back)
         .rect (0, 0, length, 7)
         .scale_border ()
         .hint ()
         .display (context)
      ;
      
      opak::use (aspect_scrollbar._front)
         .rect (front_position, 1, front_length, 5)
         .scale_border ()
         .hint ()
         .display (context)
      ;
   }
   else
   {
      opak::use (aspect_scrollbar._back)
         .rect (0, 0, 7, length)
         .scale_border ()
         .hint ()
         .display (context)
      ;

      opak::use (aspect_scrollbar._front)
         .rect (1, front_position, 5, front_length)
         .scale_border ()
         .hint ()
         .display (context)
      ;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~PrimScrollBar
==============================================================================
*/

PrimScrollBar::~PrimScrollBar ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: bbox_changed
==============================================================================
*/

void PrimScrollBar::bbox_changed ()
{
   float length = _length.get_cur_value ();

   Type type = _type.get_cur_value ();
   
   Rect bbox;

   if (type == Type_HORIZONTAL)
   {
      bbox = Rect (0, 0, length, 7);
   }
   else
   {
      bbox = Rect (0, 0, 7, length);
   }
   
   bbox._size.upper_integral ();
   
   set_bounding_box (bbox);
}



/*
==============================================================================
Name: something_changed
==============================================================================
*/

void PrimScrollBar::something_changed ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

