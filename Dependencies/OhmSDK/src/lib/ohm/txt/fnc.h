/*****************************************************************************

        fnc.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 62250

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



#if defined (ohm_txt_fnc_CURRENT_HEADER)
   #error Recursive inclusion of ohm/txt/fnc header.
#endif
#define  ohm_txt_fnc_CURRENT_HEADER

#if ! defined (ohm_txt_fnc_HEADER_INCLUDED)
#define  ohm_txt_fnc_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"

#include <string>



namespace ohm
{
namespace txt
{



std::string make_string_pattern (char c, long n);
std::string make_string_pattern (const char *str_0, long n);
std::string make_string_pattern (const std::string &str, long n);

template <class DT, class ST>
inline bool copy_string (DT *dest_0, const ST *src_0, long max_len);
template <class DT, class ST>
bool  strncpy_0 (DT *dest_0, const ST *src_0, long buf_len);
template <class DT, class ST>
bool  strncat_0 (DT *dest_0, const ST *src_0, long buf_len);

bool  cat_string (char *dest_0, const char *src_0, long &rem_len);

template <class CT>
size_t   get_strlen (const CT *text_0);

void  find_replace (std::string &txt, const std::string &find_txt, const std::string &repl_txt, std::string::size_type pos = 0, bool backward_flag = false, long max_nbr = 0);

template <class CO>
void  split_string (CO &token_list, const std::string &txt, const std::string &delimiter);

bool  is_substr_matching (const char *main_0, const char *substr_0);

template <class C16>
int   conv_utf8_to_utf16 (std::basic_string <C16> &utf16, const char utf8_0 []);

template <class C16>
int   conv_utf16_to_utf8 (std::string &utf8, const C16 utf16_0 []);



}  // namespace txt
}  // namespace ohm



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/txt/fnc.hpp"



#endif   // ohm_txt_fnc_HEADER_INCLUDED

#undef ohm_txt_fnc_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
