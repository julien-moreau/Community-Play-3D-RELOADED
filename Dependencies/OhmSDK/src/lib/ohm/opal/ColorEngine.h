/*****************************************************************************

        ColorEngine.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54317

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



#if ! defined (ohm_opal_ColorEngine_HEADER_INCLUDED)
#define  ohm_opal_ColorEngine_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Color.h"
#include "ohm/opal/ColorLab.h"
#include "ohm/opal/ColorXYZ.h"
#include "ohm/opal/PixArgb.h"
#include "ohm/util/SingletonBase.h"



namespace ohm
{
namespace opal
{



class Bitmap;
class ColorProcessorInterface;

class ColorEngine
:  public util::SingletonBase <ColorEngine>
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum {   NBR_GRADIENT_COLOR = 256   };
   
   class ColorXYZGradient
   {
   public:
      ColorXYZ    _cie_xyz_arr [NBR_GRADIENT_COLOR];
   };

   class ColorGradient
   {
   public:
      Color       _color_arr [NBR_GRADIENT_COLOR];
   };

   class PixArgbGradient
   {
   public:
      PixArgb     _pix_argb_arr [NBR_GRADIENT_COLOR];
   };
   
   virtual        ~ColorEngine ();
   
   void           to_CIEXYZ (ColorXYZ & cie_xyz, const ColorLab & cie_lab_d50);
   void           to_sRGB (Color & color, const ColorXYZ & cie_xyz);
   void           to_sRGB (ColorGradient & dst_gradient, const ColorXYZGradient & src_gradient);
   void           to_sRGB (PixArgb & color, const ColorXYZ & cie_xyz);
   void           to_sRGB (PixArgbGradient & dst_gradient, const ColorXYZGradient & src_gradient);
   
   void           to_screen (Color & color, const ColorXYZ & cie_xyz);
   void           to_screen (Color & color, const Color & color_srgb);
   
   void           make_cielab_d50_luminosity_gradient (ColorXYZGradient & gradient, float a, float b);
   void           make_cielab_d50_luminosity_gradient (ColorXYZGradient & gradient, float a, float b, float L_min, float L_max);

   void           make_cielab_d50_luminosity_gradient (PixArgbGradient & gradient, float a, float b, float L_min, float L_max);
   void           make_cielab_d50_luminosity_gradient (PixArgbGradient & gradient, const ColorLab & color_min, const ColorLab & color_max, const ColorProcessorInterface * color_proc_ptr = 0);
   
   void           colorize_premultiply_in_place (Bitmap & dst, const PixArgbGradient & gradient);
   void           colorize_premultiply_in_place (Bitmap & dst, const PixArgbGradient & gradient, int idx_min, int idx_max);
   void           colorize_premultiply_in_place (Bitmap & dst, const Color & color);
   void           colorize_premultiply_in_place (Bitmap & dst, const PixArgb & color);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
                  ColorEngine ();
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   ohm_util_SingletonBase_INVITE_SINGLETON (ColorEngine)



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ColorEngine (const ColorEngine &other);
   ColorEngine &        operator = (const ColorEngine &other);
   bool           operator == (const ColorEngine &other);
   bool           operator != (const ColorEngine &other);

}; // class ColorEngine



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_ColorEngine_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
