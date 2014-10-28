/*****************************************************************************

        TraceTools.cpp
        Copyright (c) 2007 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 20605

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

#include "ohm/archi/types.h"
#include "ohm/txt/ascii/ConvDigit.h"
#include "ohm/util/TraceTools.h"

#include <cassert>
#include <cctype>



namespace ohm
{
namespace util
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void  TraceTools::dump_mem_as_hex (char *txt_0, long dest_max_len, const void *buf_ptr, long nbr_bytes)
{
   assert (txt_0 != 0);
   assert (dest_max_len > 0);
   assert (buf_ptr != 0);
   assert (nbr_bytes >= 0);

   const long     end_pos = dest_max_len - 1;
   long           src_pos = 0;
   long           dest_pos = 0;

   const archi::UByte * buf_b_ptr =
      reinterpret_cast <const archi::UByte *> (buf_ptr);
   while (dest_pos < end_pos && src_pos < nbr_bytes)
   {
      const int   b = buf_b_ptr [src_pos];
      ++ src_pos;

      const char  nibble_high =
         txt::ascii::ConvDigit::conv_digit_to_char (b >> 4);
      const char  nibble_low  =
         txt::ascii::ConvDigit::conv_digit_to_char (b & 15);

      txt_0 [dest_pos] = nibble_high;
      ++ dest_pos;

      if (dest_pos < end_pos)
      {
         txt_0 [dest_pos] = nibble_low;
         ++ dest_pos;
      }

      if (dest_pos < end_pos)
      {
         txt_0 [dest_pos + 2] = ' ';
         ++ dest_pos;
      }
   }

   if (dest_pos >= end_pos && src_pos < nbr_bytes)
   {
      truncate_string (txt_0, dest_max_len);
   }

   else
   {
      txt_0 [dest_pos] = '\0';
   }
}



// Replaces non-ASCII characters with a tilde (~)
void  TraceTools::dump_mem_as_filtered_string (char *txt_0, long dest_max_len, const void *buf_ptr, long nbr_bytes)
{
   assert (txt_0 != 0);
   assert (dest_max_len > 0);
   assert (buf_ptr != 0);
   assert (nbr_bytes >= 0);

   const long     end_pos = dest_max_len - 1;

   const archi::UByte * buf_b_ptr =
      reinterpret_cast <const archi::UByte *> (buf_ptr);

   long           pos;
   for (pos = 0; pos < nbr_bytes && pos < end_pos; ++pos)
   {
      using namespace std;

      unsigned char  c = buf_b_ptr [pos];
      if (! isprint (c))
      {
         c = '~';
      }
      txt_0 [pos] = c;
   }

   if (pos >= end_pos && pos < nbr_bytes)
   {
      truncate_string (txt_0, dest_max_len);
   }
   else
   {
      txt_0 [pos] = '\0';
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void  TraceTools::truncate_string (char *txt_0, long dest_max_len)
{
   assert (txt_0 != 0);
   assert (dest_max_len > 0);

   txt_0 [dest_max_len - 1] = '\0';

   if (dest_max_len >= 2)
   {
      txt_0 [dest_max_len - 2] = '.';
   }
   if (dest_max_len >= 3)
   {
      txt_0 [dest_max_len - 3] = '.';
   }
   if (dest_max_len >= 3)
   {
      txt_0 [dest_max_len - 4] = '.';
   }
}



}  // namespace util
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
