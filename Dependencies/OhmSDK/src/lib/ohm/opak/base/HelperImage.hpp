/*****************************************************************************

        HelperImage.hpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 63288

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



#if defined (ohm_opak_HelperImage_CURRENT_CODEHEADER)
   #error Recursive inclusion of HelperImage code header.
#endif
#define  ohm_opak_HelperImage_CURRENT_CODEHEADER

#if ! defined (ohm_opak_HelperImage_CODEHEADER_INCLUDED)
#define  ohm_opak_HelperImage_CODEHEADER_INCLUDED



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

HelperImage use (const opal::Image & image)
{
   HelperImage ret_val (image);
   
   return ret_val;
}



/*
==============================================================================
Name : ctor
Note :
   Assume that the lifespan of 'image' is greater than the one of HelperImage
==============================================================================
*/

HelperImage::HelperImage (const opal::Image & image)
:  _image_ptr (&image)
,  _type (Type_POINT)
,  _margin ()
,  _color (1.f, 1.f, 1.f, 1.f)
,  _scale_flag (false)
,  _margined_flag (false)
,  _hint_flag (false)
{
   assert (&image != 0);
}



/*
==============================================================================
Name : ctor
Note :
   Assume that the lifespan of 'image' is greater than the one of HelperImage
==============================================================================
*/

HelperImage::HelperImage (const HelperImage & other)
:  _image_ptr (other._image_ptr)
,  _type (Type_POINT)
,  _margin ()
,  _color (1.f, 1.f, 1.f, 1.f)
,  _scale_flag (false)
,  _margined_flag (false)
,  _hint_flag (false)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

HelperImage::~HelperImage ()
{
}



/*
==============================================================================
Name : point
==============================================================================
*/

HelperImage &  HelperImage::point (const opa::Point & pt)
{
   _type = Type_POINT;
   
   _pt = pt;
   
   return *this;
}



/*
==============================================================================
Name : point
==============================================================================
*/

HelperImage &  HelperImage::point (float x, float y)
{
   _type = Type_POINT;
   
   _pt = opa::Point (x, y);
   
   return *this;
}



/*
==============================================================================
Name : scale
==============================================================================
*/

HelperImage &  HelperImage::scale ()
{
   assert (_type == Type_POINT);
   
   _scale_flag = true;
   
   return *this;
}



/*
==============================================================================
Name : rect
==============================================================================
*/

HelperImage &  HelperImage::rect (const opa::Rect & rect)
{
   _type = Type_RECT;
   
   _rect = rect;
   
   return *this;
}



/*
==============================================================================
Name : rect
==============================================================================
*/

HelperImage &  HelperImage::rect (const opa::Point & origin, const opa::Size & size)
{
   _type = Type_RECT;
   
   _rect = opa::Rect (origin, size);
   
   return *this;
}



/*
==============================================================================
Name : rect
==============================================================================
*/

HelperImage &  HelperImage::rect (float x, float y, float width, float height)
{
   _type = Type_RECT;
   
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

HelperImage &  HelperImage::margin (const opa::Margin & margin)
{
   assert (_type == Type_RECT);
   
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

HelperImage &  HelperImage::margin (float left, float top, float right, float bottom)
{
   assert (_type == Type_RECT);
   
   _margined_flag = true;
   
   _margin += opa::Margin (left, top, right, bottom);
   
   return *this;
}



/*
==============================================================================
Name : quad
==============================================================================
*/

HelperImage &  HelperImage::quad (const opa::Quad & quad)
{
   _type = Type_QUAD;
   
   _quad = quad;
   
   return *this;
}



/*
==============================================================================
Name : tex_rect
==============================================================================
*/

HelperImage &  HelperImage::tex_rect (const opa::Rect & rect)
{
   assert ((_type == Type_RECT) || (_type == Type_QUAD));
   
   _tex_rect = rect;
   
   if (_type == Type_RECT)
   {
      _type = Type_RECT_TEX_RECT;
   }
   else
   {
      _type = Type_QUAD_TEX_RECT;
   }
   
   return *this;
}



/*
==============================================================================
Name : color
Description :
   Set color
==============================================================================
*/

HelperImage &  HelperImage::color (const opal::Color & color)
{
   _color = color;
   
   return *this;
}



/*
==============================================================================
Name : hint
==============================================================================
*/

HelperImage &  HelperImage::hint ()
{
   _hint_flag = true;
   
   return *this;
}



/*
==============================================================================
Name : display
==============================================================================
*/

void  HelperImage::display (opak::GraphicContext & context)
{
   switch (_type)
   {
   case Type_POINT:
      display_type_point (context);
      break;

   case Type_RECT:
      display_type_rect (context);
      break;

   case Type_RECT_TEX_RECT:
      display_type_rect_tex_rect (context);
      break;

   case Type_QUAD:
      display_type_quad (context);
      break;

   case Type_QUAD_TEX_RECT:
      display_type_quad_tex_rect (context);
      break;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



#endif   // ohm_opak_HelperImage_CODEHEADER_INCLUDED

#undef ohm_opak_HelperImage_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
