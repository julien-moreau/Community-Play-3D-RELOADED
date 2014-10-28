/*****************************************************************************

        ColorEngine.cpp
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

#include "ohm/opal/ColorEngine.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/lang/fnc.h"

#include <cassert>
#include <cmath>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

ColorEngine::~ColorEngine ()
{
}



/*
==============================================================================
Name : to_CIEXYZ
==============================================================================
*/

void  ColorEngine::to_CIEXYZ (ColorXYZ & cie_xyz, const ColorLab & cie_lab_d50)
{
   float fy = (cie_lab_d50._L + 16.f) * (1.f / 116.f);
   float fx = fy + cie_lab_d50._a * (1.f / 500.f);
   float fz = fy - cie_lab_d50._b * (1.f / 200.f);
   
   static const float delta = 6.f / 29.f;
   static const float tds = 3.f * delta * delta;

   static const float d50_X = 0.96422f;
   static const float d50_Z = 0.82521f;
   
   if (fy > delta)
   {
      cie_xyz._Y = fy * fy * fy;
   }
   else
   {
      cie_xyz._Y = (fy - (16.f / 116.f)) * tds;
   }

   if (fx > delta)
   {
      cie_xyz._X = fx * fx * fx * d50_X;
   }
   else
   {
      cie_xyz._X = (fx - (16.f / 116.f)) * tds * d50_X;
   }

   if (fz > delta)
   {
      cie_xyz._Z = fz * fz * fz * d50_Z;
   }
   else
   {
      cie_xyz._Z = (fz - (16.f / 116.f)) * tds * d50_Z;
   }
}



/*
==============================================================================
Name : to_sRGB
==============================================================================
*/

void  ColorEngine::to_sRGB (Color & color, const ColorXYZ & cie_xyz)
{
   /*
   3.1338561 -0.9787684  0.0719453
   -1.6168667  1.9161415 -0.2289914
   -0.4906146  0.0334540  1.4052427
   */
   
   color._a = 1.f;
   color._r = lang::limit (
      cie_xyz._X * 3.1338561f    + cie_xyz._Y * (-1.6168667f) + cie_xyz._Z * (-0.4906146f),
      0.f, 1.f
   );
   
   if (color._r <= 0.0031308f)
   {
      color._r *= 12.92f;
   }
   else
   {
      color._r = (1.055f) * pow (color._r, 0.4166666f) - 0.055f;
   }
   
   color._g = lang::limit (
      cie_xyz._X * (-0.9787684f) + cie_xyz._Y * 1.9161415f    + cie_xyz._Z * 0.0334540f,
      0.f, 1.f
   );

   if (color._g <= 0.0031308f)
   {
      color._g *= 12.92f;
   }
   else
   {
      color._g = (1.055f) * pow (color._g, 0.4166666f) - 0.055f;
   }

   color._b = lang::limit (
      cie_xyz._X * 0.0719453f    + cie_xyz._Y * (-0.2289914f) + cie_xyz._Z * 1.4052427f,
      0.f, 1.f
   );
   
   if (color._b <= 0.0031308f)
   {
      color._b *= 12.92f;
   }
   else
   {
      color._b = (1.055f) * pow (color._b, 0.4166666f) - 0.055f;
   }
   
   lang::limit_in_place (color._r, 0.f, 1.f);
   lang::limit_in_place (color._g, 0.f, 1.f);
   lang::limit_in_place (color._b, 0.f, 1.f);
}



/*
==============================================================================
Name : to_sRGB
==============================================================================
*/

void  ColorEngine::to_sRGB (ColorGradient & dst_gradient, const ColorXYZGradient & src_gradient)
{
   for (size_t i = 0 ; i < NBR_GRADIENT_COLOR ; ++i)
   {
      to_sRGB (dst_gradient._color_arr [i], src_gradient._cie_xyz_arr [i]);
   }
}



/*
==============================================================================
Name : to_sRGB
==============================================================================
*/

void  ColorEngine::to_sRGB (PixArgb & color, const ColorXYZ & cie_xyz)
{
   Color temp;
   to_sRGB (temp, cie_xyz);

   color._a = 0xFF;
   color._r = archi::UByte (temp._r * 255.f);
   color._g = archi::UByte (temp._g * 255.f);
   color._b = archi::UByte (temp._b * 255.f);
}



/*
==============================================================================
Name : to_sRGB
==============================================================================
*/

void  ColorEngine::to_sRGB (PixArgbGradient & dst_gradient, const ColorXYZGradient & src_gradient)
{
   for (size_t i = 0 ; i < NBR_GRADIENT_COLOR ; ++i)
   {
      to_sRGB (dst_gradient._pix_argb_arr [i], src_gradient._cie_xyz_arr [i]);
   }
}



/*
==============================================================================
Name : to_screen
==============================================================================
*/

void  ColorEngine::to_screen (Color & color, const ColorXYZ & cie_xyz)
{
   to_sRGB (color, cie_xyz);
}



/*
==============================================================================
Name : to_screen
==============================================================================
*/

void  ColorEngine::to_screen (Color & color, const Color & color_srgb)
{
   // for now
   
   color = color_srgb;
}



/*
==============================================================================
Name : make_cielab_d50_luminosity_gradient
==============================================================================
*/

void  ColorEngine::make_cielab_d50_luminosity_gradient (ColorXYZGradient & gradient, float a, float b)
{
   float inv_l_nbr_color = 100.f / float (NBR_GRADIENT_COLOR - 1);
   
   ColorLab cie_lab_d50;
   cie_lab_d50._a = a;
   cie_lab_d50._b = b;
   
   for (size_t i = 0 ; i < NBR_GRADIENT_COLOR ; ++i)
   {
      cie_lab_d50._L = float (i) * inv_l_nbr_color;
      
      to_CIEXYZ (gradient._cie_xyz_arr [i], cie_lab_d50);
   }
}



/*
==============================================================================
Name : make_cielab_d50_luminosity_gradient
==============================================================================
*/

void  ColorEngine::make_cielab_d50_luminosity_gradient (ColorXYZGradient & gradient, float a, float b, float L_min, float L_max)
{
   float inv_l_nbr_color = 1.f / float (NBR_GRADIENT_COLOR - 1);
   
   ColorLab cie_lab_d50;
   cie_lab_d50._a = a;
   cie_lab_d50._b = b;
   
   for (size_t i = 0 ; i < NBR_GRADIENT_COLOR ; ++i)
   {
      cie_lab_d50._L = L_min + (L_max - L_min) * float (i) * inv_l_nbr_color;
      
      to_CIEXYZ (gradient._cie_xyz_arr [i], cie_lab_d50);
   }
}



/*
==============================================================================
Name : make_cielab_d50_luminosity_gradient
==============================================================================
*/

void  ColorEngine::make_cielab_d50_luminosity_gradient (PixArgbGradient & gradient, float a, float b, float L_min, float L_max)
{
   float inv_l_nbr_color = 1.f / float (NBR_GRADIENT_COLOR - 1);
   
   ColorLab cie_lab_d50;
   cie_lab_d50._a = a;
   cie_lab_d50._b = b;
   
   float lmin = L_min;
   float lmax = L_max;
   
   if (lmax < lmin)
   {
      std::swap (lmin, lmax);
   }
      
   for (size_t i = 0 ; i < NBR_GRADIENT_COLOR ; ++i)
   {
      cie_lab_d50._L = L_min + (L_max - L_min) * float (i) * inv_l_nbr_color;
      
      lang::limit_in_place (cie_lab_d50._L, lmin, lmax);
      
      ColorXYZ cie_xyz;
      
      to_CIEXYZ (cie_xyz, cie_lab_d50);
      
      to_sRGB (gradient._pix_argb_arr [i], cie_xyz);
   }
}



/*
==============================================================================
Name : make_cielab_d50_luminosity_gradient
==============================================================================
*/

void  ColorEngine::make_cielab_d50_luminosity_gradient (PixArgbGradient & gradient, const ColorLab & color_min, const ColorLab & color_max, const ColorProcessorInterface * color_proc_ptr)
{
   float inv_l_nbr_color = 1.f / float (NBR_GRADIENT_COLOR - 1);
   
   ColorLab cie_lab_d50;
   
   float lmin = color_min._L;
   float lmax = color_max._L;
   float dl = color_max._L - color_min._L;
   
   if (lmax < lmin)
   {
      std::swap (lmin, lmax);
   }

   float da = color_max._a - color_min._a;
   float db = color_max._b - color_min._b;
   
   for (size_t i = 0 ; i < NBR_GRADIENT_COLOR ; ++i)
   {
      cie_lab_d50._L = color_min._L + dl * float (i) * inv_l_nbr_color;
      cie_lab_d50._L = color_min._a + da * float (i) * inv_l_nbr_color;
      cie_lab_d50._L = color_min._b + db * float (i) * inv_l_nbr_color;
      
      lang::limit_in_place (cie_lab_d50._L, lmin, lmax);
      
      ColorXYZ cie_xyz;
      
      to_CIEXYZ (cie_xyz, cie_lab_d50);
      
      to_sRGB (gradient._pix_argb_arr [i], cie_xyz);
   }
}



/*
==============================================================================
Name : colorize_premultiply_in_place
Note :
   The function assumes that dst is *not* premultiplied.
==============================================================================
*/

void  ColorEngine::colorize_premultiply_in_place (Bitmap & dst, const PixArgbGradient & gradient)
{
   const size_t dst_size = dst.size ();
   
   for (size_t i = 0 ; i < dst_size ; ++i)
   {
      PixArgb & dst_pix = dst [i];
      
      // refer to red channel only
      archi::UByte idx = dst_pix._r;
      
      const PixArgb & src_pix = gradient._pix_argb_arr [idx];
      
      int r = src_pix._r;
      int g = src_pix._g;
      int b = src_pix._b;
      int a = dst_pix._a;
      
      r = (r * a) >> 8;
      g = (g * a) >> 8;
      b = (b * a) >> 8;
      
      // preserve dst alpha
      dst_pix._r = r;
      dst_pix._g = g;
      dst_pix._b = b;
   }
}



/*
==============================================================================
Name : colorize_premultiply_in_place
Note :
   The function assumes that dst is *not* premultiplied.
==============================================================================
*/

void  ColorEngine::colorize_premultiply_in_place (Bitmap & dst, const PixArgbGradient & gradient, int idx_min, int idx_max)
{
   lang::limit (idx_min, 0, int (NBR_GRADIENT_COLOR - 1));
   lang::limit (idx_max, 0, int (NBR_GRADIENT_COLOR - 1));
   
   if ((idx_min == 0) && (idx_max == NBR_GRADIENT_COLOR - 1))
   {
      colorize_premultiply_in_place (dst, gradient);
   }
   else if (idx_min == idx_max)
   {
      const size_t dst_size = dst.size ();
      
      const PixArgb & src_pix = gradient._pix_argb_arr [idx_min];
      
      for (size_t i = 0 ; i < dst_size ; ++i)
      {
         PixArgb & dst_pix = dst [i];
         
         int r = src_pix._r;
         int g = src_pix._g;
         int b = src_pix._b;
         int a = dst_pix._a;
         
         r = (r * a) >> 8;
         g = (g * a) >> 8;
         b = (b * a) >> 8;
         
         // preserve dst alpha
         dst_pix._r = r;
         dst_pix._g = g;
         dst_pix._b = b;
      }
   }
   else
   {
      const size_t dst_size = dst.size ();
      
      if (dst_size > NBR_GRADIENT_COLOR)
      {
         // make a table
         
         archi::UByte idx_arr [NBR_GRADIENT_COLOR];
         
         size_t i;
         
         for (i = 0 ; i < NBR_GRADIENT_COLOR ; ++i)
         {
            float idx_f
               = float (idx_min) + float (i) * (1.f / 255.f) * (idx_max - idx_min);
               
            idx_arr [i] = archi::UByte (idx_f);
         }
         
         for (i = 0 ; i < dst_size ; ++i)
         {
            PixArgb & dst_pix = dst [i];
            
            // refer to red channel only
            archi::UByte idx = dst_pix._r;
            
            idx = idx_arr [idx];
            
            const PixArgb & src_pix = gradient._pix_argb_arr [idx];
            
            int r = src_pix._r;
            int g = src_pix._g;
            int b = src_pix._b;
            int a = dst_pix._a;
            
            r = (r * a) >> 8;
            g = (g * a) >> 8;
            b = (b * a) >> 8;
            
            // preserve dst alpha
            dst_pix._r = r;
            dst_pix._g = g;
            dst_pix._b = b;
         }
      }
      else
      {
         // just calculate in place
         
         for (size_t i = 0 ; i < dst_size ; ++i)
         {
            PixArgb & dst_pix = dst [i];
            
            // refer to red channel only
            archi::UByte idx = dst_pix._r;
            
            // 0 -> idx_min
            // 255 -> idx_max
            
            float idx_f
               = float (idx_min) + float (idx) * (1.f / 255.f) * (idx_max - idx_min);
            
            lang::limit_in_place (idx_f, 0.f, 255.f);
            
            idx = archi::UByte (idx_f);
            
            const PixArgb & src_pix = gradient._pix_argb_arr [idx];
            
            int r = src_pix._r;
            int g = src_pix._g;
            int b = src_pix._b;
            int a = dst_pix._a;
            
            r = (r * a) >> 8;
            g = (g * a) >> 8;
            b = (b * a) >> 8;
            
            // preserve dst alpha
            dst_pix._r = r;
            dst_pix._g = g;
            dst_pix._b = b;
         }
      }
   }
}



/*
==============================================================================
Name : colorize_premultiply_in_place
Note :
   The function assumes that dst is *not* premultiplied.
==============================================================================
*/

void  ColorEngine::colorize_premultiply_in_place (Bitmap & dst, const Color & color)
{
   PixArgb pix;
   pix._a = archi::UByte (color._a * 255.f);
   pix._r = archi::UByte (color._r * 255.f);
   pix._g = archi::UByte (color._g * 255.f);
   pix._b = archi::UByte (color._b * 255.f);
   
   colorize_premultiply_in_place (dst, pix);
}



/*
==============================================================================
Name : colorize_premultiply_in_place
Note :
   The function assumes that dst is *not* premultiplied.
==============================================================================
*/

void  ColorEngine::colorize_premultiply_in_place (Bitmap & dst, const PixArgb & color)
{
   const size_t dst_size = dst.size ();
   
   for (size_t i = 0 ; i < dst_size ; ++i)
   {
      PixArgb & dst_pix = dst [i];
      
      int r = color._r;
      int g = color._g;
      int b = color._b;
      int a = dst_pix._a;
      
      r = (r * a) >> 8;
      g = (g * a) >> 8;
      b = (b * a) >> 8;
      
      // preserve dst alpha
      dst_pix._r = r;
      dst_pix._g = g;
      dst_pix._b = b;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

ColorEngine::ColorEngine ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
