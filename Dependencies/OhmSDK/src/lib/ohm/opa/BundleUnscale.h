/*****************************************************************************

        BundleUnscale.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 52448

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



#if ! defined (ohm_opa_BundleUnscale_HEADER_INCLUDED)
#define  ohm_opa_BundleUnscale_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Bundle.h"
#include "ohm/opal/AutoFramePtr.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opa/private/FrameUnscale.h"
#include "ohm/opa/private/PrimUnscale.h"


#include <memory>



namespace ohm
{
namespace opa
{



class BundleUnscale
:  public opa::Bundle
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
                  BundleUnscale ();
                  BundleUnscale (opa::Bundle & parent_bundle, size_t slot_nbr = 0);
   virtual        ~BundleUnscale ();
   
   void           set_origin (const opa::Point & origin, double transition_time = 0.0, opak::TransitionFunction transition_function = opak::step);
   const opa::Point &
                  get_origin () const;
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   opal::AutoFramePtr <FrameUnscale>
                  _frame_aptr;
   opak::AutoPrimPtr <PrimUnscale>
                  _layer_aptr;
   


/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BundleUnscale (const BundleUnscale &other);
   BundleUnscale &      operator = (const BundleUnscale &other);
   bool           operator == (const BundleUnscale &other);
   bool           operator != (const BundleUnscale &other);

}; // class BundleUnscale



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opa
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "ohm/opa/BundleUnscale.hpp"



#endif   // ohm_opa_BundleUnscale_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
