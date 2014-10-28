/*****************************************************************************

        Cropper.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 31833

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

#include "ohm/opal/dsp/Cropper.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/opa/Rect.h"
#include "ohm/lang/fnc.h"

#include <cstring>
#include <cassert>



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

Cropper::Cropper ()
:  _x (0)
,  _y (0)
,  _width (0)
,  _height (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Cropper::~Cropper ()
{
}



/*
==============================================================================
Name : set
Description :
   Set the crop rectangle expressed in src coordinates
==============================================================================
*/

void  Cropper::set (archi::Int32 x, archi::Int32 y, archi::UInt32 width, archi::UInt32 height)
{
   _x = x;
   _y = y;
   _width = width;
   _height = height;
}



/*
==============================================================================
Name : process
==============================================================================
*/

void  Cropper::process (Bitmap & dst, const Bitmap & src)
{
   dst.resize (_width, _height);
   
   dst.clear_rect (0, 0, _width, _height);
   
   size_t start_y = lang::max (- _y, 0);
   size_t end_y = lang::min (archi::Int32 (src.get_height ()) - _y, _height);
   if (start_y >= end_y)
   {
      return;
   }

   size_t start_x = lang::max (- _x, 0);
   size_t end_x = lang::min (archi::Int32 (src.get_width ()) - _x, _width);
   if (start_x >= end_x)
   {
      return;
   }
   size_t width = end_x - start_x;
   
   for (size_t y = start_y ; y < end_y ; ++y)
   {
      using namespace std;
      
      PixArgb * dst_ptr = &dst (start_x, y);
      const PixArgb * src_ptr = &src (_x + start_x, _y + y);
      
      memcpy (dst_ptr, src_ptr, width * sizeof (PixArgb));
   }
   
   /*
   for (size_t y = 0 ; y < _height ; ++y)
   {
      bool ok_flag = true;
      ok_flag &= y + _y >= 0;
      ok_flag &= y + _y < src.get_height ();
      
      if (ok_flag)
      for (size_t x = 0 ; x < _width ; ++x)
      {
         bool ok_flag = true;
         ok_flag &= x + _x >= 0;
         ok_flag &= x + _x < src.get_width ();
         
         if (ok_flag)
         dst (x, y) = src (x + _x, y + _y);
      }
   }
   */
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
