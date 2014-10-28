/*****************************************************************************

        ascii_fnc.hpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 31964

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



#if defined (ohm_txt_ascii_ascii_fnc_CURRENT_CODEHEADER)
   #error Recursive inclusion of ascii_fnc code header.
#endif
#define  ohm_txt_ascii_ascii_fnc_CURRENT_CODEHEADER

#if ! defined (ohm_txt_ascii_ascii_fnc_CODEHEADER_INCLUDED)
#define  ohm_txt_ascii_ascii_fnc_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/math/FloatMultiPrecScan.h"
#include "ohm/txt/ascii/ConvDigit.h"

#include <cctype>



namespace ohm
{
namespace txt
{
namespace ascii
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name: change_case
Description:
   Change the case of a character.
Input parameters:
   - c: The character which case is to be changed.
   - lower_case_flag: true if the character should be converted to lower case,
      false if it should be converted to upper case.
Returns: The converted character.
Throws: Nothing
==============================================================================
*/

char  change_case (char c, bool lower_case_flag)
{
   using namespace std;

   if (lower_case_flag)
   {
      c = tolower (c);
   }
   else
   {
      c = toupper (c);
   }

   return (c);
}



// Returns true if conversion is OK.
// false: val == 0: failed to decode a number
// In case of success, stop_pos_ptr indicates the stop
// position (after the last translated character).
// Important: overflows are not checked!
template <int NW>
bool  conv_str_to_floatmultiprec (math::FloatMultiPrec <NW> &val, const char *txt_0, int base, long *stop_pos_ptr)
{
   assert (&val != 0);
   assert (txt_0 != 0);
   assert (base > 0);
   assert (base <= 'Z' - 'A' + 1 + 10);

   bool           ok_flag = true;

   bool           neg_flag = false;
   long           cur_pos = 0;
   while (isspace (txt_0 [cur_pos]))
   {
      ++ cur_pos;
   }
   if (txt_0 [cur_pos] == '+')
   {
      neg_flag = false;
      ++ cur_pos;
   }
   else if (txt_0 [cur_pos] == '-')
   {
      neg_flag = true;
      ++ cur_pos;
   }
   else if (txt_0 [cur_pos] == '\0')
   {
      ok_flag = false;
   }

   long           nbr_digits = 0;
   long           start_pos = -1;
   long           decimal_index = -1;
   long           exponent = 0;
   if (ok_flag)
   {
      while (isspace (txt_0 [cur_pos]))
      {
         ++ cur_pos;
      }

      if (ConvDigit::is_digit_from_base (txt_0 [cur_pos], base))
      {
         start_pos = cur_pos;
         nbr_digits = 1;
         ++ cur_pos;
      }
      else if (txt_0 [cur_pos] == '.')
      {
         start_pos = cur_pos;
         nbr_digits = 0;
         decimal_index = 0;
         ++ cur_pos;
      }
      else
      {
         ok_flag = false;
      }
   }

   if (ok_flag)
   {
      bool           scan_flag = true;
      while (scan_flag)
      {
         if (ConvDigit::is_digit_from_base (txt_0 [cur_pos], base))
         {
            ++ nbr_digits;
         }

         else if (txt_0 [cur_pos] == '.' && decimal_index < 0)
         {
            decimal_index = nbr_digits;
         }

         else
         {
            scan_flag = false;

            if (nbr_digits < 1)
            {
               ok_flag = false;
            }
            else if (toupper (txt_0 [cur_pos]) == 'E')
            {
               char *         stop_0;
               const char *   start_0 = txt_0 + cur_pos + 1;
               exponent = strtol (start_0, &stop_0, base);
               if (stop_0 == start_0)
               {
                  ok_flag = false;
               }
               cur_pos = long (stop_0 - txt_0);
            }
         }

         if (scan_flag)
         {
            ++ cur_pos;
         }
      }
   }

   if (ok_flag)
   {
      math::FloatMultiPrecScan <NW> scanner;
      scanner.start (base);

      for (int digit_index = 0; digit_index < nbr_digits; ++digit_index)
      {
         long        digit_pos = start_pos + digit_index;
         if (decimal_index >= 0 && digit_index >= decimal_index)
         {
            ++ digit_pos;
         }

         const int      digit =
            ConvDigit::conv_char_to_digit (txt_0 [digit_pos]);
         scanner.add_digit (digit);
      }

      if (decimal_index < 0)
      {
         exponent += nbr_digits - 1;
      }
      else
      {
         exponent += decimal_index - 1;
      }

      val = scanner.get_number (exponent);

      if (neg_flag)
      {
         val.neg ();
      }
   }

   if (! ok_flag)
   {
      val.clear ();
      cur_pos = 0;
   }

   if (stop_pos_ptr != 0)
   {
      *stop_pos_ptr = cur_pos;
   }

   return (ok_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace ascii
}  // namespace txt
}  // namespace ohm



#endif   // ohm_txt_ascii_ascii_fnc_CODEHEADER_INCLUDED

#undef ohm_txt_ascii_ascii_fnc_CURRENT_CODEHEADER



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
