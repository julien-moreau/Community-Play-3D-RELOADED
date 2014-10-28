/*****************************************************************************

        fnc.h
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 61457

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



#if defined (ohm_lang_fnc_CURRENT_HEADER)
   #error Recursive inclusion of ohm/lang/fnc header.
#endif
#define  ohm_lang_fnc_CURRENT_HEADER

#if ! defined (ohm_lang_fnc_HEADER_INCLUDED)
#define  ohm_lang_fnc_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#if defined (min) || defined (max)
#error min or max is defined as a macro!!
#endif



namespace ohm
{
namespace lang
{

class CodeTxt;



template <class T>
inline T       max (T a, T b);
template <class T>
inline void    max_in_place (T &a, T b);

template <class T>
inline T       min (T a, T b);
template <class T>
inline void    min_in_place (T &a, T b);

template <class T>
inline T       limit (T a, T b, T c);
template <class T>
inline void    limit_in_place (T &a, T b, T c);

template <class T>
inline void    sort_2_elt (T &mi, T &ma, T a, T b);

template <class T>
inline T       abs (T x);

template <class T>
inline int     sgn (T x);

template <class T>
inline int     cmp (const T & a, const T & b);

const char *   get_code_txt (long code, const CodeTxt * const code_meta_list []);
const char *   get_code_txt (long code, const CodeTxt code_list []);

const char *   get_code_txt_nn (long code, const CodeTxt * const code_meta_list []);
const char *   get_code_txt_nn (long code, const CodeTxt code_list []);



}  // namespace lang
}  // namespace ohm



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ CODE HEADER INCLUSION \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.hpp"



#endif   // ohm_lang_fnc_HEADER_INCLUDED

#undef ohm_lang_fnc_CURRENT_HEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
