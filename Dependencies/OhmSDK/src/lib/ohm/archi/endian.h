/*****************************************************************************

        endian.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 5142

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



#if defined (ohm_archi_endian_CURRENT_HEADER)
   #error Recursive inclusion of ohm/archi/endian header.
#endif
#define  ohm_archi_endian_CURRENT_HEADER

#if ! defined (ohm_archi_endian_HEADER_INCLUDED)
#define  ohm_archi_endian_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/Float32Rev.h"
#include "ohm/archi/Float64Rev.h"
#include "ohm/archi/Int16Rev.h"
#include "ohm/archi/Int32Rev.h"
#include "ohm/archi/Int64Rev.h"
#include "ohm/archi/UInt16Rev.h"
#include "ohm/archi/UInt32Rev.h"



namespace ohm
{
namespace archi
{



#if (ohm_archi_ENDIAN == ohm_archi_ENDIAN_LITTLE)



typedef  Float32Rev  Float32BE;
typedef  Float64Rev  Float64BE;
typedef  Int16Rev    Int16BE;
typedef  Int32Rev    Int32BE;
typedef  Int64Rev    Int64BE;
typedef  UInt16Rev   UInt16BE;
typedef  UInt32Rev   UInt32BE;

typedef  Float32     Float32LE;
typedef  Float64     Float64LE;
typedef  Int16       Int16LE;
typedef  Int32       Int32LE;
typedef  Int64       Int64LE;
typedef  UInt16      UInt16LE;
typedef  UInt32      UInt32LE;



#else    // ohm_archi_ENDIAN_LITTLE



typedef  Float32     Float32BE;
typedef  Float64     Float64BE;
typedef  Int16       Int16BE;
typedef  Int32       Int32BE;
typedef  Int64       Int64BE;
typedef  UInt16      UInt16BE;
typedef  UInt32      UInt32BE;

typedef  Float32Rev  Float32LE;
typedef  Float64Rev  Float64LE;
typedef  Int16Rev    Int16LE;
typedef  Int32Rev    Int32LE;
typedef  Int64Rev    Int64LE;
typedef  UInt16Rev   UInt16LE;
typedef  UInt32Rev   UInt32LE;



#endif   // ohm_archi_ENDIAN_LITTLE



inline Int16   preserve_endian (Int16 x);
inline UInt16  preserve_endian (UInt16 x);
inline Int32   preserve_endian (Int32 x);
inline UInt32  preserve_endian (UInt32 x);
inline Int64   preserve_endian (Int64 x);
inline Int16   preserve_endian (Int16Rev x);
inline UInt16  preserve_endian (UInt16Rev x);
inline Int32   preserve_endian (Int32Rev x);
inline UInt32  preserve_endian (UInt32Rev x);
inline Int64   preserve_endian (Int64Rev x);



}  // namespace archi
}  // namespace ohm



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/endian.hpp"



#endif   // ohm_archi_endian_HEADER_INCLUDED

#undef ohm_archi_endian_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
