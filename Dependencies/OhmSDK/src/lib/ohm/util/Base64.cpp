/*****************************************************************************

        Base64.cpp
        Copyright (c) 2005 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130) // "'operator' : logical operation on address of string constant"
   #pragma warning (1 : 4223) // "nonstandard extension used : non-lvalue array converted to pointer"
   #pragma warning (1 : 4705) // "statement has no effect"
   #pragma warning (1 : 4706) // "assignment within conditional expression"
   #pragma warning (4 : 4786) // "identifier was truncated to '255' characters in the debug information"
   #pragma warning (4 : 4800) // "forcing value to bool 'true' or 'false' (performance warning)"
   #pragma warning (4 : 4355) // "'this' : used in base member initializer list"
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/util/Base64.h"

#include <stdexcept>

#include <cassert>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

/*
==============================================================================
Name : dtor
Description :
==============================================================================
*/

Base64::~Base64 ()
{
}



/*
==============================================================================
Name : encode
Description :
   Encode from data to base64 encoded data
==============================================================================
*/

void  Base64::encode (std::string &dst_base64, const ohm::archi::UByte *src_ptr, size_t src_size, size_t line_wrap, const char * line_end_0)
{
   assert (line_wrap >= 0);
   if (line_wrap > 0)
   {
      assert (line_end_0 != 0);
   }
   
   // RFC 1521 :
   // The encoding process represents 24-bit groups of input bits
   
   size_t         aligned_24 = src_size / 3;
   size_t         rem_24     = src_size % 3;
   
   size_t         line_char_cnt = 0;
   
   for (size_t i = 0; i < aligned_24; ++i)
   {
      int            j;
      
      // RFC 1521 :
      // Proceeding from left to right,
      // a 24-bit input group is formed by concatenating 3 8-bit input groups
      
      ohm::archi::UInt32   byte3 = 0;
      for (j = 0 ; j < 3 ; ++j)
      {
         byte3 |= src_ptr [3 * i + j] << ((2 - j) * 8);
      }
      
      // RFC 1521 :
      // These 24 bits are then treated as 4 concatenated 6-bit groups
      // each of which is translated into a single digit in the base 64
      // alphabet.
      
      for (j = 0 ; j < 4 ; ++j)
      {
         const int      digit = (byte3 >> (18 - 6 * j)) & 63;
         
         const char     c = encode_digit (digit);
         
         dst_base64 += c;
         
         bool           last_char_flag =
               (rem_24 == 0)
            && (i == aligned_24 - 1)
            && (j == 3);
         
         if (! last_char_flag)
         {
            insert_cr_if_needed (
               dst_base64,
               line_char_cnt,
               line_wrap,
               line_end_0
            );
         }
      }
   }
   
   if (rem_24 == 0)
   {
      // RFC 1521 :
      // the final quantum of encoding input is an integral multiple of 24 bits;
      // here, the final unit of encoded output will be an integral multiple of
      // 4 characters with no "=" padding
   }
   else if (rem_24 == 1)
   {
      // RFC 1521 :
      // the final quantum of encoding input is exactly 8 bits; here, the final
      // unit of encoded output will be two characters followed by two "="
      // padding characters
      
      // When fewer than 24 input bits are available in an input group, zero bits
      // are added (on the right) to form an integral number of 6-bit groups
      
      const archi::UByte   byte = src_ptr [src_size - 1];   // last byte
      
      const int      digit1 = byte >> 2;
      const int      digit2 = (byte & 3) << 4;  // padded with 0 on the right
      
      const char     c1 = encode_digit (digit1);
      const char     c2 = encode_digit (digit2);
      
      dst_base64 += c1;
      insert_cr_if_needed (dst_base64, line_char_cnt, line_wrap, line_end_0);
      
      dst_base64 += c2;
      insert_cr_if_needed (dst_base64, line_char_cnt, line_wrap, line_end_0);
      
      dst_base64 += '=';
      insert_cr_if_needed (dst_base64, line_char_cnt, line_wrap, line_end_0);
      
      dst_base64 += '=';
   }
   else if (rem_24 == 2)
   {
      // RFC 1521 :
      // the final quantum of encoding input is exactly 16 bits; here, the final
      // unit of encoded output will be three characters followed by one "="
      // padding character
      
      // When fewer than 24 input bits are available in an input group, zero bits
      // are added (on the right) to form an integral number of 6-bit groups
      
      const ohm::archi::UByte byte1 = src_ptr [src_size - 2];
      const ohm::archi::UByte byte2 = src_ptr [src_size - 1];
      
      const int      digit1 = byte1 >> 2;
      const int      digit2 = ((byte1 & 3) << 4) | (byte2 >> 4);
      const int      digit3 = (byte2 & 15) << 2;   // padded with 0 on the right
      
      const char     c1 = encode_digit (digit1);
      const char     c2 = encode_digit (digit2);
      const char     c3 = encode_digit (digit3);
      
      dst_base64 += c1;
      insert_cr_if_needed (dst_base64, line_char_cnt, line_wrap, line_end_0);
      
      dst_base64 += c2;
      insert_cr_if_needed (dst_base64, line_char_cnt, line_wrap, line_end_0);
      
      dst_base64 += c3;
      insert_cr_if_needed (dst_base64, line_char_cnt, line_wrap, line_end_0);
      
      dst_base64 += '=';
   }
   else
   {
      assert (false);
   }
}



/*
==============================================================================
Name : encode
Description :
   Encode from data to base64 encoded data
==============================================================================
*/

void  Base64::encode (std::string &dst_base64, const std::vector <ohm::archi::UByte> &src, size_t line_wrap, const char * line_end_0)
{
   encode (dst_base64, &src [0], src.size (), line_wrap, line_end_0);
}



/*
==============================================================================
Name : decode
Description :
   Encode from base54 encoded data to original data.
   The stream may contain white characters.
==============================================================================
*/

void  Base64::decode (std::vector <ohm::archi::UByte> &dst, const std::string &src_base64)
{
   bool loop_flag = true;
   
   size_t         i = 0;
   
   while (loop_flag)
   {
      int            available_char = 0;
      char           src_c [4];
      
      while (loop_flag && (available_char < 4))
      {
         if (i < src_base64.size ())
         {
            const char     c = src_base64 [i];
            bool           white_flag =
                  (c == ' ')
               || (c == '\t')
               || (c == '\n')
               || (c == '\r')
               || (c == '=');
            
            if (! white_flag)
            {
               src_c [available_char] = c;
               ++ available_char;
            }
            
            ++ i;
         }
         else
         {
            loop_flag = false;
         }
      }
      
      if (available_char == 4)
      {
         archi::UInt32  byte3 = 0;
         int            j;
         for (j = 0 ; j < 4 ; ++j)
         {
            const char c = src_c [j];
            const int digit_6 = decode_char (c);

            byte3 |= (digit_6 << (18 - (6 * j)));
         }
         
         for (j = 0 ; j < 3 ; ++j)
         {
            ohm::archi::UByte byte = (byte3 >> (16 - (j * 8))) & 255;
            dst.push_back (byte);
         }
      }
      else if (available_char == 0)
      {
         // ok, valid
      }
      else if (available_char == 1)
      {
         assert (false);
      }
      else if (available_char == 2)
      {
         int            digit1 = decode_char (src_c [0]);
         int            digit2 = decode_char (src_c [1]);
         
         if ((digit2 & 15) != 0)
         {
            assert (false);
            throw std::runtime_error (
               "ohm::util::Base64::decode(): "
               "Source is not padded correctly: 0 bits pad on last digit."
            );
         }
         
         ohm::archi::UByte byte = (digit1 << 2) + (digit2 >> 4);
         dst.push_back (byte);
      }
      else if (available_char == 3)
      {
         int            digit1 = decode_char (src_c [0]);
         int            digit2 = decode_char (src_c [1]);
         int            digit3 = decode_char (src_c [2]);
         
         if ((digit3 & 3) != 0)
         {
            assert (false);
            throw std::runtime_error (
               "ohm::util::Base64::decode(): "
               "Source is not padded correctly: 0 bits pad on last digit."
            );
         }
         
         ohm::archi::UByte byte1 = (digit1 << 2) | (digit2 >> 4);
         ohm::archi::UByte byte2 = ((digit2 & 15) << 4) | (digit3 >> 2);
         
         dst.push_back (byte1);
         dst.push_back (byte2);
      }
      else
      {
         assert (false);
      }
   }
}



/*
==============================================================================
Name : encode_digit
Description :
   Encode one 6-bit digit to a base64 character alphabet
==============================================================================
*/

char  Base64::encode_digit (int digit_6)
{
   /*
   Value Encoding  Value Encoding  Value Encoding  Value Encoding
       0 A            17 R            34 i            51 z
       1 B            18 S            35 j            52 0
       2 C            19 T            36 k            53 1
       3 D            20 U            37 l            54 2
       4 E            21 V            38 m            55 3
       5 F            22 W            39 n            56 4
       6 G            23 X            40 o            57 5
       7 H            24 Y            41 p            58 6
       8 I            25 Z            42 q            59 7
       9 J            26 a            43 r            60 8
      10 K            27 b            44 s            61 9
      11 L            28 c            45 t            62 +
      12 M            29 d            46 u            63 /
      13 N            30 e            47 v
      14 O            31 f            48 w         (pad) =
      15 P            32 g            49 x
      16 Q            33 h            50 y
   */
   
   assert (digit_6 >= 0);
   assert (digit_6 < 64);
   
   char           ret_val;
   
   if (digit_6 <= 25)
   {
      ret_val = 'A' + (digit_6);
   }
   else if (digit_6 <= 51)
   {
      ret_val = 'a' + (digit_6 - 26);
   }
   else if (digit_6 <= 61)
   {
      ret_val = '0' + (digit_6 - 52);
   }
   else if (digit_6 == 62)
   {
      ret_val = '+';
   }
   else if (digit_6 == 63)
   {
      ret_val = '/';
   }
   else
   {
      assert (false);
   }
   
   return (ret_val);
}



/*
==============================================================================
Name : decode_char
Description :
   decode on a base64 character alphabet to one 6-bit digit
==============================================================================
*/

int   Base64::decode_char (char c)
{
   /*
   Value Encoding  Value Encoding  Value Encoding  Value Encoding
       0 A            17 R            34 i            51 z
       1 B            18 S            35 j            52 0
       2 C            19 T            36 k            53 1
       3 D            20 U            37 l            54 2
       4 E            21 V            38 m            55 3
       5 F            22 W            39 n            56 4
       6 G            23 X            40 o            57 5
       7 H            24 Y            41 p            58 6
       8 I            25 Z            42 q            59 7
       9 J            26 a            43 r            60 8
      10 K            27 b            44 s            61 9
      11 L            28 c            45 t            62 +
      12 M            29 d            46 u            63 /
      13 N            30 e            47 v
      14 O            31 f            48 w         (pad) =
      15 P            32 g            49 x
      16 Q            33 h            50 y
   */
   
   bool           char_ok_flag =
         ((c >= 'A') && (c <= 'Z'))
      || ((c >= 'a') && (c <= 'z'))
      || ((c >= '0') && (c <= '9'))
      || (c == '+')
      || (c == '/')
   ;
   
   if (! char_ok_flag)
   {
      throw std::runtime_error (
         "ohm::util::Base64::decode_char(): "
         "Invalid character in base64 source."
      );
   }
   
   int            ret_val = -1;
   
   if ((c >= 'A') && (c <= 'Z'))
   {
      ret_val = c - 'A' + 0;
   }
   else if ((c >= 'a') && (c <= 'z'))
   {
      ret_val = c - 'a' + 26;
   }
   else if ((c >= '0') && (c <= '9'))
   {
      ret_val = c - '0' + 52;
   }
   else if (c == '+')
   {
      ret_val = 62;
   }
   else if (c == '/')
   {
      ret_val = 63;
   }
   else
   {
      assert (false);
   }
   
   assert (ret_val >= 0);
   assert (ret_val < 64);
   
   return (ret_val);
}





/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name : ctor
Description :
==============================================================================
*/

Base64::Base64 ()
{
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void  Base64::insert_cr_if_needed (std::string &dst_base64, size_t &line_char_cnt, size_t line_wrap, const char *line_end_0)
{
   ++ line_char_cnt;
         
   if (line_wrap != 0)
   {
      if (line_char_cnt >= line_wrap)
      {
         dst_base64 += std::string (line_end_0);
         line_char_cnt = 0;
      }
   }
}



}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
