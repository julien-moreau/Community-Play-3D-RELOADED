/*****************************************************************************

        ArgInt.cpp
        Copyright (c) 2002 Ohm Force

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
   #pragma warning (1 : 4130 4223 4705 4706)
   #pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.h"
#include "ohm/math/fnc.h"
#include "ohm/txt/ascii/ascii_fnc.h"
#include "ohm/txt/ascii/ConvDigit.h"
#include "ohm/txt/out/ArgInt.h"
#include "ohm/txt/out/DevInterface.h"
#include "ohm/txt/out/FmtInfo.h"

#include <cassert>
#include <cmath>



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
   - output: Object used to display the number.
Throws: Nothing
==============================================================================
*/

ArgInt::ArgInt (DevInterface &output, const FmtInfo &fmt)
:  ArgNumBase (output, fmt)
,  _size (1)
,  _sign_val (0)
,  _sign_size (0)
,  _sign_char ('+')
,  _val (0)
,  _base (10)
,  _digit_arr ()
{
   // Nothing
}



/*
==============================================================================
Name: print
Description:
   Function called by the main class. It prints parameter value, given the
   format passed in ctor to the virtual output device also specified in ctor.
Input parameters:
   - val: Value to print.
Throws: Nothing
==============================================================================
*/

void  ArgInt::print (const IntType &val)
{
   set_displayable (true);
   evaluate_format_and_size (val);
   compute_digits ();

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



/*
==============================================================================
Name: get_nbr_displayed_chars
Description:
   Return the number of displayed characters before any padding with
   whitespace, but once truncation done.
Returns:
   Number of displayed characters.
Throws: Nothing
==============================================================================
*/

long  ArgInt::get_nbr_displayed_chars () const
{
   return (_sign_size + conv_nbr_digits_to_nbr_char (_size));
}



/*
==============================================================================
Name: print_number_sign
Description:
   Print number sign if required.
Throws: Nothing
==============================================================================
*/

void  ArgInt::print_number_sign ()
{
   if (_sign_size > 0)
   {
      use_output ().print_char (_sign_char);
   }
}



/*
==============================================================================
Name: print_digit_sequence
Description:
   Print a digit sequence from the number. 0's are never converted to space or
   truncated here.
Input parameters:
   - higher_pos: Power of _base of the left digit.
   - nbr_digits: Number of digits to display.
Throws: Nothing
==============================================================================
*/

void  ArgInt::print_digit_sequence (long higher_pos, long nbr_digits)
{
   const FmtInfo &   fmt = use_fmt ();
   const bool     lower_case_flag =
      (fmt.get_case_mode () == FmtInfo::CaseMode_LOWER);

   DevInterface & output = use_output ();

   for (long pos = 0; pos < nbr_digits; ++pos)
   {
      const int      digit = _digit_arr [higher_pos - pos];
      const char     digit_char =
         ascii::ConvDigit::conv_digit_to_char (digit);
      const char     digit_char_case =
         ascii::change_case (digit_char, lower_case_flag);
      output.print_char (digit_char_case);
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name: evaluate_format_and_size
Description:
   Fills the format structure.
Input parameters:
   - val: Number to print.
Throws: Nothing
==============================================================================
*/

void  ArgInt::evaluate_format_and_size (const IntType &val)
{
   using namespace std;

   _val = val;
   _base = use_fmt ().get_base ();
   _size = log_int (_val, _base) + 1;

   // Compute space required for sign and get sign character
   _sign_val = lang::sgn (val);
   _sign_size = 0;
   _sign_char = ' ';
   if (   use_fmt ().get_sign_mode () != FmtInfo::SignMode_NONE
       || _sign_val < 0)
   {
      _sign_size = 1;
   }
   if (_sign_val < 0)
   {
      _sign_char = '-';
   }
   else if (use_fmt ().get_sign_mode () == FmtInfo::SignMode_ALWAYS)
   {
      _sign_char = '+';
   }

   // Check if it can be displayed
   const long     nbr_disp_char = get_nbr_displayed_chars ();
   if (nbr_disp_char > use_fmt ().get_max_len ())
   {
      set_displayable (false);
   }

   format_filler ();
}



void  ArgInt::compute_digits ()
{
   IntType        val = _val;
   for (int p = 0; p < _size; ++p)
   {
      const IntType  x = val / _base;
      const int      d = static_cast <int> (val - x * _base);
      _digit_arr [p] = lang::abs (d);
      val = x;
   }
}



/*
==============================================================================
Name: print_value
Description:
   Print the whole value. Formating should be done before.
Throws: Nothing
==============================================================================
*/

void  ArgInt::print_value ()
{
   // Pre-fill and sign
   print_pre_fill_and_sign (_size);

   // Value
   print_positive_integer (_size, -1);

   // Post-fill
   print_post_fill ();
}



/*
==============================================================================
Name: log_int
Description:
   Computes floor (log (abs (val)) / log (base)), actually the number of
   displayed digits - 1
Input parameters:
   - val: The value
   - base: base of the logarithm, >= 2
Returns: result... and 0 for val == 0
Throws: Nothing
==============================================================================
*/

int   ArgInt::log_int (const IntType &val, int base)
{
   assert (base >= 2);

   int            val_logbase = -1;
   IntType        x = val;
   do
   {
      x /= base;
      ++ val_logbase;
   }
   while (x != 0);

   return (val_logbase);
}



}  // namespace out
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
