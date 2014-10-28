/*****************************************************************************

        BundlePedestrianGhost.h
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70478

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



#if ! defined (client_BundlePedestrianGhost_HEADER_INCLUDED)
#define  client_BundlePedestrianGhost_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/PrimPedestrian.h"


#include "ohm/opa/Bundle.h"
#include "ohm/opak/AutoPrimPtr.h"
#include "ohm/opak/LayerTransparency.h"



namespace model
{
class Pedestrian;
class SelectionChangeInfo;
}


namespace client
{



class BundlePedestrianGhost
:  public ohm::opa::Bundle
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
                  BundlePedestrianGhost (model::Pedestrian & pedestrian, ohm::opa::Bundle & parent);
   virtual        ~BundlePedestrianGhost ();
   
   void           update (const model::SelectionChangeInfo & info);
   


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
   void           update_obj_x (const model::SelectionChangeInfo & info);
   void           update_obj_y (const model::SelectionChangeInfo & info);


   model::Pedestrian &
                  _pedestrian;
   ohm::opa::Bundle &
                  _parent;

   ohm::opak::AutoPrimPtr <ohm::opak::LayerTransparency>
                  _layer_aptr;
   ohm::opak::AutoPrimPtr <PrimPedestrian>
                  _prim_aptr;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  BundlePedestrianGhost ();
                  BundlePedestrianGhost (const BundlePedestrianGhost &other);
   BundlePedestrianGhost &
                  operator = (const BundlePedestrianGhost &other);
   bool           operator == (const BundlePedestrianGhost &other);
   bool           operator != (const BundlePedestrianGhost &other);

}; // class BundlePedestrianGhost



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include  "client/BundlePedestrianGhost.hpp"



#endif   // client_BundlePedestrianGhost_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
