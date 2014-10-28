/*****************************************************************************

        ArgNumBase.cpp
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
#include "ohm/txt/out/ArgNumBase.h"
#include "ohm/txt/out/DevInterface.h"
#include "ohm/txt/out/FmtInfo.h"

#include <cassert>



namespace ohm
{
namespace txt
{
namespace out
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



ArgNumBase::ArgNumBase (DevInterface &output, const FmtInfo &fmt)
:  ArgBase (output, fmt)
,  _pre_fill_size (0)
,  _post_fill_size (0)
,  _pre_fill_char (' ')
,  _post_fill_char (' ')
{
   // Nothing
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// We suppose that all digits are 1-character long.
long  ArgNumBase::conv_nbr_digits_to_nbr_char (long nbr_digits) const
{
   assert (nbr_digits >= 0);

   long           nbr_chars = nbr_digits;
   const int      group_size = use_fmt ().get_group_size ();
   if (group_size > 0)
   {
      nbr_chars += (nbr_digits - 1) / group_size;
   }

   return (nbr_chars);
}



long  ArgNumBase::conv_nbr_char_to_nbr_digits (long nbr_chars) const
{
   assert (nbr_chars >= 0);

   long           nbr_digits = nbr_chars;
   const int      group_size = use_fmt ().get_group_size ();
   if (group_size > 0)
   {
      const long     group_size_with_sep = group_size + 1;
      const long     nbr_full_groups = nbr_chars / group_size_with_sep;
      const long     rem_group_size = nbr_chars - nbr_full_groups * group_size_with_sep;
      nbr_digits = nbr_full_groups * group_size + rem_group_size;
   }

   return (nbr_digits);
}



/*
==============================================================================
Name: format_filler
Description:
   Setup the filler depending on user choice.
Throws: Nothing
==============================================================================
*/

void  ArgNumBase::format_filler ()
{
   _pre_fill_size = 0;
   _post_fill_size = 0;
   _pre_fill_char = ' ';
   if (use_fmt ().has_max_len ())
   {
      const long     nbr_disp_char = get_nbr_displayed_chars ();
      const long     empty_space = use_fmt ().get_max_len () - nbr_disp_char;

      switch (use_fmt ().get_justify_mode ())
      {
      case  FmtInfo::JustifyMode_CENTER:
         _pre_fill_size = empty_space / 2;
         _post_fill_size = empty_space - _pre_fill_size;
         break;
      case  FmtInfo::JustifyMode_RIGHT:
         _pre_fill_size = empty_space;
         _pre_fill_char = use_fmt ().get_whitespace ();
         break;
      case  FmtInfo::JustifyMode_LEFT_PAD:
         _post_fill_size = empty_space;
         break;
      case  FmtInfo::JustifyMode_LEFT:
      default:
         break;
      }
   }
}



void  ArgNumBase::print_pre_fill_and_sign (long int_size)
{
   assert (int_size > 0);

   const bool     print_sign_first_flag = (_pre_fill_char == '0');

   if (print_sign_first_flag)
   {
      print_number_sign ();
   }

   print_pre_fill (int_size);

   if (! print_sign_first_flag)
   {
      print_number_sign ();
   }
}



void  ArgNumBase::print_post_fill ()
{
   for (long postfill_cnt = 0; postfill_cnt < _post_fill_size; ++postfill_cnt)
   {
      use_output ().print_char (_post_fill_char);
   }
}



void  ArgNumBase::print_positive_integer (long int_size, long offset)
{
   assert (int_size > 0);

   const int      group_size = use_fmt ().get_group_size ();

   if (group_size <= 0)
   {
      print_digit_sequence (offset + int_size, int_size);
   }

   else
   {
      const char     sep_char = ' ';
      const long     nbr_groups = (int_size + group_size - 1) / group_size;
      for (long grp = 0; grp < nbr_groups; ++grp)
      {
         if (grp > 0)
         {
            use_output ().print_char (sep_char);
         }
         const long     full_grp_higher_pos = (nbr_groups - grp) * group_size;
         const long     lower_pos = full_grp_higher_pos - group_size;
         const long     higher_pos = lang::min (full_grp_higher_pos, int_size);
         print_digit_sequence (offset + higher_pos, higher_pos - lower_pos);
      }
   }
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void  ArgNumBase::print_pre_fill (long int_size)
{
   const int      group_size = use_fmt ().get_group_size ();

   // Simple filling
   if (   group_size == 0
       || _pre_fill_char != '0'
       || use_fmt ().get_justify_mode () != FmtInfo::JustifyMode_RIGHT)
   {
      for (long prefill_cnt = 0; prefill_cnt < _pre_fill_size; ++prefill_cnt)
      {
         use_output ().print_char (_pre_fill_char);
      }
   }

   // Print the zeros as they were part of the number
   else
   {
      const char     sep_char = ' ';
      const int      group_size = use_fmt ().get_group_size ();
      const long     full_size =   _pre_fill_size
                                 + conv_nbr_digits_to_nbr_char (int_size);
      const long     group_size_with_sep = group_size + 1;
      for (long prefill_cnt = 0; prefill_cnt < _pre_fill_size; ++prefill_cnt)
      {
         char           c = _pre_fill_char;
         if ((full_size - prefill_cnt - 1) % group_size_with_sep == group_size)
         {
            c = sep_char;
         }
         use_output ().print_char (c);
      }
   }
}



}  // namespace out
}  // namespace txt
}  // namespace ohm



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
