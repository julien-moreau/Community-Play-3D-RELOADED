/*****************************************************************************

        PrimToolTip.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54611

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

#include "ohm/opa/prim/PrimToolTip.h"
#include "ohm/opa/private/AspectWidget.h"
#include "ohm/opak/base/PrimitiveHelper.h"
#include "ohm/opal/LabelString.h"
#include "ohm/opal/FontManager.h"

#include <cassert>



namespace ohm
{
namespace opa
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: PrimToolTip
==============================================================================
*/

PrimToolTip::PrimToolTip ()
:  _label (*this, &PrimToolTip::label_changed)
{
}



/*
==============================================================================
Name: display
==============================================================================
*/

void PrimToolTip::display (opak::GraphicContext & context)
{
   opak::Primitive::Auto auto_primitive (*this, context);
   opak::Opacifiable::Auto auto_opacifiable (*this, context);
   
   const AspectWidget & aspect = context.get <AspectWidget> ();

   const std::string & label = _label.get_cur_value ();
   
   opal::LabelString lbl_str (Constants::_default_font_0, 10.f, label, opal::Color (.5f, 0.f));
   
   opa::Rect typo_bounds = lbl_str.get_typographic_bounds ();
   
   float tool_tip_width = typo_bounds._size._width + 7 * 2 + 20;
   
   if (tool_tip_width >= 61)
   {
      opak::PrimitiveHelper::display (
         context,
         aspect._tool_tip_back,
         opa::Rect (-10, - 8, tool_tip_width, 36),
         true, true
      );
   }
   else
   {
      opak::PrimitiveHelper::display (
         context,
         aspect._tool_tip_back2,
         opa::Rect (-10, - 8, tool_tip_width, 36),
         true
      );
   }
   
   opak::PrimitiveHelper::display (
      context,
      lbl_str,
      opa::Point (7, 12),
      0.f
   );
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: ~PrimToolTip
==============================================================================
*/

PrimToolTip::~PrimToolTip ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name: label_changed
==============================================================================
*/

void PrimToolTip::label_changed ()
{
   const std::string & label = _label.get_cur_value ();
   
   opal::LabelString lbl_str (Constants::_default_font_0, 10.f, label, opal::Color (.35f, 0.f));
   
   opa::Rect typo_bounds = lbl_str.get_typographic_bounds ();
   
   opa::Rect bbox (
      -10, - 8, typo_bounds._size._width + 7 * 2 + 20, 36
   );
   
   set_bounding_box (bbox);
}



}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

