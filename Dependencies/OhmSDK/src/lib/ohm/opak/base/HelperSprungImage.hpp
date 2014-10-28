/*****************************************************************************

        HelperSprungImage.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 67460

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



#if defined (ohm_opak_HelperSprungImage_CURRENT_CODEHEADER)
   #error Recursive inclusion of HelperSprungImage code header.
#endif
#define  ohm_opak_HelperSprungImage_CURRENT_CODEHEADER

#if ! defined (ohm_opak_HelperSprungImage_CODEHEADER_INCLUDED)
#define  ohm_opak_HelperSprungImage_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : use
==============================================================================
*/

HelperSprungImage use (const opal::SprungImage & image)
{
   HelperSprungImage ret_val (image);
   
   return ret_val;
}



/*
==============================================================================
Name : ctor
Note :
   Assume that the lifespan of 'image' is greater than the one of HelperSprungImage
==============================================================================
*/

HelperSprungImage::HelperSprungImage (const opal::SprungImage & image)
:  _image_ptr (&image)
,  _margin ()
,  _color (1.f, 1.f, 1.f, 1.f)
,  _scale_border_flag (false)
,  _margined_flag (false)
,  _hint_flag (false)
,  _hint_absolute_flag (false)
{
   assert (&image != 0);
}



/*
==============================================================================
Name : ctor
Note :
   Assume that the lifespan of 'image' is greater than the one of HelperSprungImage
==============================================================================
*/

HelperSprungImage::HelperSprungImage (const HelperSprungImage & other)
:  _image_ptr (other._image_ptr)
,  _margin ()
,  _color (1.f, 1.f, 1.f, 1.f)
,  _scale_border_flag (false)
,  _margined_flag (false)
,  _hint_flag (false)
,  _hint_absolute_flag (false)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

HelperSprungImage::~HelperSprungImage ()
{
}



/*
==============================================================================
Name : rect
==============================================================================
*/

HelperSprungImage &  HelperSprungImage::rect (const opa::Rect & rect)
{
   _rect = rect;
   
   return *this;
}



/*
==============================================================================
Name : rect
==============================================================================
*/

HelperSprungImage &  HelperSprungImage::rect (const opa::Point & origin, const opa::Size & size)
{
   _rect = opa::Rect (origin, size);
   
   return *this;
}



/*
==============================================================================
Name : rect
==============================================================================
*/

HelperSprungImage &  HelperSprungImage::rect (float x, float y, float width, float height)
{
   _rect = opa::Rect (x, y, width, height);
   
   return *this;
}



/*
==============================================================================
Name : margin
Description :
   Add *untransformed* margins (that is multiplied by context inv_px)
==============================================================================
*/

HelperSprungImage &  HelperSprungImage::margin (const opa::Margin & margin)
{
   _margined_flag = true;
   
   _margin += margin;
   
   return *this;
}



/*
==============================================================================
Name : margin
Description :
   Add *untransformed* margins (that is multiplied by context inv_px)
==============================================================================
*/

HelperSprungImage &  HelperSprungImage::margin (float left, float top, float right, float bottom)
{
   _margined_flag = true;
   
   _margin += opa::Margin (left, top, right, bottom);
   
   return *this;
}



/*
==============================================================================
Name : color
Description :
   Set color
==============================================================================
*/

HelperSprungImage &  HelperSprungImage::color (const opal::Color & color)
{
   _color = color;
   
   return *this;
}



/*
==============================================================================
Name : scale_border
==============================================================================
*/

HelperSprungImage &  HelperSprungImage::scale_border ()
{
   _scale_border_flag = true;
   
   return *this;
}



/*
==============================================================================
Name : hint
==============================================================================
*/

HelperSprungImage &  HelperSprungImage::hint ()
{
   _hint_flag = true;
   _hint_absolute_flag = false;
   
   return *this;
}



/*
==============================================================================
Name : hint_absolute
==============================================================================
*/

HelperSprungImage &  HelperSprungImage::hint_absolute ()
{
   _hint_flag = true;
   _hint_absolute_flag = true;
   
   return *this;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_HelperSprungImage_CODEHEADER_INCLUDED

#undef ohm_opak_HelperSprungImage_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
