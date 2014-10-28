/*****************************************************************************

        FTLibrary.cpp
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 39942

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

#include "ohm/opal/freetype/FTLibrary.h"
#include "ohm/opal/LabelString.h"
#include "ohm/opal/FontManager.h"
#include "ohm/opal/PixArgb.h"
#include "ohm/opal/def.h"

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

FTLibrary::~FTLibrary ()
{
}



/*
==============================================================================
Name : register_user_input_thread
==============================================================================
*/

void  FTLibrary::register_user_input_thread ()
{
   FT_Error err = FT_Init_FreeType (&_user_input_thread_library);
   
   assert (err == 0);

   _user_input_thread_id = sys::ThreadMgr::get_current_thread_id ();
}



/*
==============================================================================
Name : register_video_thread
==============================================================================
*/

void  FTLibrary::register_video_thread ()
{
   FT_Error err = FT_Init_FreeType (&_video_thread_library);
   
   assert (err == 0);

   _video_thread_id = sys::ThreadMgr::get_current_thread_id ();
}



/*
==============================================================================
Name : unregister_user_input_thread
==============================================================================
*/

void  FTLibrary::unregister_user_input_thread ()
{
   assert (sys::ThreadMgr::get_current_thread_id () == _user_input_thread_id);
   
   FT_Error err = FT_Done_FreeType (_user_input_thread_library);
   assert (err == 0);
}



/*
==============================================================================
Name : unregister_video_thread
==============================================================================
*/

void  FTLibrary::unregister_video_thread ()
{
   assert (sys::ThreadMgr::get_current_thread_id () == _video_thread_id);
   
   FT_Error err = FT_Done_FreeType (_video_thread_library);
   assert (err == 0);
}



/*
==============================================================================
Name : get
==============================================================================
*/

FT_Library &   FTLibrary::get ()
{
   sys::ThreadMgr::IdType cur_thread_id = sys::ThreadMgr::get_current_thread_id ();
   
   FT_Library * library_ptr = 0;
   
   if (cur_thread_id == _user_input_thread_id)
   {
      assert (_user_input_thread_library != 0);
      
      library_ptr = &_user_input_thread_library;
   }
   else if (cur_thread_id == _video_thread_id)
   {
      assert (_video_thread_library != 0);
      
      library_ptr = &_video_thread_library;
   }
   else
   {
      assert (_user_input_thread_id != sys::ThreadMgr::get_invalid_thread_id ());
      assert (_video_thread_id != sys::ThreadMgr::get_invalid_thread_id ());
      assert (_user_input_thread_id != _video_thread_id);
   
      assert (false);
   }
   
   assert (library_ptr != 0);
   
   return *library_ptr;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

FTLibrary::FTLibrary ()
:  _user_input_thread_id (sys::ThreadMgr::get_invalid_thread_id ())
,  _video_thread_id (sys::ThreadMgr::get_invalid_thread_id ())
,  _user_input_thread_library (0)
,  _video_thread_library (0)
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
