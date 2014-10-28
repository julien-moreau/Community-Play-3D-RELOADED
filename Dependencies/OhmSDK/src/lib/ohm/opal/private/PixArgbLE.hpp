/*****************************************************************************

        PixArgbLE.hpp
        Copyright (c) 2004 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 20247

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



#if defined (ohm_opal_PixArgbLE_CURRENT_CODEHEADER)
   #error Recursive inclusion of PixArgbLE code header.
#endif
#define  ohm_opal_PixArgbLE_CURRENT_CODEHEADER

#if ! defined (ohm_opal_PixArgbLE_CODEHEADER_INCLUDED)
#define  ohm_opal_PixArgbLE_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/fnc.h"

#include <cassert>



namespace ohm
{
namespace opal
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



PixArgbLE::PixArgbLE ()
{
   *((archi::UInt32 *)&_b) = 0;
}



PixArgbLE::PixArgbLE (int a, int r, int g, int b)
:  _a (a)
,  _r (r)
,  _g (g)
,  _b (b)
{
   // Nothing
}



PixArgbLE::PixArgbLE (const PixArgbLE &other)
{
   assert (&other != 0);

   // copy all in one instruction
   *((WholeStruct *) this) = *((WholeStruct *) &other);
}



PixArgbLE & PixArgbLE::operator = (const PixArgbLE &other)
{
   assert (&other != 0);

   *((WholeStruct *) this) = *((WholeStruct *) &other);

   return *this;
}



bool  PixArgbLE::operator == (const PixArgbLE &other)
{
   assert (&other != 0);

   return (*((WholeStruct *) this) == *((WholeStruct *) &other));
}



bool  PixArgbLE::operator != (const PixArgbLE &other)
{
   assert (&other != 0);

   return (*((WholeStruct *) this) != *((WholeStruct *) &other));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ GLOBAL OPERATOR DEFINITIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

}  // namespace opal
}  // namespace ohm



#endif   // ohm_opal_PixArgbLE_CODEHEADER_INCLUDED

#undef ohm_opal_PixArgbLE_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
