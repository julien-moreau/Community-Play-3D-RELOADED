/*****************************************************************************

        fnc.cpp
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



#if defined (_MSC_VER)
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/txt/unicode/ConvCi.h"
#include "ohm/txt/unicode/PredEq.h"
#include "ohm/txt/Err.h"
#include "ohm/txt/fnc.h"
#include "ohm/util/trace.h"

#include <cassert>
#include <cstring>
#include <cctype>



namespace ohm
{
namespace txt
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name: make_string_pattern
Description:
   Fill a string with a character.
Input parameters:
   - c: Filling character
   - n: Number of character to fill. >= 0.
Returns: The string.
Throws: std::string related exceptions
==============================================================================
*/

std::string make_string_pattern (char c, long n)
{
   assert (n >= 0);

   std::string    final_str;
   final_str.reserve (n);

   std::string    filler;
   filler += c;
   while (n != 0)
   {
      if (n & 1)
      {
         final_str += filler;
      }
      filler += filler;
      n /= 2;
   }

   return (final_str);
}



/*
==============================================================================
Name: make_string_pattern
Description:
   Generate a string filled with a pattern.
Input parameters:
   - str_0: Pointer on the pattern string. Cannot be empty.
   - n: Number of characters in the final string. Thus, if n is not a multiple
      of the str length, the last pattern will be truncated. >= 0.
Returns: The patternized string.
Throws: std::string related exceptions
==============================================================================
*/

std::string make_string_pattern (const char *str_0, long n)
{
   assert (n >= 0);

   using namespace std;

   std::string    final_str;
   final_str.reserve (n);

   const size_t   len = strlen (str_0);
   assert (len > 0);
   size_t         n2 = n / len;
   const size_t   residue = n - len * n2;

   std::string    filler (str_0);
   while (n2 != 0)
   {
      if ((n2 & 1) != 0)
      {
         final_str += filler;
      }
      filler += filler;
      n2 /= 2;
   }

   final_str += filler.substr (0, residue);

   return (final_str);
}



/*
==============================================================================
Name: make_string_pattern
Description:
   Generate a string filled with a pattern.
Input parameters:
   - str: The pattern string. Can't be empty.
   - n: Number of characters in the final string. Thus, if n is not a multiple
      of the str length, the last pattern will be truncated. >= 0.
Returns: The patternized string.
Throws: std::string related exceptions
==============================================================================
*/

std::string make_string_pattern (const std::string &str, long n)
{
   assert (n >= 0);

   return (make_string_pattern (str.c_str (), n));
}



/*
==============================================================================
Name: cat_string
Description:
   Concatenates two strings and truncates the copied string if it doesn't fit
   in the specified maximum length.
Input parameters:
   - src_0: Pointer on the nul-terminated string to be concatenated.
Output parameters:
Input/output parameters:
   - dest_0: Pointer on the nul-terminated string receiving the new string at
      its end.
   - rem_len: Number of remaining characters, without the trailing '\0'.
      Should be >= 0. At exit, contains the available number of characters for
      next concatenations.
Returns: true if concatenated string was truncated.
Throws: Nothing
==============================================================================
*/

bool  cat_string (char *dest_0, const char *src_0, long &rem_len)
{
   assert (rem_len >= 0);
   assert (dest_0 != 0);
   assert (src_0 != 0);

   using namespace std;

   const long     dest_len = long (strlen (dest_0));
   bool           trunc_flag = (src_0 [0] != '\0');

   // Is there room enough to copy something ?
   if (dest_len <= rem_len)
   {
      dest_0 += dest_len;
      trunc_flag = copy_string (dest_0, src_0, rem_len);
      rem_len -= long (strlen (dest_0));
   }

   return (trunc_flag);
}



void  find_replace (std::string &txt, const std::string &find_txt, const std::string &repl_txt, std::string::size_type pos, bool backward_flag, long max_nbr)
{
   assert (&txt != 0);
   assert (&find_txt != 0);
   assert (&repl_txt != 0);

   const bool     nolimit_flag = (max_nbr == 0);
   const std::string::size_type  len     = find_txt.length ();
   const std::string::size_type  len_new = repl_txt.length ();
   do
   {
      if (backward_flag)
      {
         pos = txt.rfind (find_txt, pos);
      }
      else
      {
         pos = txt.find (find_txt, pos);
      }

      if (pos != std::string::npos)
      {
         txt.replace (pos, len, repl_txt);
         if (! backward_flag)
         {
            pos += len_new;
         }
         -- max_nbr;
      }
   }
   while (pos != std::string::npos && (nolimit_flag || max_nbr > 0));
}



bool  is_substr_matching (const char *main_0, const char *substr_0)
{
   assert (main_0 != 0);
   assert (substr_0 != 0);

   bool           match_flag = true;
   long           pos = 0;
   while (match_flag && main_0 [pos] != '\0' && substr_0 [pos] != '\0')
   {
      match_flag = (main_0 [pos] == substr_0 [pos]);
      ++ pos;
   }

   if (match_flag)
   {
      match_flag = (substr_0 [pos] == '\0');
   }

   return (match_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
