/*****************************************************************************

        SelectionStateMgr.cpp
        Copyright (c) 2012 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71440

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

#include "client/SelectionStateMgr.h"
#include "model/Root.h"

#include "ohm/flip/private/DocumentBase.h"
#include "ohm/flip/Object.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace client
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

SelectionStateMgr::SelectionStateMgr (ohm::flip::Object & object)
:  _object (object)
,  _ext_selection_arr ()
,  _main_selected_flag (false)
,  _ext_selected_flag (false)
{
   init ();
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

SelectionStateMgr::~SelectionStateMgr ()
{
   // nothing
}



/*
==============================================================================
Name : update
==============================================================================
*/

void  SelectionStateMgr::update (bool selected_flag, ohm::archi::Int32 user_id)
{
   using namespace ohm;
   
   assert (user_id >= 0);
   
   if (user_id == _object.use_document ().get_user_id ())
   {
      _main_selected_flag = selected_flag;
   }
   else
   {
      size_t max_user_id = lang::max (_ext_selection_arr.size (), size_t (user_id + 1));
      _ext_selection_arr.resize (max_user_id, false);
      
      _ext_selection_arr [user_id] = selected_flag;
      
      if (!selected_flag)
      {
         for (size_t i = 0 ; i < _ext_selection_arr.size () ; ++i)
         {
            selected_flag |= _ext_selection_arr [i];
         }
      }
      
      _ext_selected_flag = selected_flag;
   }
}



/*
==============================================================================
Name : get_selected
==============================================================================
*/

bool  SelectionStateMgr::get_selected () const
{
   return _main_selected_flag;
}



/*
==============================================================================
Name : get_ext_selected
==============================================================================
*/

bool  SelectionStateMgr::get_ext_selected () const
{
   return _ext_selected_flag;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : init
==============================================================================
*/

void  SelectionStateMgr::init ()
{
   model::Root & root = _object.get_ancestor <model::Root> ();
   
   model::ClientDataColl & client_data_coll = root.use_client_data_coll ();
   
   model::ClientDataColl::iterator it = client_data_coll.begin ();
   model::ClientDataColl::iterator it_end = client_data_coll.end ();
   
   for (; it != it_end ; ++it)
   {
      model::ClientData & client_data = *it;
      
      init (client_data.use_selection ());
   }
}



/*
==============================================================================
Name : init
==============================================================================
*/

void  SelectionStateMgr::init (model::Selection & selection)
{
   using namespace ohm;
   
   bool found_flag = selection.contains (_object, flip::FindMask_CUR);
   
   if (found_flag)
   {
      update (true, selection.get_user_id ());
   }
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace client



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
