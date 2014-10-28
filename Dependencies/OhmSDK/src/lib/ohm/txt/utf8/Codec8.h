/*****************************************************************************

        Codec8.h
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 20587

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



#if ! defined (ohm_txt_utf8_Codec8_HEADER_INCLUDED)
#define  ohm_txt_utf8_Codec8_HEADER_INCLUDED

#pragma once
#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/txt/unicode/Char.h"

#include <string>



namespace ohm
{
namespace txt
{
namespace utf8
{



class Codec8
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   static bool    is_valid_utf8_byte (char c);
   static bool    is_valid_utf8_lead_byte (char c);
   static bool    is_valid_utf8_follow_byte (char c);
   static bool    is_char_single_byte (char c);

   static int     get_char_seq_len_ucs (int &len, unicode::Char ucs4);
   static int     get_char_seq_len_utf (int &len, char utf8_lead_byte);

   static int     encode_char (std::string &utf8, unicode::Char ucs4);
   static int     encode_char (char utf8_0 [], unicode::Char ucs4);
   static int     encode_char (char utf8_ptr [], unicode::Char ucs4, int &len);

   static int     decode_char (unicode::Char &ucs4, const char utf8_ptr []);
   static int     decode_char (unicode::Char &ucs4, const char utf8_ptr [], int &len);

   static int     check_char_valid (const char utf8_ptr []);
   static int     check_string_valid (const char utf8_0 []);
   static int     check_string_valid (const std::string &utf8);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

                  Codec8 ();
                  Codec8 (const Codec8 &other);
   Codec8 &       operator = (const Codec8 &other);
   bool           operator == (const Codec8 &other);
   bool           operator != (const Codec8 &other);

}; // class Codec8



}  // namespace utf8
}  // namespace txt
}  // namespace ohm



//#include  "ohm/txt/utf8/Codec8.hpp"



#endif   // ohm_txt_utf8_Codec8_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
