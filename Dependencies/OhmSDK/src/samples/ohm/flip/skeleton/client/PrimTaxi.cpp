/*****************************************************************************

        PrimTaxi.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70481

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

#include "client/PrimTaxi.h"

#include "client/Constants.h"

#include "ohm/opal/GraphicContextInterface.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

PrimTaxi::PrimTaxi ()
:  _color (*this, &PrimTaxi::something_changed)
,  _width (*this, &PrimTaxi::width_changed)
,  _selected_flag (*this, &PrimTaxi::something_changed)
,  _ext_selected_flag (*this, &PrimTaxi::something_changed)
,  _conflict_flag (*this, &PrimTaxi::something_changed)
,  _highlight_flag (*this, &PrimTaxi::something_changed)
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  PrimTaxi::display (ohm::opak::GraphicContext & context)
{
   using namespace ohm;
   
   Primitive::Auto auto_primitive (*this, context);
   
   const Aspect & aspect = context.get <Aspect> ();
   
   model::Taxi::Color color = _color.get_cur_value ();
   
   float width = _width.get_cur_value ();
   
   opa::Rect rect (0, 0, width, Constants::_metric_taxi_height);
   
   context.get ().set_blend_mode_multiply ();
   
   opak::use (aspect._taxi_shadow)
      .rect (rect)
      .display (context);
   
   context.get ().set_blend_mode_normal ();
   
   bool selected_flag = _selected_flag.get_cur_value ();
   bool ext_selected_flag = _ext_selected_flag.get_cur_value ();
   bool conflict_flag = _conflict_flag.get_cur_value ();

   const int selection = AspectMain::get_selection (
      selected_flag,
      ext_selected_flag,
      conflict_flag
   );
   
   if (selection != AspectMain::Selection_OFF)
   {
      opak::use (aspect._taxi_selection_arr [selection])
         .rect (rect)
         .display (context);
   }
   
   opak::use (aspect._taxi_back)
      .rect (rect)
      .display (context);
   
   opal::Color c = aspect._taxi_color_arr [color];
   
   float highlight_flag = _highlight_flag.get_cur_value ();
   
   if (highlight_flag > 0.f)
   {
      c = opal::Color::interpolate (c, aspect._taxi_color_h_arr [color], highlight_flag);
   }
   
   opak::use (aspect._taxi_main)
      .rect (rect)
      .color (c)
      .display (context);

   opak::use (aspect._taxi_front)
      .rect (rect)
      .display (context);

   context.get ().set_blend_mode_screen ();
   context.get ().set_alpha (.25f);
   
   opak::use (aspect._taxi_reflect)
      .rect (rect)
      .display (context);
   
   context.get ().set_alpha (1.f);
   context.get ().set_blend_mode_normal ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimTaxi::~PrimTaxi ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : something_changed
==============================================================================
*/

void  PrimTaxi::something_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : width_changed
==============================================================================
*/

void  PrimTaxi::width_changed ()
{
   using namespace ohm;
   
   float width = _width.get_cur_value ();
   
   set_bounding_box (opa::Rect (0, 0, width, Constants::_metric_taxi_height));
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
