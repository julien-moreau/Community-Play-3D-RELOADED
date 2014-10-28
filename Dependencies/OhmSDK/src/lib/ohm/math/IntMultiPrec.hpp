/*****************************************************************************

        IntMultiPrec.hpp
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 42510

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



#if ! defined (ohm_math_IntMultiPrec_CODEHEADER_INCLUDED)
#define  ohm_math_IntMultiPrec_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/lang/fnc.h"
#include "ohm/math/bit.h"
#include "ohm/math/BitMask.h"

#include <cstring>



namespace ohm
{
namespace math
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// Data is not initialised! A value must be set before using the object.
template <int NW>
IntMultiPrec <NW>::IntMultiPrec ()
:/*   _digits ()
,*/   _neg_flag ()
{
   // Nothing
}



// 0 is considered as +0
template <int NW>
IntMultiPrec <NW>::IntMultiPrec (archi::Int64 val)
:/*   _digits ()
,*/   _neg_flag ()
{
   set_val (val);
}



template <int NW>
IntMultiPrec <NW>::IntMultiPrec (archi::UInt64 val, bool neg_flag)
:/*   _digits ()
,*/   _neg_flag ()
{
   set_val (val, neg_flag);
}



template <int NW>
IntMultiPrec <NW>::IntMultiPrec (const IntMultiPrec <NW> &other)
:/*   _digits ()
 ,*/  _neg_flag (other._neg_flag)
{
   for (int k = 0; k < NBR_WORDS; ++k)
   {
      _digits [k] = other._digits [k];
   }
}



template <int NW>
IntMultiPrec <NW> &  IntMultiPrec <NW>::operator = (const IntMultiPrec <NW> &other)
{
   assert (&other != 0);

   if (this != &other)
   {
      for (int k = 0; k < NBR_WORDS; ++k)
      {
         _digits [k] = other._digits [k];
      }
      _neg_flag = other._neg_flag;
   }

   return (*this);
}



template <int NW>
bool  IntMultiPrec <NW>::comp_eq (const IntMultiPrec <NW> &other) const
{
   assert (&other != 0);

   bool           eq_flag = (_neg_flag == other._neg_flag);
   if (! eq_flag)
   {
      eq_flag = true;
      for (int k = 0; k < NBR_WORDS && eq_flag; ++k)
      {
         eq_flag = ((_digits [k] | other._digits [k]) == 0);
      }
   }

   else
   {
      for (int k = 0; k < NBR_WORDS && eq_flag; ++k)
      {
         eq_flag = (_digits [k] == other._digits [k]);
      }
   }

   return (eq_flag);
}



// Returns -1 if lhs < rhs, 0 if lhs == rhs, +1 if lhs > rhs
// both_null_flag is can be set only when the result is null.
template <int NW>
int   IntMultiPrec <NW>::comp_magnitude (const IntMultiPrec <NW> &other, bool &lhs_null_flag, bool &rhs_null_flag) const
{
   int            mag_comp = 0;  // 0 = eq, -1 = lt, +1 = gt
   IntSgl         lhs_null_test = 0;
   IntSgl         rhs_null_test = 0;
   int            k;
   for (k = NBR_WORDS - 1; k >= 0 && mag_comp == 0; --k)
   {
      const IntSgl      d_lhs = _digits [k];
      const IntSgl      d_rhs = other._digits [k];

      lhs_null_test |= d_lhs;
      rhs_null_test |= d_rhs;

      if (d_lhs < d_rhs)
      {
         mag_comp = -1;
      }
      else if (d_lhs > d_rhs)
      {
         mag_comp = 1;
      }
   }
   for ( ; k >= 0; --k)
   {
      lhs_null_test |= _digits [k];
      rhs_null_test |= other._digits [k];
   }

   lhs_null_flag = (lhs_null_test == 0);
   rhs_null_flag = (rhs_null_test == 0);

   return (mag_comp);
}



template <int NW>
bool  IntMultiPrec <NW>::comp_lt (const IntMultiPrec <NW> &other) const
{
   assert (&other != 0);

   bool           lhs_null_flag;
   bool           rhs_null_flag;
   const int      mag_comp =
      comp_magnitude (other, lhs_null_flag, rhs_null_flag);

   // LHS <= 0 <= RHS
   if (_neg_flag && ! other._neg_flag)
   {
      // One of them is non-null
      return (! lhs_null_flag || ! rhs_null_flag);
   }

   // LHS < RHS <= 0
   else if (_neg_flag && other._neg_flag && mag_comp > 0)
   {
      return (true);
   }

   // 0 <= LHS < RHS
   else if (! _neg_flag && ! other._neg_flag && mag_comp < 0)
   {
      return (true);
   }

   // Other cases
   return (false);
}



template <int NW>
void  IntMultiPrec <NW>::clear ()
{
   _neg_flag = false;
   clear_and_keep_sign ();
}



template <int NW>
void  IntMultiPrec <NW>::clear_and_keep_sign ()
{
   for (int k = 0; k < NBR_WORDS; ++k)
   {
      _digits [k] = 0;
   }
}



// 0 is considered as +0
template <int NW>
void  IntMultiPrec <NW>::set_val (archi::Int64 val)
{
   if (val < 0)
   {
      set_val (archi::UInt64 (-val), true);
   }
   else
   {
      set_val (archi::UInt64 (val), false);
   }
}



template <int NW>
void  IntMultiPrec <NW>::set_val (archi::UInt64 val, bool neg_flag)
{
   _neg_flag = neg_flag;

   const IntSgl   mask = BitMask <IntSgl>::make (WORD_PREC);
   int            k = 0;
   while (val != 0 && k < NBR_WORDS)
   {
      _digits [k] = IntSgl (val) & mask;
      val >>= WORD_PREC;
      ++ k;
   }

   while (k < NBR_WORDS)
   {
      _digits [k] = 0;
      ++ k;
   }
}



template <int NW>
void  IntMultiPrec <NW>::set_val (archi::UInt64 val, bool neg_flag, int left_bitshift)
{
   assert (left_bitshift >= 0);

   int            word_shift = left_bitshift / WORD_PREC;
   const int      bit_shift = left_bitshift - word_shift * WORD_PREC;

   if (word_shift >= NBR_WORDS)
   {
      clear ();
   }

   else
   {
      int            k;

      for (k = 0; k < word_shift; ++k)
      {
         _digits [k] = 0;
      }

      assert (k == word_shift);

      const IntSgl   mask = BitMask <IntSgl>::make (WORD_PREC);
      _digits [k] = IntSgl (val << bit_shift) & mask;
      val >>= WORD_PREC - bit_shift;
      ++ k;

      while (val != 0 && k < NBR_WORDS)
      {
         _digits [k] = IntSgl (val) & mask;
         val >>= WORD_PREC;
         ++ k;
      }

      while (k < NBR_WORDS)
      {
         _digits [k] = 0;
         ++ k;
      }
   }

   _neg_flag = neg_flag;
}



template <int NW>
void  IntMultiPrec <NW>::set_val_bits (int beg, int end)
{
   assert (beg >= 0);
   assert (beg < end);
   assert (end <= GLOBAL_PREC);

   const int      beg_word = beg / WORD_PREC;
   const int      end_word = (end + WORD_PREC - 1) / WORD_PREC;

   const IntSgl   mask = BitMask <IntSgl>::make (WORD_PREC);

   int            k;
   for (k = 0; k < beg_word; ++k)
   {
      _digits [k] = 0;
   }
   for ( ; k < end_word; ++k)
   {
      _digits [k] = mask;
   }
   for ( ; k < NBR_WORDS; ++k)
   {
      _digits [k] = 0;
   }

   const int      offset_beg = beg - beg_word * WORD_PREC;
   _digits [beg_word    ] &= (~IntSgl (0)) << offset_beg;

   const int      offset_end = end_word * WORD_PREC - end;
   _digits [end_word - 1] &= mask >> offset_end;
}



template <int NW>
archi::UInt64  IntMultiPrec <NW>::get_val_abs_part (int bit_index) const
{
   assert (bit_index >= 0);
   assert (bit_index < GLOBAL_PREC);

   archi::UInt64  val = 0;
   const int      nbr_bits_to_fill = sizeof (val) * CHAR_BIT;
   int            nbr_bits_filled = 0;

   int            word_pos = bit_index / WORD_PREC;
   const int      bit_pos = bit_index - word_pos * WORD_PREC;

   if (bit_pos > 0)
   {
      val = _digits [word_pos] >> bit_pos;

      ++ word_pos;
      nbr_bits_filled += WORD_PREC - bit_pos;
   }

   while (nbr_bits_filled < nbr_bits_to_fill && word_pos < NBR_WORDS)
   {
      val += archi::UInt64 (_digits [word_pos]) << nbr_bits_filled;

      ++ word_pos;
      nbr_bits_filled += WORD_PREC;
   }

   return (val);
}



template <int NW>
void  IntMultiPrec <NW>::neg ()
{
   _neg_flag = ! _neg_flag;
}



// If there is an overflow, the MSW can keep the carry.
template <int NW>
void  IntMultiPrec <NW>::add (const IntMultiPrec <NW> &other)
{
   if (_neg_flag == other._neg_flag)
   {
      add_mag (other);
   }

   else
   {
      if (is_lower_mag_than (other))
      {
         sub_mag_inv (other);
         _neg_flag = ! _neg_flag;
      }

      else
      {
         sub_mag (other);
      }
   }
}



// Same remark as addition.
template <int NW>
void  IntMultiPrec <NW>::sub (const IntMultiPrec <NW> &other)
{
   if (_neg_flag != other._neg_flag)
   {
      add_mag (other);
   }

   else
   {
      if (is_lower_mag_than (other))
      {
         sub_mag_inv (other);
         _neg_flag = ! _neg_flag;
      }

      else
      {
         sub_mag (other);
      }
   }
}



template <int NW>
void  IntMultiPrec <NW>::add_small (int val)
{
   assert (val >= (-1 << WORD_PREC));
   assert (val <= (+1 << WORD_PREC));

   if (_neg_flag)
   {
      val = -val;
   }

   const IntSglSgn   old = IntSglSgn (_digits [0]);
   IntSglSgn      result = IntSglSgn (old + val);
   if (result < 0 && (find_size_words () <= 1))
   {
      _neg_flag = ! _neg_flag;
      result = -result;
   }

   _digits [0] = IntSgl (result);

   propagate_carry ();
}



template <int NW>
void  IntMultiPrec <NW>::mul (IntMultiPrec <NBR_WORDS * 2> &result, const IntMultiPrec <NW> &other) const
{
   ohm_lang_CHECK_CST (HdRoom, (IntMultiPrec <NBR_WORDS * 2>::HEADROOM_PREC == HEADROOM_PREC));
   ohm_lang_CHECK_CST (WPrec, (IntMultiPrec <NBR_WORDS * 2>::WORD_PREC == WORD_PREC));

   assert (&result != 0);
   assert (&other != 0);

   TmpDigitArray  sum;
   mul_mag (sum, other);

   result.copy_and_propagate_carry (result._digits, sum, NBR_WORDS * 2);
   result._neg_flag = _neg_flag ^ other._neg_flag;
}



// Keeps only the upper part of the result.
template <int NW>
void  IntMultiPrec <NW>::mul_high (const IntMultiPrec <NW> &other, bool round_flag)
{
   assert (&other != 0);

   _neg_flag ^= other._neg_flag;

   TmpDigitArray  sum;
   mul_mag (sum, other);

   if (round_flag)
   {
      sum [NBR_WORDS] += IntSgl (1) << (WORD_PREC - 1);
   }

   IntDbl         carry = simulate_carry (sum, NBR_WORDS);
   copy_and_propagate_carry (_digits, sum + NBR_WORDS, NBR_WORDS, carry);
}



// Keeps only the upper bits of the result and scale them word-wise.
// Returns the number of words used during the normalisation to shift
// the data (0 if result is alreay normalised).
template <int NW>
int   IntMultiPrec <NW>::mul_normalise (const IntMultiPrec <NW> &other, bool round_flag)
{
   assert (is_headroom_empty ());
   assert (&other != 0);

   _neg_flag ^= other._neg_flag;

   TmpDigitArray  sum;
   mul_mag (sum, other);

   IntSgl         digits_tmp [NBR_WORDS * 2];
   copy_and_propagate_carry (digits_tmp, sum, NBR_WORDS * 2);

   // Finds the upper word
   int            top_pos = NBR_WORDS * 2 - 1;
   while (top_pos >= 0 && digits_tmp [top_pos] == 0)
   {
      -- top_pos;
   }

   int            shift = 0;

   // Number is 0.
   if (top_pos < 0)
   {
      clear ();
   }

   else
   {
      shift = NBR_WORDS * 2 - 1 - top_pos;

      if (round_flag && top_pos >= NBR_WORDS)
      {
         int            pre_pos = top_pos - NBR_WORDS;
         int            post_pos = lang::min (top_pos + 1, NBR_WORDS * 2 - 1);
         digits_tmp [pre_pos] += IntSgl (1) << (WORD_PREC - 1);
         propagate_carry (digits_tmp + pre_pos, post_pos - pre_pos + 1);

         if (top_pos < post_pos && digits_tmp [post_pos] != 0)
         {
            ++ top_pos;
            -- shift;
            assert (shift >= 0);
         }
      }

      const int      nbr_copy = lang::min (top_pos + 1, int (NBR_WORDS));
      int            k;
      for (k = 0; k < nbr_copy; ++k)
      {
         _digits [NBR_WORDS - 1 - k] = digits_tmp [top_pos - k];
      }
      for (k = NBR_WORDS - 1 - nbr_copy; k >= 0; --k)
      {
         _digits [k] = 0;
      }
   }

   assert (is_normalised ());

   return (shift);
}



// Result has its headroom cleared.
template <int NW>
void  IntMultiPrec <NW>::shift_left (int nbr_bits)
{
   assert (nbr_bits >= 0);

   int            word_shift = nbr_bits / WORD_PREC;
   const int      bit_shift = nbr_bits - word_shift * WORD_PREC;

   if (word_shift > 0)
   {
      lang::min_in_place (word_shift, int (NBR_WORDS));
      shift_left_words_nocheck (word_shift);
   }

   if (bit_shift > 0 && word_shift < NBR_WORDS)
   {
      const IntSgl   mask = BitMask <IntSgl>::make (WORD_PREC);
      IntDbl         partial = 0;

      for (int k = 0; k < NBR_WORDS; ++k)
      {
         partial += IntDbl (_digits [k]) << bit_shift;
         _digits [k] = IntSgl (partial) & mask;
         partial >>= WORD_PREC;
      }
   }
}



// Result has its headroom cleared.
template <int NW>
void  IntMultiPrec <NW>::shift_left_words (int nbr_words)
{
   assert (nbr_words >= 0);

   if (nbr_words > 0)
   {
      lang::min_in_place (nbr_words, NBR_WORDS);
      shift_left_words_nocheck (nbr_words);
   }
}



// Result has its headroom cleared, but takes into account the headroom bits.
template <int NW>
void  IntMultiPrec <NW>::shift_right (int nbr_bits, bool round_flag)
{
   assert (nbr_bits >= 0);

   int            word_shift = nbr_bits / WORD_PREC;
   int            bit_shift = nbr_bits - word_shift * WORD_PREC;

   if (word_shift >= NBR_WORDS)
   {
      word_shift = NBR_WORDS;
      bit_shift = 0;
      round_flag = false;
   }

   else if (round_flag && nbr_bits > 0)
   {
      int            r_word = word_shift;
      int            r_bit = bit_shift - 1;
      if (r_bit < 0)
      {
         r_bit = WORD_PREC - 1;
         -- r_word;
      }

      _digits [r_word] += IntSgl (1) << r_bit;

      propagate_carry ();
   }

   const IntSgl   mask = BitMask <IntSgl>::make (WORD_PREC);

   if (word_shift > 0)
   {
      int            k;

      for (k = word_shift; k < NBR_WORDS - 1; ++k)
      {
         _digits [k - word_shift] = _digits [k];
      }

      // Spreads headroom, if any
      const IntSgl   top = _digits [NBR_WORDS - 1];
      _digits [NBR_WORDS - 1 - word_shift] = top & mask;
      _digits [NBR_WORDS     - word_shift] = top >> WORD_PREC;

      for (k = NBR_WORDS + 1 - word_shift; k < NBR_WORDS; ++k)
      {
         _digits [k] = 0;
      }
   }

   if (bit_shift > 0)
   {
      IntSgl         partial = 0;

      for (int k = NBR_WORDS - 1; k >= 0; --k)
      {
         const IntSgl   old = _digits [k];
         _digits [k] = (partial + (old >> bit_shift)) & mask;
         partial = old << (WORD_PREC - bit_shift);
      }
   }
}



// Result has its headroom cleared.
// However, the right shift takes into account the headroom bits.
template <int NW>
void  IntMultiPrec <NW>::shift (int nbr_bits, bool round_flag)
{
   if (nbr_bits > 0)
   {
      shift_left (nbr_bits);
   }
   else if (nbr_bits < 0)
   {
      shift_right (-nbr_bits, round_flag);
   }
}



// Rounds the number to the closest integer with the N lower bits cleared.
template <int NW>
void  IntMultiPrec <NW>::round_to (int nbr_bits)
{
   assert (nbr_bits > 0);
   assert (nbr_bits < GLOBAL_PREC);

   const int      bit_index = nbr_bits - 1;
   int            r_word = bit_index / WORD_PREC;
   int            r_bit = bit_index - r_word * WORD_PREC;

   IntSgl         val = _digits [r_word];
   val += IntSgl (1) << r_bit;
   val &= IntSgl (-2) << r_bit;
   _digits [r_word] = val;

   for (int pos = 0; pos < r_word; ++pos)
   {
      _digits [pos] = 0;
   }

   propagate_carry ();
}



template <int NW>
int   IntMultiPrec <NW>::normalise ()
{
   bool           null_flag;

   return (normalise (null_flag));
}



template <int NW>
int   IntMultiPrec <NW>::normalise (bool &null_flag)
{
   assert (&null_flag != 0);

   null_flag = false;

   int            word_shift = 0;

   const int      size = find_size_words ();
   if (size == NBR_WORDS + 1)
   {
      shift_right (WORD_PREC, true);
      word_shift = -1;
   }

   else if (size == 0)
   {
      null_flag = true;
   }

   else if (size < NBR_WORDS)
   {
      word_shift = NBR_WORDS - size;
      shift_left_words_nocheck (word_shift);
   }

   assert (is_normalised ());

   return (word_shift);
}



template <int NW>
bool  IntMultiPrec <NW>::is_normalised () const
{
   const int      size_words = find_size_words ();

   return (size_words == NBR_WORDS || size_words == 0);
}



template <int NW>
bool  IntMultiPrec <NW>::is_lower_mag_than (const IntMultiPrec <NW> &other) const
{
   assert (&other != 0);

   int            k = NBR_WORDS - 1;

   // Strictly > because we want k positive or 0 after the loop.
   while (k > 0 && _digits [k] == other._digits [k])
   {
      -- k;
   }

   return (_digits [k] < other._digits [k]);
}



template <int NW>
bool  IntMultiPrec <NW>::is_headroom_empty () const
{
   const IntSgl   overflow_val = IntSgl (1) << WORD_PREC;

   return (_digits [NBR_WORDS - 1] < overflow_val);
}



template <int NW>
bool  IntMultiPrec <NW>::is_null () const
{
   bool        null_flag = true;
   for (int k = 0; k < NBR_WORDS && null_flag; ++k)
   {
      if (_digits [k] != 0)
      {
         null_flag = false;
      }
   }

   return (null_flag);
}



template <int NW>
bool  IntMultiPrec <NW>::get_bit (int index) const
{
   assert (index >= 0);
   assert (index < REAL_CAPACITY);

   int            word_index = lang::min (index / WORD_PREC, NBR_WORDS - 1);
   int            bit_index = index - word_index * WORD_PREC;

   return ((_digits [word_index] & (IntSgl (1) << bit_index)) != 0);
}



template <int NW>
void  IntMultiPrec <NW>::chg_bit (int index, bool flag)
{
   if (flag)
   {
      set_bit (index);
   }
   else
   {
      clr_bit (index);
   }
}



template <int NW>
void  IntMultiPrec <NW>::clr_bit (int index)
{
   assert (index >= 0);
   assert (index < REAL_CAPACITY);

   int            word_index = lang::min (index / WORD_PREC, NBR_WORDS - 1);
   int            bit_index = index - word_index * WORD_PREC;

   _digits [word_index] &= ~(IntSgl (1) << bit_index);
}



template <int NW>
void  IntMultiPrec <NW>::set_bit (int index)
{
   assert (index >= 0);
   assert (index < REAL_CAPACITY);

   int            word_index = lang::min (index / WORD_PREC, NBR_WORDS - 1);
   int            bit_index = index - word_index * WORD_PREC;

   _digits [word_index] |= (IntSgl (1) << bit_index);
}



template <int NW>
void  IntMultiPrec <NW>::clr_bits (int beg, int end)
{
   assert (beg >= 0);
   assert (beg < end);
   assert (end <= GLOBAL_PREC);

   const int      beg_word = (beg + WORD_PREC - 1) / WORD_PREC;
   const int      end_word = end / WORD_PREC;

   const IntSgl   mask = BitMask <IntSgl>::make (WORD_PREC);

   for (int k = beg_word; k < end_word; ++k)
   {
      _digits [k] = 0;
   }

   const int      offset_beg = beg_word * WORD_PREC - beg;
   const int      offset_end = end - end_word * WORD_PREC;
   const IntSgl   beg_mask = mask >> offset_beg;
   const IntSgl   end_mask = (~IntSgl (0)) << offset_end;

   if (beg_word > end_word)
   {
      _digits [end_word] &= beg_mask | end_mask;
   }

   else
   {
      _digits [beg_word - 1] &= beg_mask;
      _digits [end_word    ] &= end_mask;
   }
}



// Returns -1 if number is 0.
// Result is in [-1 ; REAL_CAPACITY-1]
template <int NW>
int   IntMultiPrec <NW>::find_highest_bit_set () const
{
   int            hbs = -1;

   for (int k = NBR_WORDS - 1; k >= 0 && hbs < 0; --k)
   {
      const IntSgl   val = _digits [k];
      if (val != 0)
      {
         hbs = get_prev_pow2 (val) + k * WORD_PREC;
      }
   }

   return (hbs);
}



// Result is in [0 ; REAL_CAPACITY]
template <int NW>
int   IntMultiPrec <NW>::find_size_bits () const
{
   return (find_highest_bit_set () + 1);
}



// Returns NBR_WORDS + 1 in case of headroom use.
template <int NW>
int   IntMultiPrec <NW>::find_size_words () const
{
   int            word_size = NBR_WORDS;

   if (! is_headroom_empty ())
   {
      word_size = NBR_WORDS + 1;
   }

   else
   {
      while (word_size > 0 && _digits [word_size - 1] == 0)
      {
         -- word_size;
      }
   }

   return (word_size);
}



template <int NW>
bool  IntMultiPrec <NW>::is_negative () const
{
   return (_neg_flag);
}



template <int NW>
void  IntMultiPrec <NW>::set_sign (bool neg_flag)
{
   _neg_flag = neg_flag;
}



template <int NW>
void  IntMultiPrec <NW>::bin_not ()
{
   const IntSgl   mask = BitMask <IntSgl>::make (WORD_PREC);

   for (int k = 0; k < NBR_WORDS - 1; ++k)
   {
      const IntSgl   val = _digits [k];
      _digits [k] = (~val) & mask;
   }

   _digits [NBR_WORDS - 1] = ~_digits [NBR_WORDS - 1];
}



template <int NW>
void  IntMultiPrec <NW>::bin_or (const IntMultiPrec <NW> &other)
{
   assert (&other != 0);

   for (int k = 0; k < NBR_WORDS; ++k)
   {
      _digits [k] |= other._digits [k];
   }
}



template <int NW>
void  IntMultiPrec <NW>::bin_and (const IntMultiPrec <NW> &other)
{
   assert (&other != 0);

   for (int k = 0; k < NBR_WORDS; ++k)
   {
      _digits [k] &= other._digits [k];
   }
}



template <int NW>
void  IntMultiPrec <NW>::bin_xor (const IntMultiPrec <NW> &other)
{
   assert (&other != 0);

   for (int k = 0; k < NBR_WORDS; ++k)
   {
      _digits [k] ^= other._digits [k];
   }
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <int NW>
void  IntMultiPrec <NW>::shift_left_words_nocheck (int nbr_words)
{
   assert (nbr_words >= 0);
   assert (nbr_words <= NBR_WORDS);

   int            k;

   for (k = NBR_WORDS - 1 - nbr_words; k >= 0; --k)
   {
      _digits [k + nbr_words] = _digits [k];
   }

   for (k = 0; k < nbr_words; ++k)
   {
      _digits [k] = 0;
   }
}



template <int NW>
void  IntMultiPrec <NW>::add_mag (const IntMultiPrec <NW> &other)
{
   for (int k = 0; k < NBR_WORDS; ++k)
   {
      _digits [k] += other._digits [k];
   }

   propagate_carry ();
}



template <int NW>
void  IntMultiPrec <NW>::sub_mag (const IntMultiPrec <NW> &other)
{
   for (int k = 0; k < NBR_WORDS; ++k)
   {
      _digits [k] -= other._digits [k];
   }

   propagate_carry ();
}



template <int NW>
void  IntMultiPrec <NW>::sub_mag_inv (const IntMultiPrec <NW> &other)
{
   for (int k = 0; k < NBR_WORDS; ++k)
   {
      _digits [k] = other._digits [k] - _digits [k];
   }

   propagate_carry ();
}



template <int NW>
void  IntMultiPrec <NW>::mul_mag (TmpDigitArray &result, const IntMultiPrec <NW> &other) const
{
   memset (result, 0, sizeof (result));

   // This could be optimised by iterating on the k-th element of sum []
   // and then summing all terms of index i+j = k.
   for (int j = 0; j < NBR_WORDS; ++j)
   {
      const IntDbl   op (_digits [j]);
      for (int i = 0; i < NBR_WORDS; ++i)
      {
         const IntDbl   partial = op * other._digits [i];
         result [i + j] += partial;
      }
   }
}



template <int NW>
void  IntMultiPrec <NW>::propagate_carry ()
{
   propagate_carry (_digits, NBR_WORDS);
}



// On the output, the digits are clean (no carry), except the headroom
// of the MSW.
template <int NW>
void  IntMultiPrec <NW>::propagate_carry (IntSgl digits [], int nbr_digits)
{
   assert (nbr_digits > 0);

   const IntSgl   mask = BitMask <IntSgl>::make (WORD_PREC);
   IntSglSgn      carry = 0;
   for (int k = 0; k < nbr_digits - 1; ++k)
   {
      IntSgl         x = digits [k];
      x = IntSgl (x + carry);

      digits [k] = x & mask;

      carry = IntSglSgn (x) >> WORD_PREC;
   }

   // MSW is not masked, so we can tolerate an overflow of a few bits.
   IntSgl         x = digits [nbr_digits - 1];
   x = IntSgl (x + carry);
   digits [nbr_digits - 1] = x;

   // Overflow may occur, but the result should always remain positive.
   assert (digits [nbr_digits - 1] == x);
}



template <int NW>
typename IntMultiPrec <NW>::IntDbl  IntMultiPrec <NW>::simulate_carry (const IntDbl digits [], int nbr_digits)
{
   assert (digits != 0);
   assert (nbr_digits > 0);

   IntDbl         carry = 0;
   for (int k = 0; k < nbr_digits - 1; ++k)
   {
      const IntDbl   x = digits [k];
      carry = (carry + x) >> WORD_PREC;
   }

   return (carry);
}



// On the output, the digits are clean (no carry), except the headroom
// of the MSW.
template <int NW>
void  IntMultiPrec <NW>::copy_and_propagate_carry (IntSgl digits_dest [], const IntDbl digits_src [], int nbr_digits, IntDbl carry)
{
   assert (digits_dest != 0);
   assert (digits_src != 0);
   assert (nbr_digits > 0);

   const IntSgl   mask = BitMask <IntSgl>::make (WORD_PREC);
   for (int k = 0; k < nbr_digits - 1; ++k)
   {
      IntDbl         x = digits_src [k];
      x += carry;

      IntSgl         y = IntSgl (x);
      y &= mask;
      digits_dest [k] = y;

      carry = x >> WORD_PREC;
   }

   // MSW is not masked, so we can tolerate an overflow of a few bits.
   IntDbl         x = digits_src [nbr_digits - 1];
   x += carry;
   digits_dest [nbr_digits - 1] = IntSgl (x);

   // Overflow may occur, but the result should always remain positive.
   assert (digits_dest [nbr_digits - 1] == x);
}



/*\\\ STATIC NON-MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <int NW>
static inline bool   operator == (const IntMultiPrec <NW> &lhs, const IntMultiPrec <NW> &rhs)
{
   return (lhs.comp_eq (rhs));
}



template <int NW>
static inline bool   operator != (const IntMultiPrec <NW> &lhs, const IntMultiPrec <NW> &rhs)
{
   return (! (lhs == rhs));
}



template <int NW>
static inline bool   operator < (const IntMultiPrec <NW> &lhs, const IntMultiPrec <NW> &rhs)
{
   return (lhs.comp_lt (rhs));
}



template <int NW>
static inline bool   operator <= (const IntMultiPrec <NW> &lhs, const IntMultiPrec <NW> &rhs)
{
   return (! rhs.comp_lt (lhs));
}



template <int NW>
static inline bool   operator > (const IntMultiPrec <NW> &lhs, const IntMultiPrec <NW> &rhs)
{
   return (rhs.comp_lt (lhs));
}



template <int NW>
static inline bool   operator >= (const IntMultiPrec <NW> &lhs, const IntMultiPrec <NW> &rhs)
{
   return (! lhs.comp_lt (rhs));
}



}  // namespace math
}  // namespace ohm



#endif   // ohm_math_IntMultiPrec_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
