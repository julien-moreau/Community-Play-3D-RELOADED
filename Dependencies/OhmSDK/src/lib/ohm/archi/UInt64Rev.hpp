/*****************************************************************************

        UInt64Rev.hpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 22953

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



#if ! defined (ohm_archi_UInt64Rev_CODEHEADER_INCLUDED)
#define  ohm_archi_UInt64Rev_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/fnc.h"

#include <cassert>



namespace ohm
{
namespace archi
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



UInt64Rev::UInt64Rev ()
:  _rev_val ()
{
   // Nothing
}



UInt64Rev::UInt64Rev (const UInt64Rev &other)
:  _rev_val (other._rev_val)
{
   assert (&other != 0);
}



UInt64Rev::UInt64Rev (UInt64 val)
:  _rev_val (revert_byte_order_s64 (val))
{
   // Nothing
}



UInt64Rev & UInt64Rev::operator = (const UInt64Rev &other)
{
   assert (&other != 0);

   _rev_val = other._rev_val;

   return (*this);
}



UInt64Rev::operator UInt64 () const
{
   return (revert_byte_order_s64 (_rev_val));
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace archi
}  // namespace ohm



#endif   // ohm_archi_UInt64Rev_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
