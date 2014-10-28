/*****************************************************************************

        FramePath.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 53340

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



#if defined (ohm_opal_FramePath_CURRENT_HEADER)
   #error Recursive inclusion of ohm/FramePath header.
#endif
#define  ohm_opal_FramePath_CURRENT_HEADER

#if ! defined (ohm_opal_FramePath_HEADER_INCLUDED)
#define  ohm_opal_FramePath_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <vector>
#include <memory>



namespace ohm
{
namespace opal
{



class Frame;

// size_t represented the loop_nbr when a frame
// is simulated as included multiple times in the
// same parent frame

typedef std::vector <std::pair <Frame *, size_t> > FramePath;



}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_FramePath_HEADER_INCLUDED

#undef ohm_opal_FramePath_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
