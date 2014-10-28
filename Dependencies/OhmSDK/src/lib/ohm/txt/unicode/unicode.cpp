/*****************************************************************************

        unicode.cpp
        Copyright (c) 2009 Ohm Force

        Author: Raphael Dinge <raphael.dinge@ohmforce.com>
        Revision: 36318

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

#include "ohm/txt/unicode/unicode.h"
#include "ohm/txt/utf8/Codec8.h"
#include "ohm/txt/utf8/def.h"
#include "ohm/txt/Err.h"

#include <cassert>



namespace ohm
{
namespace txt
{
namespace unicode
{



/*
==============================================================================
Name: conv_utf8_to_unicode
Input parameters:
   - src_0: Utf8 source string, '\0'-terminated.
Output parameters:
   - dest: Unicode destination string.
Returns:
   0 if successful, otherwise one of txt::Err
==============================================================================
*/

int   conv_utf8_to_unicode (StringU & dest, const char src_0 [])
{
   int err = Err_OK;
   size_t i = 0;

   while ((err == txt::Err_OK) && (src_0 [i] != '\0'))
   {
      txt::unicode::Char   ucs4_char;
      int len = 0;

      err = utf8::Codec8::decode_char (ucs4_char, &src_0 [i], len);
      
      if (err == Err_OK)
      {
         i += len;
         dest += ucs4_char;
      }
   }
   
   return err;
}



/*
==============================================================================
Name: conv_unicode_to_utf8
Input parameters:
   - src_0: Utf8 source string, '\0'-terminated.
Output parameters:
   - dest: Unicode destination string.
Returns:
   0 if successful, otherwise one of txt::Err
==============================================================================
*/

int   conv_unicode_to_utf8 (std::string & dest, const Char src_0 [])
{
   int err = Err_OK;
   size_t i = 0;
   
   while ((err == txt::Err_OK) && (src_0 [i] != 0L))
   {
      char utf8_0 [utf8::MAX_BYTE_SEQ_LEN + 1];
   
      err = utf8::Codec8::encode_char (utf8_0, src_0 [i]);
      
      if (err == Err_OK)
      {
         ++i;
         dest += utf8_0;
      }
   }
   
   return err;
}



}  // namespace unicode
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
