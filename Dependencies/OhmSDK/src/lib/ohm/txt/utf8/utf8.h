/*****************************************************************************

        utf8.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 67633

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



#if defined (ohm_txt_utf8_utf8_fnc_CURRENT_HEADER)
   #error Recursive inclusion of ohm/txt/utf8/utf8_fnc header.
#endif
#define  ohm_txt_utf8_utf8_fnc_CURRENT_HEADER

#if ! defined (ohm_txt_utf8_utf8_fnc_HEADER_INCLUDED)
#define  ohm_txt_utf8_utf8_fnc_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/txt/unicode/Char.h"

#include <string>



namespace ohm
{
namespace txt
{
namespace utf8
{



bool  strncpy_0 (char dest_0 [], const char src_0 [], std::string::size_type buf_len_byte);
bool  strncat_0 (char dest_0 [], const char src_0 [], std::string::size_type buf_len_byte);
bool  truncate_0 (char str_0 [], std::string::size_type buf_len_byte = std::string::npos);
bool  truncate (std::string & str, std::string::size_type buf_len_byte = std::string::npos);



#if defined (ohm_archi_SUPPORT_WCHAR_T)

int   encode_wstring (std::string &utf8, const std::wstring &ucs2);
int   encode_wstring (std::string &utf8, const wchar_t ucs2_0 []);

int   decode_wstring (std::wstring &ucs2, const std::string &utf8);
int   decode_wstring (std::wstring &ucs2, const char utf8_0 []);

#endif   // ohm_archi_SUPPORT_WCHAR_T



int   conv_to_lower_case (std::string &dest, const char src_0 [], const std::string::size_type max_sz = std::string::npos);
int   conv_to_upper_case (std::string &dest, const char src_0 [], const std::string::size_type max_sz = std::string::npos);

int   strstr_ci (const char str_0 [], const char *subset_0, long &subset_pos);



}  // namespace utf8
}  // namespace txt
}  // namespace ohm


/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



#endif   // ohm_txt_utf8_utf8_fnc_HEADER_INCLUDED

#undef ohm_txt_utf8_utf8_fnc_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
