/*****************************************************************************

        HelperSprungImage.h
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



#if ! defined (ohm_opak_HelperSprungImage_HEADER_INCLUDED)
#define  ohm_opak_HelperSprungImage_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Margin.h"
#include "ohm/opa/Rect.h"
#include "ohm/opal/Color.h"
#include "ohm/opal/SprungImage.h"



namespace ohm
{
namespace opak
{



class GraphicContext;

// DON'T INHERIT FROM THIS CLASS

class HelperSprungImage
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   inline         HelperSprungImage (const opal::SprungImage & image);
   inline         HelperSprungImage (const HelperSprungImage &other);
   inline         ~HelperSprungImage ();  // NOT VIRTUAL
   
   inline HelperSprungImage &
                  rect (const opa::Rect & rect);
   inline HelperSprungImage &
                  rect (const opa::Point & origin, const opa::Size & size);
   inline HelperSprungImage &
                  rect (float x, float y, float width, float height);
   inline HelperSprungImage &
                  margin (const opa::Margin & margin);
   inline HelperSprungImage &
                  margin (float left, float top, float right, float bottom);
   inline HelperSprungImage &
                  color (const opal::Color & color);
   
   inline HelperSprungImage &
                  scale_border ();
   inline HelperSprungImage &
                  hint ();    // relative (preserve size)
   inline HelperSprungImage &
                  hint_absolute ();    // absolute (preserve point)
   
   void           display (opak::GraphicContext & context);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   const opal::SprungImage *
                  _image_ptr;
   
   opa::Rect      _rect;
   opa::Margin    _margin;
   opal::Color    _color;
   
   bool           _scale_border_flag;
   bool           _margined_flag;
   bool           _hint_flag;
   bool           _hint_absolute_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  HelperSprungImage ();
   HelperSprungImage &  operator = (const HelperSprungImage &other);
   bool           operator == (const HelperSprungImage &other);
   bool           operator != (const HelperSprungImage &other);

}; // class HelperSprungImage



inline HelperSprungImage   use (const opal::SprungImage & image);



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#include "ohm/opak/base/HelperSprungImage.hpp"



#endif   // ohm_opak_HelperSprungImage_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
