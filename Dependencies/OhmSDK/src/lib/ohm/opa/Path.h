/*****************************************************************************

        Path.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 44406

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



#if ! defined (ohm_opa_Path_HEADER_INCLUDED)
#define  ohm_opa_Path_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Point.h"
#include "ohm/opa/Rect.h"

#include <vector>


namespace ohm
{
namespace opa
{



class Path
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   static void    make_rect (Path & path, const Rect & rect);
   static void    make_ellipse (Path & path, const Rect & rect);
   
                  Path ();
   virtual        ~Path ();
   
   void           clear ();
   void           move_to (const Point & pt);
   void           line_to (const Point & pt);
   void           curve_to (const Point & cpt1, const Point & cpt2, const Point & pt);
   void           close ();
   
   bool           is_empty () const;
   const std::vector <Point> &
                  get_polygon () const;
   bool           is_closed () const;
   const Rect &   get_integral_bounding_box () const;
   
   bool           is_in (const Point & pt, float radius);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           update_integral_bounding_box () const;
   void           recursive_curve_subdivide (int level, const Point & pt1, const Point & pt2, const Point & pt3, const Point & pt4);
   
   std::vector <Point>
                  _point_arr;
   bool           _closed_flag;
   
   float          _min_x;
   float          _min_y;
   float          _max_x;
   float          _max_y;
   
   mutable bool   _integral_bb_need_update_flag;
   mutable Rect   _integral_bounding_box;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  Path (const Path &other);
   Path &         operator = (const Path &other);
   bool           operator == (const Path &other);
   bool           operator != (const Path &other);

}; // class Path



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opa_Path_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
