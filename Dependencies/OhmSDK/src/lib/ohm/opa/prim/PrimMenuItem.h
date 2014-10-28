/*****************************************************************************

        PrimMenuItem.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69920

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



#if ! defined (ohm_opa_PrimMenuItem_HEADER_INCLUDED)
#define  ohm_opa_PrimMenuItem_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/prim/PrimWidget.h"
#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/base/Sizable.h"
#include "ohm/opak/base/TypeEnum.h"
#include "ohm/opak/base/TypeString.h"
#include "ohm/opal/SprungImage.h"
#include "ohm/opal/Image.h"
#include "ohm/opal/LabelString.h"


namespace ohm
{
namespace opa
{



class PrimMenuItem
:  public PrimWidget
,  public opak::Sizable
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum MarkType
   {
                  MarkType_NONE = 0,
                  MarkType_CHECK,
                  MarkType_CHECK_MULTI,
   };
   
                  PrimMenuItem ();

   // Video Thread
   virtual void   display (opak::GraphicContext & context);
   
   opak::TypeBool <PrimMenuItem>
                  _rollover_flag;
   opak::TypeBool <PrimMenuItem>
                  _arrow_flag;
   opak::TypeEnum <PrimMenuItem, MarkType>
                  _mark;
   opak::TypeString <PrimMenuItem>
                  _label;
   opak::TypeBool <PrimMenuItem>
                  _section_flag;
   

   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual        ~PrimMenuItem ();

   // WidgetBase
   virtual void   do_enabled_flag_changed ();
   
   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           this_changed ();

   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimMenuItem (const PrimMenuItem &other);
   PrimMenuItem & operator = (const PrimMenuItem &other);
   bool           operator == (const PrimMenuItem &other) const;
   bool           operator != (const PrimMenuItem &other) const;

}; // class PrimMenuItem



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/PrimMenuItem.hpp"



#endif   // ohm_opa_PrimMenuItem_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

