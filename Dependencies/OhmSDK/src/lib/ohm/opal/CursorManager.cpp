/*****************************************************************************

        CursorManager.cpp
        Copyright (c) 2009 Ohm Force

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

#include "ohm/opal/CursorManager.h"
#include "ohm/opal/ImageLoader.h"
#include "ohm/opal/Cursor.h"
#include "ohm/opa/Point.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/lang/fnc.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
==============================================================================
*/

CursorManager::~CursorManager ()
{
   release_all ();
}



/*
==============================================================================
Name : get
==============================================================================
*/

Cursor & CursorManager::get (const std::string & path, const opa::Point & hot_point)
{
   Key key;
   key._path = path;
   key._hot_point = hot_point;
   
   Cursor * cursor_ptr = get (key);
   
   assert (cursor_ptr != 0);
   
   return *cursor_ptr;
}



/*
==============================================================================
Name : get
==============================================================================
*/

Cursor & CursorManager::get (const std::pair <std::string, opa::Point> & cursor)
{
   Key key;
   key._path = cursor.first;
   key._hot_point = cursor.second;
   
   Cursor * cursor_ptr = get (key);
   
   assert (cursor_ptr != 0);
   
   return *cursor_ptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
==============================================================================
*/

CursorManager::CursorManager ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : release_all
==============================================================================
*/

void  CursorManager::release_all ()
{
   KeyCursorPtrMap::iterator it = _key_cursor_ptr_map.begin ();
   KeyCursorPtrMap::iterator it_end = _key_cursor_ptr_map.end ();
   
   for (; it != it_end ; ++it)
   {
      Cursor * cursor_ptr = it->second;
      assert (cursor_ptr != 0);
      
      delete cursor_ptr;
   }
   
   _key_cursor_ptr_map.clear ();
}




/*
==============================================================================
Name : get
==============================================================================
*/

Cursor * CursorManager::get (const Key & key)
{
   KeyCursorPtrMap::iterator it = _key_cursor_ptr_map.find (key);
   
   if (it == _key_cursor_ptr_map.end ())
   {
      Bitmap bitmap;
      bool premultiply_flag = true;
      bool ok_flag = ImageLoader::use_instance ().load (
         bitmap, key._path, premultiply_flag
      );
      assert (ok_flag);
      
      if (!ok_flag)
      {
         return 0;
      }
      
      Cursor * cursor_ptr = new Cursor (bitmap, key._hot_point);
      
      std::pair <KeyCursorPtrMap::iterator, bool> ret
         = _key_cursor_ptr_map.insert (
         std::make_pair (key, cursor_ptr)
      );
      assert (ret.second);
      
      it = ret.first;
   }
   
   return it->second;
}
   


/*
==============================================================================
Name : ImageContainer::operator <
==============================================================================
*/

bool  CursorManager::Key::operator < (const Key & other) const
{
   int c = lang::cmp (_path, other._path);
   if (c != 0) return c < 0;
   
   c = lang::cmp (_hot_point, other._hot_point);
   return c < 0;
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
