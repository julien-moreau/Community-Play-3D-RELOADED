/*****************************************************************************

        PrimPushButtonTiny.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 61117

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



#if ! defined (ohm_opa_PrimPushButtonTiny_HEADER_INCLUDED)
#define  ohm_opa_PrimPushButtonTiny_HEADER_INCLUDED

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
#include "ohm/opa/private/AspectWidgetPushButtonTiny.h"



namespace ohm
{
namespace opa
{



class PrimPushButtonTiny
:  public virtual ohm::opak::Primitive
,  public PrimWidget
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PrimPushButtonTiny ();

   // Video Thread
   virtual void   display (opak::GraphicContext & context);

   ohm::opak::TypeEnum <PrimPushButtonTiny, AspectWidgetPushButtonTiny::Type, AspectWidgetPushButtonTiny::Type_CANCEL_SIGN>
                  _type;
   ohm::opak::TypeEnum <PrimPushButtonTiny, AspectWidgetPushButtonTiny::State, AspectWidgetPushButtonTiny::State_OFF>
                  _state;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual        ~PrimPushButtonTiny ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           invalidate_on_change ();
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimPushButtonTiny (const PrimPushButtonTiny &other);
   PrimPushButtonTiny & operator = (const PrimPushButtonTiny &other);
   bool           operator == (const PrimPushButtonTiny &other) const;
   bool           operator != (const PrimPushButtonTiny &other) const;

}; // class PrimPushButtonTiny



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/PrimPushButtonTiny.hpp"



#endif   // ohm_opa_PrimPushButtonTiny_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

