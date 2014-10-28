/*****************************************************************************

        ConvNum.cpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 21246

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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/lang/fnc.h"
#include "ohm/txt/ascii/ConvDigit.h"
#include "ohm/txt/unicode/Categ.h"
#include "ohm/txt/unicode/Char.h"
#include "ohm/txt/unicode/CharData.h"
#include "ohm/txt/utf8/Codec8.h"
#include "ohm/txt/utf8/ConvNum.h"
#include "ohm/txt/Err.h"

#include <cassert>



namespace ohm
{
namespace txt
{
namespace utf8
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



archi::Int64   ConvNum::conv_str_to_int64 (const char *txt_0, int base)
{
   assert (txt_0 != 0);
   assert (base > 0);
   assert (base <= 'Z' - 'A' + 1 + 10);

   archi::Int64   val = 0;
   conv_str_to_int64 (val, txt_0, base);  // Ignores overflow or failure

   return (val);
}



//#* LDS: some duplicated code with ascii::conv_str_to_int64() to factorise
// Returns true if conversion is OK.
// false :
// - val == 0: failed to decode a number
// - val == archi::INT64_MAX_VAL: positive overflow
// - val == archi::INT64_MIN_VAL: negative overflow
// In case of success (even overflow), stop_pos_ptr indicates the stop
// position (after the last translated character).
bool  ConvNum::conv_str_to_int64 (archi::Int64 &val, const char *txt_0, int base, long *stop_pos_ptr)
{
   assert (&val != 0);
   assert (txt_0 != 0);
   assert (base > 0);
   assert (base <= 'Z' - 'A' + 1 + 10);

   val = 0;

   int            s = 1;
   long           pos = 0;

   // Skips whitespaces
   bool           skip_flag = true;
   do
   {
      unicode::Char  c;
      int            char_len;
      int            ret_val = Codec8::decode_char (c, txt_0 + pos, char_len);
      if (ret_val != Err_OK)
      {
         // Will produce a decoding failure later in the function
         skip_flag = false;
      }
      else if (c == 0)
      {
         skip_flag = false;
      }
      else
      {
         const unicode::Categ categ = unicode::CharData::get_categ (c);
         const unicode::CategClass  cat_c = unicode::Categ_conv_to_class (categ);
         if (   cat_c == unicode::CategClass_OTHER
             || cat_c == unicode::CategClass_SEPARATOR)
         {
            pos += char_len;
         }
         else
         {
            skip_flag = false;
         }
      }
   }
   while (skip_flag);

   // Sign
   if (txt_0 [pos] == '-')
   {
      s = -1;
      ++ pos;
   }
   else if (txt_0 [pos] == '+')
   {
      ++ pos;
   }

   // Digits
   bool           overflow_flag = false;
   int            nbr_decoded_digits = 0;
   while (ascii::ConvDigit::is_digit_from_base (txt_0 [pos], base))
   {
      const archi::Int64   old_val = val;

      const int      digit = ascii::ConvDigit::conv_char_to_digit (txt_0 [pos]);
      val *= base;
      val += digit * s;

      if ((val / base) != old_val)
      {
         overflow_flag = true;
      }

      ++ nbr_decoded_digits;
      ++ pos;
   }

   if (overflow_flag)
   {
      if (s > 0)
      {
         val = archi::INT64_MAX_VAL;
      }
      else
      {
         val = archi::INT64_MIN_VAL;
      }
   }

   if (stop_pos_ptr != 0)
   {
      *stop_pos_ptr = pos;
   }

   return ((! overflow_flag) && (nbr_decoded_digits > 0));
}



int   ConvNum::conv_int64_to_str (char *txt_0, archi::Int64 val, long max_len, int base)
{
   assert (txt_0 != 0);
   assert (max_len > 0);
   assert (base > 0);
   assert (base <= 'Z' - 'A' + 1 + 10);

   // Counts the number of required digits
   int            nbr_digits = 0;
   archi::Int64   x = val;
   do
   {
      x /= base;
      ++ nbr_digits;
   }
   while (x != 0);

   const int      sign_offset = (val < 0) ? 1 : 0;
   const int      nbr_char = nbr_digits + sign_offset;

   int            ret_val = 0;
   if (nbr_char > max_len)
   {
      ret_val = Err_NO_ROOM;
   }

   else
   {
      if (val < 0)
      {
         txt_0 [0] = '-';
      }

      for (int pos = 0; pos < nbr_digits; ++pos)
      {
         const archi::Int64   x = val / base;
         const int      d = lang::abs (static_cast <int> (val - x * base));
         txt_0 [nbr_char - 1 - pos] = ascii::ConvDigit::conv_digit_to_char (d);
         val = x;
      }

      txt_0 [nbr_char] = '\0';
   }

   return (ret_val);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace utf8
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
