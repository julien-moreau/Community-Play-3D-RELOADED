/*****************************************************************************

        FloatMultiPrec.h
        Copyright (c) 2009 Ohm Force

        Author: Laurent de Soras <laurent.de.soras@ohmforce.com>
        Revision: 42523

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



#if ! defined (ohm_math_FloatMultiPrec_HEADER_INCLUDED)
#define  ohm_math_FloatMultiPrec_HEADER_INCLUDED
#pragma once

#if defined (_MSC_VER)
   #pragma warning (4 : 4250) // "Inherits via dominance."
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "ohm/math/IntMultiPrec.h"
#include "ohm/lang/def.h"

#include <cfloat>



namespace ohm
{
namespace math
{



template <int NW>
class FloatMultiPrec
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

   enum {         NBR_WORDS   = NW  };

   typedef  IntMultiPrec <NW> MantissaType;

   enum {         WORD_PREC      = MantissaType::WORD_PREC  };
   enum {         GLOBAL_PREC    = MantissaType::GLOBAL_PREC   };
   enum {         REAL_PREC      = GLOBAL_PREC - WORD_PREC + 1 };

                  FloatMultiPrec ();
                  FloatMultiPrec (double val);
                  FloatMultiPrec (int int_val);
                  FloatMultiPrec (archi::Int64 int_val);
                  FloatMultiPrec (const MantissaType &int_val);
                  FloatMultiPrec (const FloatMultiPrec <NW> &other);
   virtual        ~FloatMultiPrec () {}

   FloatMultiPrec <NW> &
                  operator = (const FloatMultiPrec <NW> &other);

   bool           is_null () const;
   bool           comp_eq (const FloatMultiPrec <NW> &other) const;
   bool           comp_lt (const FloatMultiPrec <NW> &other) const;

#if 0 // We don't want implicit conversions to double - lot of headaches.
                  operator double () const;
#endif
   double         to_dbl () const;
   void           from_dbl (double val);

   void           clear ();
   void           exp2_int (int exp_bit);
   void           scale2_int (int l2);

   void           neg ();
   void           abs ();
   void           recip ();
   void           floor ();
   void           ceil ();

   void           log2 ();
   int            compute_log2_int () const;

   void           round_last_mantissa_bits (int nbr_bits);
   void           round_first_mantissa_bits (int nbr_bits);

   FloatMultiPrec <NW> &
                  operator += (const FloatMultiPrec <NW> &other);
   FloatMultiPrec <NW> &
                  operator -= (const FloatMultiPrec <NW> &other);
   FloatMultiPrec <NW> &
                  operator *= (const FloatMultiPrec <NW> &other);
   FloatMultiPrec <NW> &
                  operator /= (const FloatMultiPrec <NW> &other);

   FloatMultiPrec <NW>
                  operator - () const;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

   enum {         DBL_EXP_SHIFT  = DBL_MANT_DIG - 1   };
   enum {         DBL_EXP_WIDTH  = 64 - 1 - (DBL_MANT_DIG - 1) };
   enum {         DBL_BIAS       = (1 << (DBL_EXP_WIDTH - 1)) - 1 };

   // Insures that the double to FloatMultiPrec conversion is lossless.
   ohm_lang_CHECK_CST (SizeCvDbl, (GLOBAL_PREC - WORD_PREC >= DBL_MANT_DIG));

   enum {         EXP_MIN        = INT_MIN / WORD_PREC + 1  };

   void           fix_result ();
   int            compute_log2_int (int &highest_bit_pos) const;

   // N = _mantissa * 2 ^ ((_exp_w - NBR_WORDS + 1) * WORD_PREC)
   // N = XXXX.XXXX XXXX ... * 2 ^ (_exp_w * WORD_PREC)
   MantissaType   _mantissa;     // Should be normalised.
   int            _exp_w;        // Exponent in words.



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class FloatMultiPrec



/*\\\ STATIC NON-MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <int NW>
static inline bool   operator == (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs);
template <int NW>
static inline bool   operator != (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs);
template <int NW>
static inline bool   operator < (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs);
template <int NW>
static inline bool   operator <= (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs);
template <int NW>
static inline bool   operator > (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs);
template <int NW>
static inline bool   operator >= (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs);

template <int NW>
static inline FloatMultiPrec <NW>   operator + (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs);
template <int NW>
static inline FloatMultiPrec <NW>   operator - (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs);
template <int NW>
static inline FloatMultiPrec <NW>   operator * (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs);
template <int NW>
static inline FloatMultiPrec <NW>   operator / (const FloatMultiPrec <NW> &lhs, const FloatMultiPrec <NW> &rhs);



}  // namespace math
}  // namespace ohm



#include "ohm/math/FloatMultiPrec.hpp"



#endif   // ohm_math_FloatMultiPrec_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
