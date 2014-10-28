/*****************************************************************************

        PredStringCiLt.hpp
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 29282

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



#if defined (ohm_txt_utf8_PredStringCiLt_CURRENT_CODEHEADER)
   #error Recursive inclusion of PredStringCiLt code header.
#endif
#define  ohm_txt_utf8_PredStringCiLt_CURRENT_CODEHEADER

#if ! defined (ohm_txt_utf8_PredStringCiLt_CODEHEADER_INCLUDED)
#define  ohm_txt_utf8_PredStringCiLt_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/txt/unicode/ConvCi.h"
#include "ohm/txt/utf8/Codec8.h"

#include <cassert>


namespace ohm
{
namespace txt
{
namespace utf8
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



bool  PredStringCiLt::operator () (const std::string &lhs, const std::string &rhs) const
{
   bool           inf_flag = false;    // Default

   const char *   lhs_0 = lhs.c_str ();
   const char *   rhs_0 = rhs.c_str ();
   const std::string::size_type  len_l = lhs.size ();
   const std::string::size_type  len_r = rhs.size ();
   std::string::size_type  pos_l = 0;
   std::string::size_type  pos_r = 0;

   bool           cont_flag = true;
   do
   {
      // '0' is ordered before any other character so we don't bother to
      // really test end of string.
      assert (pos_l <= len_l);
      assert (pos_r <= len_r);

      long           c_l;
      int            c_l_len;
      Codec8::decode_char (c_l, lhs_0 + pos_l, c_l_len);

      long           c_r;
      int            c_r_len;
      Codec8::decode_char (c_r, rhs_0 + pos_r, c_r_len);

      const long     c_l_u = unicode::ConvCi::convert (c_l);
      const long     c_r_u = unicode::ConvCi::convert (c_r);

      if (c_l_u != c_r_u || c_l_u == 0)
      {
         cont_flag = false;
         if (c_l_u < c_r_u)
         {
            inf_flag = true;
         }
      }

      pos_l += c_l_len;
      pos_r += c_r_len;
   }
   while (cont_flag);

   return (inf_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace utf8
}  // namespace txt
}  // namespace ohm



#endif   // ohm_txt_utf8_PredStringCiLt_CODEHEADER_INCLUDED

#undef ohm_txt_utf8_PredStringCiLt_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
