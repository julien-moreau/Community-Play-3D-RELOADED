/*****************************************************************************

        MenuBase.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70519

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

#include "ohm/opal/MenuBase.h"
#include "ohm/opal/MenuContainer.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
   Root version
==============================================================================
*/

MenuBase::MenuBase (int)
:  _modified_flag (false)
,  _parent_ptr (0)
{
}



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

MenuBase::MenuBase (MenuContainer & parent)
:  _modified_flag (false)
,  _parent_ptr (&parent)
{
   assert (&parent != 0);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

MenuBase::~MenuBase ()
{
}



/*
==============================================================================
Name : set_identifier
==============================================================================
*/

void  MenuBase::set_identifier (const std::string & identifier)
{
   assert (_identifier.empty ());
   
   _identifier = identifier;
}



/*
==============================================================================
Name : get_identifier
==============================================================================
*/

const std::string &  MenuBase::get_identifier () const
{
   return _identifier;
}



/*
==============================================================================
Name : get_parent
==============================================================================
*/

MenuContainer *   MenuBase::get_parent () const
{
   // might be null
   
   return _parent_ptr;
}



/*
==============================================================================
Name : was_modified
==============================================================================
*/

bool  MenuBase::was_modified () const
{
   return _modified_flag;
}



/*
==============================================================================
Name : is_updating
==============================================================================
*/

bool  MenuBase::is_updating () const
{
   assert (_parent_ptr != 0);
   
   return _parent_ptr->is_updating ();
}



/*
==============================================================================
Name : signal_pre
==============================================================================
*/

void  MenuBase::signal_pre ()
{
   assert (_parent_ptr != 0);
   
   _parent_ptr->signal_pre ();
}



/*
==============================================================================
Name : signal_post
==============================================================================
*/

void  MenuBase::signal_post ()
{
   assert (_parent_ptr != 0);
   
   _parent_ptr->signal_post ();
}



/*
==============================================================================
Name : synchronize
==============================================================================
*/

void  MenuBase::synchronize ()
{
   _modified_flag = false;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : modify
==============================================================================
*/

void  MenuBase::modify ()
{
   _modified_flag = true;
   
   if (_parent_ptr != 0)
   {
      _parent_ptr->modify ();
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
