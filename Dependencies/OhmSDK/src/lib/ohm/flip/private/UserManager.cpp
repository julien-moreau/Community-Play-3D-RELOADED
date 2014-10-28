/*****************************************************************************

        UserManager.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 49422

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

#include "ohm/flip/private/UserManager.h"
#include "ohm/flip/private/SeedGenerator.h"
#include "ohm/lang/fnc.h"

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
   Server version
==============================================================================
*/

UserManager::UserManager ()
:  _user_name ("")
,  _self_user_id (SeedGenerator::SERVER_USER_ID)
,  _user_name_id_map ()
{
}



/*
==============================================================================
Name : ctor
Description :
   Client version
==============================================================================
*/

UserManager::UserManager (const std::string & user_name)
:  _user_name (user_name)
,  _self_user_id (0)
,  _user_name_id_map ()
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

UserManager::~UserManager ()
{
}



/*
==============================================================================
Name : create_user_id
Description :
   Allocate a new user id for user name
   Server only
==============================================================================
*/

archi::Int32   UserManager::create_user_id (const std::string & user_name)
{
   assert (_user_name == "");
   assert (_user_name_id_map.find (user_name) == _user_name_id_map.end ());
   
   assert (SeedGenerator::EDITOR_USER_ID_START_NBR > 0);
   archi::Int32 max_user_id = SeedGenerator::EDITOR_USER_ID_START_NBR - 1;
   
   UserNameIdMap::const_iterator it = _user_name_id_map.begin ();
   const UserNameIdMap::const_iterator it_end = _user_name_id_map.end ();
   
   for (; it != it_end ; ++it)
   {
      archi::Int32 sub_user_id = it->second;
      assert (sub_user_id >= SeedGenerator::EDITOR_USER_ID_START_NBR);
      
      lang::max_in_place (max_user_id, sub_user_id);
   }
   
   archi::Int32 user_id = max_user_id + 1;
   
   _user_name_id_map [user_name] = user_id;
   
   return user_id;
}



/*
==============================================================================
Name : assign
==============================================================================
*/

void  UserManager::assign (const UserNameIdMap & user_map)
{
   _user_name_id_map = user_map;
}



/*
==============================================================================
Name : set_user_id
Description :
   set a user name/id pair
Note :
   This is called in the following case
   - server send a new name/id pair on connection of a user
   - client or server set name/id pairs while reading documents
   For a single persistent session, user id are not redistributed
==============================================================================
*/

void  UserManager::set_user_id (const std::string & user_name, archi::Int32 user_id)
{
   assert (user_name != "");
   assert (user_id != SeedGenerator::SERVER_USER_ID);
   assert (user_id >= SeedGenerator::EDITOR_USER_ID_START_NBR);
   assert (user_id <= SeedGenerator::EDITOR_USER_ID_MAX);
   
   if (user_name == _user_name)
   {
      _self_user_id = user_id;
   }
   else
   {
      // we must first receive our user id
      
      assert (_self_user_id != 0);
   }
   
   _user_name_id_map [user_name] = user_id;
}



/*
==============================================================================
Name : has_user
==============================================================================
*/

bool  UserManager::has_user (const std::string & user_name) const
{
   return _user_name_id_map.find (user_name) != _user_name_id_map.end ();
}



/*
==============================================================================
Name : get_user_id
==============================================================================
*/

archi::Int32   UserManager::get_user_id (const std::string & user_name) const
{
   UserNameIdMap::const_iterator it = _user_name_id_map.find (user_name);
   assert (it != _user_name_id_map.end ());
   
   return it->second;
}



/*
==============================================================================
Name : get_self_user_id
==============================================================================
*/

archi::Int32   UserManager::get_self_user_id () const
{
   return _self_user_id;
}



/*
==============================================================================
Name : get_self_user_name
==============================================================================
*/

const std::string &  UserManager::get_self_user_name () const
{
   // check that this is not master
   assert (_user_name != "");
   
   return _user_name;
}



/*
==============================================================================
Name : get_user_name_id_map
==============================================================================
*/

const UserManager::UserNameIdMap &  UserManager::get_user_name_id_map () const
{
   return _user_name_id_map;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
