/*****************************************************************************

        FloatMultiPrec.hpp
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 50334

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



#if ! defined (ohm_math_FloatMultiPrec_CODEHEADER_INCLUDED)
#define  ohm_math_FloatMultiPrec_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/math/BitMask.h"
#include "ohm/math/fnc.h"

#include <stdexcept>

#include <cassert>



namespace ohm
{
namespace math
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



// Data is not initialised!
template <int NW>
FloatMultiPrec <NW>::FloatMultiPrec ()
:  _mantissa ()
,  _exp_w ()
{
   // Nothing
}



template <int NW>
FloatMultiPrec <NW>::FloatMultiPrec (double val)
:  _mantissa ()
,  _exp_w ()
{
   from_dbl (val);
}



template <int NW>
FloatMultiPrec <NW>::FloatMultiPrec (int int_val)
:  _mantissa (archi::Int64 (int_val))
,  _exp_w (NBR_WORDS - 1)
{
   _exp_w -= _mantissa.normalise ();
}



template <int NW>
FloatMultiPrec <NW>::FloatMultiPrec (archi::Int64 int_val)
:  _mantissa (int_val)
,  _exp_w (NBR_WORDS - 1)
{
   _exp_w -= _mantissa.normalise ();
}



template <int NW>
FloatMultiPrec <NW>::FloatMultiPrec (const MantissaType &int_val)
:  _mantissa (int_val)
,  _exp_w (NBR_WORDS - 1)
{
   _exp_w -= _mantissa.normalise ();
}



template <int NW>
FloatMultiPrec <NW>::FloatMultiPrec (const FloatMultiPrec <NW> &other)
:  _mantissa (other._mantissa)
,  _exp_w (other._exp_w)
{
   assert (&other != 0);
   assert (_mantissa.is_normalised ());
}



template <int NW>
FloatMultiPrec <NW> &   FloatMultiPrec <NW>::operator = (const FloatMultiPrec <NW> &other)
{
   assert (&other != 0);

   if (&other != this)
   {
      _mantissa = other._mantissa;
      _exp_w    = other._exp_w;
   }

   return (*this);
}



template <int NW>
bool  FloatMultiPrec <NW>::is_null () const
{
   return (_mantissa.is_null ());
}



template <int NW>
bool  FloatMultiPrec <NW>::comp_eq (const FloatMultiPrec <NW> &other) const
{
   assert (&other != 0);
   assert (_mantissa.is_normalised ());
   assert (other._mantissa.is_normalised ());

   return (_exp_w == other._exp_w && _mantissa == other._mantissa);
}



template <int NW>
bool  FloatMultiPrec <NW>::comp_lt (const FloatMultiPrec &other) const
{
   assert (&other != 0);

   bool           lhs_null_flag;
   bool           rhs_null_flag;
   const int      mag_comp =
      _mantissa.comp_magnitude (other._mantissa, lhs_null_flag, rhs_null_flag);

   // All null: exit now
   if (lhs_null_flag && rhs_null_flag)
   {
      return (false);
   }

   const int      exp_dif = _exp_w - other._exp_w;
   const bool     lhs_neg_flag = _mantissa.is_negative ();
   const bool     rhs_neg_flag = other._mantissa.is_negative ();

   // LHS <= 0 <= RHS
   if (lhs_neg_flag && ! rhs_neg_flag)
   {
      return (true);
   }

   // LHS <= 0, RHS <= 0
   else if (lhs_neg_flag && rhs_neg_flag)
   {
      if (rhs_null_flag)
      {
         return (true);
      }
      else if (lhs_null_flag)
      {
         return (false);
      }

      // LHS < RHS < 0
      else if (exp_dif > 0)
      {
         return (true);
      }
      else if (exp_dif == 0 && mag_comp > 0)
      {
         return (true);
      }
   }

   // 0 <= LHS, 0 <= RHS
   else if (! lhs_neg_flag && ! rhs_neg_flag)
   {
      if (lhs_null_flag)
      {
         return (true);
      }
      else if (rhs_null_flag)
      {
         return (false);
      }

      // 0 < LHS < RHS
      else if (exp_dif < 0)
      {
         return (true);
      }
      else if (exp_dif == 0 && mag_comp < 0)
      {
         return (true);
      }
   }

   // Other cases
   return (false);
}



#if 0

template <int NW>
FloatMultiPrec <NW>::operator double () const
{
   return (to_dbl ());
}

#endif



template <int NW>
double   FloatMultiPrec <NW>::to_dbl () const
{
   assert (_mantissa.is_normalised ());

   double         result = 0;

   const int      highest_bit = _mantissa.find_highest_bit_set ();
   if (highest_bit >= 0)
   {
      // Sign
      const archi::UInt64  sign =
           _mantissa.is_negative ()
         ? ohm_archi_DECL_UINT64 (0x8000000000000000)
         : 0;

      // Mantissa
      // Note: DBL_MANT_DIG includes implicitely the highest bit set, which
      // is not coded in the double mantissa. Therefore the bit at position
      // DBL_MANT_DIG - 1 is this very bit. We'll have to filter it out for
      // regular (not denormalised) numbers.
      // But keep one more digit in order to be able to do the rounding.
      archi::UInt64        mantissa_bin =
         _mantissa.get_val_abs_part (highest_bit + 1 - DBL_MANT_DIG - 1);

      // Because our data is stored normalised, relatively to words and not
      // bits, we have to compute the bit offset required to get fully
      // normalised data.
      const int      denorm_offset = highest_bit - (GLOBAL_PREC - WORD_PREC);
      const int      exponent = _exp_w * WORD_PREC + denorm_offset;
      int            biased_exp = exponent + DBL_BIAS;

      // +/- Infinity
      if (exponent > DBL_MAX_EXP)
      {
         const archi::UInt64  shifted_exp =
            BitMask <archi::UInt64>::make (DBL_EXP_WIDTH) << DBL_EXP_SHIFT;

         *reinterpret_cast <archi::UInt64 *> (&result) =
            sign | shifted_exp;
      }

      // Denormalised number (or zero)
      else if (biased_exp <= 0)
      {
         int            mant_shift = 1 - biased_exp;
         if (mant_shift < DBL_MANT_DIG + 1)
         {
            mantissa_bin >>= mant_shift;

            // Rounding
            ++ mantissa_bin;
            mantissa_bin >>= 1;

            *reinterpret_cast <archi::UInt64 *> (&result) =
               sign | mantissa_bin;
         }
      }

      // Normalised number
      else
      {
         // Rounding
         ++ mantissa_bin;
         if (mantissa_bin >= (archi::UInt64 (2) << DBL_MANT_DIG))
         {
            mantissa_bin = 0;
            ++ biased_exp;
         }
         else
         {
            mantissa_bin >>= 1;
         }

         // Masks the mantissa
         const archi::UInt64  mantissa_mask =
            BitMask <archi::UInt64>::make (DBL_MANT_DIG - 1);
         mantissa_bin &= mantissa_mask;

         // Exponent
         const archi::UInt64  shifted_exp =
            archi::UInt64 (biased_exp) << DBL_EXP_SHIFT;

         *reinterpret_cast <archi::UInt64 *> (&result) =
            sign | shifted_exp | mantissa_bin;
      }
   }

   return (result);
}



template <int NW>
void  FloatMultiPrec <NW>::from_dbl (double val)
{
   const archi::UInt64  val_int =
      *reinterpret_cast <const archi::UInt64 *> (&val);

   const archi::UInt64  mantissa_mask =
      BitMask <archi::UInt64>::make (DBL_MANT_DIG - 1);
   archi::UInt64  mant = val_int & mantissa_mask;

   const int      exp_mask =  BitMask <int>::make (DBL_EXP_WIDTH);
   const int      biased_exp = int (val_int >> DBL_EXP_SHIFT) & exp_mask;

   const bool     neg_flag = ((val_int >> 63) != 0);
   _mantissa.set_sign (neg_flag);

   // Checks that the number is not a NaN nor an infinite.
   if (biased_exp == exp_mask)
   {
      throw std::runtime_error (
         "ohm::math::FloatMultiPrec::from_dbl(): provided data is NaN"
      );
   }

   int            exp_bit = biased_exp - DBL_BIAS;

   // Real number of bits of the mantissa (including the MSB)
   int            bit_mant = DBL_MANT_DIG;

   // Denormal or 0
   if (biased_exp == 0)
   {
      if (mant == 0)
      {
         // These values will generate a mantissa of 0 with
         // an exponent of 0.
         bit_mant = GLOBAL_PREC - WORD_PREC + 1;
         exp_bit = 0;
      }

      else
      {
         bit_mant = get_prev_pow2 (mant) + 1;

         // DBL_MANT_DIG-1 is the maximum size occupied by the full mantissa
         // in denormal mode.
         exp_bit += -(DBL_MANT_DIG-1) + bit_mant;
      }
   }

   // Normal
   else
   {
      // Adds the implicit MSB. mant has now DBL_MANT_DIG bits.
      mant |= ohm_archi_DECL_UINT64 (1) << (DBL_MANT_DIG - 1);
   }

   int            mant_shift = GLOBAL_PREC - WORD_PREC + 1 - bit_mant;
   _exp_w = div_int_floor (exp_bit, int (WORD_PREC));

   int            mant_shift_fix = exp_bit - _exp_w * WORD_PREC;
   mant_shift += mant_shift_fix;

   _mantissa.set_val (mant, neg_flag, mant_shift);

   assert (_mantissa.is_normalised ());
}



template <int NW>
void  FloatMultiPrec <NW>::clear ()
{
   _mantissa.clear ();
   _exp_w = 0;
}



template <int NW>
void  FloatMultiPrec <NW>::exp2_int (int exp_bit)
{
   int            mant_shift = GLOBAL_PREC - WORD_PREC + 1 - 1;
   _exp_w = div_int_floor (exp_bit, int (WORD_PREC));

   int            mant_shift_fix = exp_bit - _exp_w * WORD_PREC;
   mant_shift += mant_shift_fix;

   _mantissa.set_val (1, false, mant_shift);
}



// Multiplies by 2 ^ l2
template <int NW>
void  FloatMultiPrec <NW>::scale2_int (int l2)
{
   if (! _mantissa.is_null ())
   {
      const int      cur_high = _mantissa.find_highest_bit_set ();
      const int      cur_high_rel = cur_high - (GLOBAL_PREC - WORD_PREC);
      const int      new_high_rel_raw = cur_high_rel + l2;
      const int      delta_exp_w =
         div_int_floor (new_high_rel_raw, int (WORD_PREC));
      const int      new_high_rel = new_high_rel_raw - delta_exp_w * WORD_PREC;
      const int      shift_b = new_high_rel - cur_high_rel;

      _mantissa.shift (shift_b, true);
      _exp_w += delta_exp_w;

      fix_result ();
   }
}



template <int NW>
void  FloatMultiPrec <NW>::neg ()
{
   _mantissa.neg ();
}



template <int NW>
void  FloatMultiPrec <NW>::abs ()
{
   _mantissa.set_sign (false);
}



// Computes 1/x
template <int NW>
void  FloatMultiPrec <NW>::recip ()
{
   assert (! _mantissa.is_null ());

   const FloatMultiPrec <NW>  divisor (*this);

   // Starts with an evaluation in double
   // We need the scaling to avoid over/underflow during the conversion
   // to double.
   const int      scale_w = _exp_w;
   _exp_w = 0;

   const double   den = to_dbl ();
   const double   guess = 1.0 / den;
   from_dbl (guess);

   _exp_w -= scale_w;

   // - 1 because the LSB is probably the result of a rounding.
   int            nbr_exact_bits = DBL_MANT_DIG - 1;

   // Newton-Raphson: f(x) = 1/x - divisor
   // X_{n+1} = X_n * (2 - divisor * X_n)
   FloatMultiPrec <NW>  two;
   two.exp2_int (1);
   while (nbr_exact_bits < GLOBAL_PREC)
   {
      FloatMultiPrec <NW>  tmp (*this);
      tmp *= divisor;
      tmp -= two;
      tmp.neg ();
      operator *= (tmp);

      // Convergence is quadratic
      nbr_exact_bits <<= 1;
   }

   fix_result ();
}



template <int NW>
void  FloatMultiPrec <NW>::floor ()
{
   // Position of the unity bit within the mantissa
   int         unit_bit_pos = GLOBAL_PREC - WORD_PREC;
   unit_bit_pos -= _exp_w * WORD_PREC;

   // Absolute value < 1
   if (unit_bit_pos >= GLOBAL_PREC)
   {
      if (_mantissa.is_negative ())
      {
         exp2_int (0);
         _mantissa.set_sign (true);
      }
      else
      {
         clear ();
      }
   }

   // Contains decimal information
   else if (unit_bit_pos > 0)
   {
      if (_mantissa.is_negative ())
      {
         MantissaType   fix;
         fix.set_val_bits (0, unit_bit_pos);

         _mantissa.sub (fix);
      }

      _mantissa.clr_bits (0, unit_bit_pos);
   }
}



template <int NW>
void  FloatMultiPrec <NW>::ceil ()
{
   _mantissa.neg ();
   floor ();
   _mantissa.neg ();
}



/*

First, we compute easily floor(log2(x)). We keep a number in [1 ; 2[.

Then:

*** Solution 1 **

             +oo    1       x - 1 (2*n + 1)
ln (x) = 2 * sum ------- * (-----)
             n=0 2*n + 1    x + 1

(x-1)/(x+1) is in range [0 ; 1/3[
((x-1)/(x+1))^2 is in range [0 ; 1/9[
Therefore the summed term is divided by at most 1/9 at each iteration.
Thus, each iteration brings at least 3 more bits of precision.

Each step requires:
- 1 div (3 Newton iterations: 6 mul, 3 add)
- 2 mul
- 1 add
Total per bit: 8/3 mul, 4/3 add

*** Solution 2 ***

Log2

Square the number repeatedly. Every time it goes over 2, divide it by 2 and
write a "1" bit, else just write a "0" bit.

Each step requires:
- 1 mul
- 1 comparison (just one bit to test)
- 1 shift

Solution 2 is prefered.

Note: highest resolution is achieved for numbers in range [0.5 ; 2]

*/

template <int NW>
void  FloatMultiPrec <NW>::log2 ()
{
   assert (! _mantissa.is_negative ());
   assert (! _mantissa.is_null ());

   int            highest_bit_pos;

   // Integer part of the logarithm
   const int      log2_int = compute_log2_int (highest_bit_pos);

   // Outputs a result in range [0 ; 1[, where 1 is at bit GLOBAL_PREC.
   const int      shift = GLOBAL_PREC - highest_bit_pos;
   assert (shift > 0);
   _mantissa.shift_left (shift);
   _mantissa.set_bit (GLOBAL_PREC); // Restores the unity bit located in the headroom.

   MantissaType   nbr (_mantissa);
   _mantissa.clear ();
   _exp_w = -1;

   for (int bit_index = GLOBAL_PREC - 1; bit_index >= 0; -- bit_index)
   {
      nbr.mul_high (nbr, true);
      assert (! nbr.get_bit (GLOBAL_PREC + 2));

      if (nbr.get_bit (GLOBAL_PREC + 1))
      {
         nbr.shift_right (1, true);
         nbr.set_bit (GLOBAL_PREC); // Restores the unity bit located in the headroom.

         _mantissa.set_bit (bit_index);
      }
   }

   if (log2_int != 0)
   {
      const FloatMultiPrec <NW>  log2_int_flt (log2_int);
      operator += (log2_int_flt);
   }
}



template <int NW>
int   FloatMultiPrec <NW>::compute_log2_int () const
{
   assert (! _mantissa.is_null ());

   int            highest_bit_pos;

   return (compute_log2_int (highest_bit_pos));
}



template <int NW>
void  FloatMultiPrec <NW>::round_last_mantissa_bits (int nbr_bits)
{
   assert (nbr_bits > 0);
   assert (nbr_bits <= REAL_PREC);

   _mantissa.round_to (nbr_bits);
}



template <int NW>
void  FloatMultiPrec <NW>::round_first_mantissa_bits (int nbr_bits)
{
   assert (nbr_bits > 0);
   assert (nbr_bits <= REAL_PREC);

   const int      highest_bit = _mantissa.find_highest_bit_set ();
   if (highest_bit >= 0)
   {
      const int      nbr_bits_to_clear = highest_bit + 1 - nbr_bits;
      if (nbr_bits_to_clear > 0)
      {
         _mantissa.round_to (nbr_bits_to_clear);
      }
   }
}



template <int NW>
FloatMultiPrec <NW> &   FloatMultiPrec <NW>::operator += (const FloatMultiPrec <NW> &other)
{
   assert (&other != 0);

   bool           normal_flag = false;

   const int      exp_dif = _exp_w - other._exp_w;
   const bool     rhs_null_flag = other._mantissa.is_null ();
   if (! rhs_null_flag)
   {
      const bool     lhs_null_flag = _mantissa.is_null ();
      if (lhs_null_flag)
      {
         *this = other;
      }

      // Simplest case: the magnitude difference is so huge that one of the
      // arguments absorbs the other one.
      else if (lang::abs (exp_dif) >= NBR_WORDS)
      {
         if (exp_dif < 0)
         {
            *this = other;
         }
      }

      else
      {
         normal_flag = true;
      }
   }

   if (normal_flag)
   {
      MantissaType   mant_tmp;
      const MantissaType * rhs_mant_ptr = &other._mantissa;
      int            exp_w_result = other._exp_w;

      // Simplest scale case: rhs bigger than lhs: scale down the lhs.
      if (exp_dif < 0)
      {
         _mantissa.shift_right (exp_dif * -WORD_PREC, true);
      }

      // lhs bigger than rhs: we need to scale down the rhs,
      // therefore we have to operate on a copy.
      else if (exp_dif > 0)
      {
         mant_tmp = other._mantissa;
         rhs_mant_ptr = &mant_tmp;

         mant_tmp.shift_right (exp_dif * WORD_PREC, true);

         exp_w_result = _exp_w;
      }

      _mantissa.add (*rhs_mant_ptr);

      bool           null_flag;
      const int      exp_fix = _mantissa.normalise (null_flag);

      if (null_flag)
      {
         _exp_w = 0;
      }

      else
      {
         exp_w_result -= exp_fix;
         _exp_w = exp_w_result;

         fix_result ();
      }
   }

   return (*this);
}



template <int NW>
FloatMultiPrec <NW> &   FloatMultiPrec <NW>::operator -= (const FloatMultiPrec <NW> &other)
{
   assert (&other != 0);

   _mantissa.neg ();
   operator += (other);
   _mantissa.neg ();

   fix_result ();

   return (*this);
}



/*

We have:

(1)   N1      = m1 * 2 ^ ((e1 - NBR_WORDS + 1) * WORD_PREC)
(2)   N2      = m2 * 2 ^ ((e2 - NBR_WORDS + 1) * WORD_PREC)

We want mm and ee such that:

(3)   N1 * N2 = mm * 2 ^ ((ee - NBR_WORDS + 1) * WORD_PREC)

We compute the product by developping (1) and (2):

      N1 * N2 =   m1 * 2 ^ ((e1 - NBR_WORDS + 1) * WORD_PREC)
                * m2 * 2 ^ ((e2 - NBR_WORDS + 1) * WORD_PREC)
              = m1 * m2 * 2 ^ (  (e1 - NBR_WORDS + 1) * WORD_PREC
                         + (e2 - NBR_WORDS + 1) * WORD_PREC)
(4)   N1 * N2 = m1 * m2 * 2 ^ ((e1 + e2 - 2 * NBR_WORDS + 2) * WORD_PREC)

We also know what will give the mantissa multiplication in normalised mode.
Here, we can ignore the shifting information resulting of the normalisation:

(5)   mm = m1 * m2 * 2 ^ (-NBR_WORDS * WORD_PREC)

Let's insert (5) into (3):

      N1 * N2 =   m1 * m2 * 2 ^ (-NBR_WORDS * WORD_PREC)
                * 2 ^ ((ee - NBR_WORDS + 1) * WORD_PREC)
              = m1 * m2 * 2 ^ (  (ee - NBR_WORDS + 1) * WORD_PREC
                               - NBR_WORDS * WORD_PREC)
(6)   N1 * N2 = m1 * m2 * 2 ^ ((ee - 2 * NBR_WORDS + 1) * WORD_PREC)

Combining (4) and (6), we get:

      (ee - 2 * NBR_WORDS + 1) * WORD_PREC =
         (e1 + e2 - 2 * NBR_WORDS + 2) * WORD_PREC
      ee - 2 * NBR_WORDS + 1 = e1 + e2 - 2 * NBR_WORDS + 2
      ee = e1 + e2 - 2 * NBR_WORDS + 2 + 2 * NBR_WORDS - 1
      ee = e1 + e2 + 1

*/

template <int NW>
FloatMultiPrec <NW> &   FloatMultiPrec <NW>::operator *= (const FloatMultiPrec <NW> &other)
{
   assert (&other != 0);

   const int      shift = _mantissa.mul_normalise (other._mantissa, true);
   if (_mantissa.is_null ())
   {
      _exp_w = 0;
   }
   else
   {
      _exp_w += other._exp_w + 1 - shift;

      fix_result ();
   }

   return (*this);
}



template <int NW>
FloatMultiPrec <NW> &   FloatMultiPrec <NW>::operator /= (const FloatMultiPrec <NW> &other)
{
   assert (&other != 0);
   assert (! other._mantissa.is_null ());

   if (_mantissa.is_null ())
   {
      _mantissa.set_sign (
         _mantissa.is_negative () ^ other._mantissa.is_negative ()
      );
   }

   else
   {
      FloatMultiPrec <NW>  rcp (other);
      rcp.recip ();

      operator *= (rcp);

      fix_result ();
   }

   return (*this);
}



template <int NW>
FloatMultiPrec <NW>  FloatMultiPrec <NW>::operator - () const
{
   FloatMultiPrec <NW>  tmp (*this);
   tmp.neg ();

   return (tmp);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <int NW>
void  FloatMultiPrec <NW>::fix_result ()
{
   if (_exp_w < EXP_MIN)
   {
      _mantissa.clear_and_keep_sign ();
      _exp_w = 0;
   }
}



template <int NW>
int   FloatMultiPrec <NW>::compute_log2_int (int &highest_bit_pos) const
{
   assert (&highest_bit_pos != 0);

   highest_bit_pos = _mantissa.find_highest_bit_set ();

   const int      log2_int_part = highest_bit_pos - (GLOBAL_PREC - WORD_PREC);
   const int      log2_int = _exp_w * WORD_PREC + log2_int_part;

   return (log2_int);
}



/*\\\ STATIC NON-MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <int NW>
static inline bool   operator == (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs)
{
   assert (&lhs != 0);
   assert (&rhs != 0);

   return (lhs.comp_eq (rhs));
}



template <int NW>
static inline bool   operator != (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs)
{
   assert (&lhs != 0);
   assert (&rhs != 0);

   return (! (lhs == rhs));
}



template <int NW>
static inline bool   operator < (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs)
{
   assert (&lhs != 0);
   assert (&rhs != 0);

   return (lhs.comp_lt (rhs));
}



template <int NW>
static inline bool   operator <= (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs)
{
   assert (&lhs != 0);
   assert (&rhs != 0);

   return (! rhs.comp_lt (lhs));
}



template <int NW>
static inline bool   operator > (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs)
{
   assert (&lhs != 0);
   assert (&rhs != 0);

   return (rhs.comp_lt (lhs));
}



template <int NW>
static inline bool   operator >= (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs)
{
   assert (&lhs != 0);
   assert (&rhs != 0);

   return (! lhs.comp_lt (rhs));
}



template <int NW>
static inline FloatMultiPrec <NW>   operator + (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs)
{
   assert (&lhs != 0);
   assert (&rhs != 0);

   FloatMultiPrec <NW>  tmp (lhs);
   tmp += (rhs);

   return (tmp);
}



template <int NW>
static inline FloatMultiPrec <NW>   operator - (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs)
{
   assert (&lhs != 0);
   assert (&rhs != 0);

   FloatMultiPrec <NW>  tmp (lhs);
   tmp -= (rhs);

   return (tmp);
}



template <int NW>
static inline FloatMultiPrec <NW>   operator * (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs)
{
   assert (&lhs != 0);
   assert (&rhs != 0);

   FloatMultiPrec <NW>  tmp (lhs);
   tmp *= (rhs);

   return (tmp);
}



template <int NW>
static inline FloatMultiPrec <NW>   operator / (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs)
{
   assert (&lhs != 0);
   assert (&rhs != 0);

   FloatMultiPrec <NW>  tmp (lhs);
   tmp /= (rhs);

   return (tmp);
}



}  // namespace math
}  // namespace ohm



#endif   // ohm_math_FloatMultiPrec_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
