/*****************************************************************************

        FmtInfo.cpp
        Copyright (c) 2002 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 30475

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
#include "ohm/txt/out/FmtInfo.h"

#include <cassert>
#include <climits>
#include <cctype>



namespace std { }



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
Throws: Nothing
==============================================================================
*/

FmtInfo::FmtInfo ()
:  _max_len (LONG_MAX)
,  _base (10)
,  _nbr_decimals (6)
,  _group_size (0)
,  _exp_size (0)
,  _justify_mode (JustifyMode_LEFT)
,  _float_mode (FloatMode_AUTO)
,  _case_mode (CaseMode_NONE)
,  _sign_mode (SignMode_NONE)
,  _whitespace (' ')
,  _zero_trail_flag (false)
,  _truncate_flag (false)
{
   // Nothing
}



/*
==============================================================================
Name: parse_options
Description:
   Parse options from a string
Input parameters:
   - opt_ptr: pointer on the option string. It has not to finish with '\0'.
   - opt_len: Length of the string. Should be >= 0
   - param_ptr: Pointer on the array of option parameters.
   - max_param: Maximum number of parameters in the array. >= 0.
Returns: A negative number if an error occured (format error), or 0 if ok.
Throws: Nothing
==============================================================================
*/

int   FmtInfo::parse_options (const char *opt_ptr, long opt_len, const long param_ptr [], long max_param)
{
   assert (opt_ptr != 0);
   assert (opt_len >= 0);
   assert (param_ptr != 0);
   assert (max_param >= 0);

   using namespace std;

   ParserState    state (opt_ptr, opt_len, param_ptr, max_param);

   while (   state._ret_val == 0
          && state._scan_pos < state._opt_len)
   {
      const char     opt_code = opt_ptr [state._scan_pos];
      ++state._scan_pos;

      switch (tolower (opt_code))
      {

      // Length
      case  'm':
         parse_max_length_option (state);
         break;

      // Alignement
      case  'l':
         _justify_mode = JustifyMode_LEFT;
         break;

      case  'r':
         _justify_mode = JustifyMode_RIGHT;
         break;

      case  'c':
         _justify_mode = JustifyMode_CENTER;
         break;

      case  'k':
         _justify_mode = JustifyMode_LEFT_PAD;
         break;

      // Whitespace
      case  'w':
         parse_whitespace_option (state);
         break;

      // Number of decimals
      case  'p':
         parse_nbr_decimals_option (state);
         break;

      // Group size
      case  's':
         parse_group_size_option (state);
         break;

      // Zero trail
      case  'o':
         _zero_trail_flag = false;
         break;

      case  'v':
         _zero_trail_flag = true;
         break;

      // Rounding mode (truncate)
      case  't':
         _truncate_flag = true;
         break;

      // Floating point display mode
      case  'e':
         parse_fp_disp_mode_option (state, FloatMode_SCI);
         break;

      case  'f':
         _float_mode = FloatMode_NORM;
         break;

      case  'g':
         parse_fp_disp_mode_option (state, FloatMode_AUTO);
         break;

      // Base + Case
      case  'b':
         _case_mode = (isupper (opt_code) ? CaseMode_UPPER : CaseMode_LOWER);
         _base = 2;
         break;

      case  'd':
         _case_mode = (isupper (opt_code) ? CaseMode_UPPER : CaseMode_LOWER);
         _base = 10;
         break;

      case  'x':
         _case_mode = (isupper (opt_code) ? CaseMode_UPPER : CaseMode_LOWER);
         _base = 16;
         break;

      // Sign
      case  '-':
         _sign_mode = SignMode_MINUS;
         break;

      case  '+':
         _sign_mode = SignMode_ALWAYS;
         break;

      // Invalid option : ignore
      default:
         break;

      }
   }

   return (state._ret_val);
}



long  FmtInfo::get_max_len () const
{
   return (_max_len);
}



bool  FmtInfo::has_max_len () const
{
   return (_max_len < LONG_MAX);
}



int   FmtInfo::get_base () const
{
   return (_base);
}



int   FmtInfo::get_nbr_decimals () const
{
   return (_nbr_decimals);
}



int   FmtInfo::get_group_size () const
{
   return (_group_size);
}


int   FmtInfo::get_exp_size () const
{
   return (_exp_size);
}


FmtInfo::JustifyMode FmtInfo::get_justify_mode () const
{
   return (_justify_mode);
}



FmtInfo::FloatMode   FmtInfo::get_float_mode () const
{
   return (_float_mode);
}



FmtInfo::CaseMode FmtInfo::get_case_mode () const
{
   return (_case_mode);
}



FmtInfo::SignMode FmtInfo::get_sign_mode () const
{
   return (_sign_mode);
}



char  FmtInfo::get_whitespace () const
{
   return (_whitespace);
}



bool  FmtInfo::has_zero_trail () const
{
   return (_zero_trail_flag);
}



bool  FmtInfo::is_truncated () const
{
   return (_truncate_flag);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



FmtInfo::ParserState::ParserState (const char *opt_ptr, long opt_len, const long param_ptr [], long max_param)
:  _opt_ptr (opt_ptr)
,  _opt_len (opt_len)
,  _param_ptr (param_ptr)
,  _max_param (max_param)
,  _ret_val (0)
,  _cur_param (0)
,  _scan_pos (0)
{
   assert (_opt_ptr != 0);
   assert (_opt_len >= 0);
   assert (_param_ptr != 0);
   assert (_max_param >= 0);
}



void  FmtInfo::parse_max_length_option (FmtInfo::ParserState &state)
{
   const long     param = parse_parameter (state);
   if (state._ret_val == 0)
   {
      _max_len = lang::max (param, 0L);
   }
}



void  FmtInfo::parse_whitespace_option (FmtInfo::ParserState &state)
{
   if (state._scan_pos >= state._opt_len)
   {
      state._ret_val = -1; // Unexpected end
   }
   else
   {
      _whitespace = state._opt_ptr [state._scan_pos];
      ++state._scan_pos;
   }
}



void  FmtInfo::parse_nbr_decimals_option (FmtInfo::ParserState &state)
{
   const long     param = parse_parameter (state);
   if (state._ret_val == 0)
   {
      _nbr_decimals = lang::max (param, 0L);
   }
}



void  FmtInfo::parse_group_size_option (FmtInfo::ParserState &state)
{
   const long     param = parse_parameter (state);
   if (state._ret_val == 0)
   {
      _group_size = lang::max (param, 0L);
   }
}



void  FmtInfo::parse_fp_disp_mode_option (ParserState &state, FloatMode type)
{
   long           param = 0;
   if (is_parameter (state))
   {
      param = parse_parameter (state);
   }

   if (state._ret_val == 0)
   {
      _float_mode = type;
      _exp_size = param;
   }
}



bool  FmtInfo::is_parameter (const ParserState &state) const
{
   assert (&state != 0);

   bool           param_flag = false;
   if (state._scan_pos < state._opt_len)
   {
      const char     c = state._opt_ptr [state._scan_pos];
      if (c == '!' || c == '$' || (c >= '0' && c <= '9'))
      {
         param_flag = true;
      }
   }

   return (param_flag);
}



/*
==============================================================================
Name: parse_parameter
Description:
   Parse an option parameter. It can be made of:
   - A positive integer number, used directly.
   - '!', it uses the next parameter in the list, The parameter counter is
      only affected by '!', not by the '$' parameter access.
   - '$' followed by a positive integer number, it's an index for explicite
      parameter use.
Input/output parameters:
   - state: Current state of the parser. _ret_val is set to a negative number
      if parsing error occurs. If it's ok, _scan_pos is just after the
      parameter end.
Returns: The parsed value.
Throws: Nothing
==============================================================================
*/

long  FmtInfo::parse_parameter (FmtInfo::ParserState &state)
{
   long           ret_val = 0;

   assert (state._ret_val == 0);

   // End check
   if (state._scan_pos >= state._opt_len)
   {
      state._ret_val = -1; // Unexpected end
   }

   // Use next parameter
   else if (state._opt_ptr [state._scan_pos] == '!')
   {
      ++state._scan_pos;

      if (state._cur_param >= state._max_param)
      {
         state._ret_val = -1; // Too few parameters
      }
      else
      {
         ret_val = state._param_ptr [state._cur_param];
         ++state._cur_param;
      }
   }

   // Explicit parameter
   else if (state._opt_ptr [state._scan_pos] == '$')
   {
      ++state._scan_pos;

      const long     param_index = parse_parameter (state) - 1;   // [1 ; N]
      if (state._ret_val == 0)
      {
         if (   param_index < 0
             || param_index >= state._max_param)
         {
            state._ret_val = -1; // Wrong parameter index
         }
         else
         {
            ret_val = state._param_ptr [param_index];
         }
      }
   }

   // Explicit value
   else
   {
      const long     param = parse_number (state);
      if (state._ret_val == 0)
      {
         ret_val = param;
      }
   }

   return (ret_val);
}



/*
==============================================================================
Name: parse_number
Description:
   Extract a positive integer base-10 number.
Input/output parameters:
   - state: Current state of the parser. _ret_val is set to a negative number
      if parsing error occurs, for example if it can't extract any number from
      the string. If all is ok, _scan_pos points just after the number.
Returns: The value
Throws: Nothing
==============================================================================
*/

long  FmtInfo::parse_number (FmtInfo::ParserState &state)
{
   using namespace std;

   assert (state._ret_val == 0);

   long           val = 0;
   bool           at_least_one_digit_flag = false;

   while (   state._ret_val == 0
          && state._scan_pos < state._opt_len
          && isdigit (state._opt_ptr [state._scan_pos]))
   {
      if (val > (LONG_MAX / 10) - 1)
      {
         state._ret_val = -1; // Too big number
      }
      else
      {
         val *= 10;
         val += state._opt_ptr [state._scan_pos] - '0';
         ++state._scan_pos;
         at_least_one_digit_flag = true;
      }
   }

   if (! at_least_one_digit_flag)
   {
      state._ret_val = -1; // Unexpected end or missing number
   }

   return (val);
}



}  // namespace out
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
