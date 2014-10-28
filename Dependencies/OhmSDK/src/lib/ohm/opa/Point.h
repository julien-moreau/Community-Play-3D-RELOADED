/*****************************************************************************

        Point.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 65617

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



#if defined (ohm_opa_Point_CURRENT_HEADER)
   #error Recursive inclusion of ohm/opa/Point header.
#endif
#define  ohm_opa_Point_CURRENT_HEADER

#if ! defined (ohm_opa_Point_HEADER_INCLUDED)
#define  ohm_opa_Point_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"



namespace ohm
{
namespace opa
{



class Size;

class Point
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   inline         Point ();
   inline         Point (float x, float y);
   inline         Point (const Point & other);
   inline Point & operator = (const Point & other);

   inline Point & operator += (const Point & point);
   Point &        operator += (const Size & size);
   inline Point & operator -= (const Point & point);
   Point &        operator -= (const Size & size);
   
   inline Point   operator - () const;
   
   inline bool    operator == (const Point &other) const;
   inline bool    operator != (const Point &other) const;
   inline bool    operator < (const Point &other) const;

   void           lower_integral ();
   void           upper_integral ();
   void           nearest_integral ();
   
#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   void           from_CGPoint (const CGPoint & point);
   CGPoint        to_CGPoint () const;
#endif

   float          _x;
   float          _y;
   
   static const Point
                  _zero;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class Point



/*\\\ PUBLIC OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

Point             operator + (const Point & l_op, const Point & r_op);
Point             operator - (const Point & l_op, const Point & r_op);
Point             operator + (const Point & l_op, const Size & r_op);
Point             operator - (const Point & l_op, const Size & r_op);
Point             lower_integral (const Point & other);
Point             upper_integral (const Point & other);
Point             nearest_integral (const Point & other);



}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Point.hpp"


#endif   // ohm_opa_Point_HEADER_INCLUDED

#undef ohm_opa_Point_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
