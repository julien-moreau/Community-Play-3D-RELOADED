/*****************************************************************************

        RectCat.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 36751

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

#include "ohm/opal/win32/RectCat.h"

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
Throws : Nothing
==============================================================================
*/

RectCat::RectCat ()
:  _nbr_rect (0)
{
   
}



/*
==============================================================================
Name : dtor
Description :
Throws : Nothing
==============================================================================
*/

RectCat::~RectCat ()
{
   
}



/*
==============================================================================
Name : push_back_rect
Description :
   push a rect in the container.
   Can be called at interrupt time
Throws : Nothing
==============================================================================
*/

void  RectCat::push_back (const opa::Rect & rect)
{
   if (_nbr_rect < BUF_SIZE)
   {
      _rect_buf [_nbr_rect] = rect;
      ++ _nbr_rect;
   }
   else
   {
      _rect_buf [0] = opa::Rect (0, 0, 10000, 10000);
      _nbr_rect = 1;
   }
}



/*
==============================================================================
Name : flush
Description :
   Flush the container.
Throws : Nothing
==============================================================================
*/

void  RectCat::flush ()
{
   _nbr_rect = 0;
}



/*
==============================================================================
Name : get_nbr_rect
Description :
   Do calculate implicitly.
Throws : Nothing
==============================================================================
*/

int   RectCat::get_nbr_rect ()
{
   concat ();
   
   return _nbr_rect;
}



/*
==============================================================================
Name : get_rect
Description :
Throws : Nothing
==============================================================================
*/

const opa::Rect & RectCat::get_rect (int index) const
{
   assert (index >= 0);
   assert (index < BUF_SIZE);
   
   return _rect_buf [index];
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : concat
Description :
Throws : Nothing
==============================================================================
*/

void  RectCat::concat ()
{
   if (_nbr_rect != 0)
   {
      opa::Rect result_rect_arr [BUF_SIZE];
      int result_nbr_rect = 0;
      
      opa::Rect in_rect_arr [BUF_SIZE];
      int in_nbr_rect = 0;
      
      // copy buf to in
      int i;
      for (i = 0 ; i < _nbr_rect ; ++i)
      {
         in_rect_arr [i] = _rect_buf [i];
      }
      
      in_nbr_rect = _nbr_rect;
      
      do_recursive_concat (
         result_rect_arr, result_nbr_rect,
         in_rect_arr, in_nbr_rect
      );
      
      for (i = 0 ; i < result_nbr_rect ; ++i)
      {
         _rect_buf [i] = result_rect_arr [i];
      }
      _nbr_rect = result_nbr_rect;
   }
}



/*
==============================================================================
Name : do_recursive_concat
Description :
Throws : Nothing
==============================================================================
*/

void  RectCat::do_recursive_concat (opa::Rect * out_rect_arr, int & out_nbr_rect, opa::Rect * in_rect_arr, int & in_nbr_rect)
{
   if (in_nbr_rect == 1)
   {
      assert (out_nbr_rect < BUF_SIZE);
      out_rect_arr [out_nbr_rect] = in_rect_arr [0];
      ++ out_nbr_rect;
   }
   else
   {
      bool been_merge_flag = false;
      bool exit_flag = false;
      opa::Rect u_rect;
      
      int i = 1;
      while ((i < in_nbr_rect) && !exit_flag)
      {
         if (need_merge (in_rect_arr [0], in_rect_arr [i]))
         {
            u_rect = in_rect_arr [0] | in_rect_arr [i];
            been_merge_flag = true;
            exit_flag = true;
         }
         else
         {
            ++ i;
         }
      }
      
      int rect_select = i;
      
      int next_rect = 0;
      
      if (been_merge_flag)
      {
         // make new in_rect_arr
         int j;
         for (j = rect_select + 1 ; j < in_nbr_rect ; ++j)
         {
            in_rect_arr [j - 1] = in_rect_arr [j];
         }
         -- in_nbr_rect;
         
         in_rect_arr [0] = u_rect;
         
         next_rect = 0;
      }
      else
      {
         // make new out_rect_arr
         out_rect_arr [out_nbr_rect] = in_rect_arr [0];
         ++ out_nbr_rect;
         
         // in_rect_arr [0] has been treated
         -- in_nbr_rect;
         next_rect = 1;
      }
      
      do_recursive_concat (out_rect_arr, out_nbr_rect, &in_rect_arr [next_rect], in_nbr_rect);
   }
}



/*
==============================================================================
Name : need_merge
Description :
Throws : Nothing
==============================================================================
*/

bool  RectCat::need_merge (const opa::Rect & r1, const opa::Rect & r2)
{
   bool ret_val_flag = false;
   
   const float r1_surf = rect_surf (r1);
   const float r2_surf = rect_surf (r2);
   const float total_surf = rect_surf (r1 | r2);
   
   const float lost_draw_surf = total_surf - (r1_surf + r2_surf);
   
   if (lost_draw_surf > OVERLAP_SURF)
   {
      // merging is not good
      ret_val_flag = false;
   }
   else
   {
      ret_val_flag = true;
   }
   
   return ret_val_flag;
}



/*
==============================================================================
Name : rect_surf
Description :
Throws : Nothing
==============================================================================
*/

float RectCat::rect_surf (const opa::Rect & r)
{
   float ret_val = 0;
   
   ret_val = r._size._width * r._size._height;
   
   if (ret_val < 0)
   {
      ret_val = 0;
   }
   
   return ret_val;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
