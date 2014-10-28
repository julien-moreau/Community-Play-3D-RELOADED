/*****************************************************************************

        SprungImage.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 59357

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
#include "ohm/opak/SprungImage.h"
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

SprungImage::SprungImage ()
:  _image (*this, &SprungImage::image_changed)
,  _rect (*this, &SprungImage::image_changed)
,  _image_changed_flag (false)
{
   _color.init (1.f, 1.f, 1.f, 1.f);
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  SprungImage::display (GraphicContext & context)
{
   Primitive::Auto auto_primitive (*this, context);
   Opacifiable::Auto auto_opacifiable (*this, context);
   BlendModable::Auto auto_blend_modable (*this, context);
   
   opal::Color color = _color.get_cur_value ();
   opa::Size size = _size.get_cur_value ();
   bool scale_border_flag = _scale_border_flag.get_cur_value ();
   bool hinted_flag = _hinted_flag.get_cur_value ();
   
   if (_image_changed_flag)
   {
      std::string path = _image.get_cur_value ();
      opa::Rect rect = _rect.get_cur_value ();
   
      opal::Bitmap bitmap;
      bool ok_flag = opal::ImageLoader::use_instance ().load (bitmap, path, true);
      
      if (ok_flag)
      {
         _sprung_image_tex = opal::SprungImage (context.get (), bitmap, rect);
      }
      else
      {
         opal::SprungImage empty;
         _sprung_image_tex = empty;
      }
      
      _image_changed_flag = false;
   }
   
   PrimitiveHelper::display (
      context,
      _sprung_image_tex,
      color,
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

SprungImage::~SprungImage ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : image_changed
==============================================================================
*/

void  SprungImage::image_changed ()
{
   invalidate_all ();
   
   _image_changed_flag = true;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
