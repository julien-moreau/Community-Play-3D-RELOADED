/*****************************************************************************

        HelperImage.h
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



#if ! defined (ohm_opak_HelperImage_HEADER_INCLUDED)
#define  ohm_opak_HelperImage_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Margin.h"
#include "ohm/opa/Rect.h"
#include "ohm/opa/Quad.h"
#include "ohm/opal/Color.h"
#include "ohm/opal/Image.h"



namespace ohm
{
namespace opak
{



class GraphicContext;

// DON'T INHERIT FROM THIS CLASS

class HelperImage
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   inline         HelperImage (const opal::Image & image);
   inline         HelperImage (const HelperImage &other);
   inline         ~HelperImage ();  // NOT VIRTUAL
   
   inline HelperImage &
                  point (const opa::Point & pt);
   inline HelperImage &
                  point (float x, float y);
   inline HelperImage &
                  scale ();

   inline HelperImage &
                  rect (const opa::Rect & rect);
   inline HelperImage &
                  rect (const opa::Point & origin, const opa::Size & size);
   inline HelperImage &
                  rect (float x, float y, float width, float height);
   inline HelperImage &
                  margin (const opa::Margin & margin);
   inline HelperImage &
                  margin (float left, float top, float right, float bottom);

   inline HelperImage &
                  quad (const opa::Quad & quad);

   inline HelperImage &
                  tex_rect (const opa::Rect & rect);

   inline HelperImage &
                  color (const opal::Color & color);
   
   inline HelperImage &
                  hint ();
   
   inline void    display (opak::GraphicContext & context);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           display_type_point (opak::GraphicContext & context);
   void           display_type_rect (opak::GraphicContext & context);
   void           display_type_rect_tex_rect (opak::GraphicContext & context);
   void           display_type_quad (opak::GraphicContext & context);
   void           display_type_quad_tex_rect (opak::GraphicContext & context);

   enum Type
   {
                  Type_POINT = 0,
                  Type_RECT,
                  Type_RECT_TEX_RECT,
                  Type_QUAD,
                  Type_QUAD_TEX_RECT
   };
   
   const opal::Image *
                  _image_ptr;
   
   Type           _type;
   
   opa::Point     _pt;
   
   opa::Rect      _rect;
   opa::Margin    _margin;
   
   opa::Rect      _tex_rect;
   
   opa::Quad      _quad;
   
   opal::Color    _color;
   
   bool           _scale_flag;
   bool           _margined_flag;
   bool           _hint_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  HelperImage ();
   HelperImage &  operator = (const HelperImage &other);
   bool           operator == (const HelperImage &other);
   bool           operator != (const HelperImage &other);

}; // class HelperImage



inline HelperImage   use (const opal::Image & image);



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/HelperImage.hpp"



#endif   // ohm_opak_HelperImage_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
