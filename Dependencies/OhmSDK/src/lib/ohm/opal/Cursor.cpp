/*****************************************************************************

        Cursor.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 33064

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

#include "ohm/opal/Cursor.h"
#include "ohm/opal/Bitmap.h"
#include "ohm/opal/ImageLoader.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include "ohm/opal/win32/CursorImpl.h"

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   #include "ohm/opal/carbon/CursorImpl.h"

#else
   #error Operation System not supported

#endif

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

Cursor::Cursor (const Bitmap & bitmap, const opa::Point & hot_point)
:  _impl_ptr (0)
{
   _impl_ptr = new CursorImpl;
   
   create (bitmap, hot_point);
}



/*
==============================================================================
Name : ctor
==============================================================================
*/

Cursor::Cursor (const char * path_0, const opa::Point & hot_point)
:  _impl_ptr (0)
{
   _impl_ptr = new CursorImpl;
   
   Bitmap bitmap;
   
   ImageLoader::use_instance ().load (bitmap, path_0);
   
   create (bitmap, hot_point);
}



/*
==============================================================================
Name : dtor
==============================================================================
*/

Cursor::~Cursor ()
{
}



/*
==============================================================================
Name : set
==============================================================================
*/

void  Cursor::set ()
{
   assert (_impl_ptr != 0);
   
   _impl_ptr->set ();
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : create
==============================================================================
*/

void  Cursor::create (const Bitmap & bitmap, const opa::Point & hot_point)
{
   assert (_impl_ptr != 0);
   
   _impl_ptr->create (bitmap, hot_point);
}



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
