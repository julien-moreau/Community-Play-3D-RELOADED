/*****************************************************************************

        PrimListItem.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 40939

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



#if ! defined (ohm_opa_PrimListItem_HEADER_INCLUDED)
#define  ohm_opa_PrimListItem_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/prim/PrimWidget.h"
#include "ohm/opak/base/Sizable.h"
#include "ohm/opak/base/TypeString.h"



namespace ohm
{
namespace opa
{



class PrimListItem
:  public PrimWidget
,  public opak::Sizable
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

                  PrimListItem ();

   // Video Thread
   virtual void   display (opak::GraphicContext & context);
   
   opak::TypeBool <PrimListItem>
                  _selected_flag;
   opak::TypeString <PrimListItem>
                  _label;
   

   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual        ~PrimListItem ();

   // WidgetBase
   virtual void   do_enabled_flag_changed ();
   
   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           this_changed ();

   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimListItem (const PrimListItem &other);
   PrimListItem & operator = (const PrimListItem &other);
   bool           operator == (const PrimListItem &other) const;
   bool           operator != (const PrimListItem &other) const;

}; // class PrimListItem



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/PrimListItem.hpp"



#endif   // ohm_opa_PrimListItem_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

