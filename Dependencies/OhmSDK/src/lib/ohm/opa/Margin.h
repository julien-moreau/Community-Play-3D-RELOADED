/*****************************************************************************

        Margin.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 38302

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



#if defined (ohm_opa_Margin_CURRENT_HEADER)
   #error Recursive inclusion of ohm/opa/Margin header.
#endif
#define  ohm_opa_Margin_CURRENT_HEADER

#if ! defined (ohm_opa_Margin_HEADER_INCLUDED)
#define  ohm_opa_Margin_HEADER_INCLUDED

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



class Rect;

class Margin
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  Margin ();
                  Margin (const Rect & rect);
                  Margin (float left, float top, float right, float bottom);
                  Margin (const Margin & other);
   Margin &       operator = (const Margin & other);

   Margin &       operator += (const Margin & margin);
   Margin &       operator -= (const Margin & margin);

   bool           operator == (const Margin &other) const;
   bool           operator != (const Margin &other) const;
   bool           operator < (const Margin &other) const;
   
   bool           is_none () const;
   void           integral ();
   void           round ();
   
   float          _left;
   float          _top;
   float          _right;
   float          _bottom;
   
   
   static const Margin
                  _zero;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class Margin



/*\\\ PUBLIC OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

Margin            operator + (const Margin & l_op, const Margin & r_op);
Margin            operator - (const Margin & l_op, const Margin & r_op);
Margin            integral (const Margin & margin);



}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opa_Margin_HEADER_INCLUDED

#undef ohm_opa_Margin_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
