/*****************************************************************************

        BundleTaxiGhost.cpp
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

#include "client/BundleTaxiGhost.h"

#include "client/Constants.h"
#include "client/SkeletonGui.h"

#include "model/Taxi.h"
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

BundleTaxiGhost::BundleTaxiGhost (model::Taxi & taxi, ohm::opa::Bundle & parent)
:  ohm::opa::Bundle (parent, "BundleTaxiGhost")
,  _taxi (taxi)
,  _parent (parent)
{
   using namespace ohm;
   
   _layer_aptr = new opak::LayerTransparency;
   parent.get_slot_layer ().push_back (*_layer_aptr);
   
   _layer_aptr->_opacity = .5f;
   
   _prim_aptr = new PrimTaxi;
   _layer_aptr->push_back (*_prim_aptr);
   
   // post
   
   _prim_aptr->_origin._x = _taxi.get_position_x ();
   _prim_aptr->_origin._y = _taxi.get_position_y ();
   _prim_aptr->_width = _taxi.get_width ();
   _prim_aptr->_color = _taxi.get_color ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

BundleTaxiGhost::~BundleTaxiGhost ()
{
}



/*
==============================================================================
Name : update
==============================================================================
*/

void  BundleTaxiGhost::update (const model::SelectionChangeInfo & info)
{
   const int type = info.get_change_type ();
   
   switch (type)
   {
   case model::Selection::Change_OBJ_MOVE_X:
      update_obj_x (info);
      break;

   case model::Selection::Change_TAXI_WIDTH:
      update_taxi_width (info);
      break;

   case model::Selection::Change_TAXI_COLOR:
      update_taxi_color (info);
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

void  BundleTaxiGhost::update_obj_x (const model::SelectionChangeInfo & info)
{
   using namespace ohm;
   
   const archi::Float64 position_x_delta = info.use_data <archi::Float64> ();
   
   _prim_aptr->_origin._x = _taxi.get_position_x () + position_x_delta;
}



/*
==============================================================================
Name : update_taxi_width
==============================================================================
*/

void  BundleTaxiGhost::update_taxi_width (const model::SelectionChangeInfo & info)
{
   using namespace ohm;
   
   const archi::Float64 width_delta = info.use_data <archi::Float64> ();
   
   _prim_aptr->_width = _taxi.get_width () + width_delta;
}



/*
==============================================================================
Name : update_taxi_color
==============================================================================
*/

void  BundleTaxiGhost::update_taxi_color (const model::SelectionChangeInfo & info)
{
   _prim_aptr->_color = info.use_data <model::Taxi::Color> ();
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
