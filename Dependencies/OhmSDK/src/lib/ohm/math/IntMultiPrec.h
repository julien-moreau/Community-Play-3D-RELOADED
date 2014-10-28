/*****************************************************************************

        IntMultiPrec.h
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



#if ! defined (ohm_math_IntMultiPrec_HEADER_INCLUDED)
#define  ohm_math_IntMultiPrec_HEADER_INCLUDED
#pragma once

#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/archi/types.h"
#include "ohm/lang/def.h"

#include <climits>



namespace ohm
{
namespace math
{



template <int NW>
class IntMultiPrec
{

   friend class IntMultiPrec <NW / 2>;

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum {         NBR_WORDS   = NW  };

   typedef  archi::UInt32  IntSgl;     // For storage, unsigned.
   typedef  archi::Int32   IntSglSgn;  // For carry propagation, signed.
   typedef  archi::Int64   IntDbl;     // For temporary accumulation, signed.
   ohm_lang_CHECK_CST (SglDbl, (sizeof (IntDbl) == 2 * sizeof (IntSgl)));

   enum {         HEADROOM_PREC  = 8   }; // >= 2
   enum {         WORD_CAPACITY  = sizeof (IntSgl) * CHAR_BIT  };
   enum {         WORD_PREC      = WORD_CAPACITY - (HEADROOM_PREC + 1) / 2 };
   ohm_lang_CHECK_CST (HdrPrec, (NBR_WORDS <= 1 << (HEADROOM_PREC - 1)));

   enum {         GLOBAL_PREC    = WORD_PREC * NBR_WORDS };
   enum {         REAL_CAPACITY  = WORD_PREC * (NBR_WORDS - 1) + WORD_CAPACITY   };

                  IntMultiPrec ();
                  IntMultiPrec (archi::Int64 val);
                  IntMultiPrec (archi::UInt64 val, bool neg_flag);
                  IntMultiPrec (const IntMultiPrec <NW> &other);
   virtual        ~IntMultiPrec () {}

   IntMultiPrec <NW> &
                  operator = (const IntMultiPrec <NW> &other);

   int            comp_magnitude (const IntMultiPrec <NW> &other, bool &lhs_null_flag, bool &rhs_null_flag) const;
   bool           comp_eq (const IntMultiPrec <NW> &other) const;
   bool           comp_lt (const IntMultiPrec <NW> &other) const;

   void           clear ();
   void           clear_and_keep_sign ();
   void           set_val (archi::Int64 val);
   void           set_val (archi::UInt64 val, bool neg_flag);
   void           set_val (archi::UInt64 val, bool neg_flag, int left_bitshift);
   void           set_val_bits (int beg, int end);
   archi::UInt64  get_val_abs_part (int bit_index) const;

   void           neg ();
   void           add (const IntMultiPrec <NW> &other);
   void           sub (const IntMultiPrec <NW> &other);
   void           add_small (int val);
   void           mul (IntMultiPrec <NBR_WORDS * 2> &result, const IntMultiPrec <NW> &other) const;
   void           mul_high (const IntMultiPrec <NW> &other, bool round_flag);
   int            mul_normalise (const IntMultiPrec <NW> &other, bool round_flag);
   void           shift_left (int nbr_bits);
   void           shift_left_words (int nbr_words);
   void           shift_right (int nbr_bits, bool round_flag);
   void           shift (int nbr_bits, bool round_flag);
   void           round_to (int nbr_bits);
   int            normalise ();
   int            normalise (bool &null_flag);
   bool           is_normalised () const;
   bool           is_lower_mag_than (const IntMultiPrec <NW> &other) const;
   bool           is_headroom_empty () const;
   bool           is_null () const;

   bool           get_bit (int index) const;
   void           chg_bit (int index, bool flag);
   void           clr_bit (int index);
   void           set_bit (int index);
   void           clr_bits (int beg, int end);
   int            find_highest_bit_set () const;
   int            find_size_bits () const;
   int            find_size_words () const;
   bool           is_negative () const;
   void           set_sign (bool neg_flag);

   void           bin_not ();
   void           bin_or (const IntMultiPrec <NW> &other);
   void           bin_and (const IntMultiPrec <NW> &other);
   void           bin_xor (const IntMultiPrec <NW> &other);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   typedef  IntDbl   TmpDigitArray [NBR_WORDS * 2];   // For the multiplication

   void           shift_left_words_nocheck (int nbr_words);
   void           add_mag (const IntMultiPrec <NW> &other);
   void           sub_mag (const IntMultiPrec <NW> &other);
   void           sub_mag_inv (const IntMultiPrec <NW> &other);
   void           mul_mag (TmpDigitArray &result, const IntMultiPrec <NW> &other) const;
   void           propagate_carry ();
   static void    propagate_carry (IntSgl digits [], int nbr_digits);
   static IntDbl  simulate_carry (const IntDbl digits_src [], int nbr_digits);
   static void    copy_and_propagate_carry (IntSgl digits_dest [], const IntDbl digits_src [], int nbr_digits, IntDbl carry = 0);

   IntSgl         _digits [NBR_WORDS]; // Stored in LSW...MSW order (little endian)
   bool           _neg_flag;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class IntMultiPrec



/*\\\ STATIC NON-MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <int NW>
static inline bool   operator == (const IntMultiPrec <NW> &lhs, const IntMultiPrec <NW> &rhs);
template <int NW>
static inline bool   operator != (const IntMultiPrec <NW> &lhs, const IntMultiPrec <NW> &rhs);
template <int NW>
static inline bool   operator < (const IntMultiPrec <NW> &lhs, const IntMultiPrec <NW> &rhs);
template <int NW>
static inline bool   operator <= (const IntMultiPrec <NW> &lhs, const IntMultiPrec <NW> &rhs);
template <int NW>
static inline bool   operator > (const IntMultiPrec <NW> &lhs, const IntMultiPrec <NW> &rhs);
template <int NW>
static inline bool   operator >= (const IntMultiPrec <NW> &lhs, const IntMultiPrec <NW> &rhs);



}  // namespace math
}  // namespace ohm



#include "ohm/math/IntMultiPrec.hpp"



#endif   // ohm_math_IntMultiPrec_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
