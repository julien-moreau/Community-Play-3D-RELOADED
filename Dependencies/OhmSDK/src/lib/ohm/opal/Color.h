/*****************************************************************************

        Color.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54636

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



#if ! defined (ohm_opal_Color_HEADER_INCLUDED)
#define  ohm_opal_Color_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/PixArgb.h"



namespace ohm
{
namespace opal
{



class Bitmap;
class ColorLab;

class Color
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static Color   interpolate (const Color & color_0, const Color & color_1, float mix_factor);
   
                  Color ();
                  ~Color ();
                  Color (float a, float r, float g, float b);
                  Color (int a8, int r8, int g8, int b8);
                  Color (float r, float g, float b);
                  Color (int r8, int g8, int b8);
                  Color (float a, float gray);
                  Color (int a8, int gray8);
   explicit       Color (float gray);
   explicit       Color (int gray8);

                  Color (const Color & other);
                  Color (const ColorLab & other);

   Color &        operator = (const Color & other);
   Color &        operator = (const ColorLab & other);
   
   bool           operator == (const Color &other) const;
   bool           operator != (const Color &other) const;
   
   bool           operator < (const Color & other) const;
   
   void           colorize (Bitmap & bitmap);
   
   PixArgb        to_PixArgb () const;
   
   float          _a;
   float          _r;
   float          _g;
   float          _b;
   
   static const Color
                  _none;
   static const Color
                  _black;
   static const Color
                  _white;
   static const Color
                  _yellow;
   static const Color
                  _red;

}; // class Color



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_Color_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
