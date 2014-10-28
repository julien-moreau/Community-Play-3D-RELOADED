/*****************************************************************************

        utf16_fnc.cpp
        Copyright (c) 2011 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 60312

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

#include "ohm/txt/unicode/ConvCi.h"
#include "ohm/txt/unicode/PredEq.h"
#include "ohm/txt/utf16/Codec16.h"
#include "ohm/txt/utf16/utf16_fnc.h"
#include "ohm/txt/Err.h"

#include <cassert>



namespace ohm
{
namespace txt
{
namespace utf16
{



// Position is set to -1 if not found or error.
// An empty string is always found at position 0.
int   strstr_ci (const Char16 str_0 [], const Char16 *subset_0, long &subset_pos)
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
               Codec16 <Char16>::decode_char (
               c_str,
               &str_0 [pos_str],
               c_str_len
            );
            if (ret_val != Err_OK)
            {
               subset_pos = -1;
               return (ret_val);
            }

            unicode::Char  c_sub;
            int            c_sub_len;
            ret_val = Codec16 <Char16>::decode_char (
               c_sub,
               &subset_0 [pos_sub],
               c_sub_len
            );
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
         Codec16 <Char16>::get_char_seq_len_utf (step, str_0 [subset_pos]);
         subset_pos += step;
      }

      subset_pos = -1;
      return (Err_NOT_FOUND);
   }

   return (Err_OK);
}



}  // namespace utf16
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
