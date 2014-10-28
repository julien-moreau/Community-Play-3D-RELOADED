/*****************************************************************************

        PrimMenuRoot.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49845

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



#if ! defined (ohm_opa_PrimMenuRoot_HEADER_INCLUDED)
#define  ohm_opa_PrimMenuRoot_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/prim/PrimWidget.h"
#include "ohm/opak/base/PrimitiveContainer.h"
#include "ohm/opak/base/Sizable.h"
#include "ohm/opak/base/Opacifiable.h"
#include "ohm/opak/base/TypeString.h"
#include "ohm/opal/SprungImage.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/LabelString.h"


namespace ohm
{
namespace opa
{



class PrimMenuRoot
:  public PrimWidget
,  public opak::PrimitiveContainer
,  public opak::Sizable
,  public opak::Opacifiable
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PrimMenuRoot ();

   // Video Thread
   virtual void   display (opak::GraphicContext & context);
   
   opak::TypeFloat <PrimMenuRoot>
                  _arrow_left_opacity;
   opak::TypeFloat <PrimMenuRoot>
                  _arrow_right_opacity;
   

   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual        ~PrimMenuRoot ();

   // WidgetBase
   virtual void   do_enabled_flag_changed ();
   
   // opak::Sizable
   virtual void   do_size_changed ();
   
   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           arrow_opacity_changed ();

   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimMenuRoot (const PrimMenuRoot &other);
   PrimMenuRoot & operator = (const PrimMenuRoot &other);
   bool           operator == (const PrimMenuRoot &other) const;
   bool           operator != (const PrimMenuRoot &other) const;

}; // class PrimMenuRoot



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/PrimMenuRoot.hpp"



#endif   // ohm_opa_PrimMenuRoot_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

