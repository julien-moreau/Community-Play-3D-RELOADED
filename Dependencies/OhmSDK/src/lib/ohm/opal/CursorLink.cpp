/*****************************************************************************

        CursorLink.cpp
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71200

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

#include "ohm/opal/CursorLink.h"
#include "ohm/opal/CursorManager.h"
#include "ohm/opal/Application.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

CursorLink::CursorLink ()
:  _set_flag (false)
,  _depth (0)
{
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

CursorLink::~CursorLink ()
{
   try
   {
#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
      unset ();
#endif
   }
   catch (...)
   {
      assert (false);
   }
}



/*
==============================================================================
Name : set_depth
==============================================================================
*/

void  CursorLink::set_depth (int depth)
{
   _depth = depth;
}



/*
==============================================================================
Name : show
Description :
   Change the state of cursor
   (property of a cursor, does not affect cursor priority)
==============================================================================
*/

void  CursorLink::show (bool show_flag)
{
   if (show_flag)
   {
      Application::use_instance ().show_cursor (this, true);
   }
   else
   {
      Application::use_instance ().show_cursor (this, false);
   }
   
   _set_flag = true;
}



/*
==============================================================================
Name : shield
Description :
   Hide the cursor, and makes it automatically reappear as the user moves
   the mouse.
   (property of a cursor, does not affect cursor priority)
==============================================================================
*/

void  CursorLink::obscure ()
{
   Application::use_instance ().obscure_cursor (this);
   
   _set_flag = true;
}



/*
==============================================================================
Name : set
Description :
   (change the cursor priority)
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  CursorLink::set (StandardMouseCursor cursor)
{
   Application::use_instance ().set_cursor (this, _depth, cursor);
   
   _set_flag = true;
}
#endif



/*
==============================================================================
Name : set
Description :
   (change the cursor priority)
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  CursorLink::set (Cursor & cursor)
{
   Application::use_instance ().set_cursor (this, _depth, cursor);
   
   _set_flag = true;
}
#endif



/*
==============================================================================
Name : set
Description :
   (change the cursor priority)
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  CursorLink::set (const std::string & path, const opa::Point & hot_point)
{
   Cursor & cursor = CursorManager::use_instance ().get (path, hot_point);
   
   set (cursor);
}
#endif



/*
==============================================================================
Name : set_default
Description :
   (change the cursor priority)
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  CursorLink::set_default ()
{
   set (StandardMouseCursor_DEFAULT);
}
#endif



/*
==============================================================================
Name : force
Description :
   (change the cursor priority)
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  CursorLink::force (StandardMouseCursor cursor)
{
   Application::use_instance ().set_cursor (this, 1000000, cursor);
   
   _set_flag = true;
}
#endif



/*
==============================================================================
Name : force
Description :
   (change the cursor priority)
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  CursorLink::force (Cursor & cursor)
{
   Application::use_instance ().set_cursor (this, 1000000, cursor);
   
   _set_flag = true;
}
#endif



/*
==============================================================================
Name : force
Description :
   (change the cursor priority)
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  CursorLink::force (const std::string & path, const opa::Point & hot_point)
{
   Cursor & cursor = CursorManager::use_instance ().get (path, hot_point);
   
   force (cursor);
}
#endif



/*
==============================================================================
Name : force_default
Description :
   (change the cursor priority)
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  CursorLink::force_default ()
{
   force (StandardMouseCursor_DEFAULT);
}
#endif



/*
==============================================================================
Name : unset
Description :
   (change the cursor priority)
==============================================================================
*/

#if ! defined (ohm_opal_NEW_CURSOR_API_FLAG)
void  CursorLink::unset ()
{
   if (_set_flag)
   {
      Application::use_instance ().release_cursor (this);
      
      _set_flag = false;
   }
}
#endif



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
