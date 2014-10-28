/*****************************************************************************

        PixArgb.h
        Copyright (c) 2004 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 17991

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



#if defined (ohm_opal_PixArgb_CURRENT_HEADER)
   #error Recursive inclusion of ohm/PixArgb header.
#endif
#define  ohm_opal_PixArgb_CURRENT_HEADER

#if ! defined (ohm_opal_PixArgb_HEADER_INCLUDED)
#define  ohm_opal_PixArgb_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/opal/private/PixArgbBE.h"
#include "ohm/opal/private/PixArgbLE.h"



namespace ohm
{
namespace opal
{



#if (ohm_archi_ENDIAN == ohm_archi_ENDIAN_LITTLE)
   
   typedef PixArgbLE PixArgb;
   
#elif (ohm_archi_ENDIAN == ohm_archi_ENDIAN_BIG)
   
   typedef PixArgbBE PixArgb;
   
#endif



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_PixArgb_HEADER_INCLUDED

#undef ohm_opal_PixArgb_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
