/*****************************************************************************

        PrimPushButton.h
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



#if ! defined (ohm_opa_PrimPushButton_HEADER_INCLUDED)
#define  ohm_opa_PrimPushButton_HEADER_INCLUDED

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



namespace ohm
{
namespace opa
{



class PrimPushButton
:  public virtual ohm::opak::Primitive
,  public ohm::opak::Sizable
,  public PrimWidget
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum State
   {
      State_NORMAL = 0,
      State_ROLLOVER,
      State_PRESSED
   };

                  PrimPushButton ();

   // Video Thread
   virtual void   display (opak::GraphicContext & context);

   ohm::opak::TypeEnum <PrimPushButton, State, State_NORMAL>
                  _state;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual        ~PrimPushButton ();

   // opak::Sizable
   virtual void   do_size_changed ();

   // PrimWidget
   virtual void   do_enabled_flag_changed ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           state_changed ();
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimPushButton (const PrimPushButton &other);
   PrimPushButton &  operator = (const PrimPushButton &other);
   bool           operator == (const PrimPushButton &other) const;
   bool           operator != (const PrimPushButton &other) const;

}; // class PrimPushButton



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/PrimPushButton.hpp"



#endif   // ohm_opa_PrimPushButton_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

