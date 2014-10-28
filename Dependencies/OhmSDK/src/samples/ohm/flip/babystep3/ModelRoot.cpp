/*****************************************************************************

        ModelRoot.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70401

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

#include "ModelRoot.h"

#include "ohm/flip/ClassDescription.h"



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : declare
==============================================================================
*/

void  ModelRoot::declare ()
{
   using namespace ohm::flip;
   
   ClassDescManager::use_instance ().declare_basic_types ();
   
   ClassDescription <ModelRoot>::use ().set_name ("ohm.studio.ModelRoot");
   ClassDescription <ModelRoot>::use ().push_var_desc (&ModelRoot::_point_x, "_point_x");
   ClassDescription <ModelRoot>::use ().push_var_desc (&ModelRoot::_point_y, "_point_y");
   
   ClassDescription <ModelRoot>::use ().enable_root ("v1.0");
   
   ClassDescManager::declare (ClassDescription <ModelRoot>::use ());
   
   ClassDescManager::use_instance ().post_check ();
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

ModelRoot::ModelRoot (ohm::flip::DocumentBase & document)
:  ohm::flip::RootBase (document)
,  _point_x (document)
,  _point_y (document)

,  _tx_session_guard (document)
{
}



/*
==============================================================================
Name : set_point
Description :
   Make a transaction in which '_point' is changed to 'point'.
   This transaction can be undone.
==============================================================================
*/
   
void  ModelRoot::set_point (float x, float y)
{
   // prepare undo
   _tx_session_guard.prepare_record (use_scribe ());
   
   if (!_tx_session_guard.start ()) return;

   _point_x = x;
   _point_y = y;
   
   set_scribe_metadata ("Change Point");
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name : point_changed
Description :
   Returns true iff _point_* changed.
==============================================================================
*/
   
bool  ModelRoot::point_changed () const
{
   return _point_x.did_value_change () || _point_y.did_value_change ();
}



/*
==============================================================================
Name : get_point_x
Description :
   Returns the point x as stored by the model.
==============================================================================
*/
   
float ModelRoot::get_point_x () const
{
   return _point_x;
}



/*
==============================================================================
Name : get_point_y
Description :
   Returns the point y as stored by the model.
==============================================================================
*/
   
float ModelRoot::get_point_y () const
{
   return _point_y;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
