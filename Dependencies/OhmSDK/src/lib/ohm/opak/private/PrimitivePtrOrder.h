/*****************************************************************************

        PrimitivePtrOrder.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 34975

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



#if ! defined (ohm_opak_PrimitivePtrOrder_HEADER_INCLUDED)
#define  ohm_opak_PrimitivePtrOrder_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <list>



namespace ohm
{
namespace opak
{



class Primitive;

class PrimitivePtrOrder
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  PrimitivePtrOrder (Primitive & primitive);
                  PrimitivePtrOrder (const PrimitivePtrOrder &other);
   virtual        ~PrimitivePtrOrder ();

   void           set_order (size_t order);
   Primitive *    get_primitive_ptr ();
   
   void           kill ();
   void           resurrect ();
   bool           is_dying () const;
   
   bool           operator < (const PrimitivePtrOrder & other) const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum State
   {
                  State_RESIDENT = 0,
                  State_DYING,
   };
   
   Primitive *    _primitive_ptr;
   size_t         _order;
   State          _state;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimitivePtrOrder ();
   PrimitivePtrOrder &
                  operator = (const PrimitivePtrOrder &other);
   bool           operator == (const PrimitivePtrOrder &other);
   bool           operator != (const PrimitivePtrOrder &other);

}; // class PrimitivePtrOrder

typedef std::list <PrimitivePtrOrder>  PrimitivePtrOrderList;



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opak
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/PrimitivePtrOrder.hpp"



#endif   // ohm_opak_PrimitivePtrOrder_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
