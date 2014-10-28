/*****************************************************************************

        PrimPedestrian.cpp
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

#include "client/PrimPedestrian.h"

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

PrimPedestrian::PrimPedestrian ()
:  _selected_flag (*this, &PrimPedestrian::something_changed)
,  _ext_selected_flag (*this, &PrimPedestrian::something_changed)
,  _conflict_flag (*this, &PrimPedestrian::something_changed)
,  _highlight_flag (*this, &PrimPedestrian::something_changed)
{
   using namespace ohm;
   
   set_bounding_box (opa::Rect (
      0, 0, Constants::_metric_pedestrian_width, Constants::_metric_pedestrian_height
   ));
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  PrimPedestrian::display (ohm::opak::GraphicContext & context)
{
   using namespace ohm;
   
   Primitive::Auto auto_primitive (*this, context);
   
   const Aspect & aspect = context.get <Aspect> ();
   
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
      opak::use (aspect._pedestrian_selection_arr [selection])
         .display (context);
   }
   
   opak::use (aspect._pedestrian_main)
      .display (context);
   
   float highlight_flag = _highlight_flag.get_cur_value ();
   
   if (highlight_flag > 0.f)
   {
      context.get ().set_alpha (highlight_flag);
      
      opak::use (aspect._pedestrian_main_h)
         .display (context);
      
      context.get ().set_alpha (1.f);
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimPedestrian::~PrimPedestrian ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : something_changed
==============================================================================
*/

void  PrimPedestrian::something_changed ()
{
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
