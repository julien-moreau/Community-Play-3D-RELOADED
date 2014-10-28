/*****************************************************************************

        BundlePedestrianGhost.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "client/BundlePedestrianGhost.h"

#include "client/Constants.h"
#include "client/SkeletonGui.h"

#include "model/Pedestrian.h"
#include "model/SelectionChangeInfo.h"

#include "ohm/opa/BundleView.h"
#include "ohm/opa/EventBundleView.h"
#include "ohm/opal/Frame.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

BundlePedestrianGhost::BundlePedestrianGhost (model::Pedestrian & pedestrian, ohm::opa::Bundle & parent)
:  ohm::opa::Bundle (parent, "BundlePedestrianGhost")
,  _pedestrian (pedestrian)
,  _parent (parent)
{
   using namespace ohm;
   
   _layer_aptr = new opak::LayerTransparency;
   parent.get_slot_layer ().push_back (*_layer_aptr);
   
   _layer_aptr->_opacity = .5f;
   
   _prim_aptr = new PrimPedestrian;
   _layer_aptr->push_back (*_prim_aptr);
   
   // post
   
   _prim_aptr->_origin._x = _pedestrian.get_position_x ();
   _prim_aptr->_origin._y = _pedestrian.get_position_y ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BundlePedestrianGhost::~BundlePedestrianGhost ()
{
}



/*
==============================================================================
Name : update
==============================================================================
*/

void  BundlePedestrianGhost::update (const model::SelectionChangeInfo & info)
{
   const int type = info.get_change_type ();
   
   switch (type)
   {
   case model::Selection::Change_OBJ_MOVE_X:
      update_obj_x (info);
      break;

   case model::Selection::Change_OBJ_MOVE_Y:
      update_obj_y (info);
      break;
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : update_obj_x
==============================================================================
*/

void  BundlePedestrianGhost::update_obj_x (const model::SelectionChangeInfo & info)
{
   using namespace ohm;
   
   const archi::Float64 position_x_delta = info.use_data <archi::Float64> ();
   
   _prim_aptr->_origin._x = _pedestrian.get_position_x () + position_x_delta;
}



/*
==============================================================================
Name : update_obj_y
==============================================================================
*/

void  BundlePedestrianGhost::update_obj_y (const model::SelectionChangeInfo & info)
{
   using namespace ohm;
   
   const archi::Float64 position_y_delta = info.use_data <archi::Float64> ();
   
   _prim_aptr->_origin._y = _pedestrian.get_position_y () + position_y_delta;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
