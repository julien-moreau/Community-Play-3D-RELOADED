/*****************************************************************************

        Downsampler.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38770

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

#include "ohm/opal/dsp/Downsampler.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/lang/fnc.h"

#include <cassert>
#include <cmath>
#include <vector>


namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Downsampler::Downsampler ()
:  _method (Method_NEAREST)
,  _width (0)
,  _height (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Downsampler::~Downsampler ()
{
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  Downsampler::set (Method method)
{
   _method = method;
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  Downsampler::set (archi::UInt32 width, archi::UInt32 height)
{
   _width = width;
   _height = height;
}



/*
==============================================================================
Name : process
==============================================================================
*/

void  Downsampler::process (Bitmap & dst, const Bitmap & src)
{
   // lanczos2 (x) = sinc (pi * x) * sinc (pic * x * .5); |x| < 2
   // 0 otherwise
   // normalize
   
   
   // quick nearest to test

   if (_method == Method_NEAREST)
   {
      dst.resize (_width, _height);
      
      float xf = float (src.get_width () - 1) / float (_width - 1);
      float yf = float (src.get_height () - 1) / float (_height - 1);
      
      for (size_t y = 0 ; y < _height ; ++y)
      {
         for (size_t x = 0 ; x < _width ; ++x)
         {
            dst (x, y) = src (size_t (floor (x * xf)), size_t (floor (y * yf)));
         }
      }
   }
   if (_method == Method_BILINEAR)
   {
      dst.resize (_width, _height);
      
      process_bilinear (dst, src);
   }
   else if (_method == Method_LANCZOS2)
   {
      Bitmap tmp;
      tmp.resize (_width, src.get_height ());
      
      process_lanczos2_x (tmp, src);
      
      dst.resize (_width, _height);
      
      process_lanczos2_y (dst, tmp);
   }
   else if (_method == Method_TURKOWSKI)
   {
      size_t dst_width = src.get_width ();
      size_t dst_height = src.get_height ();
      
      Bitmap tmp;
      
      bool first_flag = true;
      
      bool loop_flag = true;
      
      while (loop_flag)
      {
         const Bitmap * tsrc = &(first_flag ? src : tmp);
         first_flag = false;
         
         int break_nbr = 0;
         
         if ((dst_width >> 1) >= _width)
         {
            dst_width >>= 1;
            dst.resize (dst_width, dst_height);
            
            process_turkowski_x (dst, *tsrc);
            
            dst.swap (tmp);
            tsrc = &tmp;
            
            ++break_nbr;
         }
         
         if ((dst_height >> 1) >= _height)
         {
            dst_height >>= 1;
            dst.resize (dst_width, dst_height);
            
            process_turkowski_y (dst, *tsrc);
            
            dst.swap (tmp);
            
            ++break_nbr;
         }
         
         loop_flag = break_nbr == 0;
      }

#if 1
      dst.resize (_width, _height);
      
      process_bilinear (dst, tmp);
#elif 1
      dst.resize (_width, tmp.get_height ());
      process_lanczos2_x (dst, tmp);
      dst.swap (tmp);
      
      dst.resize (_width, _height);
      process_lanczos2_y (dst, tmp);
#endif
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process_bilinear
==============================================================================
*/

void  Downsampler::process_bilinear (Bitmap & dst, const Bitmap & src)
{
   float xf = float (src.get_width () - 1) / float (_width - 1);
   float yf = float (src.get_height () - 1) / float (_height - 1);
   
   archi::UInt32 xpp = archi::UInt32 (xf * 65535.f);
   archi::UInt32 ypp = archi::UInt32 (yf * 65535.f);
   
   const PixArgb * src_ptr = &src (0, 0);
   size_t scanline_width = src.get_width ();
   
   archi::UInt32 y1616 = 0;

   for (size_t y = 0 ; y < _height ; ++y)
   {
      archi::UInt32 yd = y1616 >> 16;
      archi::UInt32 yp = (y1616 & 65535) >> 8;
      
      const PixArgb * csrc_ptr = src_ptr;
      csrc_ptr += scanline_width * yd;
      
      archi::UInt32 x1616 = 0;
   
      for (size_t x = 0 ; x < _width ; ++x)
      {
         archi::UInt32 xd = x1616 >> 16;
         archi::UInt32 xp = (x1616 & 65535) >> 8;
         
         const PixArgb & spix00 = csrc_ptr [xd];
         const PixArgb & spix10 = csrc_ptr [xd + 1];
         const PixArgb & spix01 = csrc_ptr [xd + scanline_width];
         const PixArgb & spix11 = csrc_ptr [xd + 1 + scanline_width];
         
         PixArgb & dpix = dst (x, y);
         
         archi::UInt32 r0_16 = spix00._r << 8;
         r0_16 += archi::UInt32 (spix10._r - spix00._r) * xp;
         archi::UInt32 r1_16 = spix01._r << 8;
         r1_16 += archi::UInt32 (spix11._r - spix01._r) * xp;
         archi::UInt32 r_24 = r0_16 << 8;
         r_24 += archi::UInt32 (r1_16 - r0_16) * yp;

         archi::UInt32 g0_16 = spix00._g << 8;
         g0_16 += archi::UInt32 (spix10._g - spix00._g) * xp;
         archi::UInt32 g1_16 = spix01._g << 8;
         g1_16 += archi::UInt32 (spix11._g - spix01._g) * xp;
         archi::UInt32 g_24 = g0_16 << 8;
         g_24 += archi::UInt32 (g1_16 - g0_16) * yp;
         
         archi::UInt32 b0_16 = spix00._b << 8;
         b0_16 += archi::UInt32 (spix10._b - spix00._b) * xp;
         archi::UInt32 b1_16 = spix01._b << 8;
         b1_16 += archi::UInt32 (spix11._b - spix01._b) * xp;
         archi::UInt32 b_24 = b0_16 << 8;
         b_24 += archi::UInt32 (b1_16 - b0_16) * yp;
         
         archi::UInt32 a0_16 = spix00._a << 8;
         a0_16 += archi::UInt32 (spix10._a - spix00._a) * xp;
         archi::UInt32 a1_16 = spix01._a << 8;
         a1_16 += archi::UInt32 (spix11._a - spix01._a) * xp;
         archi::UInt32 a_24 = a0_16 << 8;
         a_24 += archi::UInt32 (a1_16 - a0_16) * yp;
         
         dpix._r = r_24 >> 16;
         dpix._g = g_24 >> 16;
         dpix._b = b_24 >> 16;
         dpix._a = a_24 >> 16;;
         
         x1616 += xpp;
      }
      
      y1616 += ypp;
   }
}



/*
==============================================================================
Name : process_lanczos2_x
==============================================================================
*/

void  Downsampler::process_lanczos2_x (Bitmap & dst, const Bitmap & src)
{
   const float src_to_dst_factor = float (dst.get_width () - 1) / float (src.get_width () - 1);
   const float dst_to_src_factor = 1.f / src_to_dst_factor;
   
   int rad = 2 * int (ceil (dst_to_src_factor));
   
   for (size_t y = 0 ; y < dst.get_height () ; ++y)
   {
      for (size_t x = 0 ; x < dst.get_width () ; ++x)
      {
         float x_src = float (x) * dst_to_src_factor;
         
         float xd = floor (x_src);
         int xdd = int (xd);
         float phase = x_src - xd;
         
         float r = 0;
         float g = 0;
         float b = 0;
         float a = 0;
         float l = 0;
         
         for (int i = -rad ; i <= rad ; ++i)
         {
            float lancz = lanczos2 (float (i) * src_to_dst_factor - phase);
            
            int xpos = xdd + i;
            lang::max_in_place (xpos, 0);
            lang::min_in_place (xpos, int (src.get_width () - 1));
            
            const PixArgb & src_pix = src (xpos, y);
            
            r += float (src_pix._r) * lancz;
            g += float (src_pix._g) * lancz;
            b += float (src_pix._b) * lancz;
            a += float (src_pix._a) * lancz;
            
            l += lancz;
         }
         
         float il = 1.f / l;
         r *= il;
         g *= il;
         b *= il;
         a *= il;
         
         lang::limit_in_place (r, 0.f, 255.f);
         lang::limit_in_place (g, 0.f, 255.f);
         lang::limit_in_place (b, 0.f, 255.f);
         lang::limit_in_place (a, 0.f, 255.f);
         
         PixArgb dst_pix;
         dst_pix._r = archi::UByte (r);
         dst_pix._g = archi::UByte (g);
         dst_pix._b = archi::UByte (b);
         dst_pix._a = archi::UByte (a);
         
         dst (x, y) = dst_pix;
      }
   }
}



/*
==============================================================================
Name : process_lanczos2_y
==============================================================================
*/

void  Downsampler::process_lanczos2_y (Bitmap & dst, const Bitmap & src)
{
   const float src_to_dst_factor = float (dst.get_height () - 1) / float (src.get_height () - 1);
   const float dst_to_src_factor = 1.f / src_to_dst_factor;
   
   int rad = 2 * int (ceil (dst_to_src_factor));
   
   for (size_t x = 0 ; x < dst.get_width () ; ++x)
   {
      for (size_t y = 0 ; y < dst.get_height () ; ++y)
      {
         float y_src = float (y) * dst_to_src_factor;
         
         float yd = floor (y_src);
         int ydd = int (yd);
         float phase = y_src - yd;
         
         float r = 0;
         float g = 0;
         float b = 0;
         float a = 0;
         float l = 0;
         
         for (int i = -rad ; i <= rad ; ++i)
         {
            float lancz = lanczos2 (float (i) * src_to_dst_factor - phase);
            
            int ypos = ydd + i;
            lang::max_in_place (ypos, 0);
            lang::min_in_place (ypos, int (src.get_height () - 1));
            
            const PixArgb & src_pix = src (x, ypos);
            
            r += float (src_pix._r) * lancz;
            g += float (src_pix._g) * lancz;
            b += float (src_pix._b) * lancz;
            a += float (src_pix._a) * lancz;
            
            l += lancz;
         }
         
         float il = 1.f / l;
         r *= il;
         g *= il;
         b *= il;
         a *= il;
         
         lang::limit_in_place (r, 0.f, 255.f);
         lang::limit_in_place (g, 0.f, 255.f);
         lang::limit_in_place (b, 0.f, 255.f);
         lang::limit_in_place (a, 0.f, 255.f);
         
         PixArgb dst_pix;
         dst_pix._r = archi::UByte (r);
         dst_pix._g = archi::UByte (g);
         dst_pix._b = archi::UByte (b);
         dst_pix._a = archi::UByte (a);
         
         dst (x, y) = dst_pix;
      }
   }
}



/*
==============================================================================
Name : process_turkowski_x
==============================================================================
*/

void  Downsampler::process_turkowski_x (Bitmap & dst, const Bitmap & src)
{
   const PixArgb * src_ptr = &src (0, 0);
   
   const size_t height = dst.get_height ();
   const size_t dst_width = dst.get_width ();
   const int src_width = src.get_width ();
   
   for (size_t y = 0 ; y < height ; ++y)
   {
      for (size_t x = 0 ; x < dst_width ; ++x)
      {
         int s0 = x << 1;
         
         int sn3 = s0 - 3;
         lang::max_in_place (sn3, 0);

         int sp3 = s0 + 3;
         lang::min_in_place (sp3, src_width - 1);
         
         int r = -(src_ptr [sn3]._r + src_ptr [sp3]._r);
         int g = -(src_ptr [sn3]._g + src_ptr [sp3]._g);
         int b = -(src_ptr [sn3]._b + src_ptr [sp3]._b);
         int a = -(src_ptr [sn3]._a + src_ptr [sp3]._a);
         
         int sn1 = s0 - 1;
         lang::max_in_place (sn1, 0);

         int sp1 = s0 + 1;
         lang::min_in_place (sp1, src_width - 1);
         
         r += 9 * (src_ptr [sn1]._r + src_ptr [sp1]._r);
         g += 9 * (src_ptr [sn1]._g + src_ptr [sp1]._g);
         b += 9 * (src_ptr [sn1]._b + src_ptr [sp1]._b);
         a += 9 * (src_ptr [sn1]._a + src_ptr [sp1]._a);

         r += 16 * (src_ptr [s0]._r);
         g += 16 * (src_ptr [s0]._g);
         b += 16 * (src_ptr [s0]._b);
         a += 16 * (src_ptr [s0]._a);
         
         lang::limit_in_place (r, 0, 8160);
         lang::limit_in_place (g, 0, 8160);
         lang::limit_in_place (b, 0, 8160);
         lang::limit_in_place (a, 0, 8160);
         
         PixArgb & dpix = dst (x, y);
         dpix._r = r >> 5;
         dpix._g = g >> 5;
         dpix._b = b >> 5;
         dpix._a = a >> 5;
      }
      
      src_ptr += src_width;
   }
}



/*
==============================================================================
Name : process_turkowski_y
==============================================================================
*/

void  Downsampler::process_turkowski_y (Bitmap & dst, const Bitmap & src)
{
   const PixArgb * src_ptr = &src (0, 0);
   
   const size_t width = dst.get_width ();
   const size_t dst_height = dst.get_height ();
   const int src_height = src.get_height ();
   const int src_width = src.get_width ();
   
   for (size_t x = 0 ; x < width ; ++x)
   {
      for (size_t y = 0 ; y < dst_height ; ++y)
      {
         int s0 = y << 1;
         
         int sn3 = s0 - 3;
         lang::max_in_place (sn3, 0);
         sn3 *= src_width;

         int sp3 = s0 + 3;
         lang::min_in_place (sp3, src_height - 1);
         sp3 *= src_width;
         
         int r = -(src_ptr [sn3]._r + src_ptr [sp3]._r);
         int g = -(src_ptr [sn3]._g + src_ptr [sp3]._g);
         int b = -(src_ptr [sn3]._b + src_ptr [sp3]._b);
         int a = -(src_ptr [sn3]._a + src_ptr [sp3]._a);
         
         int sn1 = s0 - 1;
         lang::max_in_place (sn1, 0);
         sn1 *= src_width;

         int sp1 = s0 + 1;
         lang::min_in_place (sp1, src_height - 1);
         sp1 *= src_width;
         
         r += 9 * (src_ptr [sn1]._r + src_ptr [sp1]._r);
         g += 9 * (src_ptr [sn1]._g + src_ptr [sp1]._g);
         b += 9 * (src_ptr [sn1]._b + src_ptr [sp1]._b);
         a += 9 * (src_ptr [sn1]._a + src_ptr [sp1]._a);
         
         s0 *= src_width;
         
         r += 16 * (src_ptr [s0]._r);
         g += 16 * (src_ptr [s0]._g);
         b += 16 * (src_ptr [s0]._b);
         a += 16 * (src_ptr [s0]._a);
         
         lang::limit_in_place (r, 0, 8160);
         lang::limit_in_place (g, 0, 8160);
         lang::limit_in_place (b, 0, 8160);
         lang::limit_in_place (a, 0, 8160);
         
         PixArgb & dpix = dst (x, y);
         dpix._r = r >> 5;
         dpix._g = g >> 5;
         dpix._b = b >> 5;
         dpix._a = a >> 5;
      }
      
      src_ptr += 1;
   }
   
   /*const PixArgb * src_ptr = &src (0, 0);
   
   const size_t height = dst.get_height ();
   const size_t dst_width = dst.get_width ();
   const size_t src_width = src.get_width ();
   
   const int s1 = src_width;
   const int s3 = 3 * s1;
   
   for (size_t x = 0 ; x < dst_width ; ++x)
   {
      const PixArgb * csrc_ptr = src_ptr;
      
      csrc_ptr += 6 * src_width;
      
      for (size_t y = 3 ; y < height - 3 ; ++y)
      {
         int r = -(csrc_ptr [-s3]._r + csrc_ptr [s3]._r);
         int g = -(csrc_ptr [-s3]._g + csrc_ptr [s3]._g);
         int b = -(csrc_ptr [-s3]._b + csrc_ptr [s3]._b);
         int a = -(csrc_ptr [-s3]._a + csrc_ptr [s3]._a);
         
         r += 9 * (csrc_ptr [-s1]._r + csrc_ptr [s1]._r);
         g += 9 * (csrc_ptr [-s1]._g + csrc_ptr [s1]._g);
         b += 9 * (csrc_ptr [-s1]._b + csrc_ptr [s1]._b);
         a += 9 * (csrc_ptr [-s1]._a + csrc_ptr [s1]._a);

         r += 16 * (csrc_ptr [0]._r);
         g += 16 * (csrc_ptr [0]._g);
         b += 16 * (csrc_ptr [0]._b);
         a += 16 * (csrc_ptr [0]._a);
         
         lang::limit_in_place (r, 0, 8160);
         lang::limit_in_place (g, 0, 8160);
         lang::limit_in_place (b, 0, 8160);
         lang::limit_in_place (a, 0, 8160);
         
         PixArgb & dpix = dst (x, y);
         dpix._r = r >> 5;
         dpix._g = g >> 5;
         dpix._b = b >> 5;
         dpix._a = a >> 5;
         
         csrc_ptr += 2 * src_width;
      }
      
      src_ptr += 1;
   }*/
}



/*
==============================================================================
Name : box
==============================================================================
*/

float Downsampler::box (float x)
{
   if (lang::abs (x) < 1.f)
   {
      return 1.f;
   }
   else
   {
      return 0.f;
   }
}



/*
==============================================================================
Name : tent
==============================================================================
*/

float Downsampler::tent (float x)
{
   if (lang::abs (x) < 1.f)
   {
      return 1.f - lang::abs (x);
   }
   else
   {
      return 0.f;
   }
}



/*
==============================================================================
Name : lanczos2
==============================================================================
*/

float Downsampler::lanczos2 (float x)
{
   if (lang::abs (x) < 2.f)
   {
      return float (sinc (3.141592653589793f * x) * sinc (1.570796326794897f * x));
   }
   else
   {
      return 0.f;
   }
}



/*
==============================================================================
Name : sinc
==============================================================================
*/

float Downsampler::sinc (float x)
{
   if (x != 0.f)
   {
      return float (sin (x)) / x;
   }
   else
   {
      return 1.f;
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
