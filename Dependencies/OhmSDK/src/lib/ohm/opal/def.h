/*****************************************************************************

        def.h
        Copyright (c) 2008 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 71201

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



#if ! defined (ohm_opal_def_HEADER_INCLUDED)
#define  ohm_opal_def_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

// don't forget to add #include "ohm/opal/def.h"

#define  ohm_opal_NEW_CURSOR_API_FLAG

#define  ohm_opal_API_UNDEFINED        (1)
#define  ohm_opal_API_OPENGL           (2)
#define  ohm_opal_API_DIRECTX          (3)

#define  ohm_opal_TEXT_API_UNDEFINED   (1)
#define  ohm_opal_TEXT_API_NATIVE      (2)
#define  ohm_opal_TEXT_API_FREETYPE    (3)



#if (ohm_archi_SYS == ohm_archi_SYS_WIN)
   
   #define ohm_opal_API       (ohm_opal_API_DIRECTX)
   #define ohm_opal_TEXT_API  (ohm_opal_TEXT_API_FREETYPE)

#elif (ohm_archi_SYS == ohm_archi_SYS_MACOS)
   
   #define ohm_opal_API       (ohm_opal_API_OPENGL)
   #define ohm_opal_TEXT_API  (ohm_opal_TEXT_API_FREETYPE)

#else

   #define ohm_opal_API       (ohm_opal_API_UNDEFINED)
   #define ohm_opal_TEXT_API  (ohm_opal_TEXT_API_UNDEFINED)

#error opal is not supported on this platform

#endif




#endif   // ohm_opal_def_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
