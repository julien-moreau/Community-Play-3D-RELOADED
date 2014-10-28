/*****************************************************************************

        MarkFinder.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 32323

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

#include "ohm/opal/dsp/MarkFinder.h"
#include "ohm/opal/Bitmap.h"
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

MarkFinder::MarkFinder ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

MarkFinder::~MarkFinder ()
{
}



/*
==============================================================================
Name : process
==============================================================================
*/

void  MarkFinder::process (std::vector <size_t> & row_width_arr, std::vector <size_t> & col_height_arr, const Bitmap & src)
{
   assert (src.get_width () > 0);
   assert (src.get_height () > 0);
   
   PixArgb pix = src (0, 0);
   int row_width = 1;
   
   for (size_t x = 1 ; x < src.get_width () ; ++x)
   {
      PixArgb tpix = src (x, 0);
      
      if (tpix != pix)
      {
         row_width_arr.push_back (row_width);
         row_width = 1;
         pix = tpix;
      }
      else
      {
         ++row_width;
      }
   }
   
   row_width_arr.push_back (row_width);
   
   pix = src (0, 0);
   int col_height = 1;
   
   for (size_t y = 1 ; y < src.get_height () ; ++y)
   {
      PixArgb tpix = src (0, y);
      
      if (tpix != pix)
      {
         col_height_arr.push_back (col_height);
         col_height = 1;
         pix = tpix;
      }
      else
      {
         ++col_height;
      }
   }
   
   col_height_arr.push_back (col_height);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
