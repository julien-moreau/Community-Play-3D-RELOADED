/*****************************************************************************

        PrimitiveHelper.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 58260

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



#if ! defined (ohm_opak_PrimitiveHelper_HEADER_INCLUDED)
#define  ohm_opak_PrimitiveHelper_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace opa
{
class Rect;
class Point;
class Quad;
}



namespace opal
{
class Image;
class SprungImage;
class Color;
class LabelString;
class AreaString;
}



namespace opak
{



class GraphicContext;

class PrimitiveHelper
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static void    display (opak::GraphicContext & context, const opa::Rect & rect, const opal::Color & color, bool hinted_flag);

   static void    display (opak::GraphicContext & context, const opal::Image & image, const opa::Point & pt, bool scale_image_flag, bool hinted_flag);
   static void    display (opak::GraphicContext & context, const opal::Image & image, const opa::Rect & rect, bool hinted_flag);
   static void    display (opak::GraphicContext & context, const opal::Image & image, const opa::Rect & rect, const opa::Rect & tex_rect, bool hinted_flag);
   static void    display (opak::GraphicContext & context, const opal::Image & image, const opa::Quad & quad, bool hinted_flag);

   static void    display (opak::GraphicContext & context, const opal::Image & mask, const opal::Color & color, const opa::Point & pt, bool scale_image_flag, bool hinted_flag);
   static void    display (opak::GraphicContext & context, const opal::Image & mask, const opal::Color & color, const opa::Rect & rect, bool hinted_flag);

   static void    display (opak::GraphicContext & context, const opal::SprungImage & sprung_image, const opa::Rect & rect, bool transform_borders_flag, bool hinted_flag);
   static void    display (opak::GraphicContext & context, const opal::SprungImage & sprung_mask, const opal::Color & color, const opa::Rect & rect, bool transform_borders_flag, bool hinted_flag);

   static void    display (opak::GraphicContext & context, const opal::LabelString & label_str, const opa::Point & pt, float flush_factor);
   static bool    display (opak::GraphicContext & context, const opal::AreaString & area_str, const opa::Point & pt);
   static bool    display (opak::GraphicContext & context, const opal::AreaString & area_str, const opa::Point & pt, size_t caret_first, size_t caret_second, const opal::Color & caret_color, const opal::Color & selection_color);

   static void    display_line (opak::GraphicContext & context, const opal::Image & line_image, const opa::Point & pt_0, const opa::Point & pt_1, float & phase, bool hinted_flag);
   static void    display_line (opak::GraphicContext & context, const opal::Image & line_mask, const opal::Color & color, const opa::Point & pt_0, const opa::Point & pt_1, float & phase, bool hinted_flag);

   static void    display_point (opak::GraphicContext & context, const opal::Image & pt_image, const opa::Point & pt, bool hinted_flag);
   static void    display_point (opak::GraphicContext & context, const opal::Image & pt_mask, const opal::Color & color, const opa::Point & pt, bool hinted_flag);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
                  PrimitiveHelper ();
   virtual        ~PrimitiveHelper ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimitiveHelper (const PrimitiveHelper &other);
   PrimitiveHelper &
                  operator = (const PrimitiveHelper &other);
   bool           operator == (const PrimitiveHelper &other);
   bool           operator != (const PrimitiveHelper &other);

}; // class PrimitiveHelper



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/base/PrimitiveHelper.hpp"



#endif   // ohm_opak_PrimitiveHelper_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
