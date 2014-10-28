/*****************************************************************************

        ArgFloat.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 33366

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
#include "ohm/txt/out/ArgFloat.h"
#include "ohm/txt/out/DevInterface.h"
#include "ohm/txt/out/FmtInfo.h"

#include <cassert>
#include <cmath>



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
   - fmt: User format for display
Output parameters:
Input/output parameters:
   - output: Object used to display the number.
Throws: Nothing
==============================================================================
*/

ArgFloat::ArgFloat (DevInterface &output, const FmtInfo &fmt)
:  ArgNumBase (output, fmt)
,  _printer ()
,  _mnt_fmt ()
,  _exp_fmt ()
,  _base (10)
,  _pos_prn (0)
,  _infinite_flag (false)
,  _scientific_flag (false)
,  _zero_flag (false)
{
   // Nothing
}



/*
==============================================================================
Name: print
Description:
   Function called by the main  class. It prints parameter value, given the
   format passed in ctor to the virtual output device also specified in ctor.
Input parameters:
   - val: Value to print.
Throws: Nothing
==============================================================================
*/

void  ArgFloat::print (const FloatType &val)
{
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



/*
==============================================================================
Name: get_nbr_displayed_chars
Description:
   Return the number of displayed characters before any padding with
   whitespace, but once truncation done.
Returns: Number of displayed characters.
Throws: Nothing
==============================================================================
*/

long  ArgFloat::get_nbr_displayed_chars () const
{
   long           nbr_disp_char =
        _mnt_fmt._sign_size
      + conv_nbr_digits_to_nbr_char (_mnt_fmt._int_part_size)
      + _mnt_fmt._dot_size
      + _mnt_fmt._dec_part_size;
   if (_scientific_flag)
   {
      nbr_disp_char +=   1
                       + _exp_fmt._sign_size
                       + _exp_fmt._size;
   }

   return (nbr_disp_char);
}



/*
==============================================================================
Name: print_number_sign
Description:
   Print mantissa sign if needed.
Throws: Nothing
==============================================================================
*/

void  ArgFloat::print_number_sign ()
{
   if (_mnt_fmt._sign_size > 0)
   {
      use_output ().print_char (_mnt_fmt._sign_char);
   }
}



/*
==============================================================================
Name: print_digit_sequence
Description:
   Print a digit sequence from the mantissa. 0's are never converted to space
   or truncated here.
Input parameters:
   - higher_pos: Power of _base of the left digit.
   - nbr_digits: Number of digits to display.
Throws: Nothing
==============================================================================
*/

void  ArgFloat::print_digit_sequence (long higher_pos, long nbr_digits)
{
   using namespace std;

   const bool     lower_case_flag =
      (use_fmt ().get_case_mode () == FmtInfo::CaseMode_LOWER);

   for (long cnt = 0; cnt < nbr_digits; ++cnt)
   {
      const long     dig_exp = higher_pos - cnt;
      const int      digit = compute_dig_at (dig_exp);
      const char     digit_char =
         ascii::ConvDigit::conv_digit_to_char (digit);
      const char     digit_char_case =
         ascii::change_case (digit_char, lower_case_flag);
      use_output ().print_char (digit_char_case);
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



int   ArgFloat::get_exp () const
{
   int            val_log = 0;

   if (! _zero_flag)
   {
      val_log = _printer.get_exp ();
   }

   return (val_log);
}



/*
==============================================================================
Name: evaluate_format_and_size
Description:
   Fills the format structure
Input parameters:
   - val: Number to print.
Throws: Nothing
==============================================================================
*/

void  ArgFloat::evaluate_format_and_size (const ArgFloat::FloatType &val)
{
   const FmtInfo& fmt = use_fmt ();
   double         val_abs = fabs (val);

   // Computes the space required to display the sign
   _mnt_fmt._sign_val = lang::sgn (val);
   _mnt_fmt._sign_size = 0;
   if (   fmt.get_sign_mode () != FmtInfo::SignMode_NONE
       || _mnt_fmt._sign_val < 0)
   {
      _mnt_fmt._sign_size = 1;
   }

   // Next
   _base = fmt.get_base ();

   if (val == 0)
   {
      _zero_flag = true;
   }
   else
   {
      _printer.set_nbr (val_abs, _base);
      _pos_prn = get_exp ();
      _printer.start ();
   }

   _mnt_fmt._dec_part_size = 0;

   int            val_log = get_exp ();

   // We could need a second pass because after being rounded, a number may
   // have its integer part changed as it has a different power of _base.
   // For example, if we want to print 999.99 in 5 characters, number will be
   // rounded to 1000. So integer part len should be reevaluated.
   bool           need_second_pass_flag = false;

   do
   {
      find_display_format ();
      format_exponent ();

      const long     nbr_disp_char = format_mantissa_integer_part ();

      // Decimal part
      if (is_displayable ())
      {
         _mnt_fmt._dec_part_size = fmt.get_nbr_decimals ();
         if (_mnt_fmt._dec_part_size > 0)
         {
            // -1 = decimal dot for mantissa
            lang::min_in_place (
               _mnt_fmt._dec_part_size,
               fmt.get_max_len () - nbr_disp_char - 1L
            );
            _mnt_fmt._dec_part_size = lang::max (_mnt_fmt._dec_part_size, 0L);
         }
         // It is possible to have _dec_part_size == 0 and no room for the dot
         // whereas we planned to display some decimals. In this case, dot will
         // be suppressed further.
      }

      // At this point we've got the maximum number of possibly displayed
      // decimals. This number will now be optimized to take the rounding mode
      // and the 0-optimization into account.

      // Rounding mode to nearest: checks if the exponent has changed.
      if (! _zero_flag && ! fmt.is_truncated () && ! need_second_pass_flag)
      {
         const long     digit_pos = _exp_fmt._value - _mnt_fmt._dec_part_size;
         BigFloat       val_abs_big (val_abs);
         BigFloat       fixer (_base);
         fixer = math::ipow (fixer, digit_pos);
         fixer.scale2_int (-1);
         val_abs_big += fixer;

         val_abs = val_abs_big.to_dbl ();
         _printer.set_nbr (val_abs, _base);
         _pos_prn = get_exp ();
         _printer.start ();

         const int      val_log_old = val_log;
         val_log = get_exp ();
         if (val_log != val_log_old)
         {
            need_second_pass_flag = true;
         }
      }
      else
      {
         need_second_pass_flag = false;
      }
   }
   while (need_second_pass_flag);

   // 0-optimization
   if (! use_fmt ().has_zero_trail ())
   {
      if (_mnt_fmt._dec_part_size > 0)
      {
         if (_zero_flag)
         {
            _mnt_fmt._dec_part_size = 0;
         }

         else
         {
            const long     nbr_nz_digits = get_decimal_trail_length ();
            lang::min_in_place (_mnt_fmt._dec_part_size, nbr_nz_digits);
         }
      }

      // Delete the decimal dot if it isn't needed anymore.
      if (_mnt_fmt._dec_part_size == 0)
      {
         _mnt_fmt._dot_size = 0;
      }
   }

   // Filler
   format_filler ();
}



/*
==============================================================================
Name: find_display_format
Description:
   Choose the display format (normal or scientific), depending on the user
   preference and number range.
Throws: Nothing.
==============================================================================
*/

void  ArgFloat::find_display_format ()
{
   switch (use_fmt ().get_float_mode ())
   {
   case  FmtInfo::FloatMode_AUTO:
      {
         const int      val_log = get_exp ();
         if (   val_log >= 10    // 10 000 000 000 and more
             || val_log < -use_fmt ().get_nbr_decimals ())
         {
            _scientific_flag = true;
         }
         else
         {
            _scientific_flag = false;
         }
      }
      break;

   case  FmtInfo::FloatMode_NORM:
      _scientific_flag = false;
      break;

   case  FmtInfo::FloatMode_SCI:
      _scientific_flag = true;
      break;

   default:
      assert (false);
      break;
   }
}



/*
==============================================================================
Name: format_exponent
Description:
   Fill the exponent structure. Should be called even if format display is not
   scientific.
Throws: Nothing
==============================================================================
*/

void  ArgFloat::format_exponent ()
{
   const int      val_log = get_exp ();

   _exp_fmt._value = 0;
   _exp_fmt._sign_size = 0;
   _exp_fmt._size = 0;
   _exp_fmt._character = 'e';
   _exp_fmt._sign_char = '+';
   if (_scientific_flag)
   {
      const int      group_size = lang::max (use_fmt ().get_group_size (), 1);
      long           group_value =
         math::floor_int (double (val_log) / group_size);
      _exp_fmt._value = group_value * group_size;
      if (   _exp_fmt._value < 0
          || use_fmt ().get_sign_mode () != FmtInfo::SignMode_NONE)
      {
         _exp_fmt._sign_size = 1;
      }

      if (_exp_fmt._value < 0)
      {
         _exp_fmt._sign_char = '-';
      }

      _exp_fmt._size = use_fmt ().get_exp_size ();

      long           val_abs = lang::abs (_exp_fmt._value);
      long           nbr_digits = 0;
      do
      {
         val_abs /= _base;
         ++ nbr_digits;
      }
      while (val_abs > 0);
      lang::max_in_place (_exp_fmt._size, nbr_digits);

      if (use_fmt ().get_case_mode () == FmtInfo::CaseMode_UPPER)
      {
         _exp_fmt._character = 'E';
      }
   }
}



/*
==============================================================================
Name: format_mantissa_integer_part
Description:
   Check if number can be displayed or not. Do some basic formating (sign, dot
   and integer part size).
Returns:
   Minimum number of characters which should be displayed. This is
   actually the length of the mantissa without decimal part.
Throws: Nothing
==============================================================================
*/

long  ArgFloat::format_mantissa_integer_part ()
{
   const int      val_log = get_exp ();

   // Sign
   _mnt_fmt._sign_char = ' ';
   if (_mnt_fmt._sign_val < 0)
   {
      _mnt_fmt._sign_char = '-';
   }
   else if (use_fmt ().get_sign_mode () == FmtInfo::SignMode_ALWAYS)
   {
      _mnt_fmt._sign_char = '+';
   }

   // Integer part.
   if (_scientific_flag)
   {
      _mnt_fmt._int_part_size = val_log - _exp_fmt._value + 1;
   }
   else
   {
      _mnt_fmt._int_part_size = 1 + lang::max (val_log, 0);
   }

   // Dot or not ? This flag may be furtherly modified by the 0 optimization
   // or truncate/round mode
   _mnt_fmt._dot_size = 1;
   if (use_fmt ().get_nbr_decimals () <= 0)
   {
      _mnt_fmt._dot_size = 0;
   }

   // Check if it can be displayed
   long     nbr_disp_char =
        _mnt_fmt._sign_size
      + conv_nbr_digits_to_nbr_char (_mnt_fmt._int_part_size);
   if (_scientific_flag)
   {
      nbr_disp_char += 1 + _exp_fmt._sign_size + _exp_fmt._size;
   }

   if (nbr_disp_char > use_fmt ().get_max_len ())
   {
      set_displayable (false);
   }

   return (nbr_disp_char);
}



/*
==============================================================================
Name: get_decimal_trail_length
Description: Compute the length of the non-zero decimal part.
Returns: Length of this part. Range : [0 ; max_decimals].
Throws: Nothing
==============================================================================
*/

long  ArgFloat::get_decimal_trail_length ()
{
   assert (! _zero_flag);

   using namespace std;

   long           len = 0;
   long           nz_len = 0;
   const long     start_pos = _exp_fmt._value - 1;
   bool           cont_flag = true;
   do
   {
      const int      digit_exp = start_pos - len;
      const int      digit = compute_dig_at (digit_exp, cont_flag);
      ++ len;
      if (digit != 0)
      {
         nz_len = len;
      }
   }
   while (cont_flag && len < _mnt_fmt._dec_part_size);

   return (nz_len);
}



/*
==============================================================================
Name: print_value
Description:
   Print the whole value. Formating and bias for rounding should be done
   before.
Throws: Nothing
==============================================================================
*/

void  ArgFloat::print_value ()
{
   // Pre-fill and mantissa sign
   print_pre_fill_and_sign (_mnt_fmt._int_part_size);

   // Mantissa
   print_positive_integer (_mnt_fmt._int_part_size, _exp_fmt._value - 1);

   if (_mnt_fmt._dot_size > 0)
   {
      use_output ().print_char ('.');
      print_digit_sequence (
         _exp_fmt._value - 1,
         _mnt_fmt._dec_part_size
      );
   }

   // Exponent
   if (_scientific_flag)
   {
      use_output ().print_char (_exp_fmt._character);
      if (_exp_fmt._sign_size > 0)
      {
         use_output ().print_char (_exp_fmt._sign_char);
      }
      print_exponent ();
   }

   // Post-fill
   print_post_fill ();
}



/*
==============================================================================
Name: print_exponent
Description:
   Print the absolute value of the exponent.
Throws: Nothing
==============================================================================
*/

void  ArgFloat::print_exponent ()
{
   assert (_exp_fmt._size > 0);

   const bool     lower_case_flag =
      (use_fmt ().get_case_mode () == FmtInfo::CaseMode_LOWER);

   long           val = lang::abs (_exp_fmt._value);
   long           scale = math::ipowp (_base, _exp_fmt._size - 1);
   for (long pos = 0; pos < _exp_fmt._size; ++pos)
   {
      const int      digit = (val / scale) % _base;
      const char     digit_char =
         ascii::ConvDigit::conv_digit_to_char (digit);
      const char     digit_char_case =
         ascii::change_case (digit_char, lower_case_flag);
      use_output ().print_char (digit_char_case);
      val -= digit * scale;
      scale /= _base;
   }
}



int   ArgFloat::compute_dig_at (int dig_exp)
{
   bool           significant_flag;

   return (compute_dig_at (dig_exp, significant_flag));
}



int   ArgFloat::compute_dig_at (int dig_exp, bool &significant_flag)
{
   int            digit = 0;
   significant_flag = true;

   if (_zero_flag)
   {
      significant_flag = false;
   }

   else
   {
      const int      val_log = get_exp ();

      if (dig_exp <= val_log)
      {
         if (dig_exp == _pos_prn)
         {
            if (_pos_prn == val_log)
            {
               _printer.start ();
            }
         }
         else
         {
            // Rewinds
            if (dig_exp > _pos_prn)
            {
               _printer.start ();
               _pos_prn = val_log;
            }

            // Skips digits
            while (dig_exp < _pos_prn)
            {
               if (_printer.is_rem_elt ())
               {
                  _printer.iterate ();
               }
               -- _pos_prn;
            }

            assert (_pos_prn == dig_exp);
         }

         if (_printer.is_rem_elt ())
         {
            digit = _printer.get_digit ();
         }
         else
         {
            significant_flag = false;
         }
      }
   }

   return (digit);
}



}  // namespace out
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
