/*****************************************************************************

        WordBreakPropTablePrev.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 21437

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



#if ! defined (ohm_txt_unicode_WordBreakPropTablePrev_HEADER_INCLUDED)
#define  ohm_txt_unicode_WordBreakPropTablePrev_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/txt/unicode/WordBreakProp.h"



namespace ohm
{
namespace txt
{
namespace unicode
{



enum WordBreakPropTablePrev
{

   WordBreakPropTablePrev_ALETTER_MIDLETTER = WordBreakProp_NBR_ELT,
   WordBreakPropTablePrev_ALETTER_MIDNUMLET,
   WordBreakPropTablePrev_NUMERIC_MIDNUM,
   WordBreakPropTablePrev_NUMERIC_MIDNUMLET,

   WordBreakPropTablePrev_NBR_ELT

}; // enum WordBreakPropTablePrev



}  // namespace unicode
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/unicode/WordBreakPropTablePrev.hpp"



#endif   // ohm_txt_unicode_WordBreakPropTablePrev_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
