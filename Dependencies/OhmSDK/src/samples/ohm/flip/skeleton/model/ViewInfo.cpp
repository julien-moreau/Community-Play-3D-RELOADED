/*****************************************************************************

        ViewInfo.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70481

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

#include "model/ViewInfo.h"

#include "ohm/flip/ClassDescription.h"

#include <cassert>



namespace model
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : declare
Note :
   Rather than putting a _display_name into the client, we will fetch it
   when we would need it, as it might actually change
   User name is guaranteed to be unique and non changing, it is the one
   used for authentification
==============================================================================
*/

void  ViewInfo::declare ()
{
   using namespace ohm::flip;
   
   ClassDescription <ViewInfo>::use ().set_name ("skeleton.ViewInfo");
   ClassDescription <ViewInfo>::use ().push_var_desc (&ViewInfo::_focus_x, "_focus_x");
   ClassDescription <ViewInfo>::use ().push_var_desc (&ViewInfo::_focus_y, "_focus_y");
   
   ClassDescManager::declare (ClassDescription <ViewInfo>::use ());
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

ViewInfo::ViewInfo (ohm::flip::DocumentBase & document)
:  ohm::flip::Object (document)
,  _focus_x (document)
,  _focus_y (document)

,  _tx_session_guard (document)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

ViewInfo::~ViewInfo ()
{
}



/*
==============================================================================
Name : tx_set_focus
==============================================================================
*/

void  ViewInfo::tx_set_focus (ohm::archi::Float64 focus_x, ohm::archi::Float64 focus_y)
{
   if (!_tx_session_guard.start ())
   {
      return;
   }

   ext_set_focus (focus_x, focus_y);
   
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name : ext_set_focus
==============================================================================
*/

void  ViewInfo::ext_set_focus (ohm::archi::Float64 focus_x, ohm::archi::Float64 focus_y)
{
   _focus_x = focus_x;
   _focus_y = focus_y;
}



/*
==============================================================================
Name : focus_changed
==============================================================================
*/

bool  ViewInfo::focus_changed () const
{
   return _focus_x.did_value_change () || _focus_y.did_value_change ();
}



/*
==============================================================================
Name : get_focus_x
==============================================================================
*/

ohm::archi::Float64  ViewInfo::get_focus_x () const
{
   return _focus_x;
}



/*
==============================================================================
Name : get_focus_y
==============================================================================
*/

ohm::archi::Float64  ViewInfo::get_focus_y () const
{
   return _focus_y;
}



/*
==============================================================================
Name : start_session
==============================================================================
*/

bool  ViewInfo::start_session ()
{
   return _tx_session_guard.start ();
}



/*
==============================================================================
Name : commit_session
==============================================================================
*/

void  ViewInfo::commit_session ()
{
   _tx_session_guard.commit ();
}



/*
==============================================================================
Name : revert_session
==============================================================================
*/

void  ViewInfo::revert_session ()
{
   _tx_session_guard.revert ();
}



/*
==============================================================================
Name : session_set_focus
==============================================================================
*/

void  ViewInfo::session_set_focus (ohm::archi::Float64 focus_x, ohm::archi::Float64 focus_y)
{
   using namespace ohm;
   
   flip::TxSessionGuard::AutoWield auto_wield (_tx_session_guard);
   
   ext_set_focus (focus_x, focus_y);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace model



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
