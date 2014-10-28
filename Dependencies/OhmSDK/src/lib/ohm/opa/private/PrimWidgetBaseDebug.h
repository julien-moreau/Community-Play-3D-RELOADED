/*****************************************************************************

        PrimWidgetBaseDebug.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 54597

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



#if ! defined (ohm_opa_PrimWidgetBaseDebug_HEADER_INCLUDED)
#define  ohm_opa_PrimWidgetBaseDebug_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/Color.h"
#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/base/Sizable.h"
#include "ohm/opak/base/TypeMargin.h"
#include "ohm/opak/base/TypeRect.h"



namespace ohm
{
namespace opa
{



class PrimWidgetBaseDebug
:  public virtual opak::Primitive
,  public virtual opak::Sizable
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  PrimWidgetBaseDebug ();
   
   // Video Thread
   virtual void   display (opak::GraphicContext & context);
   
   opak::TypeRect <PrimWidgetBaseDebug>
                  _min_bbox;
   opak::TypeMargin <PrimWidgetBaseDebug>
                  _margin;
                  
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   virtual        ~PrimWidgetBaseDebug ();
   
   // opak::Sizable
   virtual void   do_size_changed ();



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           invalidate_on_change ();
   
   void           stroke (opak::GraphicContext & context, const Rect & rect, const opal::Color & color);
   
   void           update_bbox ();
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimWidgetBaseDebug (const PrimWidgetBaseDebug &other);
   PrimWidgetBaseDebug &   operator = (const PrimWidgetBaseDebug &other);
   bool           operator == (const PrimWidgetBaseDebug &other);
   bool           operator != (const PrimWidgetBaseDebug &other);

}; // class PrimWidgetBaseDebug



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opak/PrimWidgetBaseDebug.hpp"



#endif   // ohm_opa_PrimWidgetBaseDebug_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
