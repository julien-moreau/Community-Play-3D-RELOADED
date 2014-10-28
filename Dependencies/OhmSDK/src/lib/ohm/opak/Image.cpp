/*****************************************************************************

        Image.cpp
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
#include "ohm/opak/Image.h"
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

Image::Image ()
:  _image (*this, &Image::image_changed)
,  _image_changed_flag (false)
{
   _color.init (1.f, 1.f, 1.f, 1.f);
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  Image::display (GraphicContext & context)
{
   Primitive::Auto auto_primitive (*this, context);
   Opacifiable::Auto auto_opacifiable (*this, context);
   BlendModable::Auto auto_blend_modable (*this, context);
   
   opal::Color color = _color.get_cur_value ();
   bool hinted_flag = _hinted_flag.get_cur_value ();
   
   if (_image_changed_flag)
   {
      std::string path = _image.get_cur_value ();
   
      opal::Bitmap bitmap;
      bool ok_flag = opal::ImageLoader::use_instance ().load (bitmap, path, true);
      
      if (ok_flag)
      {
         _image_tex = opal::Image (context.get (), bitmap);
      }
      else
      {
         opal::Image empty;
         _image_tex = empty;
      }
      
      _image_changed_flag = false;
   }
   
   bool scale_image_flag = _scale_flag.get_cur_value ();
   
   PrimitiveHelper::display (
      context,
      _image_tex,
      color,
      opa::Point::_zero,
      scale_image_flag, hinted_flag
   );
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

Image::~Image ()
{
}



/*
==============================================================================
Name : do_scale_flag_changed
==============================================================================
*/

void  Image::do_scale_flag_changed ()
{
   update_bbox ();
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : image_changed
==============================================================================
*/

void  Image::image_changed ()
{
   update_bbox ();
   
   _image_changed_flag = true;
}



/*
==============================================================================
Name : update_bbox
==============================================================================
*/

void  Image::update_bbox ()
{
   std::string path = _image.get_cur_value ();
   
   opal::Bitmap bitmap;
   bool ok_flag = opal::ImageLoader::use_instance ().load (bitmap, path, true);
   
   if (ok_flag)
   {
      bool scale_flag = _scale_flag.get_cur_value ();
      float width = float (bitmap.get_width ());
      float height = float (bitmap.get_height ());
      
      if (scale_flag)
      {
         opa::Rect rect (0, 0, width, height);
      
         set_bounding_box (rect);
      }
      else
      {
         opa::Margin margin (0, 0, width, height);
      
         set_bounding_box (opa::Rect::_zero, margin);
      }
   }
   else
   {
      set_bounding_box (opa::Rect::_zero);
   }
   
   invalidate_all ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
