/*****************************************************************************

        PrimMusicTime.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 66738

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

#include "ohm/opa/prim/PrimMusicTime.h"
#include "ohm/opa/private/AspectWidget.h"

#include <cassert>
#include <cmath>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: PrimMusicTime
==============================================================================
*/

PrimMusicTime::PrimMusicTime ()
:  _bar (*this, &PrimMusicTime::something_changed)
,  _beat (*this, &PrimMusicTime::something_changed)
,  _rem (*this, &PrimMusicTime::something_changed)
,  _tick (*this, &PrimMusicTime::something_changed)
{
}



/*
==============================================================================
Name: ~PrimMusicTime
==============================================================================
*/

PrimMusicTime::~PrimMusicTime ()
{
}



/*
==============================================================================
Name: display
==============================================================================
*/

void  PrimMusicTime::display (opak::GraphicContext & context)
{
   PrimWidget::Auto auto_primitive (*this, context);
   
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetMusicTime & aspect_music_time = aspect._music_time;
   
   opa::Size size = _size.get_cur_value ();
   opa::Rect rect (0, 0, 0, size._height);
   
   int state = AspectWidgetComboBox::State_OFF;
   
   float x_left = 0.f;
   
   for (int i = 0 ; i < 4 ; ++i)
   {
      float x_right = float (floor (double (i + 1) * size._width * .25 - .5));
      
      rect._origin._x = x_left;
      rect._size._width = x_right - x_left;
      
      opal::SprungImage image;
      
      int left_off = 0;
      int right_off = 0;
      
      if (i == 0)
      {
         left_off = 1;
         image = aspect_music_time._button_left_arr [state];
      }
      else if (i == 3)
      {
         image = aspect_music_time._button_right_arr [state];
      }
      else
      {
         right_off = 1;
         image = aspect_music_time._button_middle_arr [state];
      }
      
      opak::use (image)
         .rect (rect)
         .margin (float (left_off), 1, float (right_off), 1)
         .hint ()
         .display (context);
      
      if (i < 3)
      {
         opak::use (aspect_music_time._button_separator_arr [state])
            .point (opa::Point (x_right, - context.get_inv_px ()._y))
            .hint ()
            .display (context);
      }
      
      x_left = x_right + 1.f;
   }
   
   display_children (context);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : do_size_changed
==============================================================================
*/

void  PrimMusicTime::do_size_changed ()
{
   // add shadows
   
   opa::Size size = _size.get_cur_value ();
   
   opa::Rect rect;
   rect._size = size;
   
   rect += opa::Margin (1, 1, 1, 1);
   
   set_bounding_box (rect);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: something_changed
==============================================================================
*/

void  PrimMusicTime::something_changed ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

