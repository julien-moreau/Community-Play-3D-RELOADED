/*****************************************************************************

        PrimPushButtonSmall.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54712

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



#if ! defined (ohm_opa_PrimPushButtonSmall_HEADER_INCLUDED)
#define  ohm_opa_PrimPushButtonSmall_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/base/TypeEnum.h"
#include "ohm/opak/base/Sizable.h"
#include "ohm/opak/base/TypeFloat.h"
#include "ohm/opak/base/TypeString.h"
#include "ohm/opal/AreaString.h"
#include "ohm/opal/SprungImage.h"
#include "ohm/opa/prim/PrimWidget.h"
#include "ohm/opa/private/AspectWidgetPushButtonSmall.h"



namespace ohm
{
namespace opa
{



class PrimPushButtonSmall
:  public virtual ohm::opak::Primitive
,  public PrimWidget
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PrimPushButtonSmall ();

   // Video Thread
   virtual void   display (opak::GraphicContext & context);

   ohm::opak::TypeEnum <PrimPushButtonSmall, AspectWidgetPushButtonSmall::Type, AspectWidgetPushButtonSmall::Type_PLUS_SIGN>
                  _type;
   ohm::opak::TypeEnum <PrimPushButtonSmall, AspectWidgetPushButtonSmall::State, AspectWidgetPushButtonSmall::State_OFF>
                  _state;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual        ~PrimPushButtonSmall ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           invalidate_on_change ();
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimPushButtonSmall (const PrimPushButtonSmall &other);
   PrimPushButtonSmall &   operator = (const PrimPushButtonSmall &other);
   bool           operator == (const PrimPushButtonSmall &other) const;
   bool           operator != (const PrimPushButtonSmall &other) const;

}; // class PrimPushButtonSmall



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/PrimPushButtonSmall.hpp"



#endif   // ohm_opa_PrimPushButtonSmall_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

