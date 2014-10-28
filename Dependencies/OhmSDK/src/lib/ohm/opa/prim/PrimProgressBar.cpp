/*****************************************************************************

        PrimProgressBar.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67164

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

#include "ohm/opa/prim/PrimProgressBar.h"
#include "ohm/opa/private/AspectWidget.h"
#include "ohm/opa/private/AspectWidgetProgressBar.h"

#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/lang/fnc.h"
#include "ohm/sys/TimerReliable.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: PrimProgressBar
==============================================================================
*/

PrimProgressBar::PrimProgressBar ()
:  _width (*this, &PrimProgressBar::width_changed)
,  _norm_value (*this, &PrimProgressBar::something_changed)
,  _ticker (*this, &PrimProgressBar::something_changed)
{
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimProgressBar::display (opak::GraphicContext & context)
{
   if (!_ticker.vt_is_animating ())
   {
      _ticker.do_start ();
   }
   
   PrimWidget::Auto auto_primitive (*this, context);
   
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetProgressBar & aspect_progress = aspect._progress_bar;
   
   const float width = _width.get_cur_value ();
   assert (width >= 0);
   
   float norm_value = _norm_value.get_cur_value ();
   
   opak::use (aspect_progress._back)
      .rect (0, 0, width, 12)
      .margin (1, 1, 1, 1)
      .display (context);
   
   float clip_width = width - 2;
   
   bool indeterminate_flag = norm_value == -1.f;
   
   lang::limit_in_place (norm_value, 0.f, 1.f);
   
   if (!indeterminate_flag)
   {
      clip_width = (width - 2.f) * norm_value;
   }
   
   if (norm_value < 1.f)
   {
      const float pixel_per_sec = 80.f;
      //const float phase = _phase.get_cur_value ();
      const float phase = float (sys::TimerReliable::use_instance ().get ());
      
      float offset = phase * pixel_per_sec;
      
      // indeterminate width is 152 (without superpose pixel)
      offset = fmod (offset, 152.f);
      
      offset = - offset;
      
      opa::Rect clip_rect (1, 1, clip_width, 9);
      context.apply (clip_rect);
      
      context.get ().save_graphic_state ();
      context.get ().clip (clip_rect);
      
      const opal::Image & image
         = indeterminate_flag ? aspect_progress._indeterminate : aspect_progress._front;
      
      for (; offset < width ; offset += 152.f)
      {
         opak::use (image)
            .point (offset, 0)
            .display (context);
      }
      
      context.get ().restore_graphic_state ();
   }
   else
   {
      opak::use (aspect_progress._front_end)
         .rect (1, 0, width - 2, 11)
         .display (context);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~PrimProgressBar
==============================================================================
*/

PrimProgressBar::~PrimProgressBar ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: width_changed
==============================================================================
*/

void  PrimProgressBar::width_changed ()
{
   float width = _width.get_cur_value ();

   Rect bbox (-1, -1, width + 2, 14);

   bbox._size.upper_integral ();
   
   set_bounding_box (bbox);
}



/*
==============================================================================
Name: something_changed
==============================================================================
*/

void PrimProgressBar::something_changed ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

