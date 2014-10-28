/*****************************************************************************

        Rect.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69378

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



#if defined (ohm_opa_Rect_CURRENT_HEADER)
   #error Recursive inclusion of ohm/opa/Rect header.
#endif
#define  ohm_opa_Rect_CURRENT_HEADER

#if ! defined (ohm_opa_Rect_HEADER_INCLUDED)
#define  ohm_opa_Rect_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Point.h"
#include "ohm/opa/Size.h"

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #define  WIN32_LEAN_AND_MEAN
   #define  NOMINMAX
   #include <windows.h>
#endif

namespace ohm
{
namespace opa
{



class Margin;

class Rect
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Rect ();
                  Rect (const Point & origin, const Size & size);
                  Rect (const Margin & margin);
                  Rect (float x, float y, float width, float height);
                  Rect (const Rect & other);
   Rect &         operator = (const Rect & other);

   Rect &         operator += (const Point & point);
   Rect &         operator += (const Size & size);
   Rect &         operator -= (const Point & point);
   Rect &         operator -= (const Size & size);
   Rect &         operator += (const Margin & margin);
   Rect &         operator -= (const Margin & margin);
   Rect &         operator &= (const Rect & rect);
   Rect &         operator |= (const Rect & rect);
   
   Rect &         accumulate (const Rect & rect);

   bool           operator == (const Rect &other) const;
   bool           operator != (const Rect &other) const;
   bool           operator < (const Rect &other) const;
   
   void           set_left (float left);
   void           set_top (float top);
   void           set_right (float right);
   void           set_bottom (float bottom);
   
   float          get_left () const;
   float          get_top () const;
   float          get_right () const;
   float          get_bottom () const;
   
   bool           is_null () const;
   bool           is_empty () const;
   bool           is_infinite () const;
   bool           contains (const Point & point) const;
   bool           contains (const Rect & rect) const;
   void           integral ();
   void           round ();
   bool           is_intersecting (const Rect & rect) const;
   Rect           standardize () const;
   Point          get_barycenter (float norm_val_x, float norm_val_y) const;

#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   void           from_CGRect (const CGRect & rect);
   CGRect         to_CGRect () const;
#elif (ohm_archi_SYS == ohm_archi_SYS_WIN)
   void           from_RECT (const ::RECT & rect);
   ::RECT         to_RECT () const;
#endif
   
   Point          _origin;
   Size           _size;
   
   static const Rect
                  _zero;
   static const Rect
                  _union_stable;
   static const Rect
                  _intersection_stable;
   static const Rect
                  _infinity;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class Rect



/*\\\ PUBLIC OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

Rect              operator + (const Rect & l_op, const Point & r_op);
Rect              operator + (const Rect & l_op, const Size & r_op);
Rect              operator - (const Rect & l_op, const Point & r_op);
Rect              operator - (const Rect & l_op, const Size & r_op);
Rect              operator & (const Rect & l_op, const Rect & r_op);
Rect              operator | (const Rect & l_op, const Rect & r_op);
Rect              integral (const Rect & rect);
bool              are_intersecting (const Rect & rect_0, const Rect & rect_1);
Rect              standardize (const Rect & rect);



}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opa_Rect_HEADER_INCLUDED

#undef ohm_opa_Rect_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
