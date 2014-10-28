/*****************************************************************************

        PrimTip.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 68114

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



#if ! defined (ohm_opa_PrimTip_HEADER_INCLUDED)
#define  ohm_opa_PrimTip_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opak/base/LabelSimpleHelper.h"
#include "ohm/opak/base/Primitive.h"
#include "ohm/opak/base/TypeEnum.h"
#include "ohm/opak/base/TypeString.h"


namespace ohm
{
namespace opa
{



class PrimTip
:  public virtual opak::Primitive
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   enum Layout
   {
                  Layout_LEFT = 0,
                  Layout_TOP_LEFT,
                  Layout_TOP_CENTER,
                  Layout_TOP_RIGHT,
                  Layout_RIGHT,
                  Layout_DOWN_RIGHT,
                  Layout_DOWN_CENTER,
                  Layout_DOWN_LEFT,
   };
   
                  PrimTip ();

   // Video Thread
   virtual void   init_display ();
   virtual void   restore_display ();
   virtual void   display (opak::GraphicContext & context);
   
   opak::TypeEnum <PrimTip, Layout, Layout_LEFT>
                  _layout;
   opak::TypeString <PrimTip>
                  _label;
   

   
/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

   virtual        ~PrimTip ();
   
   

/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           bbox_changed ();
   void           label_changed ();
   void           compute_metrics (opa::Margin & margin, opa::Point & label_pt);

   std::auto_ptr <opak::LabelSimpleHelper>
                  _label_aptr;

   

/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  PrimTip (const PrimTip &other);
   PrimTip &      operator = (const PrimTip &other);
   bool           operator == (const PrimTip &other) const;
   bool           operator != (const PrimTip &other) const;

}; // class PrimTip



}  // namespace opa
}  // namespace ohm



//#include  "ohm/opa/PrimTip.hpp"



#endif   // ohm_opa_PrimTip_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

