/*****************************************************************************

        fnc.hpp
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



#if ! defined (ohm_txt_fnc_CODEHEADER_INCLUDED)
#define  ohm_txt_fnc_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/def.h"
#include "ohm/txt/utf16/Codec16.h"
#include "ohm/txt/utf16/def.h"
#include "ohm/txt/utf8/Codec8.h"
#include "ohm/txt/utf8/def.h"
#include "ohm/txt/ConvChar.h"

#include <cassert>



namespace ohm
{
namespace txt
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <class DT, class ST>
bool  copy_string (DT *dest_0, const ST *src_0, long max_len)
{
   return (strncpy_0 (dest_0, src_0, max_len + 1));
}



/*
==============================================================================
Name: strncpy_0
Description:
   Copies a string, limiting its number of characters to fit in the given
   buffer size. The terminal '\0' always remains.
   Doesn't generate ant logs.
Input parameters:
   - src_0: Source string, '\0'-terminated.
   - buf_len: Maximum number of characters, including the trailing '\0'.
Output parameters:
   - dest_0: Destination string.
Returns: true if string was truncated
Throws: Nothing
==============================================================================
*/

template <class DT, class ST>
bool  strncpy_0 (DT *dest_0, const ST *src_0, long buf_len)
{
   assert (dest_0 != 0);
   assert (src_0 != 0);
   assert (buf_len >= 0);

   bool           trunc_flag = true;
   if (buf_len > 0)
   {
      bool           finished_flag = false;
      for (long pos = 0; pos < buf_len && ! finished_flag; ++pos)
      {
         const ST    c = src_0 [pos];
         dest_0 [pos] = ConvChar <ST, DT>::convert (c);
         finished_flag = (c == static_cast <ST> ('\0'));
      }
      trunc_flag = ! finished_flag;
      dest_0 [buf_len - 1] = static_cast <DT> ('\0');
   }

   return (trunc_flag);
}



/*
==============================================================================
Name: strncat_0
Description:
   Appends a string to another one, limiting the total number of characters
   to fit in the given buffer size. The terminal '\0' always remains.
   Doesn't generate any logs.
Input parameters:
   - src_0: Source string, '\0'-terminated.
   - buf_len: Maximum number of characters, including the trailing '\0'.
Input/output parameters:
   - dest_0: Destination string, where the the source string is appended.
      The source string should fit in the buffer with its trailing '\0', but
      if it's not the case, the string is fixed.
Returns: true if the resulting string was truncated (also happens if the input
   string had to be fixed).
Throws: Nothing
==============================================================================
*/

template <class DT, class ST>
bool  strncat_0 (DT *dest_0, const ST *src_0, long buf_len)
{
   assert (dest_0 != 0);
   assert (src_0 != 0);
   assert (buf_len >= 0);

   long           pos = 0;
   while (pos < buf_len && dest_0 [pos] != static_cast <ST> ('\0'))
   {
      ++ pos;
   }
   assert (pos < buf_len);

   const bool     trunc_flag = strncpy_0 (dest_0 + pos, src_0, buf_len - pos);
   if (buf_len > 0)
   {
      dest_0 [buf_len - 1] = static_cast <DT> ('\0');
   }

   return (trunc_flag);
}



/*
==============================================================================
Name: get_strlen
Description:
   Counts the number of characters of a string.
   Doesn't generate any logs.
Input parameters:
   - text_0: Pointer on a null-terminated string.
Returns: Number of characters in the string.
Throws: Nothing.
==============================================================================
*/

template <class CT>
size_t   get_strlen (const CT *text_0)
{
   assert (text_0 != 0);

   size_t         len = 0;
   while (text_0 [len] != static_cast <CT> ('\0'))
   {
      ++ len;
   }

   return (len);
}



/*
==============================================================================
Name: split_string
Description:
   Splits a string given a delimiter. Case sensitive.
Input parameters:
   - txt: String to split.
   - delimiter: 
Output parameters:
   - token_list: List of elements of the string. The tokens don't have any
      delimiter attached. Class of token_list must have the standard begin(),
      end() and insert() functions.
Throws: CO and std::string related exceptions
==============================================================================
*/

template <class CO>
void  split_string (CO &token_list, const std::string &txt, const std::string &delimiter)
{
   assert (&token_list != 0);
   assert (&txt != 0);
   assert (&delimiter != 0);
   assert (! delimiter.empty ());

   token_list.erase (token_list.begin (), token_list.end ());

   std::string    token;
   std::string::size_type  pos = 0;
   do
   {
      const std::string::size_type  prev_pos = pos;

      pos = txt.find (delimiter, pos);
      if (pos == std::string::npos)
      {
         token = txt.substr (prev_pos);
      }
      else
      {
         token = txt.substr (prev_pos, pos - prev_pos);
         pos += delimiter.length ();
      }

      token_list.insert (token_list.end (), token);
   }
   while (pos != std::string::npos);
}



/*
==============================================================================
Name: conv_utf8_to_utf16
Description:
   Converts a string from UTF-8 to UTF-16.
Input parameters:
   - utf8_0: String in UTF-8
Output parameters:
   - utf16: String in UTF-16
Returns: Error code
Throws: Nothing
==============================================================================
*/

template <class C16>
int   conv_utf8_to_utf16 (std::basic_string <C16> &utf16, const char utf8_0 [])
{
   assert (&utf16 != 0);
   assert (utf8_0 != 0);

   int            ret_val = Err_OK;

   try
   {
      utf16.erase (utf16.begin (), utf16.end ());

      long           pos = 0;
      while (utf8_0 [pos] != '\0' && ret_val == Err_OK)
      {
         unicode::Char  c;
         int            char_len;
         ret_val = utf8::Codec8::decode_char (c, &utf8_0 [pos], char_len);
         if (ret_val == Err_OK)
         {
            C16            tmp_0 [utf16::MAX_WORD_SEQ_LEN + 1];
            ret_val = utf16::Codec16 <C16>::encode_char (tmp_0, c);
            assert (ret_val == Err_OK);
            utf16 += tmp_0;
            pos += char_len;
         }
      }
   }

   catch (...)
   {
      ohm_util_TRACE_ERROR ("conv_utf8_to_utf16(): Exception");
      ret_val = Err_EXCEPTION;
   }

   return (ret_val);
}



/*
==============================================================================
Name: conv_utf16_to_utf8
Description:
   Converts a string from UTF-16 to UTF-8.
Input parameters:
   - utf16_0: String in UTF-16
Output parameters:
   - utf8: String in UTF-8
Returns: Error code
Throws: Nothing
==============================================================================
*/

template <class C16>
int   conv_utf16_to_utf8 (std::string &utf8, const C16 utf16_0 [])
{
   assert (&utf8 != 0);
   assert (utf16_0 != 0);

   int            ret_val = Err_OK;

   try
   {
      utf8.erase (utf8.begin (), utf8.end ());

      long           pos = 0;
      while (utf16_0 [pos] != 0 && ret_val == Err_OK)
      {
         unicode::Char  c;
         int            char_len;
         ret_val = utf16::Codec16 <C16>::decode_char (c, &utf16_0 [pos], char_len);
         if (ret_val == Err_OK)
         {
            char        tmp_0 [utf8::MAX_BYTE_SEQ_LEN + 1];
            ret_val = utf8::Codec8::encode_char (tmp_0, c);
            assert (ret_val == Err_OK);
            utf8 += tmp_0;
            pos += char_len;
         }
      }
   }

   catch (...)
   {
      ohm_util_TRACE_ERROR ("conv_utf16_to_utf8(): Exception");
      ret_val = Err_EXCEPTION;
   }

   return (ret_val);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace txt
}  // namespace ohm



#endif   // ohm_txt_fnc_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
