/*****************************************************************************

        ScreensImpl.h
        Copyright (c) 2007 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 69761

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



#if ! defined (ohm_opal_ScreensImpl_HEADER_INCLUDED)
#define  ohm_opal_ScreensImpl_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opa/Rect.h"

#define  WIN32_LEAN_AND_MEAN
#define  NOMINMAX
#include <windows.h>

#include <vector>

#define OHM_OPAL_MULTIPLE_MONITOR_IMPL_FLAG



namespace ohm
{
namespace opal
{



class ScreensImpl
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   virtual        ~ScreensImpl ();
   
   static std::vector <opa::Rect>
                  get_layout ();
   static opa::Rect     
                  get_main_screen_without_decorations ();

   

/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:
   


/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
   
#if defined (OHM_OPAL_MULTIPLE_MONITOR_IMPL_FLAG)
      static ::BOOL CALLBACK
                  monitor_enum_proc (::HMONITOR monitor, ::HDC hdc, ::LPRECT rect_ptr, ::LPARAM data);
   
      static std::vector <opa::Rect>
                  _rect_arr;
      
#endif



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:
                  ScreensImpl ();
                  ScreensImpl (const ScreensImpl &other);
   ScreensImpl &  operator = (const ScreensImpl &other);
   bool           operator == (const ScreensImpl &other);
   bool           operator != (const ScreensImpl &other);

}; // class ScreensImpl



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_opal_ScreensImpl_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
