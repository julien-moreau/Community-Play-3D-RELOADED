/*****************************************************************************

        BundleTaxiGhost.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70477

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



#if ! defined (client_BundleTaxiGhost_HEADER_INCLUDED)
#define  client_BundleTaxiGhost_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/PrimTaxi.h"


#include "ohm/opa/Bundle.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/LayerTransparency.h"



namespace model
{
class Taxi;
class SelectionChangeInfo;
}


namespace client
{



class BundleTaxiGhost
:  public ohm::opa::Bundle
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  BundleTaxiGhost (model::Taxi & taxi, ohm::opa::Bundle & parent);
   virtual        ~BundleTaxiGhost ();
   
   void           update (const model::SelectionChangeInfo & info);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           update_obj_x (const model::SelectionChangeInfo & info);
   void           update_taxi_width (const model::SelectionChangeInfo & info);
   void           update_taxi_color (const model::SelectionChangeInfo & info);


   model::Taxi &  _taxi;
   ohm::opa::Bundle &
                  _parent;

   ohm::opak::AutoPrimPtr <ohm::opak::LayerTransparency>
                  _layer_aptr;
   ohm::opak::AutoPrimPtr <PrimTaxi>
                  _prim_aptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BundleTaxiGhost ();
                  BundleTaxiGhost (const BundleTaxiGhost &other);
   BundleTaxiGhost & operator = (const BundleTaxiGhost &other);
   bool           operator == (const BundleTaxiGhost &other);
   bool           operator != (const BundleTaxiGhost &other);

}; // class BundleTaxiGhost



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/BundleTaxiGhost.hpp"



#endif   // client_BundleTaxiGhost_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
