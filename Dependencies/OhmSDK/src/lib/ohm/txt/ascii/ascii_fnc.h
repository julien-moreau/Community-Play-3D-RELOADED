/*****************************************************************************

        ascii_fnc.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 30385

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



#if ! defined (ohm_txt_ascii_ascii_fnc_HEADER_INCLUDED)
#define  ohm_txt_ascii_ascii_fnc_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/math/FloatMultiPrec.h"

#include <string>



namespace ohm
{
namespace txt
{
namespace ascii
{



inline char change_case (char c, bool lower_case_flag);
std::string to_lcase (const std::string &txt);
std::string to_ucase (const std::string &txt);
void  to_lcase_inplace (std::string &txt);
void  to_lcase_inplace (char *txt_0);
void  to_ucase_inplace (std::string &txt);
void  to_ucase_inplace (char *txt_0);

std::string trim_spaces (const char *txt_0);

long  strstr_ci (const char *s1_0, const char *s2_0);
bool  strcmp_ci (const char *s1_0, const char *s2_0);

bool  conv_str_to_int64 (archi::Int64 &val, const char *txt_0, int base = 10, long *stop_pos_ptr = 0);
bool  conv_str_to_double (double &val, const char *txt_0, int base = 10, long *stop_pos_ptr = 0);

template <int NW>
bool  conv_str_to_floatmultiprec (math::FloatMultiPrec <NW> &val, const char *txt_0, int base = 10, long *stop_pos_ptr = 0);



}  // namespace ascii
}  // namespace txt
}  // namespace ohm



#include "ohm/txt/ascii/ascii_fnc.hpp"



#endif   // ohm_txt_ascii_ascii_fnc_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
