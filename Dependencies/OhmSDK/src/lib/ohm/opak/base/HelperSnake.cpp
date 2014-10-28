/*****************************************************************************

        HelperSnake.cpp
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 64279

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

#include "ohm/opak/base/HelperSnake.h"
#include "ohm/opak/base/GraphicContext.h"
#include "ohm/opal/GraphicContextInterface.h"

#include <cassert>



namespace ohm
{
namespace opak
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

HelperSnake::HelperSnake ()
:  _offset_y (0)
,  _context_ptr (0)
,  _pt_pos (3)
,  _cur_pt_nbr (0)
{
}



/*
==============================================================================
Name : head
==============================================================================
*/

HelperSnake &  HelperSnake::head (const opal::Image & image)
{
   _image_head = image;
   _height = float (image._height);
   
   return *this;
}



/*
==============================================================================
Name : medial
==============================================================================
*/

HelperSnake &  HelperSnake::medial (const opal::Image & image)
{
   _image_medial = image;
   _height = float (image._height);
   
   return *this;
}



/*
==============================================================================
Name : tail
==============================================================================
*/

HelperSnake &  HelperSnake::tail (const opal::Image & image)
{
   _image_tail = image;
   _height = float (image._height);
   
   return *this;
}



/*
==============================================================================
Name : offset
Description :
   All images are supposed to have the same height.
   'y' defines the distance expressed in image coordinates from the top of
   the images to the supposed center of the line.
==============================================================================
*/

HelperSnake &  HelperSnake::offset (float y)
{
   _offset_y = y;
   
   return *this;
}



/*
==============================================================================
Name : display_begin
==============================================================================
*/

HelperSnake &  HelperSnake::display_begin (opak::GraphicContext & context)
{
   assert (_context_ptr == 0);
   assert (&context != 0);
   
   _context_ptr = &context;
   
   return *this;
}



/*
==============================================================================
Name : point
==============================================================================
*/

HelperSnake &  HelperSnake::point (const opa::Point & pt)
{
   assert (_context_ptr != 0);
   
   opa::Point tpt = pt;
   
   _context_ptr->apply (tpt);
   
   ++_pt_pos;
   _pt_pos &= 3;

   _pt_ring [_pt_pos] = tpt;
   
   ++_cur_pt_nbr;
   
   if (_cur_pt_nbr == 3)
   {
      display_head (*_context_ptr);
   }
   else if (_cur_pt_nbr > 3)
   {
      display_medial (*_context_ptr);
   }
   
   return *this;
}



/*
==============================================================================
Name : display_end
==============================================================================
*/

void  HelperSnake::display_end ()
{
   assert (_context_ptr != 0);
   
   display_tail (*_context_ptr);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : display_head
==============================================================================
*/

void  HelperSnake::display_head (opak::GraphicContext & context)
{
   const opa::Point & spt = get_pt (2);
   const opa::Point & mpt = get_pt (1);
   const opa::Point & ept = get_pt (0);
   
   opa::Size vec1 (mpt._x - spt._x, mpt._y - spt._y);
   opa::Size vec2 (ept._x - spt._x, ept._y - spt._y);
   
   if ((vec1.norm_1 () > 0.f) && (vec2.norm_1 () > 0.f))
   {
      float inv_norm2 = vec1.inv_norm_2 ();

      opa::Point ppt = mpt - inv_norm2 * float (_image_head._width) * vec1;
      
      opa::Point pt_0;
      opa::Point pt_1;
      calc_pt (pt_0, pt_1, ppt, vec1);
      
      
      calc_pt (_pt_0, _pt_1, mpt, vec2);
      
      context.get ().draw (
         _image_head,
         opa::Quad (pt_0, _pt_0, _pt_1, pt_1),
         opa::Quad (_image_head)
      );
   }
   else
   {
      // degenerated case
      assert (false);
   }
}



/*
==============================================================================
Name : display_medial
==============================================================================
*/

void  HelperSnake::display_medial (opak::GraphicContext & context)
{
   const opa::Point & spt = get_pt (2);
   const opa::Point & mpt = get_pt (1);
   const opa::Point & ept = get_pt (0);
   
   opa::Size vec (ept._x - spt._x, ept._y - spt._y);
   
   if (vec.norm_1 () > 0.f)
   {
      opa::Point pt_0 = _pt_0;
      opa::Point pt_1 = _pt_1;
      
      calc_pt (_pt_0, _pt_1, mpt, vec);
      
      context.get ().draw (
         _image_medial,
         opa::Quad (pt_0, _pt_0, _pt_1, pt_1),
         opa::Quad (_image_medial)
      );
   }
   else
   {
      // degenerated case
      assert (false);
   }
}



/*
==============================================================================
Name : display_tail
==============================================================================
*/

void  HelperSnake::display_tail (opak::GraphicContext & context)
{
   const opa::Point & mpt = get_pt (1);
   const opa::Point & ept = get_pt (0);
   
   opa::Size vec (ept._x - mpt._x, ept._y - mpt._y);
   
   if (vec.norm_1 () > 0.f)
   {
      float inv_norm2 = vec.inv_norm_2 ();

      opa::Point ppt = mpt + inv_norm2 * float (_image_tail._width) * vec;
      
      opa::Point pt_0;
      opa::Point pt_1;
      calc_pt (pt_0, pt_1, ppt, vec);
      
      context.get ().draw (
         _image_tail,
         opa::Quad (_pt_0, pt_0, pt_1, _pt_1),
         opa::Quad (_image_tail)
      );
   }
   else
   {
      // degenerated case
      assert (false);
   }
}



/*
==============================================================================
Name : calc_pt
==============================================================================
*/

void  HelperSnake::calc_pt (opa::Point & pt0, opa::Point & pt1, const opa::Point & pt, const opa::Size & vec)
{
   opa::Size vecn;
   vecn._height = - vec._width;
   vecn._width = vec._height;
   
   float inv_norm2 = vecn.inv_norm_2 ();
   
   vecn._width *= inv_norm2;
   vecn._height *= inv_norm2;
   
   pt0 = pt + _offset_y * vecn;
   pt1 = pt0 - _height * vecn;
}



/*
==============================================================================
Name : get_pt
==============================================================================
*/

const opa::Point &   HelperSnake::get_pt (size_t m_pos)
{
   size_t pt_pos = _pt_pos - m_pos;
   pt_pos &= 3;

   return _pt_ring [pt_pos];
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
