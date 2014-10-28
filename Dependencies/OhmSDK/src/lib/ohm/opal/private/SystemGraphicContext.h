/*****************************************************************************

        SystemGraphicContext.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 31091

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



#if ! defined (ohm_opal_SystemGraphicContext_HEADER_INCLUDED)
#define  ohm_opal_SystemGraphicContext_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   #include <string>
   
   #define  NOMINMAX
   #define  WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <windowsx.h>
   #include <d3d9.h>
#endif   // ohm_archi_SYS



namespace ohm
{
namespace opal
{


#if (ohm_archi_SYS == ohm_archi_SYS_WIN)

   // apriori a HDC context
   #error to do
   
#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   typedef CGContextRef SystemGraphicContext;

#else
   #error Operating System not supported
#endif



}  // namespace opal
}  // namespace ohm



//#include  "ohm/opal/SystemGraphicContext.hpp"



#endif   // ohm_opal_SystemGraphicContext_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
