/*****************************************************************************

        SprungBitmap.cpp
        Copyright (c) 2012 Ohm Force

        Author: Jerome Noel <jerome.noel@ohmforce.com>
        Revision: 68147

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

#include "ohm/opal/Bitmap.h"
#include "ohm/opal/GraphicContextInterface.h"
#include "ohm/opal/ImageLoader.h"
#include "ohm/opak/SprungBitmap.h"
#include "ohm/opak/base/PrimitiveHelper.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

SprungBitmap::SprungBitmap ()
:  _bitmap (*this, &SprungBitmap::something_changed)
,  _rect (*this, &SprungBitmap::something_changed)
{
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  SprungBitmap::display (GraphicContext & context)
{
   Primitive::Auto auto_primitive (*this, context);
   Opacifiable::Auto auto_opacifiable (*this, context);
   BlendModable::Auto auto_blend_modable (*this, context);
   
   opa::Size size = _size.get_cur_value ();
   bool scale_border_flag = _scale_border_flag.get_cur_value ();
   bool hinted_flag = _hinted_flag.get_cur_value ();
   
   opal::SprungImage sprung_tex (context.get (), _bitmap.get_cur_value (), _rect.get_cur_value ());

   PrimitiveHelper::display (
      context,
      sprung_tex,
      opa::Rect (opa::Point::_zero, size),
      scale_border_flag, hinted_flag
   );
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

SprungBitmap::~SprungBitmap ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : something_changed
==============================================================================
*/

void  SprungBitmap::something_changed ()
{
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm




/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
