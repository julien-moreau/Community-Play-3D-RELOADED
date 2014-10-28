/*****************************************************************************

        AllocTools.h
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 15581

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



#if ! defined (ohm_mem_mmgr_AllocTools_HEADER_INCLUDED)
#define  ohm_mem_mmgr_AllocTools_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/mem/mmgr/AllocToolsGeneric.h"
#include "ohm/mem/mmgr/AllocToolsPod.h"



namespace ohm
{
namespace mem
{
namespace mmgr
{



template <class T>
class AllocTools
:  public AllocToolsGeneric <T>
{
   // Nothing
}; // class AllocTools



#define  ohm_mem_mmgr_AllocTools_DEF_POD(Type)  \
template <>                                     \
class AllocTools <Type>                         \
:  public AllocToolsPod <Type>                  \
{                                               \
}

ohm_mem_mmgr_AllocTools_DEF_POD (int);
ohm_mem_mmgr_AllocTools_DEF_POD (unsigned int);
ohm_mem_mmgr_AllocTools_DEF_POD (short);
ohm_mem_mmgr_AllocTools_DEF_POD (unsigned short);
ohm_mem_mmgr_AllocTools_DEF_POD (char);
ohm_mem_mmgr_AllocTools_DEF_POD (unsigned char);
ohm_mem_mmgr_AllocTools_DEF_POD (signed char);
ohm_mem_mmgr_AllocTools_DEF_POD (long);
ohm_mem_mmgr_AllocTools_DEF_POD (unsigned long);
ohm_mem_mmgr_AllocTools_DEF_POD (float);
ohm_mem_mmgr_AllocTools_DEF_POD (double);
ohm_mem_mmgr_AllocTools_DEF_POD (long double);
ohm_mem_mmgr_AllocTools_DEF_POD (void *);
ohm_mem_mmgr_AllocTools_DEF_POD (const void *);
// Other types may be put here (long long for example, or SIMD types)

#undef   ohm_mem_mmgr_AllocTools_DEF


}  // namespace mmgr
}  // namespace mem
}  // namespace ohm



//#include  "ohm/mem/AllocTools.hpp"



#endif   // ohm_mem_mmgr_AllocTools_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
