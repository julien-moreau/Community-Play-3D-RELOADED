/*****************************************************************************

        ScreensImpl.cpp
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69549

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

#include "ohm/opal/win32/ScreensImpl.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
Throws : Nothing
==============================================================================
*/

ScreensImpl::~ScreensImpl ()
{
}



/*
==============================================================================
Name : get_layout
Throws : Nothing
==============================================================================
*/

std::vector <opa::Rect> ScreensImpl::get_layout ()
{
#if defined (OHM_OPAL_MULTIPLE_MONITOR_IMPL_FLAG)
   _rect_arr.clear ();
   
   // http://msdn.microsoft.com/en-us/library/windows/desktop/dd162610(v=vs.85).aspx
   
   ::BOOL ok_flag = EnumDisplayMonitors (0, 0, monitor_enum_proc, 0);
   assert (ok_flag);
   
   return _rect_arr;
   
#else
   // #warning partial implementation
   
   const int      scr_w = ::GetSystemMetrics (SM_CXSCREEN);
   const int      scr_h = ::GetSystemMetrics (SM_CYSCREEN);
   const opa::Rect   
                  rect (
      0, 0,
      float (scr_w), float (scr_h)
   );
   
   std::vector <opa::Rect> ret_val;
   ret_val.push_back (rect);
   
   return (ret_val);
   
#endif
}



/*
==============================================================================
Name : get_main_screen_without_decorations
Throws : Nothing
==============================================================================
*/

opa::Rect   ScreensImpl::get_main_screen_without_decorations ()
{
   opa::Rect ret_val;

   ::RECT win_rect;
   ::BOOL res_flag = SystemParametersInfo (SPI_GETWORKAREA, 0, &win_rect, 0);

   if (res_flag == TRUE)
   {
      ret_val.from_RECT (win_rect);
   }
   else
   {
      const int      scr_w = ::GetSystemMetrics (SM_CXFULLSCREEN);
      const int      scr_h = ::GetSystemMetrics (SM_CYFULLSCREEN);
      ret_val = opa::Rect (
         0, 0,
         float (scr_w), float (scr_h)
      );
   }
   
   return (ret_val);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : monitor_enum_proc
Throws : Nothing
==============================================================================
*/

#if defined (OHM_OPAL_MULTIPLE_MONITOR_IMPL_FLAG)
::BOOL CALLBACK   ScreensImpl::monitor_enum_proc (::HMONITOR monitor, ::HDC hdc, ::LPRECT rect_ptr, ::LPARAM data)
{
   // http://msdn.microsoft.com/en-us/library/windows/desktop/dd145061(v=vs.85).aspx
   
   opa::Rect rect;
   rect.from_RECT (*rect_ptr);
   
   _rect_arr.push_back (rect);
   
   return TRUE;
}
#endif



#if defined (OHM_OPAL_MULTIPLE_MONITOR_IMPL_FLAG)
std::vector <opa::Rect> ScreensImpl::_rect_arr;
#endif



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
