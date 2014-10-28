/*****************************************************************************

        Codec16.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 29241

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



#if ! defined (ohm_txt_utf16_Codec16_HEADER_INCLUDED)
#define  ohm_txt_utf16_Codec16_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/def.h"
#include "ohm/txt/unicode/Char.h"

#include <string>

#include <climits>



namespace ohm
{
namespace txt
{
namespace utf16
{



template <class C16>
class Codec16
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static bool    is_valid_utf16_lead_word (C16 w);
   static bool    is_valid_utf16_end_word (C16 w);
   static bool    is_char_single_word (C16 w);

   static int     get_char_seq_len_ucs (int &len, unicode::Char ucs4);
   static int     get_char_seq_len_utf (int &len, C16 utf16_lead_word);

   static int     encode_char (std::basic_string <C16> &utf16, unicode::Char ucs4);
   static int     encode_char (C16 utf16_0 [], unicode::Char ucs4);
   static int     encode_char (C16 utf16_ptr [], unicode::Char ucs4, int &len);

   static int     decode_char (unicode::Char &ucs4, const C16 utf16_ptr []);
   static int     decode_char (unicode::Char &ucs4, const C16 utf16_ptr [], int &len);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   ohm_lang_CHECK_CST (DataSize, (sizeof (C16) * CHAR_BIT >= 16));



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  Codec16 ();
                  Codec16 (const Codec16 &other);
   Codec16 &      operator = (const Codec16 &other);
   bool           operator == (const Codec16 &other);
   bool           operator != (const Codec16 &other);

}; // class Codec16



}  // namespace utf16
}  // namespace txt
}  // namespace ohm



#include "ohm/txt/utf16/Codec16.hpp"



#endif   // ohm_txt_utf16_Codec16_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
