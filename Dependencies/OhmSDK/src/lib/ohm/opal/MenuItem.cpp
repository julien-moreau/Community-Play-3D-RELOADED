/*****************************************************************************

        MenuItem.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 57659

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

#include "ohm/opal/MenuItem.h"

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
==============================================================================
*/

MenuItem::MenuItem (MenuContainer & parent)
:  MenuContainer (0, parent)
,  _title ()
,  _title_modified_flag (false)
,  _mark_flag (false)
,  _mark_modified_flag (false)
,  _enable_flag (true)
,  _enable_modified_flag (false)
,  _keystroke ()
,  _keystroke_modified_flag (false)
,  _std_id (MenuStdId_NONE)
,  _std_id_modified_flag (false)
,  _std_id_delegate ()
,  _delegate ()
,  _delegate_modified_flag (false)
,  _quit_flag (false)
,  _preferences_flag (false)
,  _user_data_ptr (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

MenuItem::~MenuItem ()
{
}



/*
==============================================================================
Name : synchronize
==============================================================================
*/

void  MenuItem::synchronize ()
{
   _title_modified_flag = false;
   _mark_modified_flag = false;
   _enable_modified_flag = false;
   _keystroke_modified_flag = false;
   _std_id_modified_flag = false;
   _delegate_modified_flag = false;
   
   MenuContainer::synchronize ();
}



/*
==============================================================================
Name : set_title
==============================================================================
*/

void  MenuItem::set_title (const std::string & title)
{
   if (_title != title)
   {
      _title = title;
      
      modify ();
      _title_modified_flag = true;
   }
}



/*
==============================================================================
Name : was_title_modified
==============================================================================
*/

bool  MenuItem::was_title_modified () const
{
   return _title_modified_flag;
}



/*
==============================================================================
Name : get_title
==============================================================================
*/

const std::string &  MenuItem::get_title () const
{
   return _title;
}



/*
==============================================================================
Name : mark
==============================================================================
*/

void  MenuItem::mark (bool mark_flag)
{
   if (_mark_flag != mark_flag)
   {
      _mark_flag = mark_flag;
      
      modify ();
      _mark_modified_flag = true;
   }
}



/*
==============================================================================
Name : was_mark_modified
==============================================================================
*/

bool  MenuItem::was_mark_modified () const
{
   return _mark_modified_flag;
}



/*
==============================================================================
Name : is_marked
==============================================================================
*/

bool  MenuItem::is_marked () const
{
   return _mark_flag;
}



/*
==============================================================================
Name : enable
==============================================================================
*/

void  MenuItem::enable (bool enable_flag)
{
   if (_enable_flag != enable_flag)
   {
      _enable_flag = enable_flag;
      
      modify ();
      _enable_modified_flag = true;
   }
}



/*
==============================================================================
Name : was_enable_modified
==============================================================================
*/

bool  MenuItem::was_enable_modified () const
{
   return _enable_modified_flag;
}



/*
==============================================================================
Name : is_enabled
==============================================================================
*/

bool  MenuItem::is_enabled () const
{
   return _enable_flag;
}



/*
==============================================================================
Name : set_keystroke
==============================================================================
*/

void  MenuItem::set_keystroke (const Key & key)
{
   if (_keystroke != key)
   {
      _keystroke = key;
      
      modify ();
      _keystroke_modified_flag = true;
   }
}



/*
==============================================================================
Name : was_keystroke_modified
==============================================================================
*/

bool  MenuItem::was_keystroke_modified () const
{
   return _keystroke_modified_flag;
}



/*
==============================================================================
Name : get_delegate
==============================================================================
*/

const Key & MenuItem::get_keystroke () const
{
   return _keystroke;
}



/*
==============================================================================
Name : set_std_id
==============================================================================
*/

void  MenuItem::set_std_id (MenuStdId std_id)
{
   if (_std_id != std_id)
   {
      _std_id = std_id;
      
      modify ();
      _std_id_modified_flag = true;
   }
}



/*
==============================================================================
Name : was_std_id_modified
==============================================================================
*/

bool  MenuItem::was_std_id_modified () const
{
   return _std_id_modified_flag;
}



/*
==============================================================================
Name : get_std_id
==============================================================================
*/

MenuStdId   MenuItem::get_std_id () const
{
   return _std_id;
}



/*
==============================================================================
Name : bind_std_id
==============================================================================
*/

void  MenuItem::bind_std_id (const util::Delegate & delegate)
{
   _std_id_delegate = delegate;
}



/*
==============================================================================
Name : unbind_std_id
==============================================================================
*/

void  MenuItem::unbind_std_id ()
{
   _std_id_delegate.reset ();
}



/*
==============================================================================
Name : use_std_id_delegate
==============================================================================
*/

util::Delegate &  MenuItem::use_std_id_delegate ()
{
   return _std_id_delegate;
}



/*
==============================================================================
Name : bind
==============================================================================
*/

void  MenuItem::bind (const util::Delegate & delegate)
{
   if (_delegate != delegate)
   {
      _delegate = delegate;
      
      modify ();
      _delegate_modified_flag = true;
   }
}



/*
==============================================================================
Name : unbind
==============================================================================
*/

void  MenuItem::unbind ()
{
   if (_delegate.is_set ())
   {
      _delegate.reset ();
      
      modify ();
      _delegate_modified_flag = true;
   }
}



/*
==============================================================================
Name : was_delegate_modified
==============================================================================
*/

bool  MenuItem::was_delegate_modified () const
{
   return _delegate_modified_flag;
}



/*
==============================================================================
Name : use_delegate
==============================================================================
*/

util::Delegate &  MenuItem::use_delegate ()
{
   return _delegate;
}



/*
==============================================================================
Name : set_quit
==============================================================================
*/

void  MenuItem::set_quit ()
{
   _quit_flag = true;
}



/*
==============================================================================
Name : is_quit
==============================================================================
*/

bool  MenuItem::is_quit () const
{
   return _quit_flag;
}



/*
==============================================================================
Name : set_preferences
==============================================================================
*/

void  MenuItem::set_preferences ()
{
   _preferences_flag = true;
}



/*
==============================================================================
Name : is_preferences
==============================================================================
*/

bool  MenuItem::is_preferences () const
{
   return _preferences_flag;
}



/*
==============================================================================
Name : set_user_data
==============================================================================
*/

void  MenuItem::set_user_data (void * user_data_ptr)
{
   _user_data_ptr = user_data_ptr;
}



/*
==============================================================================
Name : get_user_data
==============================================================================
*/

void *   MenuItem::get_user_data () const
{
   return _user_data_ptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
