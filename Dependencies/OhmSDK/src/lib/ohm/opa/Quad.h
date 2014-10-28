/*****************************************************************************

        Quad.h
        Copyright (c) 2009 Ohm Force

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



#if ! defined (ohm_opa_Quad_HEADER_INCLUDED)
#define  ohm_opa_Quad_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Point.h"
#include "ohm/opa/Rect.h"
#include "ohm/lang/fnc.h"



namespace ohm
{
namespace opal
{
class Image;
}



namespace opa
{



class Quad
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  Quad ();
   explicit       Quad (const Rect & rect);
   explicit       Quad (const opal::Image & image);
                  Quad (const Point & pt1, const Point & pt2, const Point & pt3, const Point & pt4);
                  Quad (const Quad & other);
   virtual        ~Quad ();

   Quad &         operator = (const Quad & other);

   inline const Point &
                  operator [] (int index) const {return _pt_arr [index];}
   void           set (const Point & pt1, const Point & pt2, const Point & pt3, const Point & pt4);
   
   bool           is_affine () const;
   bool           is_rect () const;
   
   void           get_affine_transformation (float & tx, float & ty, float & a, float & b, float & c, float & d) const;
   void           get_rect (Rect & rect) const;

   void           get_bounding_box (Rect & bounding_box) const;

   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   inline bool    is_equal (float x, float y) {return lang::abs (x - y) < 1e-2;}
   
   Point          _pt_arr [4];
   
   bool           _is_rect_flag;
   bool           _is_affine_flag;
      


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   bool           operator == (const Quad &other);
   bool           operator != (const Quad &other);

}; // class Quad



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/Quad.hpp"



#endif   // ohm_opa_Quad_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
