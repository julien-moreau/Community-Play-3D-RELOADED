/*****************************************************************************

        SessionManager.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 45327

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

#include "ohm/flip/SessionManager.h"

#include <cassert>



namespace ohm
{
namespace flip
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

SessionManager::SessionManager ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

SessionManager::~SessionManager ()
{
}



/*
==============================================================================
Name : has_revision
Description :
   Returns true if we have at least one version of the document
==============================================================================
*/

bool  SessionManager::has_revision () const
{
   // return true if we have at least one version of the document
   
   return false;
}



/*
==============================================================================
Name : has_offline_work
Description :
   Returns true if :
   - head exists, and
   - head is different from revision (except revision number)
==============================================================================
*/

bool  SessionManager::has_offline_work () const
{
   // return true if head is really different from revision
   
   return false;
}



/*
==============================================================================
Name : read_revision
==============================================================================
*/

const std::string &  SessionManager::read_revision ()
{
   assert (has_revision ());
   
   assert (false);
   
   static std::string dummy;
   return dummy;
}



/*
==============================================================================
Name : read_head
==============================================================================
*/

const std::string &  SessionManager::read_head ()
{
   assert (has_offline_work ());
   
   assert (false);
   
   static std::string dummy;
   return dummy;
}



/*
==============================================================================
Name : write_revision
==============================================================================
*/

void  SessionManager::write_revision (const std::string & document)
{
   // invalidate head
   // ie after this call head == revision
}



/*
==============================================================================
Name : write_head
==============================================================================
*/

void  SessionManager::write_head (const std::string & document)
{
   // check if head and revision are different
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
