/*****************************************************************************

        utf8.cpp
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

#include "ohm/archi/def.h"
#include "ohm/txt/Err.h"
#include "ohm/txt/out/PrintNa.h"
#include "ohm/txt/unicode/ConvCi.h"
#include "ohm/txt/unicode/PredEq.h"
#include "ohm/txt/utf8/Codec8.h"
#include "ohm/txt/utf8/def.h"
#include "ohm/txt/utf8/utf8.h"
#include "ohm/util/trace.h"

#include <cassert>
#include <cctype>

#if defined (ohm_archi_SUPPORT_WCHAR_T)

#include <wctype.h>

#endif   // ohm_archi_SUPPORT_WCHAR_T


namespace ohm
{
namespace txt
{
namespace utf8
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name: strncpy_0
Description:
   Copies a string, limiting its number of characters to fit in the given
   buffer size. The terminal '\0' always remains. Multi-byte characters
   are kept valid (never truncated).
Input parameters:
   - src_0: Source string, '\0'-terminated.
   - buf_len: Maximum number of characters, including the trailing '\0'.
Output parameters:
   - dest_0: Destination string.
Returns:
   true if string was truncated, or if an invalid UTF-8 sequence is
   encountered.
Throws: Nothing
==============================================================================
*/

bool  strncpy_0 (char dest_0 [], const char src_0 [], std::string::size_type buf_len_byte)
{
   assert (dest_0 != 0);
   assert (src_0 != 0);
   assert (buf_len_byte > 0);

   bool           trunc_flag = true;
   if (buf_len_byte > 0)
   {
      std::string::size_type  pos = 0;
      bool           cont_flag = true;
      do
      {
         const char     c = src_0 [pos];
         if (c == '\0')
         {
            cont_flag = false;
         }
         else
         {
            int            char_len;
            const int      ret_val = Codec8::get_char_seq_len_utf (char_len, c);
            if (ret_val != Err_OK)
            {
               cont_flag = false;
               trunc_flag = true;
            }

            else
            {
               const std::string::size_type  pos_end = pos + char_len;
               if (pos_end >= buf_len_byte)
               {
                  cont_flag = false;
                  trunc_flag = true;
               }
               else
               {
                  do
                  {
                     dest_0 [pos] = src_0 [pos];
                     ++ pos;
                  }
                  while (pos < pos_end);
               }
            }
         }
      }
      while (cont_flag);

      assert (pos < buf_len_byte);
      dest_0 [pos] = '\0';
   }

   return (trunc_flag);
}



bool  strncat_0 (char dest_0 [], const char src_0 [], std::string::size_type buf_len_byte)
{
   bool           trunc_flag = true;

   const size_t   len = strlen (dest_0);
   if (len < buf_len_byte)
   {
      trunc_flag = strncpy_0 (dest_0 + len, src_0, buf_len_byte - len);
   }

   return (trunc_flag);
}



// Safely truncates a UTF-8 string. The string is truncated at the first
// encountered ill-formed character sequence.
// Returns true if the string is modified.
bool  truncate_0 (char str_0 [], std::string::size_type buf_len_byte)
{
   bool           trunc_flag = false;

   std::string::size_type  valid_end_pos = 0;
   std::string::size_type  pos = 0;
   while (pos < buf_len_byte && ! trunc_flag && str_0 [pos] != '\0')
   {
      unicode::Char  ucs4;
      int            char_len;
      const int      ret_val =
         Codec8::decode_char (ucs4, &str_0 [pos], char_len);
      if (ret_val != Err_OK)
      {
         trunc_flag = true;
      }
      else
      {
         pos += char_len;
         if (pos >= buf_len_byte)
         {
            trunc_flag = true;
         }

         else
         {
            valid_end_pos = pos;
         }
      }
   }

   if (trunc_flag)
   {
      assert (valid_end_pos < buf_len_byte);
      str_0 [valid_end_pos] = '\0';
   }

   return (trunc_flag);
}



/*
==============================================================================
Name: truncate
==============================================================================
*/

bool truncate (std::string & str, std::string::size_type buf_len_byte)
{
   const char * str_0 = str.c_str ();
   bool trunc_flag = false;
   std::string::size_type  valid_end_pos = 0;
   std::string::size_type  pos = 0;

   while (pos < buf_len_byte && ! trunc_flag && str_0 [pos] != '\0')
   {
      unicode::Char ucs4;
      int char_len;
      const int ret_val = Codec8::decode_char (ucs4, &str_0 [pos], char_len);
         
      if (ret_val != Err_OK)
      {
         trunc_flag = true;
      }
      else
      {
         pos += char_len;
         
         if (pos >= buf_len_byte)
         {
            trunc_flag = true;
         }
         else
         {
            valid_end_pos = pos;
         }
      }
   }

   if (trunc_flag)
   {
      assert (valid_end_pos < buf_len_byte);
      str.erase (valid_end_pos, std::string::npos);
   }

   return (trunc_flag);
}



int   conv_to_lower_case (std::string &dest, const char src_0 [], const std::string::size_type max_sz)
{
   assert (&dest != 0);
   assert (src_0 != 0);
   assert (max_sz >= 0);

   int            ret_val = Err_OK;

   dest.erase (dest.begin (), dest.end ());

   std::string::size_type  pos = 0;
   while (src_0 [pos] != '\0' && pos < max_sz && ret_val == Err_OK)
   {
      int            char_len;
      unicode::Char  c;
      ret_val = Codec8::decode_char (c, &src_0 [pos], char_len);
      if (ret_val == Err_OK)
      {
         const unicode::Char  c_lc = unicode::CharData::to_lower (c);
         char           buf_0 [MAX_BYTE_SEQ_LEN+1];
         ret_val = Codec8::encode_char (buf_0, c_lc);
         if (ret_val == Err_OK)
         {
            dest += buf_0;
         }
         else
         {
            ohm_util_TRACE_ERROR2 (
               "conv_to_lower_case(): Cannot encode character at "
               "source index %1% to UTF-8 (val: 0x%2X%).",
               pos,
               c_lc
            );
         }

         pos += char_len;
      }
      else
      {
         ohm_util_TRACE_ERROR1 (
            "conv_to_lower_case(): Cannot decode UTF-8 character at "
            "source index %1%.",
            pos
         );
      }
   }

   return (ret_val);
}



int   conv_to_upper_case (std::string &dest, const char src_0 [], const std::string::size_type max_sz)
{
   assert (&dest != 0);
   assert (src_0 != 0);
   assert (max_sz >= 0);

   int            ret_val = Err_OK;

   dest.erase (dest.begin (), dest.end ());

   std::string::size_type  pos = 0;
   while (src_0 [pos] != '\0' && pos < max_sz && ret_val == Err_OK)
   {
      int            char_len;
      unicode::Char  c;
      ret_val = Codec8::decode_char (c, &src_0 [pos], char_len);
      if (ret_val == Err_OK)
      {
         const unicode::Char  c_lc = unicode::CharData::to_upper (c);
         char           buf_0 [MAX_BYTE_SEQ_LEN+1];
         ret_val = Codec8::encode_char (buf_0, c_lc);
         if (ret_val == Err_OK)
         {
            dest += buf_0;
         }
         else
         {
            ohm_util_TRACE_ERROR2 (
               "conv_to_upper_case(): Cannot encode character at "
               "source index %1% to UTF-8 (val: 0x%2X%).",
               pos,
               c_lc
            );
         }

         pos += char_len;
      }
      else
      {
         ohm_util_TRACE_ERROR1 (
            "conv_to_upper_case(): Cannot decode UTF-8 character at "
            "source index %1%.",
            pos
         );
      }
   }

   return (ret_val);
}



// Position is set to -1 if not found or error.
// An empty string is always found at position 0.
int   strstr_ci (const char str_0 [], const char subset_0 [], long &subset_pos)
{
   assert (str_0 != 0);
   assert (subset_0 != 0);
   assert (&subset_pos != 0);

   if (subset_0 [0] != '\0')
   {
      unicode::PredEq <unicode::ConvCi>   pred;

      subset_pos = 0;
      while (str_0 [subset_pos] != '\0')
      {
         long           pos_str = subset_pos;
         long           pos_sub = 0;

         do
         {
            unicode::Char  c_str;
            int            c_str_len;
            int            ret_val =
               Codec8::decode_char (c_str, &str_0 [pos_str], c_str_len);
            if (ret_val != Err_OK)
            {
               subset_pos = -1;
               return (ret_val);
            }

            unicode::Char  c_sub;
            int            c_sub_len;
            ret_val = Codec8::decode_char (c_sub, &subset_0 [pos_sub], c_sub_len);
            if (ret_val != Err_OK)
            {
               subset_pos = -1;
               return (ret_val);
            }

            if (! pred (c_sub, c_str))
            {
               break;
            }
            
            pos_str += c_str_len;
            pos_sub += c_sub_len;
         }
         while (subset_0 [pos_sub] != '\0' && str_0 [pos_str] != '\0');

         if (subset_0 [pos_sub] == '\0')
         {
            return (Err_OK);
         }

         else if (str_0 [pos_str] == '\0')
         {
            subset_pos = -1;
            return (Err_NOT_FOUND);
         }

         int            step;
         Codec8::get_char_seq_len_utf (step, str_0 [subset_pos]);
         subset_pos += step;
      }

      subset_pos = -1;
      return (Err_NOT_FOUND);
   }

   return (Err_OK);
}



#if defined (ohm_archi_SUPPORT_WCHAR_T)



/*
==============================================================================
Name : encode_wstring
Description : Encode a 16-bit character string into a UTF-8 string.
Input parameters :
   - ucs2 : String to convert
Output parameters :
   - utf8 : Converted string
Returns:
   - true if the whole encoded string is valid
   - false if at least one character is invalid. It should never happen as
      every UCS-2 value can be encoded in UTF-8.
Throws : std::string related exception
==============================================================================
*/

int   encode_wstring (std::string &utf8, const std::wstring &ucs2)
{
   assert (&utf8 != 0);
   assert (&ucs2 != 0);

   return (encode_wstring (utf8, ucs2.c_str ()));
}



int   encode_wstring (std::string &utf8, const wchar_t ucs2_0 [])
{
   assert (&utf8 != 0);
   assert (ucs2_0 != 0);

   int            ret_val = Err_OK;
   utf8.erase ();

   long           pos = 0;
   while (ret_val == Err_OK && ucs2_0 [pos] != '\0')
   {
      char        utf8_char_0 [6+1];
      ret_val = Codec8::encode_char (utf8_char_0, ucs2_0 [pos]);
      assert (ret_val == Err_OK);
      if (ret_val == Err_OK)
      {
         utf8 += utf8_char_0;
         ++pos;
      }
   }

   return (ret_val);
}



int   decode_wstring (std::wstring &ucs4, const std::string &utf8)
{
   assert (&ucs4 != 0);
   assert (&utf8 != 0);

   return (decode_wstring (ucs4, utf8.c_str ()));
}



int   decode_wstring (std::wstring &ucs4, const char utf8_0 [])
{
   assert (&ucs4 != 0);
   assert (utf8_0 != 0);

   ucs4.erase ();

   int            ret_val = Err_OK;
   long           pos = 0;

   while (ret_val == Err_OK && utf8_0 [pos] != '\0')
   {
      unicode::Char  ucs4_char;
      int            len;

      ret_val = Codec8::decode_char (ucs4_char, &utf8_0 [pos], len);
      if (ret_val == Err_OK)
      {
         pos += len;
         ucs4 += static_cast <wchar_t> (ucs4_char);
      }
   }

   return (ret_val);
}



#endif   // ohm_archi_SUPPORT_WCHAR_T



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace utf8
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
