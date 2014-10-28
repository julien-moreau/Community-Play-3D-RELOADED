/*****************************************************************************

        ModelTweaker.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71422

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

#include "model/ModelTweaker.h"

#include <cassert>



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

ModelTweaker::ModelTweaker ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ModelTweaker::~ModelTweaker ()
{
}



/*
==============================================================================
Name : process
==============================================================================
*/

void  ModelTweaker::process (Root & root)
{
   // process_align_pedestrian_position (root);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : process_align_pedestrian_position
==============================================================================
*/

void  ModelTweaker::process_align_pedestrian_position (Root & root)
{
   SpacialObjectColl & spacial_object_coll = root.use_spacial_object_coll ();
   
   SpacialObjectColl::iterator it = spacial_object_coll.begin ();
   const SpacialObjectColl::iterator it_end = spacial_object_coll.end ();
   
   for (; it != it_end ; ++it)
   {
      SpacialObject & spacial_object = *it;
      
      Pedestrian * pedestrian_ptr = dynamic_cast <Pedestrian *> (&spacial_object);
      
      if (pedestrian_ptr != 0)
      {
         process_align_pedestrian_position (*pedestrian_ptr);
      }
   }
}



/*
==============================================================================
Name : process_align_pedestrian_position
==============================================================================
*/

void  ModelTweaker::process_align_pedestrian_position (Pedestrian & pedestrian)
{
   using namespace ohm;
   
   archi::Float64 x = 200.0;
   archi::Float64 y = pedestrian.get_position_y ();
   
   pedestrian.ext_set_position (x, y);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
