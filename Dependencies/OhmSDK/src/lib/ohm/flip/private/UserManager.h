/*****************************************************************************

        UserManager.h
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



#if ! defined (ohm_flip_UserManager_HEADER_INCLUDED)
#define  ohm_flip_UserManager_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"

#include <string>
#include <map>


namespace ohm
{
namespace flip
{



class UserManager
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   typedef std::map <std::string, archi::Int32> UserNameIdMap;
   
                  UserManager ();
                  UserManager (const std::string & user_name);
   virtual        ~UserManager ();
   
   archi::Int32   create_user_id (const std::string & user_name);
   void           assign (const UserNameIdMap & user_map);
   
   void           set_user_id (const std::string & user_name, archi::Int32 user_id);
   bool           has_user (const std::string & user_name) const;
   archi::Int32   get_user_id (const std::string & user_name) const;
   
   archi::Int32   get_self_user_id () const;
   const std::string &
                  get_self_user_name () const;
   
   const UserNameIdMap &
                  get_user_name_id_map () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   const std::string
                  _user_name;
   
   archi::Int32   _self_user_id;
   
   UserNameIdMap  _user_name_id_map;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  UserManager (const UserManager &other);
   UserManager &  operator = (const UserManager &other);
   bool           operator == (const UserManager &other);
   bool           operator != (const UserManager &other);

}; // class UserManager



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/UserManager.hpp"



#endif   // ohm_flip_UserManager_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
