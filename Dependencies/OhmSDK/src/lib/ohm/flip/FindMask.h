/*****************************************************************************

        FindMask.h
        Copyright (c) 2010 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 56568

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



#if ! defined (ohm_flip_FindMask_HEADER_INCLUDED)
#define  ohm_flip_FindMask_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace ohm
{
namespace flip
{



enum FindMask
{
   FindMask_ADDED    = 1 << 0,
   FindMask_RESIDENT = 1 << 1,
   FindMask_REMOVED  = 1 << 2,
   FindMask_POST     = 1 << 3,   // equivalent to ALL but only if we are in POST
   
   FindMask_ALL = FindMask_ADDED | FindMask_RESIDENT | FindMask_REMOVED,

   // only meaningful in POST : only consider elements that were not removed
   FindMask_CUR = FindMask_ADDED | FindMask_RESIDENT,

   // only meaningful in POST : only consider elements that were removed
   FindMask_OLD = FindMask_REMOVED,
   
   // same as if we would use std::find
   // that is everything in POST, and not removed outside of POST
   FindMask_NATURAL = FindMask_ADDED | FindMask_RESIDENT | FindMask_POST,

}; // enum FindMask




/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace flip
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



//#include "ohm/flip/FindMask.hpp"



#endif   // ohm_flip_FindMask_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
