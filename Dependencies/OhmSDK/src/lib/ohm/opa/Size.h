/*****************************************************************************

        Size.h
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



#if defined (ohm_opa_Size_CURRENT_HEADER)
   #error Recursive inclusion of ohm/opa/Size header.
#endif
#define  ohm_opa_Size_CURRENT_HEADER

#if ! defined (ohm_opa_Size_HEADER_INCLUDED)
#define  ohm_opa_Size_HEADER_INCLUDED

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



class Point;

class Size
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   inline         Size ();
   inline         Size (float width, float height);
   inline         Size (const Size & other);
   inline Size &  operator = (const Size & other);

   Size &         operator += (const Point & point);
   inline Size &  operator += (const Size & size);
   Size &         operator -= (const Point & point);
   inline Size &  operator -= (const Size & size);
   inline Size &  operator *= (float scale);
   inline Size    operator - ();
   
   inline bool    operator == (const Size & other) const;
   inline bool    operator != (const Size & other) const;
   inline bool    operator < (const Size & other) const;

   inline bool    is_null () const;
   inline bool    is_empty () const;
   inline bool    is_infinite () const;
   
   inline float   norm_1 () const;
   inline float   norm_2 () const;
   inline float   norm_2_sqr () const;
   inline float   inv_norm_2 () const;
   inline float   norm_inf () const;
   
   void           lower_integral ();
   void           upper_integral ();
   void           nearest_integral ();
   
#if (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   void           from_CGSize (const CGSize & size);
   CGSize         to_CGSize () const;
#endif

   float          _width;
   float          _height;
   
   static const Size
                  _zero;
   static const Size
                  _infinity;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class Size



/*\\\ PUBLIC OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

Size              operator + (const Size & l_op, const Point & r_op);
Size              operator - (const Size & l_op, const Point & r_op);
Size              operator + (const Size & l_op, const Size & r_op);
Size              operator - (const Size & l_op, const Size & r_op);
Size              operator * (const Size & l_op, float r_op);
Size              operator * (float l_op, const Size & r_op);
Size              lower_integral (const Size & other);
Size              upper_integral (const Size & other);
Size              nearest_integral (const Size & other);



}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Size.hpp"



#endif   // ohm_opa_Size_HEADER_INCLUDED

#undef ohm_opa_Size_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
