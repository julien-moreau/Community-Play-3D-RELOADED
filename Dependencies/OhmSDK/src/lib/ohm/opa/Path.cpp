/*****************************************************************************

        Path.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 47083

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

#include "ohm/opa/Path.h"

#include "ohm/lang/fnc.h"
#include "ohm/math/fnc.h"

#include <cassert>



namespace ohm
{
namespace opa
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : make_rect
Description :
==============================================================================
*/

void  Path::make_rect (Path & path, const Rect & rect)
{
   std::vector <Point> & pt_arr = path._point_arr;
   
   pt_arr.resize (5);
   pt_arr [0] = rect._origin;
   pt_arr [1] = rect._origin + opa::Point (rect._size._width, 0);
   pt_arr [2] = rect._origin + opa::Point (rect._size._width, rect._size._height);
   pt_arr [3] = rect._origin + opa::Point (0, rect._size._height);
   pt_arr [4] = rect._origin;
   
   path._min_x = rect._origin._x;
   path._min_y = rect._origin._y;
   path._max_x = rect._origin._x + rect._size._width;
   path._max_y = rect._origin._y + rect._size._height;
   
   path.update_integral_bounding_box ();
}



/*
==============================================================================
Name : make_ellipse
Description :
==============================================================================
*/

void  Path::make_ellipse (Path & path, const Rect & rect)
{
   // in the case of a circle, the points can be completely
   // regurarly spaced. the ellipse case is just a scale
   // of the circle case. So we are not using bezier curves,
   // but directly trigonometry.
   
   int loop = math::floor_int (rect._size._width * rect._size._height);
   loop >>= 4;
   if (loop == 0) loop = 5;
   
   float tt = 2.f * float (math::PI) / float (loop - 1);
   
   float s1 = 0;
   float c1 = 1;
   float al = sin (.5f * tt);
   al *= (2.f * al);
   float be = sin (tt);
   
   std::vector <Point> & pt_arr = path._point_arr;
   pt_arr.clear ();
   
   for (int i = 0 ; i < loop ; ++i)
   {
      float t1 = c1;
      c1 -= al * t1 + be * s1;
      s1 -= al * s1 - be * t1;
      
      float xr = rect._origin._x + (c1 * .5f + .5f) * rect._size._width;
      float yr = rect._origin._y + (s1 * .5f + .5f) * rect._size._height;
      
      pt_arr.push_back (opa::Point (xr, yr));
   }
   
   pt_arr.push_back (pt_arr [0]);

   path._min_x = rect._origin._x;
   path._min_y = rect._origin._y;
   path._max_x = rect._origin._x + rect._size._width;
   path._max_y = rect._origin._y + rect._size._height;
   
   path.update_integral_bounding_box ();
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Path::Path ()
:  _point_arr ()
,  _closed_flag (false)
,  _min_x (0)
,  _min_y (0)
,  _max_x (0)
,  _max_y (0)

,  _integral_bb_need_update_flag (true)
,  _integral_bounding_box ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Path::~Path ()
{
}



/*
==============================================================================
Name : clear
==============================================================================
*/

void  Path::clear ()
{
   _point_arr.clear ();
   _closed_flag = false;
   _min_x = 0;
   _min_y = 0;
   _max_x = 0;
   _max_y = 0;
   _integral_bb_need_update_flag = true;
   _integral_bounding_box = opa::Rect::_zero;
}



/*
==============================================================================
Name : move_to
==============================================================================
*/

void  Path::move_to (const Point & pt)
{
   _point_arr.resize (1);
   _point_arr [0] = pt;
   
   _min_x = pt._x;
   _min_y = pt._y;

   _max_x = pt._x;
   _max_y = pt._y;
   
   _integral_bb_need_update_flag = true;
}



/*
==============================================================================
Name : line_to
==============================================================================
*/

void  Path::line_to (const Point & pt)
{
   if (_point_arr.size () > 0)
   {
      _point_arr.push_back (pt);
      
      lang::min_in_place (_min_x, pt._x);
      lang::min_in_place (_min_y, pt._y);

      lang::max_in_place (_max_x, pt._x);
      lang::max_in_place (_max_y, pt._y);
      
      _integral_bb_need_update_flag = true;
   }
   else
   {
      move_to (pt);
   }
}



/*
==============================================================================
Name : curve_to
==============================================================================
*/

void  Path::curve_to (const Point & cpt1, const Point & cpt2, const Point & pt)
{
   assert (_point_arr.size () > 0);
   
   if (_point_arr.size () > 0)
   {
      const Point & pt0 = _point_arr [_point_arr.size () - 1];
      
      // we could calculate the bounding rect at this stage
      // since the curve itself is bounded by pt0, cpt1, cpt2 and pt
      // but this is not a lot precise, so we prefer to
      // calculate it in the recursion
      
      recursive_curve_subdivide (0, pt0, cpt1, cpt2, pt);
      
      _integral_bb_need_update_flag = true;
   }
}



/*
==============================================================================
Name : close
==============================================================================
*/

void  Path::close ()
{
   assert (_point_arr.size () > 0);
   
   if ((_point_arr.size () > 0) && !_closed_flag)
   {
      _point_arr.push_back (_point_arr [0]);
      
      _closed_flag = true;
      
      _integral_bb_need_update_flag = true;
   }
}



/*
==============================================================================
Name : is_empty
==============================================================================
*/

bool  Path::is_empty () const
{
   return _point_arr.empty ();
}



/*
==============================================================================
Name : get_polygon
==============================================================================
*/

const std::vector <Point> &   Path::get_polygon () const
{
   return _point_arr;
}



/*
==============================================================================
Name : is_closed
==============================================================================
*/

bool  Path::is_closed () const
{
   return _closed_flag;
}



/*
==============================================================================
Name : get_integral_bounding_box
==============================================================================
*/

const Rect &   Path::get_integral_bounding_box () const
{
   if (_integral_bb_need_update_flag)
   {
      update_integral_bounding_box ();
   }
   
   return _integral_bounding_box;
}



/*
==============================================================================
Name : is_in
Description :
   Returns true is the circle of center 'pt' and 'radius' intersect one of
   the segment composing the path.
==============================================================================
*/

bool  Path::is_in (const Point & pt, float radius)
{
   opa::Rect bbox = get_integral_bounding_box ();
   
   bbox._origin -= opa::Point (radius, radius);
   bbox._size += opa::Size (radius * 2.f, radius * 2.f);
   
   if (!bbox.contains (pt))
   {
      return false;
   }
   
   const std::vector <Point> & pt_arr = get_polygon ();
   
   size_t end = pt_arr.size () - 1;
   
   // we express the problem as
   // segment AB and point C
   // D point as AD = tAB and AD.DC = 0
   // return true if t in [0 1] and DC <= radius
   
   const float r2 = radius * radius;
   
   for (size_t i = 0 ; i < end ; ++i)
   {
      const opa::Point & pt_a = pt_arr [i];
      const opa::Point & pt_b = pt_arr [i + 1];
      
      const opa::Point ab (pt_b._x - pt_a._x, pt_b._y - pt_a._y);
      
      if ((ab._x != 0) && (ab._y != 0))
      {
         const opa::Point ac (pt._x - pt_a._x, pt._y - pt_a._y);
         
         const float abac = ab._x * ac._x + ab._y * ac._y;
         
         if (0.f <= abac)
         {
            const float ab2 = ab._x * ab._x + ab._y * ab._y;
            
            if (abac <= ab2)
            {
               // t is in [0 1]
               
               const float t = abac / ab2;
               
               const opa::Point dc (
                  ac._x - t * ab._x,
                  ac._y - t * ab._y
               );
               
               float dc2 = dc._x * dc._x + dc._y * dc._y;
               
               if (dc2 <= r2)
               {
                  return true;
               }
            }
         }
      }
   }

   return false;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_integral_bounding_box
==============================================================================
*/

void  Path::update_integral_bounding_box () const
{
   const float border = 1.f;
   
   float min_x = floor (_min_x - border);
   float min_y = floor (_min_y - border);
   float max_x = ceil (_max_x + border);
   float max_y = ceil (_max_y + border);
   
   _integral_bounding_box._origin._x = min_x;
   _integral_bounding_box._origin._y = min_y;
   _integral_bounding_box._size._width = max_x - min_x;
   _integral_bounding_box._size._height = max_y - min_y;
   
   _integral_bb_need_update_flag = false;
}



/*
==============================================================================
Name : recursive_curve_subdivide
==============================================================================
*/

void  Path::recursive_curve_subdivide (int level, const Point & pt1, const Point & pt2, const Point & pt3, const Point & pt4)
{
   // one robust condition is to say that
   // a line is flat when x1, x2, x3, x4 are equally spaced
   // and almost colinear. We use distance-1 for speed, and
   // it is shown to be robust as well
   
   float deviation
      = lang::abs (pt1._x + pt3._x - pt2._x - pt2._x)
      + lang::abs (pt1._y + pt3._y - pt2._y - pt2._y)
      + lang::abs (pt2._x + pt4._x - pt3._x - pt3._x)
      + lang::abs (pt2._y + pt4._y - pt3._y - pt3._y)
   ;
   
   bool stop_flag = (deviation <= .25f) || (level > 8);
   
   if (stop_flag)
   {
      _point_arr.push_back (pt4);

      lang::min_in_place (_min_x, pt4._x);
      lang::min_in_place (_min_y, pt4._y);

      lang::max_in_place (_max_x, pt4._x);
      lang::max_in_place (_max_y, pt4._y);
   }
   else
   {
      Point pt12 ((pt1._x + pt2._x) * .5f, (pt1._y + pt2._y) * .5f);
      Point pt23 ((pt2._x + pt3._x) * .5f, (pt2._y + pt3._y) * .5f);
      Point pt34 ((pt3._x + pt4._x) * .5f, (pt3._y + pt4._y) * .5f);
      Point pt123 ((pt12._x + pt23._x) * .5f, (pt12._y + pt23._y) * .5f);
      Point pt234 ((pt23._x + pt34._x) * .5f, (pt23._y + pt34._y) * .5f);
      Point pt1234 ((pt123._x + pt234._x) * .5f, (pt123._y + pt234._y) * .5f);
      
      ++level;
      recursive_curve_subdivide (level, pt1, pt12, pt123, pt1234);
      recursive_curve_subdivide (level, pt1234, pt234, pt34, pt4);
   }
}
   



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
