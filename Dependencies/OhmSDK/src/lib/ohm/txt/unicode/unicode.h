/*****************************************************************************

        unicode.h
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 33887

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



#if defined (ohm_txt_unicode_unicode_fnc_CURRENT_HEADER)
   #error Recursive inclusion of ohm/txt/unicode/unicode_fnc header.
#endif
#define  ohm_txt_unicode_unicode_fnc_CURRENT_HEADER

#if ! defined (ohm_txt_unicode_unicode_fnc_HEADER_INCLUDED)
#define  ohm_txt_unicode_unicode_fnc_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/txt/unicode/StringU.h"

#include <string>



namespace ohm
{
namespace txt
{
namespace unicode
{



int   conv_utf8_to_unicode (StringU & dest, const char src_0 []);
int   conv_unicode_to_utf8 (std::string & dest, const Char src_0 []);



}  // namespace unicode
}  // namespace txt
}  // namespace ohm


/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_txt_unicode_unicode_fnc_HEADER_INCLUDED

#undef ohm_txt_unicode_unicode_fnc_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
