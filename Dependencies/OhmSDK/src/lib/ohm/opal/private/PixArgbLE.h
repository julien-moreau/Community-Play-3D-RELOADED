/*****************************************************************************

        PixArgbLE.h
        Copyright (c) 2004 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 70448

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



#if defined (ohm_opal_PixArgbLE_CURRENT_HEADER)
   #error Recursive inclusion of ohm/PixArgbLE header.
#endif
#define  ohm_opal_PixArgbLE_CURRENT_HEADER

#if ! defined (ohm_opal_PixArgbLE_HEADER_INCLUDED)
#define  ohm_opal_PixArgbLE_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/archi/types.h"



namespace ohm
{
namespace opal
{



class PixArgbBE;

class PixArgbLE
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:
   
   inline         PixArgbLE ();
   inline         PixArgbLE (int a, int r, int g, int b);
   inline         PixArgbLE (const PixArgbLE &other);
   inline         PixArgbLE (const PixArgbBE &other);

   inline PixArgbLE &
                  operator = (const PixArgbLE &other);
   PixArgbLE &    operator = (const PixArgbBE &other);

   inline bool    operator == (const PixArgbLE &other);
   inline bool    operator != (const PixArgbLE &other);

   
#if defined (_MSC_VER) || defined (__MWERKS__) || defined (__GNUC__)
#pragma pack (push, 1)

   archi::UByte _b;
   archi::UByte _g;
   archi::UByte _r;
   archi::UByte _a;
   
#pragma pack (pop)

#else
   #error Unsupported compiler.
#endif


/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef  archi::UInt32  WholeStruct;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



}; // class PixArgbLE



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/opal/private/PixArgbLE.hpp"


#endif   // ohm_opal_PixArgbLE_HEADER_INCLUDED

#undef ohm_opal_PixArgbLE_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
