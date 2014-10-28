/*****************************************************************************

        PrimFlipFlop.cpp
        Copyright (c) 2009 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 38762

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

#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/Color.h"
#include "ohm/opa/Quad.h"
#include "ohm/opal/ImageManager.h"
#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/math/fnc.h"

#include "ohm/opa/private/PrimFlipFlop.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

PrimFlipFlop::PrimFlipFlop ()
:  _flip_flop_flag (*this, &PrimFlipFlop::flip_flop_flag_changed)
,  _hit_flag (*this, &PrimFlipFlop::hit_flag_changed)
{
// #warning to do
   set_bounding_box (opa::Rect (0, 0, 16, 16));
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  PrimFlipFlop::display (opak::GraphicContext & context)
{
   Primitive::Auto auto_primitive (*this, context);
   
   bool flip_flop_flag = _flip_flop_flag.get_cur_value ();
   bool hit_flag = _hit_flag.get_cur_value ();
   
   if (hit_flag)
   {
      context.get ().set_alpha (.5f);
   }
   
   opa::Rect rect (0, 0, 16, 16);
   
   bool hinted_flag = false;  // we should be already hinted

   if (flip_flop_flag)
   {
      opak::PrimitiveHelper::display (
         context, rect, opal::Color (1.f, 1.f), hinted_flag
      );
   }
   else 
   {
      opak::PrimitiveHelper::display (
         context, rect, opal::Color (1.f, 0.f), hinted_flag
      );
   }

   context.get ().set_alpha (1.f);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

PrimFlipFlop::~PrimFlipFlop ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : state_changed
==============================================================================
*/

void  PrimFlipFlop::flip_flop_flag_changed ()
{
   invalidate_all ();
}



/*
==============================================================================
Name : hit_flag_changed
==============================================================================
*/

void  PrimFlipFlop::hit_flag_changed ()
{
   invalidate_all ();
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

