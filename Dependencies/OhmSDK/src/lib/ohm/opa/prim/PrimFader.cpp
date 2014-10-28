/*****************************************************************************

        PrimFader.cpp
        Copyright (c) 2010 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 62138

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

#include "ohm/opa/prim/PrimFader.h"
#include "ohm/opa/private/AspectWidget.h"
#include "ohm/opa/private/AspectWidgetFader.h"
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
Name: PrimFader
==============================================================================
*/

PrimFader::PrimFader ()
:  _value_min (*this, &PrimFader::value_changed)
,  _value_max (*this, &PrimFader::value_changed)
,  _value (*this, &PrimFader::value_changed)
,  _discrete_flag (*this, &PrimFader::value_changed)
,  _course_len (*this, &PrimFader::bbox_changed)
,  _type (*this, &PrimFader::bbox_changed)
{
   _value_max.init (1.f);
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimFader::display (opak::GraphicContext & context)
{
   PrimWidget::Auto auto_primitive (*this, context);
   
   const AspectWidget & aspect = context.get <AspectWidget> ();
   
   const AspectWidgetFader & aspect_fader = aspect._fader;
   
   float value_min = _value_min.get_cur_value ();
   float value_max = _value_max.get_cur_value ();
   float value = _value.get_cur_value ();
   bool discrete_flag = _discrete_flag.get_cur_value ();
   const Type type = _type.get_cur_value ();
   const float course_len = _course_len.get_cur_value ();
   
   if (value_min < value_max)
   {
      value = (value - value_min) / (value_max - value_min);
   }
   
   // don't check for value in bounds, the caller might face
   // small precision problems
   
   lang::limit_in_place (value, 0.f, 1.f);
   
   if (type == Type_HORIZONTAL)
   {
      opak::use (aspect_fader._rail_hori)
         .rect (0, 6, course_len + (discrete_flag ? 1.f : 0.f), 2)
         .scale_border ()
         .hint ()
         .display (context)
      ;
      
      if (discrete_flag && (value_min < value_max))
      {
         float adv = course_len / (value_max - value_min);
         float x = 0.f;
         
         for (float t = value_min ; t <= value_max ; t += 1.f)
         {
            opak::use (aspect_fader._rail_verti)
               .rect (x, 7, 2, 2)
               .scale_border ()
               .hint ()
               .display (context)
            ;
            
            x += adv;
         }
      }

      opak::use (aspect_fader._handle)
         .point (value * course_len - 7, 0)
         .scale ()
         .hint ()
         .display (context)
      ;
   }
   else
   {
      // to do : wrong image
      
      opak::use (aspect_fader._rail_verti)
         .rect (6, 0, 2, course_len + (discrete_flag ? 1.f : 0.f))
         .scale_border ()
         .hint ()
         .display (context)
      ;
      
      if (discrete_flag && (value_min < value_max))
      {
         float adv = course_len / (value_max - value_min);
         float y = 0.f;
         
         for (float t = value_min ; t <= value_max ; t += 1.f)
         {
            opak::use (aspect_fader._rail_verti)
               .rect (7, y, 2, 2)
               .scale_border ()
               .hint ()
               .display (context)
            ;
            
            y += adv;
         }
      }

      opak::use (aspect_fader._handle)
         .point (0, value * course_len - 6)
         .scale ()
         .hint ()
         .display (context)
      ;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~PrimFader
==============================================================================
*/

PrimFader::~PrimFader ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: value_changed
==============================================================================
*/

void PrimFader::value_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name: bbox_changed
==============================================================================
*/

void PrimFader::bbox_changed ()
{
   float course_len = _course_len.get_cur_value ();

   Type type = _type.get_cur_value ();
   
   Rect bbox;

   if (type == Type_HORIZONTAL)
   {
      bbox = Rect (-7, 0, course_len + 14, 14);
   }
   else
   {
      bbox = Rect (0, -7, 14, course_len + 14);
   }
   
   bbox._size.upper_integral ();
   
   set_bounding_box (bbox);
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

