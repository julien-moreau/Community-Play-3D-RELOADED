/*****************************************************************************

        ArgString.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.h"
#include "ohm/txt/out/ArgString.h"
#include "ohm/txt/out/DevInterface.h"
#include "ohm/txt/out/FmtInfo.h"

#include <cassert>
#include <cctype>
#include <cstring>



namespace std {}



namespace ohm
{
namespace txt
{
namespace out
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name: ctor
Input parameters:
   - fmt: User format for display.
Output parameters:
Input/output parameters:
   - output: Object used to display the string.
Throws: Nothing
==============================================================================
*/

ArgString::ArgString (DevInterface &output, const FmtInfo &fmt)
:  ArgBase (output, fmt)
,  _txt_0 (0)
,  _size (0)
,  _display_offset (0)
,  _displayed_size (0)
,  _pre_fill_size (0)
,  _post_fill_size (0)
,  _pre_fill_char (' ')
,  _post_fill_char (' ')
{
   // Nothing
}



/*
==============================================================================
Name: print
Description:
   Function called by the main class. It prints parameter value,
   given the format passed in ctor to the virtual output device also specified
   in ctor.
Input parameters:
   - val: String to print
Throws: Nothing
==============================================================================
*/

void  ArgString::print (const StringType &val)
{
   assert (val != 0);

   set_displayable (true);
   evaluate_format_and_size (val);

   if (! is_displayable ())
   {
      print_not_displayable_value ();
   }
   else
   {
      print_value ();
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name: evaluate_format_and_size
Description:
   Fills the format structure
Input parameters:
   - txt_0: Pointer on the string to print.
Throws: Nothing
==============================================================================
*/

void  ArgString::evaluate_format_and_size (const StringType txt_0)
{
   using namespace std;

   _txt_0 = txt_0;
   _size = long (strlen (_txt_0));
   _display_offset = 0;
   _displayed_size = lang::min (_size, use_fmt ().get_max_len ());

   format_filler ();
}



/*
==============================================================================
Name: format_filler
Description:
   Setup the filler depending on user choice.
Throws: Nothing
==============================================================================
*/

void  ArgString::format_filler ()
{
   _pre_fill_size = 0;
   _post_fill_size = 0;
   _pre_fill_char = ' ';
   if (use_fmt ().has_max_len ())
   {
      const long     empty_space = use_fmt ().get_max_len () - _displayed_size;
      const long     non_visible_len = _size - _displayed_size;

      switch (use_fmt ().get_justify_mode ())
      {
      case  FmtInfo::JustifyMode_CENTER:
         _pre_fill_size = empty_space / 2;
         _post_fill_size = empty_space - _pre_fill_size;
         _display_offset = non_visible_len / 2;
         break;

      case  FmtInfo::JustifyMode_RIGHT:
         _pre_fill_size = empty_space;
         _pre_fill_char = use_fmt ().get_whitespace ();
         _display_offset = non_visible_len;
         break;

      case  FmtInfo::JustifyMode_LEFT_PAD:
         _post_fill_size = empty_space;
         break;

      default:
         break;
      }
   }
}



/*
==============================================================================
Name: print_value
Description:
   Print the whole string. Formating should have been done before.
Throws: Nothing
==============================================================================
*/

void  ArgString::print_value ()
{
   assert (_txt_0 != 0);

   // Prefill
   for (long prefill_cnt = 0; prefill_cnt < _pre_fill_size; ++prefill_cnt)
   {
      use_output ().print_char (_pre_fill_char);
   }

   // String
   switch (use_fmt ().get_case_mode ())
   {
   case  FmtInfo::CaseMode_LOWER:
      print_string_lower ();
      break;

   case  FmtInfo::CaseMode_UPPER:
      print_string_upper ();
      break;

   default:
      print_string ();
      break;
   }

   // Postfill
   for (long postfill_cnt = 0; postfill_cnt < _post_fill_size; ++postfill_cnt)
   {
      use_output ().print_char (' ');
   }
}



/*
==============================================================================
Name: print_string
Description:
   Prints string as it is (without case change).
Throws: Nothing
==============================================================================
*/

void  ArgString::print_string ()
{
   const char *   snippet_0 = _txt_0 + _display_offset;
   for (long cnt = 0; cnt < _displayed_size; ++cnt)
   {
      use_output ().print_char (snippet_0 [cnt]);
   }
}



/*
==============================================================================
Name: print_string_upper
Description:
   Prints string converting all character to upper case.
Throws: Nothing
==============================================================================
*/

void  ArgString::print_string_upper ()
{
   using namespace std;

   const char *   snippet_0 = _txt_0 + _display_offset;
   for (long cnt = 0; cnt < _displayed_size; ++cnt)
   {
      use_output ().print_char (toupper (snippet_0 [cnt]));
   }
}



/*
==============================================================================
Name: print_string_lower
Description:
   Prints string converting all character to lower case.
Throws: Nothing
==============================================================================
*/

void  ArgString::print_string_lower ()
{
   using namespace std;

   const char *   snippet_0 = _txt_0 + _display_offset;
   for (long cnt = 0; cnt < _displayed_size; ++cnt)
   {
      use_output ().print_char (tolower (snippet_0 [cnt]));
   }
}



}  // namespace out
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
