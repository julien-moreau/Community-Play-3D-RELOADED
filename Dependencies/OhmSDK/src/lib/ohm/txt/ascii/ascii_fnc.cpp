/*****************************************************************************

        ascii_fnc.cpp
        Copyright (c) 2008 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 30385

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

#include "ohm/lang/def.h"
#include "ohm/txt/ascii/ascii_fnc.h"
#include "ohm/txt/ascii/ConvDigit.h"
#include "ohm/txt/Err.h"

#include <cassert>
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
Name: strstr_ci
Description:
   Search the position of a string within another one, without taking the
   case into account.
   Works only on pure ASCII strings.
Input parameters:
   - s1_0: pointer on the null-terminated string as search area.
   - s2_0: pointer on the null-terminated string to find.
Returns:
   - Index of the string if found.
   - Err_NOT_FOUND if... guess what.
Throws: Nothing
==============================================================================
*/

long  strstr_ci (const char *s1_0, const char *s2_0)
{
   using namespace std;

   long           offset = 0;
   for ( ; ; )
   {
      long           cnt = 0;
      while (s2_0 [cnt] != '\0')
      {
         if (s1_0 [cnt + offset] == '\0')
         {
            return (Err_NOT_FOUND);
         }
         if (toupper (s2_0 [cnt]) != toupper (s1_0 [cnt + offset]))
         {
            break;
         }
         ++cnt;
      }

      if (s2_0 [cnt] == '\0')
      {
         return (offset);
      }

      ++offset;
   }
}



/*
==============================================================================
Name: strcmp_ci
Description:
   Evaluates the case-insensitve-equality of two strings.
   Works only on pure ASCII strings.
Input parameters:
   - s1_0: pointer on the first null-terminated string.
   - s2_0: pointer on the second null-terminated string.
Returns: true
Throws: Nothing
==============================================================================
*/

bool  strcmp_ci (const char *s1_0, const char *s2_0)
{
   using namespace std;

   long           pos = 0;
   bool           ok_flag = true;
   do
   {
      ok_flag = (toupper (s1_0 [pos]) == toupper (s2_0 [pos]));
      ++pos;
   }
   while (ok_flag && s1_0 [pos-1] != '\0');

   return (ok_flag);
}



/*
==============================================================================
Name: to_lcase
Description:
   Convertit une chaine en minuscules
Input parameters:
   - txt: Chaine a convertir
Returns: Chaine en minuscules
Throws: ?
==============================================================================
*/

std::string to_lcase (const std::string &txt)
{
   std::string    result (txt);
   to_lcase_inplace (result);

   return (result);
}



/*
==============================================================================
Name: to_ucase
Description:
   Convertit une chaine en majuscules
Input parameters:
   - txt: Chaine a convertir
Returns: Chaine en majuscules
Throws: ?
==============================================================================
*/

std::string to_ucase (const std::string &txt)
{
   std::string    result (txt);
   to_ucase_inplace (result);

   return (result);
}



/*
==============================================================================
Name: to_lcase_inplace
Description:
   Convertit une chaine en minuscules, sur place
Input/output parameters:
   - txt: Chaine a convertir
Throws: Nothing
==============================================================================
*/

void  to_lcase_inplace (std::string &txt)
{
   using namespace std;

   const std::string::size_type  len = txt.length ();
   for (std::string::size_type i = 0; i < len; ++i)
   {
      txt [i] = tolower (txt [i]);
   }
}



/*
==============================================================================
Name: to_lcase_inplace
Description:
   Convertit une chaine en minuscules, sur place
Input/output parameters:
   - txt_0: Chaine a convertir
Throws: Nothing
==============================================================================
*/

void  to_lcase_inplace (char *txt_0)
{
   assert (txt_0 != 0);

   using namespace std;

   const size_t   len = strlen (txt_0);
   for (size_t i = 0; i < len; ++i)
   {
      txt_0 [i] = tolower (txt_0 [i]);
   }
}



/*
==============================================================================
Name: to_ucase_inplace
Description:
   Convertit une chaine en majuscules, sur place
Input/output parameters:
   - txt: Chaine a convertir
Throws: Nothing
==============================================================================
*/

void  to_ucase_inplace (std::string &txt)
{
   using namespace std;

   const size_t   len = txt.length ();
   for (size_t i = 0; i < len; ++i)
   {
      txt [i] = toupper (txt [i]);
   }
}



/*
==============================================================================
Name: to_ucase_inplace
Description:
   Convertit une chaine en majuscules, sur place
Input/output parameters:
   - txt_0: Chaine a convertir
Throws: Nothing
==============================================================================
*/

void  to_ucase_inplace (char *txt_0)
{
   assert (txt_0 != 0);

   using namespace std;

   const size_t   len = strlen (txt_0);
   for (size_t i = 0; i < len; ++i)
   {
      txt_0 [i] = toupper (txt_0 [i]);
   }
}



std::string trim_spaces (const char *txt_0)
{
   assert (txt_0 != 0);

   // Skips spaces at the beginning
   size_t         pos = 0;
   while (txt_0 [pos] != '\0' && isspace (txt_0 [pos]))
   {
      ++ pos;
   }

   // Finds the last non-space position
   const size_t   copy_start_pos = pos;
   size_t         copy_end_pos = pos;
   while (txt_0 [pos] != '\0')
   {
      if (! isspace (txt_0 [pos]))
      {
         copy_end_pos = pos + 1;
      }

      ++ pos;
   }

   const size_t      len = copy_end_pos - copy_start_pos;
   const std::string txt (&txt_0 [copy_start_pos], len);

   return (txt);
}



// Returns true if conversion is OK.
// false :
// - val == 0: failed to decode a number
// - val == archi::INT64_MAX_VAL: positive overflow
// - val == archi::INT64_MIN_VAL: negative overflow
// In case of success (even overflow), stop_pos_ptr indicates the stop
// position (after the last translated character).
bool  conv_str_to_int64 (archi::Int64 &val, const char *txt_0, int base, long *stop_pos_ptr)
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
      const char     c = txt_0 [pos];
      if (c == '\0' || ! isspace (c))
      {
         skip_flag = false;
      }
      else
      {
         ++ pos;
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
   while (ConvDigit::is_digit_from_base (txt_0 [pos], base))
   {
      const archi::Int64   old_val = val;

      const int      digit = ConvDigit::conv_char_to_digit (txt_0 [pos]);
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



// Returns true if conversion is OK.
// false :
// - val == 0: failed to decode a number
// - val == +INF: positive overflow
// - val == -INF: negative overflow
// In case of success (even overflow), stop_pos_ptr indicates the stop
// position (after the last translated character).
bool  conv_str_to_double (double &val, const char *txt_0, int base, long *stop_pos_ptr)
{
   typedef  math::FloatMultiPrec <4>   BigFloat;
   ohm_lang_CHECK_CST (Resol, (BigFloat::REAL_PREC >= DBL_MANT_DIG + 10));

   BigFloat    val_big;
   bool        ok_flag = conv_str_to_floatmultiprec (val_big, txt_0, base, stop_pos_ptr);

   if (ok_flag)
   {
      val = val_big.to_dbl ();

      // Checks NaN. Here the only NaNs we can have are +/- infinity.
      if (val != val)
      {
         ok_flag = false;
      }
   }

   return (ok_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace ascii
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
