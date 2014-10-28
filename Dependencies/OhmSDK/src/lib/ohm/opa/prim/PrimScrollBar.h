/*****************************************************************************

        PrimScrollBar.h
        Copyright (c) 2011 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 57529

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



#if ! defined (ohm_opa_PrimScrollBar_HEADER_INCLUDED)
#define  ohm_opa_PrimScrollBar_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/prim/PrimWidget.h"
#include "ohm/opak/base/TypeFloat.h"
#include "ohm/opak/base/TypeEnum.h"



namespace ohm
{
namespace opa
{



class PrimScrollBar
:  public PrimWidget
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum Type
   {
                  Type_VERTICAL = 0,
                  Type_HORIZONTAL,
   };
   
                  PrimScrollBar ();

   virtual void   display (opak::GraphicContext & context);


   opak::TypeFloat <PrimScrollBar>
                  _length;
   opak::TypeEnum <PrimScrollBar, Type, Type_VERTICAL>
                  _type;

   opak::TypeFloat <PrimScrollBar>
                  _content_length;
   opak::TypeFloat <PrimScrollBar>
                  _visible_position;
   opak::TypeFloat <PrimScrollBar>
                  _visible_length;

   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual        ~PrimScrollBar ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   void           bbox_changed ();
   void           something_changed ();



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimScrollBar (const PrimScrollBar &other);
   PrimScrollBar &   operator = (const PrimScrollBar &other);
   bool           operator == (const PrimScrollBar &other) const;
   bool           operator != (const PrimScrollBar &other) const;

}; // class PrimScrollBar



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/PrimScrollBar.hpp"



#endif   // ohm_opa_PrimScrollBar_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

